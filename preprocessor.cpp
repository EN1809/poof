#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

global_variable memory_arena* TranArena = {};
#include <counted_string.cpp>
#include <stream.cpp>

enum c_token_type
{
  CTokenType_Unknown,

  CTokenType_Identifier,
  CTokenType_String,

  CTokenType_OpenBrace     = '{',
  CTokenType_CloseBrace    = '}',
  CTokenType_OpenParen     = '(',
  CTokenType_CloseParen    = ')',
  CTokenType_Dot           = '.',
  CTokenType_Comma         = ',',
  CTokenType_Semicolon     = ';',
  CTokenType_Colon         = ':',
  CTokenType_Hash          = '#',
  CTokenType_Space         = ' ',
  CTokenType_Star          = '*',
  CTokenType_Ampersand     = '&',
  CTokenType_SingleQuote   = '\'',
  CTokenType_DoubleQuote   = '"',
  CTokenType_Equals        = '=',
  CTokenType_LT            = '<',
  CTokenType_GT            = '>',
  CTokenType_Plus          = '+',
  CTokenType_Minus         = '-',
  CTokenType_Percent       = '%',
  CTokenType_Bang          = '!',
  CTokenType_Hat           = '^',
  CTokenType_Question      = '?',
  CTokenType_FSlash        = '/',
  CTokenType_BSlash        = '\\',
  CTokenType_Tilde         = '~',
  CTokenType_Backtick      = '`',
  CTokenType_Newline       = '\n',
  CTokenType_CarrigeReturn = '\r',
  CTokenType_EOF           = EOF,
};


struct c_token
{
  c_token_type Type;
  counted_string Value;
};

struct c_token_buffer
{
  c_token* Start;
  c_token* At;
  c_token* End;
};

inline void
PrintToken(c_token Token)
{
  switch (Token.Type)
  {
    case CTokenType_Identifier:
    {
      Log("%.*s", Token.Value.Count, Token.Value.Start);
    } break;

    case CTokenType_String:
    {
      Log("\"%.*s\"", Token.Value.Count, Token.Value.Start);
    } break;

    default:
    {
      Log("%c", Token.Type);
    }
  }
}

c_token_buffer
AllocateTokenBuffer(memory_arena* Memory, u32 Count)
{
  c_token_buffer Result = {};
  Result.Start = Allocate(c_token, Memory, Count);
  Result.At = Result.Start;
  Result.End = Result.Start + Count;

  return Result;
}

template <typename BufferType, typename PushType> void
Push(BufferType *Buffer, PushType Token)
{
  if (Buffer->At < Buffer->End)
  {
    *Buffer->At = Token;
    Buffer->At++;
  }
  else
  {
    Error("Ran out of space pushing onto buffer.");
  }
}

c_token
GetToken(ansi_stream* Stream)
{
  c_token Result = {};

  char At = *Stream->At;
  switch (At)
  {
    case CTokenType_OpenBrace:
    case CTokenType_CloseBrace:
    case CTokenType_OpenParen:
    case CTokenType_CloseParen:
    case CTokenType_Dot:
    case CTokenType_Comma:
    case CTokenType_Semicolon:
    case CTokenType_Colon:
    case CTokenType_Hash:
    case CTokenType_Space:
    case CTokenType_Star:
    case CTokenType_Ampersand:
    case CTokenType_SingleQuote:
    case CTokenType_DoubleQuote:
    case CTokenType_Equals:
    case CTokenType_LT:
    case CTokenType_GT:
    case CTokenType_Plus:
    case CTokenType_Minus:
    case CTokenType_Percent:
    case CTokenType_Bang:
    case CTokenType_Hat:
    case CTokenType_Question:
    case CTokenType_FSlash:
    case CTokenType_BSlash:
    case CTokenType_Tilde:
    case CTokenType_Backtick:
    case CTokenType_Newline:
    case CTokenType_CarrigeReturn:
    case CTokenType_EOF:
    {
      Result = { .Type = (c_token_type)At };
    } break;
  }

  return Result;
}

struct c_parse_result
{
  b32 Valid;
  c_token_buffer Tokens;
};

function c_parse_result
TokenizeFile(const char* FileName, memory_arena* Memory)
{
  c_parse_result Result = {};

  // TODO(Jesse): Since we store pointers directly into this buffer, we need to
  // keep the memory around.  Should we tokenize such that we allocate new
  // memory for things that need it?  (only Strings/Identifiers at the moment)
  ansi_stream SourceFileStream = AnsiStreamFromFile(FileName, Memory);
  if (!SourceFileStream.Start)
  {
    Error("Allocating stream for %s", FileName);
    return Result;
  }

  Result.Tokens = AllocateTokenBuffer(Memory, (u32)Megabytes(100));
  if (!Result.Tokens.Start)
  {
    Error("Allocating Token Buffer");
    return Result;
  }

  while(Remaining(&SourceFileStream))
  {
    c_token T = GetToken(&SourceFileStream);

    switch (T.Type)
    {
      case CTokenType_DoubleQuote:
      {
        T.Type = CTokenType_String;
        T.Value = PopQuotedString(&SourceFileStream);
      } break;

      case CTokenType_Unknown:
      {
        counted_string Value = {
          .Start = SourceFileStream.At,
        };

        while (1)
        {
          T = GetToken(&SourceFileStream);
          if (T.Type == CTokenType_Unknown && Remaining(&SourceFileStream))
          {
            SourceFileStream.At++;
          }
          else
          {
            break;
          }
        }

        Value.Count = (umm)SourceFileStream.At - (umm)Value.Start;

        T.Type = CTokenType_Identifier;
        T.Value = Value;
      } break;

      default:
      {
        SourceFileStream.At++;
      } break;
    }

    Push(&Result.Tokens, T);

    continue;
  }


  Result.Tokens.End = Result.Tokens.At;
  Result.Tokens.At = Result.Tokens.Start;
  Result.Valid = True;

  return Result;
}

function c_token
PopToken(c_parse_result* Parser)
{
  c_token Result = {};
  if (Remaining(&Parser->Tokens))
  {
    Result = *Parser->Tokens.At++;
  }
  else
  {
    Warn("Tried to pop a token on an empty stream");
  }
  return Result;
}


function c_token
RequireToken(c_parse_result* Parser, c_token_type Type)
{
  c_token Result = PopToken(Parser);

  if (Result.Type != Type)
  {
    Error("Encountered Unexpected token type");
    PrintToken(Result);
    Parser->Valid = False;
  }

  return Result;
}

void
ParseDiscriminatedUnion(c_parse_result* Parser)
{
  RequireToken(Parser, CTokenType_OpenParen);
  c_token UnionName = RequireToken(Parser, CTokenType_Identifier);

  Log("struct %.*s\n{\n  %.*s_type Type;\n\n  union\n  {\n", UnionName.Value.Count, UnionName.Value.Start, UnionName.Value.Count, UnionName.Value.Start);

  RequireToken(Parser, CTokenType_Comma);
  RequireToken(Parser, CTokenType_Newline);
  RequireToken(Parser, CTokenType_OpenBrace);

  b32 Complete = False;
  while (!Complete)
  {
    c_token Interior = PopToken(Parser);

    switch (Interior.Type)
    {
      case CTokenType_Identifier:
      {
        RequireToken(Parser, CTokenType_Semicolon);
        Log("    %.*s %.*s;", Interior.Value.Count, Interior.Value.Start, Interior.Value.Count, Interior.Value.Start);
      } break;

      case CTokenType_CloseBrace:
      {
        Log("  };\n};\n");
        RequireToken(Parser, CTokenType_CloseParen);
        Complete = True;
      } break;

      case CTokenType_Newline:
      {
        PrintToken(Interior);
      } break;
      case CTokenType_Space:
      {
      } break;

      default:
      {
        Parser->Valid = False;
      } break;
    }
  }

  if (!Parser->Valid)
  {
    Error("Parsing d_union declaration");
  }

  return;
}

struct stream_chunk
{
  counted_string String;
  stream_chunk* Next;
};

s32
main(s32 ArgCount, const char** Args)
{
  b32 Result = True;

  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;


    for (s32 FileIndex = 1;
        FileIndex < ArgCount;
        ++FileIndex)
    {
      const char* FileName = Args[FileIndex];

      c_parse_result Parser = TokenizeFile(FileName, Memory);
      if (Parser.Valid)
      {
        c_token_buffer* Tokens = &Parser.Tokens;
        while (Parser.Valid && Remaining(Tokens))
        {
          c_token Token = PopToken(&Parser);
          switch( Token.Type )
          {
            case CTokenType_Identifier:
            {
              if (StringsMatch(Token.Value, CS("d_union")))
              {
                ParseDiscriminatedUnion(&Parser);
              }
            } break;

            default: { } break;
          }

          continue;
        }
      }
      else
      {
        Error("Tokenizing File: %s", FileName);
      }

      Result = Result && Parser.Valid;
    }
  }
  else
  {
    Warn("No files passed in, exiting.");
  }

  return (s32)Result;
}
