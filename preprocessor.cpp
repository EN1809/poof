#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1


#include <bonsai_types.h>


#define InvalidDefaultWhileParsing(P, ErrorMessage) \
    default: { ParseError(P, PeekTokenPointer(P), ErrorMessage); Assert(False); } break;


# define DEBUG_PRINT (0)
#if DEBUG_PRINT
#include <bonsai_stdlib/headers/debug_print.h>

#else
_Pragma("clang diagnostic push")
_Pragma("clang diagnostic ignored \"-Wunused-macros\"")

#define DebugPrint(...)                                   \
  _Pragma("clang diagnostic push")                        \
  _Pragma("clang diagnostic ignored \"-Wunused-value\"")  \
  (__VA_ARGS__)                                           \
  _Pragma("clang diagnostic pop")

_Pragma("clang diagnostic pop") // unused-macros
#endif



// TODO(Jesse id: 293, tags: big): Constant expression evaluation for #if statements
// TODO(Jesse id: 291, tags: big): Macro functions
// TODO(Jesse id: 292, tags: big): Include graph traversal


/* #define TestMacro(a, b) a b */

/* TestMacro( */
/*     void Thing( TestMacro(int a1, int a2), );, */
/*     void Thing2( TestMacro(int a1, int a2) ); */
/* ) */

bonsai_function c_token *      PeekTokenRawPointer(parser *Parser, u32 TokenLookahead = 0);
bonsai_function c_token        PeekTokenRaw(parser *Parser, u32 Lookahead = 0);
bonsai_function u32            OffsetOfNext(parser *Parser, u32 Offset, c_token_type Close);
bonsai_function c_token *      PeekTokenPointer(parser *Parser, u32 TokenLookahead = 0);
bonsai_function c_token        PeekToken(parser *Parser, u32 Lookahead = 0);
bonsai_function c_token        PopTokenRaw(parser *Parser);
bonsai_function c_token *      PopTokenRawPointer(parser *Parser);
bonsai_function b32            TokensRemain(parser *Parser, u32 TokenLookahead = 0);
bonsai_function b32            RawTokensRemain(parser *Parser, u32 TokenLookahead = 0);
bonsai_function c_token        PopToken(parser *Parser);
bonsai_function b32            OptionalTokenRaw(parser *Parser, c_token_type Type);
bonsai_function b32            OptionalToken(parser *Parser, c_token T);
bonsai_function b32            OptionalToken(parser *Parser, c_token_type Type);
bonsai_function c_token        RequireToken(parser *Parser, c_token ExpectedToken);
bonsai_function c_token        RequireToken(parser *Parser, c_token_type ExpectedType);
bonsai_function c_token        RequireTokenRaw(parser *Parser, c_token Expected);
bonsai_function c_token        RequireTokenRaw(parser *Parser, c_token_type ExpectedType);
bonsai_function b32            TokenIsOperator(c_token_type T);
bonsai_function b32            NextTokenIsOperator(parser *Parser);
bonsai_function counted_string RequireOperatorToken(parser *Parser);

bonsai_function void TrimFirstToken(parser* Parser, c_token_type TokenType);
bonsai_function void TrimLastToken(parser* Parser, c_token_type TokenType);
bonsai_function void TrimLeadingWhitespace(parser* Parser);
bonsai_function counted_string EatBetween(parser* Parser, c_token_type Open, c_token_type Close);

bonsai_function parser * ResolveInclude(parse_context *Ctx, parser *Parser);
bonsai_function parser * ExpandMacro(parser *Parser, macro_def *Macro, memory_arena *Memory);

void DumpTokens(parser *Parser);



bonsai_function void
Rewind(parser* Parser)
{
  Rewind(&Parser->OutputTokens);
  Rewind(&Parser->Tokens);
  Parser->LineNumber = 1;

  if (Parser->Next)
  {
    Rewind(Parser->Next);
  }
}

bonsai_function b32
IsNewline(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CTokenType_CarrigeReturn;

  return Result;
}

bonsai_function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CTokenType_CarrigeReturn  ||
               Type == CTokenType_Tab            ||
               Type == CTokenType_Space;

  return Result;
}

bonsai_function b32
IsWhitespace(c_token T)
{
  b32 Result = IsWhitespace(T.Type);
  return Result;
}

bonsai_function b32
IsComment(c_token T)
{
  b32 Result = (T.Type == CTokenType_CommentSingleLine) || (T.Type == CTokenType_CommentMultiLineStart);
  return Result;
}

bonsai_function umm
Remaining(parser* Parser)
{
  umm Result = Remaining(&Parser->Tokens);
  return Result;
}

bonsai_function void
Advance(c_token_cursor* Tokens, u32 Lookahead = 0)
{
  if (Remaining(Tokens, Lookahead))
  {
    Tokens->At += Lookahead+1;
  }
  else
  {
    Warn("Attempted to advance token stream past its end.");
  }

  return;
}

bonsai_function void
AdvanceParser(parser* Parser)
{
  if (Remaining(&Parser->Tokens))
  {
    c_token *T = Parser->Tokens.At;
    if (T->Type == CTokenType_Newline ||
        T->Type == CTokenType_EscapedNewline )
    {
      ++Parser->LineNumber;
    }

    if (Remaining(&Parser->OutputTokens))
    {
      Push(*T, &Parser->OutputTokens);
    }

    ++Parser->Tokens.At;
  }
  else if (Parser->Next)
  {
    Assert( Parser->Next->Tokens.At == Parser->Next->Tokens.Start);

    *Parser = *Parser->Next;
    if (Parser->Next)
    {
      Assert(Parser->Next != Parser);
      Assert(Parser->Next->Next != Parser);
      Assert(Parser->Next->Next != Parser->Next);
    }
  }
  else
  {
    // Attempted to advance parser past its last chunk
  }

  return;
}

bonsai_function void
AdvanceTo(parser* Parser, c_token* T)
{
  while (Parser->Tokens.At != T)
  {
    AdvanceParser(Parser);
  }

  if (Parser->Tokens.At != T)
  {
    Error("Unable to find supplied token pointer on parser chain during AdvanceTo!");
  }
}

bonsai_function void
RewindUntil(parser* Parser, c_token_type Type)
{
  while (Parser->Tokens.At >= Parser->Tokens.Start)
  {
    if (Parser->Tokens.At->Type == Type)
    {
      if (Remaining(Parser))
      {
        ++Parser->Tokens.At;
      }
      break;
    }
    --Parser->Tokens.At;
  }
}

bonsai_function c_token PopTokenRaw(parser* Parser);

bonsai_function void
ParseMacroArgument(parser* Parser, c_token_buffer *Result)
{
  Result->Start = Parser->Tokens.At;

  u32 Depth = 0;
  b32 Done = False;
  while (!Done && Remaining(&Parser->Tokens))
  {
    c_token T = PeekTokenRaw(Parser);
    if (Depth == 0 && T.Type == CTokenType_Comma)
    {
      Done = True;
    }
    else if (Depth > 0 && T.Type == CTokenType_CloseParen)
    {
      --Depth;
    }
    else if (T.Type == CTokenType_OpenParen)
    {
      ++Depth;
    }

    if (!Done) PopTokenRaw(Parser);
  }

  Result->Count = (umm)(Parser->Tokens.At - Result->Start);

  if (Remaining(&Parser->Tokens)) RequireToken(Parser, CTokenType_Comma);

  return;
}

bonsai_function counted_string
EatUntilExcluding(parser* Parser, c_token_type Close)
{
  string_from_parser Builder = StartStringFromParser(Parser);
  while (Remaining(&Parser->Tokens))
  {
    if(PeekTokenRaw(Parser).Type == Close)
    {
      break;
    }
    else
    {
      PopTokenRaw(Parser);
    }
  }
  counted_string Result = FinalizeStringFromParser(&Builder);
  return Result;
}

bonsai_function counted_string
EatUntilIncluding(parser* Parser, c_token_type Close)
{
  string_from_parser Builder = StartStringFromParser(Parser);
  while (Remaining(&Parser->Tokens))
  {
    if(PopTokenRaw(Parser).Type == Close)
    {
      break;
    }
  }
  counted_string Result = FinalizeStringFromParser(&Builder);
  return Result;
}




/*****************************                ********************************/
/*****************************  Error Output  ********************************/
/*****************************                ********************************/


void
DumpTokens(parser *Parser)
{
  u32 TokenCount = (u32)TotalElements(&Parser->Tokens);

  for ( u32 TokenIndex = 0;
        TokenIndex  < TokenCount;
        ++TokenIndex)
  {
    PrintToken(Parser->Tokens.Start[TokenIndex]);
  }

  return;
}

bonsai_function void
DumpEntireParser(parser* Parser, u32 LinesToDump = u32_MAX)
{
  parser LocalParser = *Parser;
  LocalParser.OutputTokens = {};
  Rewind(&LocalParser.Tokens);

  c_token *T = PopTokenRawPointer(&LocalParser);
  while(T && LinesToDump > 0)
  {
    PrintToken(T);

    if (T->Type == CTokenType_Newline)
    {
      --LinesToDump;
    }

    T = PopTokenRawPointer(&LocalParser);
  }
}

bonsai_function void
TruncateAtNextLineEnd(parser* Parser, u32 Count)
{
  c_token* StartingAt = Parser->Tokens.At;

  while (Remaining(&Parser->Tokens))
  {
    if(PopTokenRaw(Parser).Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        break;
      }
      --Count;
    }
  }

  Parser->Tokens.End = Parser->Tokens.At;
  Parser->Tokens.At = StartingAt;
}

bonsai_function void
TruncateAtPreviousLineStart(parser* Parser, u32 Count )
{
  c_token* StartingAt = Parser->Tokens.At;

  while (Parser->Tokens.At > Parser->Tokens.Start)
  {
    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        ++Parser->Tokens.At;
        break;
      }
      --Count;
    }
    --Parser->Tokens.At;
  }

  Parser->Tokens.Start = Parser->Tokens.At;
  Parser->Tokens.At = StartingAt;
}

bonsai_function void
OutputErrorHelperLine(parser* Parser, c_token* ErrorToken, c_token Expected, counted_string ErrorString, u32 LineNumber)
{
  if (!TotalElements(&Parser->Tokens)) return;

  if ( ! IsNewline(Parser->Tokens.End[-1].Type) )
  {
    Log("\n");
  }

  Rewind(&Parser->Tokens);

  u32 SpaceCount = 0;
  u32 TabCount = 0;

  b32 DoPipes = ErrorToken->Value.Count > 3;
  u32 ErrorIdentifierLength = DoPipes ? (u32)ErrorToken->Value.Count-2 : (u32)ErrorToken->Value.Count;

  while (Remaining(&Parser->Tokens))
  {

    if (Parser->Tokens.At == ErrorToken)
    {
      for (u32 ColumnIndex = 0;
          ColumnIndex < TabCount;
          ++ColumnIndex)
      {
        Log("\t");
      }

      for (u32 ColumnIndex = 0;
          ColumnIndex < SpaceCount;
          ++ColumnIndex)
      {
        Log(" ");
      }

      if (DoPipes)
      {
        Log("|");
      }

      for (u32 ColumnIndex = 0;
          ColumnIndex < ErrorIdentifierLength;
          ++ColumnIndex)
      {
        if (DoPipes)
        {
          Log("~");
        }
        else
        {
          Log("^");
        }
      }

      if (DoPipes)
      {
        Log("|");
      }

      Log("\n");

      break;
    }

    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
      SpaceCount = 0;
      TabCount = 0;
    }
    else if (Parser->Tokens.At->Type == CTokenType_EscapedNewline)
    {
      SpaceCount = 0;
      TabCount = 0;
    }
    else if (Parser->Tokens.At->Type == CTokenType_Tab)
    {
      ++TabCount;
    }
    else if (Parser->Tokens.At->Value.Count)
    {
      SpaceCount += Parser->Tokens.At->Value.Count;
    }
    else
    {
      ++SpaceCount;
    }

    ++Parser->Tokens.At;
  }

  while (Remaining(&Parser->Tokens))
  {

    if (Parser->Tokens.At == ErrorToken)
    {
      for (u32 ColumnIndex = 0;
          ColumnIndex < TabCount;
          ++ColumnIndex)
      {
        Log("\t");
      }

      for (u32 ColumnIndex = 0;
          ColumnIndex < SpaceCount;
          ++ColumnIndex)
      {
        Log(" ");
      }

      if (DoPipes)
      {
        Log("  ");
      }

      for (u32 ColumnIndex = 0;
          ColumnIndex < ErrorIdentifierLength;
          ++ColumnIndex)
      {
        if (ColumnIndex == ErrorIdentifierLength-1)
        {
          Log("|");
        }
        else
        {
          Log(" ");
        }
      }

      counted_string TokenTypeName = ToString(ErrorToken->Type);
      Log("---> %.*s", TokenTypeName.Count, TokenTypeName.Start );

      if (ErrorToken->Value.Count)
      {
        Log("(%.*s)" , ErrorToken->Value.Count, ErrorToken->Value.Start);
      }

      if (Expected.Type)
      {
        counted_string ExpectedTypeName = ToString(Expected.Type);
        Log(" Expecting : %.*s", ExpectedTypeName.Count, ExpectedTypeName.Start);

        if (Expected.Value.Count)
        {
          Log("(%.*s)" , Expected.Value.Count, Expected.Value.Start);
        }
      }
      else
      {
        Assert(!Expected.Value.Count);
      }


      Log(" %.*s\n", ErrorString.Count, ErrorString.Start);


      for (u32 ColumnIndex = 0;
          ColumnIndex < TabCount;
          ++ColumnIndex)
      {
        Log("\t");
      }

      for (u32 ColumnIndex = 0;
          ColumnIndex < SpaceCount + ErrorToken->Value.Count;
          ++ColumnIndex)
      {
        Log(" ");
      }

      counted_string Filename = Parser->Filename.Count ? Parser->Filename : CSz("(unknown file)");
      Log("     %.*s:%u:%u\n\n", Filename.Count, Filename.Start, LineNumber, SpaceCount+TabCount);

      break;
    }

    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
      SpaceCount = 0;
      TabCount = 0;
    }
    else if (Parser->Tokens.At->Type == CTokenType_EscapedNewline)
    {
      SpaceCount = 0;
      TabCount = 0;
    }
    else if (Parser->Tokens.At->Type == CTokenType_Tab)
    {
      ++TabCount;
    }
    else if (Parser->Tokens.At->Value.Count)
    {
      SpaceCount += Parser->Tokens.At->Value.Count;
    }
    else
    {
      ++SpaceCount;
    }

    ++Parser->Tokens.At;
  }

  return;
}

bonsai_function void
ParseError(parser* Parser, c_token* ErrorToken, c_token ExpectedToken, counted_string ErrorString)
{
  RuntimeBreak();

  Assert(ErrorToken);

  u32 LinesOfContext = 4;

  Log("------------------------------------------------------------------------------------\n");

  parser LocalParser = *Parser;
  LocalParser.OutputTokens = {};
  LocalParser.Tokens.At = ErrorToken;

  parser LeadingLines  = LocalParser;
  parser ErrorLine     = LocalParser;
  parser TrailingLines = LocalParser;

  RewindUntil(&LeadingLines, CTokenType_Newline);
  LeadingLines.Tokens.End = LeadingLines.Tokens.At;
  TruncateAtPreviousLineStart(&LeadingLines, LinesOfContext);
  DumpEntireParser(&LeadingLines);

  TruncateAtPreviousLineStart(&ErrorLine, 0);
  TruncateAtNextLineEnd(&ErrorLine, 0);
  DumpEntireParser(&ErrorLine);
  if (ErrorToken >= ErrorLine.Tokens.Start &&
      ErrorToken < ErrorLine.Tokens.End)
  {
    OutputErrorHelperLine(&ErrorLine, ErrorToken, ExpectedToken, ErrorString, LocalParser.LineNumber);
  }
  else
  {
    Error("Determining where the error occured");
    Log("Error was : %.*s\n", ErrorString.Count, ErrorString.Start);
  }

  EatUntilIncluding(&TrailingLines, CTokenType_Newline);
  TrailingLines.Tokens.Start = TrailingLines.Tokens.At;
  TruncateAtNextLineEnd(&TrailingLines, LinesOfContext);
  DumpEntireParser(&TrailingLines);

  Log("------------------------------------------------------------------------------------\n");
  Parser->Valid = False;
  return;
}

bonsai_function void
ParseError(parser* Parser, counted_string ErrorString)
{
  ParseError(Parser, Parser->Tokens.At, CToken(CTokenType_Unknown), ErrorString);
  return;
}

bonsai_function void
ParseError(parser* Parser, c_token* ErrorToken, counted_string ErrorString)
{
  ParseError(Parser, ErrorToken, CToken(CTokenType_Unknown), ErrorString);
  return;
}



/*****************************                 *******************************/
/*****************************  Token Control  *******************************/
/*****************************                 *******************************/



#define DEBUG_CHECK_FOR_BREAK_HERE(Result) \
  ( (Result).Type == CTokenType_Identifier && StringsMatch(CSz("break_here"), (Result).Value) )

bonsai_function c_token*
PeekTokenRawPointer(parser* Parser, u32 Lookahead)
{
  c_token* Result = {};
  u32 TokensRemaining = (u32)Remaining(&Parser->Tokens);
  if (TokensRemaining > Lookahead)
  {
    Result = Parser->Tokens.At+Lookahead;
  }
  else
  {
    if (Parser->Next)
    {
      Assert( Parser->Next->Tokens.At == Parser->Next->Tokens.Start);
      Result = PeekTokenRawPointer(Parser->Next, Lookahead - TokensRemaining);
    }
  }

#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    RuntimeBreak();
    Result = PeekTokenRawPointer(Parser, Lookahead + 1);
  }
#endif

#if BONSAI_INTERNAL
  if (Result && Result->Type == CTokenType_Identifier) { Assert(Result->Value.Start); Assert(Result->Value.Count);  }
  if (Result) { Assert(Result->Type); }
#endif

  return Result;
}

bonsai_function c_token
PeekTokenRaw(parser* Parser, u32 Lookahead)
{
  c_token* Pointer = PeekTokenRawPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

bonsai_function u32
OffsetOfNext(parser* Parser, u32 Offset, c_token_type Close)
{
  c_token* Next = PeekTokenRawPointer(Parser, Offset);

  while (Next && Next->Type != Close)
  {
    ++Offset;
    Next = PeekTokenRawPointer(Parser, Offset);
  }

  return Offset;
}

bonsai_function c_token*
PeekTokenPointer(parser* Parser, u32 Lookahead)
{
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;

  c_token* Result = PeekTokenRawPointer(Parser, LocalLookahead);
  while (Result)
  {
    if ( Result->Type == CTokenType_CommentSingleLine)
    {
      /* TODO(Jesse, id: 213, tags: bug, parsing, needs_tests): There is a degenerate case here, what if the file ends without a newline?
       * While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
       */
      LocalLookahead = OffsetOfNext(Parser, LocalLookahead, CTokenType_Newline);
    }
    else if ( Result->Type == CTokenType_CommentMultiLineStart)
    {
      /* TODO(Jesse, id: 214, tags: bug, parsing, needs_tests): There is a degenerate case here, what if the file ends with a malformed comment?
       * While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
       */
      LocalLookahead = OffsetOfNext(Parser, LocalLookahead, CTokenType_CommentMultiLineEnd);
    }
    else if (IsWhitespace(Result->Type))
    {
    }
    else if (Result->Type == CT_NameQualifier)
    {
      ++LocalLookahead; // Skip the scope resolution operator token -> '::'
    }
    else
    {
      if (TokenHits++ == Lookahead)
      {
        break;
      }
    }

    ++LocalLookahead;
    Result = PeekTokenRawPointer(Parser, LocalLookahead);
  }

  if (Result && (IsWhitespace(*Result) || IsComment(*Result))) { Result = 0; } // Fires if the stream ends with whitespace/comment

#if 0
#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    Result = PeekTokenPointer(Parser, Lookahead+1);
  }

  if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif
#endif

  return Result;
}

bonsai_function c_token
PeekToken(parser* Parser, u32 Lookahead)
{
  c_token* Pointer = PeekTokenPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

bonsai_function c_token *
PopTokenRawPointer(parser* Parser)
{
  c_token *Result = PeekTokenRawPointer(Parser);
  if (Result)
  {
    AdvanceTo(Parser, Result);
    AdvanceParser(Parser);
  }

  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    RuntimeBreak();
    AdvanceParser(Parser);
    Result = PopTokenRawPointer(Parser);
  }

#if BONSAI_INTERNAL
  if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif

  return Result;
}

bonsai_function c_token
PopTokenRaw(parser* Parser)
{
  c_token Result = PeekTokenRaw(Parser);
  AdvanceParser(Parser);

  if (DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    if (Remaining(&Parser->Tokens)) { AdvanceParser(Parser); }
    Result = PopTokenRaw(Parser);
  }

#if BONSAI_INTERNAL
  Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif

  return Result;
}

bonsai_function b32
RawTokensRemain(parser *Parser, u32 Count)
{
  b32 Result = PeekTokenRawPointer(Parser, Count) != 0;
  return Result;
}

bonsai_function b32
TokensRemain(parser *Parser, u32 Count)
{
  b32 Result = PeekTokenPointer(Parser, Count) != 0;
  return Result;
}

bonsai_function c_token
PopToken(parser* Parser)
{
  c_token Result = {};

  c_token* Peek = PeekTokenPointer(Parser);
  if (Peek)
  {
    AdvanceTo(Parser, Peek);
    AdvanceParser(Parser);
    Result = *Peek;
  }
  else
  {
    AdvanceTo(Parser, Parser->Tokens.End);
  }

#if BONSAI_INTERNAL
  if (DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    if (Remaining(&Parser->Tokens)) { AdvanceParser(Parser); }
    Result = PopToken(Parser);
  }

  Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif

  return Result;
}

bonsai_function b32
OptionalTokenRaw(parser* Parser, c_token_type Type)
{
  b32 Result = (PeekTokenRaw(Parser).Type == Type);
  if (Result) { PopTokenRaw(Parser); }
  return Result;
}

bonsai_function b32
OptionalToken(parser* Parser, c_token T)
{
  b32 Result = (PeekToken(Parser) == T);
  if (Result) { PopToken(Parser); }
  return Result;
}

bonsai_function b32
OptionalToken(parser* Parser, c_token_type Type)
{
  b32 Result = (PeekToken(Parser).Type == Type);
  if (Result) { PopToken(Parser); }
  return Result;
}

bonsai_function c_token
RequireToken(parser* Parser, c_token ExpectedToken)
{
  c_token* PeekedToken = PeekTokenPointer(Parser);

  c_token Result = {};
  if (PeekedToken) Result = *PeekedToken;

  // TODO(Jesse, id: 348, tags: immediate, id_347) : This should go into an AreEqual bonsai_function I think..
  if ( Result.Type != ExpectedToken.Type ||
       (ExpectedToken.Value.Count > 0 && !StringsMatch(ExpectedToken.Value, Result.Value) ))
  {
    if (PeekedToken)
    {
      ParseError(Parser, PeekedToken, ExpectedToken, CS("Require Token Failed"));
    }
    else
    {
      Assert(!Parser->Next);
      if (Parser->Tokens.At-1 >= Parser->Tokens.Start)
      {
        ParseError(Parser, Parser->Tokens.At-1, ExpectedToken, FormatCountedString(TranArena, CSz("Stream ended unexpectedly in file : %S"), Parser->Filename));
      }
      else
      {
        ParseError(Parser, Parser->Tokens.At, ExpectedToken, FormatCountedString(TranArena, CSz("Stream ended unexpectedly in file : %S"), Parser->Filename));
      }
    }

    Parser->Valid = False;
    RuntimeBreak();
  }
  else
  {
    Result = PopToken(Parser);
  }

#if BONSAI_INTERNAL
  Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif

  return Result;
}

bonsai_function c_token
RequireToken(parser* Parser, c_token_type ExpectedType)
{
  c_token Result = RequireToken(Parser, CToken(ExpectedType));
  return Result;
}

bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token Expected )
{
  c_token Result = PeekTokenRaw(Parser);
  if (Result == Expected)  // TODO(Jesse id: 349, tags: id_347) : Change to a bonsai_function call
  {
    PopTokenRaw(Parser);
  }
  else
  {
    Result = {};
  }
  return Result;
}

bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token_type Expected )
{
  c_token Result = PeekTokenRaw(Parser);
  if (Result.Type == Expected)
  {
    PopTokenRaw(Parser);
  }
  else
  {
    Result = {};
  }

  return Result;
}

bonsai_function b32
TokenIsOperator(c_token_type T)
{
  b32 Result = False;
  switch (T)
  {
    case CTokenType_Ampersand:
    case CTokenType_Tilde:
    case CTokenType_Hat:
    case CTokenType_LT:
    case CTokenType_GT:
    case CTokenType_Bang:
    case CTokenType_LeftShift:
    case CTokenType_RightShift:
    case CTokenType_LessEqual:
    case CTokenType_GreaterEqual:
    case CTokenType_AreEqual:
    case CTokenType_NotEqual:
    case CTokenType_PlusEquals:
    case CTokenType_MinusEquals:
    case CTokenType_TimesEquals:
    case CTokenType_DivEquals:
    case CTokenType_ModEquals:
    case CTokenType_AndEquals:
    case CTokenType_OrEquals:
    case CTokenType_XorEquals:
    case CTokenType_Increment:
    case CTokenType_Decrement:
    case CTokenType_LogicalAnd:
    case CTokenType_LogicalOr:
    case CTokenType_Percent:
    case CTokenType_Pipe:
    case CTokenType_Plus:
    case CTokenType_Minus:
    case CTokenType_FSlash:
    case CTokenType_Question:
    case CTokenType_Star:
    case CTokenType_OpenBracket:
    case CTokenType_Arrow:
    case CTokenType_Dot:
    {
      Result = True;
    } break;

    default: {} break;
  }

  return Result;
}

bonsai_function b32
NextTokenIsOperator(parser* Parser)
{
  c_token T = PeekToken(Parser);
  b32 Result = TokenIsOperator(T.Type);
  return Result;
}

bonsai_function counted_string
RequireOperatorToken(parser* Parser)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  c_token T = PeekToken(Parser);
  switch (T.Type)
  {
    case CTokenType_Ampersand:
    case CTokenType_Tilde:
    case CTokenType_Hat:
    case CTokenType_LT:
    case CTokenType_GT:
    case CTokenType_Bang:
    case CTokenType_LeftShift:
    case CTokenType_RightShift:
    case CTokenType_LessEqual:
    case CTokenType_GreaterEqual:
    case CTokenType_AreEqual:
    case CTokenType_NotEqual:
    case CTokenType_PlusEquals:
    case CTokenType_MinusEquals:
    case CTokenType_TimesEquals:
    case CTokenType_DivEquals:
    case CTokenType_ModEquals:
    case CTokenType_AndEquals:
    case CTokenType_OrEquals:
    case CTokenType_XorEquals:
    case CTokenType_Increment:
    case CTokenType_Decrement:
    case CTokenType_LogicalAnd:
    case CTokenType_LogicalOr:
    case CTokenType_Percent:
    case CTokenType_Pipe:
    case CTokenType_Plus:
    case CTokenType_Minus:
    case CTokenType_FSlash:
    case CTokenType_Question:
    case CTokenType_Star:
    case CTokenType_Arrow:
    case CTokenType_Dot:
    {
      RequireToken(Parser, T.Type);
    } break;

    case CTokenType_OpenBracket:
    {
      RequireToken(Parser, T.Type);
      RequireToken(Parser, CTokenType_CloseBracket);
    } break;

    default: { ParseError(Parser, CSz("Expected operator.")); } break;
  }

  counted_string Result = FinalizeStringFromParser(&Builder);
  return Result;
}








bonsai_function parser
ParserFromBuffer(c_token_buffer *TokenBuf, umm AtOffset = 0)
{
  Assert(AtOffset < TokenBuf->Count);
  parser Result = {
    .Tokens = {
      .Start = TokenBuf->Start,
      .At = TokenBuf->Start + AtOffset,
      .End = TokenBuf->Start + TokenBuf->Count,
    }
  };

  return Result;
}

bonsai_function void
CopyCursorIntoCursor(c_token_cursor *Src, c_token_cursor *Dest)
{
  u32 SrcElements = (u32)TotalElements(Src);
  u32 DestElements = (u32)Remaining(Dest);
  if ( SrcElements <= DestElements )
  {
    for ( u32 TokenIndex = 0;
          TokenIndex < SrcElements;
          ++TokenIndex)
    {
      Push(Src->Start[TokenIndex], Dest);
    }
  }
  else
  {
    Error("Dest c_token_cursor was too full to hold entire source buffer!" );
  }
}

bonsai_function void
CopyMacroArgIntoCursor(c_token_buffer *Src, c_token_cursor *Dest, memory_arena *Memory)
{
  parser TempParser = ParserFromBuffer(Src);
  while ( Remaining(&TempParser) )
  {
    c_token ArgT = PeekTokenRaw(&TempParser);
    if (ArgT.Type == CT_MacroLiteral)
    {
      parser *Expanded = ExpandMacro(&TempParser, ArgT.Macro, Memory);
      CopyCursorIntoCursor(&Expanded->Tokens, Dest);
    }
    else
    {
      PopTokenRaw(&TempParser);
      Push(ArgT, Dest);
    }
  }
}

bonsai_function void
CopyBufferIntoCursor(c_token_buffer *Src, c_token_cursor *Dest)
{
  if ( Src->Count <= Remaining(Dest) )
  {
    for ( u32 TokenIndex = 0;
          TokenIndex < Src->Count;
          ++TokenIndex)
    {
      Push(Src->Start[TokenIndex], Dest);
    }
  }
  else
  {
    Error("Dest c_token_cursor was too full to hold entire source buffer!" );
  }
}

bonsai_function parser*
ExpandMacro(parser *Parser, macro_def *Macro, memory_arena *Memory)
{
  parser *Result = AllocateParserPtr(Macro->Body.Filename, (u32)Kilobytes(1), 0, Memory);

  RequireToken(Parser, CToken(CT_MacroLiteral, Macro->Name));

  switch (Macro->Type)
  {
    case type_macro_keyword:
    {
      parser BodyParser_ = Macro->Body;
      parser *BodyParser = &BodyParser_;
      while (Remaining(&BodyParser->Tokens))
      {
        c_token *T = PeekTokenRawPointer(BodyParser);
        if (T->Type == CT_MacroLiteral)
        {
          parser *Expanded = ExpandMacro(BodyParser, T->Macro, Memory);
          CopyCursorIntoCursor(&Expanded->Tokens, &Result->Tokens);
        }
        else
        {
          PopTokenRaw(BodyParser);
          Push(*T, &Result->Tokens);
        }

      }

    } break;

    case type_macro_function:
    {

      c_token *Start = PeekTokenPointer(Parser);
      parser InstanceArgs_ =
      {
        .Valid = 1,
        .Tokens = { .Start = Start, .At = Start, .End = Start, },
      };
      parser *InstanceArgs = &InstanceArgs_;

      // TODO(Jesse id: 343): I think this is busted on invalid program input.
      EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      InstanceArgs->Tokens.End = Parser->Tokens.At;

      TrimFirstToken(InstanceArgs, CTokenType_OpenParen);
      TrimLastToken(InstanceArgs, CTokenType_CloseParen);

      c_token_buffer_buffer ArgValues = {};
      if (Macro->NamedArguments.Count)
      {
        ArgValues = CTokenBufferBuffer(Macro->NamedArguments.Count, Memory);
        for ( u32 ArgIndex = 0;
              ArgIndex < ArgValues.Count;
              ++ArgIndex)
        {
          ParseMacroArgument(InstanceArgs, ArgValues.Start+ArgIndex);
        }
      }

      c_token_buffer_stream VarArgs = {};
      if (Macro->Variadic)
      {
        while (Remaining(InstanceArgs))
        {
          c_token_buffer *Arg = Push(&VarArgs, {}, Memory);
          ParseMacroArgument(InstanceArgs, Arg);
        }
      }

      Assert(Remaining(InstanceArgs) == 0);

      parser MacroBody_ = Macro->Body;
      parser *MacroBody = &MacroBody_;
      TrimLeadingWhitespace(MacroBody);
      Rewind(MacroBody);
      while (Remaining(MacroBody))
      {
        c_token T = PeekTokenRaw(MacroBody);
        switch (T.Type)
        {
          case CT_Preprocessor__VA_ARGS__:
          {
            RequireToken(MacroBody, T);
            ITERATE_OVER(&VarArgs)
            {
              c_token_buffer* Arg = GET_ELEMENT(Iter);
              CopyMacroArgIntoCursor(Arg, &Result->Tokens, Memory);
            }
          } break;

          case CTokenType_Identifier:
          {
            RequireToken(MacroBody, T);
            u32 ArgIndex = (u32)IndexOf(&Macro->NamedArguments, T.Value);
            if (ArgIndex < Macro->NamedArguments.Count)
            {
              c_token_buffer *ArgBuffer = ArgValues.Start + ArgIndex;
              CopyMacroArgIntoCursor(ArgBuffer, &Result->Tokens, Memory);
            }
            else
            {
              Push(T, &Result->Tokens);
            }
          } break;

          case CT_MacroLiteral:
          {
            parser *Expanded = ExpandMacro(MacroBody, T.Macro, Memory);
            CopyCursorIntoCursor(&Expanded->Tokens, &Result->Tokens);
          } break;

          default:
          {
            PopTokenRaw(MacroBody);
            Push(T, &Result->Tokens);
          } break;
        }

      }
      Assert(Remaining(MacroBody) == 0);

    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Error expanding macro!"));
  }

  TruncateToCurrentSize(&Result->Tokens);
  return Result;
}



/*************************                         ***************************/
/*************************  Comments & Whitespace  ***************************/
/*************************                         ***************************/


bonsai_function counted_string
EatComment(parser* Parser, c_token_type CommentT)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  switch (CommentT)
  {
    case CTokenType_CommentSingleLine:
    {
      EatUntilIncluding(Parser, CTokenType_Newline);
    } break;

    case CTokenType_CommentMultiLineStart:
    {
      EatUntilIncluding(Parser, CTokenType_CommentMultiLineEnd);
    } break;

    default:
    {
      Warn("Called EatComment on something that wasn't a comment!");
    } break;
  }

  counted_string Result = FinalizeStringFromParser(&Builder);
  return Result;
}

bonsai_function counted_string
EatComment(parser* Parser)
{
  counted_string Result = EatComment(Parser, PeekTokenRaw(Parser).Type);
  return Result;
}

bonsai_function void
EatWhitespaceAndComments(parser* Parser)
{
  c_token* T = PeekTokenPointer(Parser);
  if (!T) T = Parser->Tokens.End;
  AdvanceTo(Parser, T);
}

bonsai_function void
EatWhitespace(parser* Parser)
{
  while (Remaining(&Parser->Tokens) &&
         IsWhitespace(PeekTokenRaw(Parser).Type))
  {
    PopTokenRaw(Parser);
  }

  return;
}

bonsai_function void
TrimLeadingWhitespace(parser* Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);
  while (T && IsWhitespace(T->Type))
  {
    PopTokenRawPointer(Parser);
    T = PeekTokenRawPointer(Parser);
  }
}

bonsai_function void
TrimTrailingWhitespace(parser* Parser)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken > Parser->Tokens.Start)
  {
    if ( CurrentToken->Type == CTokenType_Space ||
         CurrentToken->Type == CTokenType_Tab )
    {
      Parser->Tokens.End = CurrentToken;
      if (Parser->Tokens.At > CurrentToken)
      {
        Parser->Tokens.At = CurrentToken;
      }
    }
    else
    {
      break;
    }

    --CurrentToken;
  }
}






bonsai_function counted_string
PopHex(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    if (IsHex(*SourceFileStream->At))
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

bonsai_function counted_string
PopNumeric(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    if (IsNumeric(*SourceFileStream->At))
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

bonsai_function counted_string
PopIdentifier(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    c_token T = PeekToken(SourceFileStream);
    if (T.Type == CTokenType_Unknown)
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

bonsai_function r64
ToFractional(counted_string S)
{
  r64 Result = 0;
  if (S.Count) { Result = (r64)ToU64(S) * Exp(10.0, -SafeTruncateToS32(S.Count)); }
  Assert(Result < 1.0f);
  return Result;
}

bonsai_function u64
BinaryStringToU64(counted_string Bin)
{
  u64 Result = 0;
  for (u64 CharIndex = 0;
      CharIndex < Bin.Count;
      ++CharIndex)
  {
    u64 Digit = (u64)(Bin.Start[CharIndex] - '0');
    Assert(Digit < 2);
    Result |= Digit << ((Bin.Count - CharIndex - 1L));
  }

  return Result;
}

bonsai_function u64
HexToU64(char C)
{
  u64 Result = 0;

  if (C >= '0' && C <= '9')
  {
    Result = ToU64(C);
  }
  else if (C >= 'a' && C <= 'f')
  {
    Result = (u64)(10 + C - 'a');
  }
  else if (C >= 'A' && C <= 'F')
  {
    Result = (u64)(10 + C - 'A');
  }
  else
  {
    InvalidCodePath();
  }

  Assert(Result < 16);
  return Result;
}

bonsai_function u64
HexStringToU64(counted_string S)
{
  u64 Result = 0;
  for (u64 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    u64 Digit = HexToU64(S.Start[CharIndex]);
    Result |= Digit << ((S.Count - CharIndex - 1L) * 4);
  }

  return Result;
}

bonsai_function c_token
ParseExponentAndSuffixes(ansi_stream *Code, r64 OriginalValue)
{
  c_token Result = {};

  r64 FinalValue = OriginalValue;

  if ( Remaining(Code) &&
       (*Code->At == 'e' || *Code->At == 'E') )
  {
    Advance(Code);

    s32 Exponent = 1;

    char ExpSign = *Code->At;
    if (ExpSign == '-')
    {
      Advance(Code);
      Exponent = -1;
    }
    else if (ExpSign == '+')
    {
      Advance(Code);
      Exponent = 1;
    }

    Exponent = Exponent * SafeTruncateToS32(ToU64(PopNumeric(Code)));

    FinalValue = Exp(FinalValue, Exponent);
  }

  if (Remaining(Code))
  {
    char Suffix = *Code->At;
    switch (Suffix)
    {
      case 'f':
      case 'F':
      {
        Result.Type = CTokenType_FloatLiteral;
        Advance(Code);
      } break;

      case 'l':
      case 'L':
      {
        // Apparently `double` and `long double` are the same storage size (8 bytes), at least in MSVC:
        // https://docs.microsoft.com/en-us/cpp/c-language/storage-of-basic-types?view=vs-2019
        Result.Type = CTokenType_DoubleLiteral;
        Advance(Code);
      } break;

      default:
      {
        Result.Type = CTokenType_DoubleLiteral;
      } break;
    }
  }

  Result.FloatValue = FinalValue;
  return Result;
}

bonsai_function void
ParseIntegerSuffixes(ansi_stream *Code)
{
  b32 Done = False;
  while (!Done)
  {
    char Suffix = *Code->At;
    switch (Suffix)
    {
      // For now, we just eat suffixes
      // TODO(Jesse id: 278): Disallow invalid suffixes lul/LUL .. LUU .. ULLLL etc..
      // Maybe use a state machine / transition table
      case 'u':
      case 'U':
      case 'l':
      case 'L':
      {
        Advance(Code);
      } break;

      default:
      {
        Done = True;
      } break;
    }
  }
}

bonsai_function c_token
ParseNumericToken(ansi_stream *Code)
{
  const char *Start = Code->At;

  counted_string IntegralString = { .Start = Code->At };
  while (Remaining(Code) && IsNumeric(*Code->At)) { Advance(Code); }
  IntegralString.Count = (umm)(Code->At - IntegralString.Start);

  u64 IntegralPortion = ToU64(IntegralString);

  c_token Result = {
    .Type = CTokenType_IntLiteral,
    .UnsignedValue = IntegralPortion,
  };

  if ( IntegralPortion == 0 &&
       (*Code->At == 'x'|| *Code->At == 'X') )
  {
    Advance(Code);
    Result.UnsignedValue = HexStringToU64(PopHex(Code));
    ParseIntegerSuffixes(Code);
  }
  else if (IntegralPortion == 0 && *Code->At == 'b')
  {
    Advance(Code);
    Result.UnsignedValue = BinaryStringToU64(PopNumeric(Code));
  }
  else if ( *Code->At == '.' )
  {
    // Float literal
    Advance(Code);
    r64 Fractional = ToFractional(PopNumeric(Code));
    Result = ParseExponentAndSuffixes(Code, (r64)IntegralPortion + Fractional);
  }
  else if ( *Code->At == 'e' || *Code->At == 'E'  )
  {
    // Float literal
    Result = ParseExponentAndSuffixes(Code, (r64)IntegralPortion);
  }
  else
  {
    //
    // Int literal
    //

    ParseIntegerSuffixes(Code);
  }

  Result.Value.Start = Start;
  Result.Value.Count = (umm)(Code->At - Start);
  return Result;
}

bonsai_function u32
EatSpacesTabsAndEscapedNewlines(ansi_stream *Code)
{
  u32 LinesEaten = 0;
  while ( PeekToken(Code).Type == CTokenType_Space ||
          PeekToken(Code).Type == CTokenType_Tab   ||
          PeekToken(Code).Type == CTokenType_FSlash )
  {
    if ( PeekToken(Code).Type == CTokenType_FSlash )
    {
      if ( PeekToken(Code, 1).Type == CTokenType_Newline )
      {
        ++LinesEaten;
        Advance(Code);
        Advance(Code);
      }
      else
      {
        break;
      }
    }
    else
    {
      Advance(Code);
    }
  }
  return LinesEaten;
}

bonsai_function macro_def*
GetByName(macro_def_stream* Stream, counted_string Name)
{
  macro_def* Result = {};
  ITERATE_OVER(Stream)
  {
    macro_def* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      Rewind(&Result->Body);
      break;
    }
  }

  return Result;
}

bonsai_function u32
CountTokensBeforeNext(parser *Parser, c_token_type T1, c_token_type T2)
{
  Assert(T1 != T2);
  u32 Result = 0;

  c_token *ResetToStart = Parser->Tokens.At;

  for (;;)
  {
    c_token AtT = PopTokenRaw(Parser);
    if (AtT.Type == T1)
    {
      ++Result;
    }

    if (AtT.Type == T2)
    {
      break;
    }
  }

  Parser->Tokens.At = ResetToStart;

  return Result;
}









bonsai_function parser *
SplitAndInsertParserInto(parser *ParserToSplit, c_token* SplitPoint, parser *ParserToInsert, c_token* SecondHalfStart, memory_arena *Memory)
{
  // TODO(Jesse id: 363): A bunch of this crap can get taken out of here..  I added it
  // during development to aid in debugging.

  Assert(SplitPoint >= ParserToSplit->Tokens.Start);
  Assert(SplitPoint < ParserToSplit->Tokens.End);

  Assert(SecondHalfStart >= ParserToSplit->Tokens.Start);
  Assert(SecondHalfStart < ParserToSplit->Tokens.End);


  parser *SecondHalfOfSplit = AllocateProtection(parser, Memory, 1, False);

  *SecondHalfOfSplit = *ParserToSplit;

  ParserToSplit->Tokens.At = SplitPoint;
  ParserToSplit->Tokens.End = SplitPoint;

  Assert(ParserToSplit->Tokens.At == ParserToSplit->Tokens.End);
  Assert(ParserToSplit->Tokens.At == SplitPoint);
  Assert(ParserToSplit->Tokens.End == SplitPoint);

  SecondHalfOfSplit->Tokens.Start = SecondHalfStart;
  SecondHalfOfSplit->Tokens.At = SecondHalfStart;

  if (ParserToInsert)
  {
    Rewind(ParserToInsert);
    ParserToSplit->Next = ParserToInsert;
    ParserToInsert->Next = SecondHalfOfSplit;
    Assert(ParserToInsert->Tokens.At    == ParserToInsert->Tokens.Start);
  }
  else
  {
    ParserToSplit->Next = SecondHalfOfSplit;
  }

  Assert(SecondHalfOfSplit->Tokens.At == SecondHalfOfSplit->Tokens.Start);

  Rewind(ParserToSplit);

  return SecondHalfOfSplit;
}

bonsai_function parser *
TokenizeAnsiStream(ansi_stream Code, memory_arena* Memory, b32 IgnoreQuotes, parse_context *Ctx)
{
  if (!Code.Start)
  {
    Error("Input AnsiStream for %S is null.", Code);
    return 0;
  }

  parser *Result = Ctx ? Push( &Ctx->AllParsers,
                               AllocateParser(Code.Filename, (u32)Megabytes(1), (u32)Megabytes(1), Memory),
                               Memory)
                       : AllocateParserPtr(Code.Filename, (u32)Megabytes(1), (u32)Megabytes(1), Memory);


  macro_def_stream MacrosThatNeedToBeParsedOut = {};

  b32 ParsingSingleLineComment = False;
  b32 ParsingMultiLineComment = False;

  u32 LineNumber = 0;
  c_token *LastTokenPushed = 0;
  while(Remaining(&Code))
  {
    const c_token FirstT = PeekToken(&Code);
    c_token PushT = { .Type = FirstT.Type, .Value = CS(Code.At, 1) };

    switch (FirstT.Type)
    {
      case CTokenType_FSlash:
      {
        const c_token SecondT = PeekToken(&Code, 1);

        switch (SecondT.Type)
        {
          case CTokenType_FSlash:
          {
            ParsingSingleLineComment = True;
            PushT.Type = CTokenType_CommentSingleLine;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          case CTokenType_Star:
          {
            ParsingMultiLineComment = True;
            PushT.Type = CTokenType_CommentMultiLineStart;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          case CTokenType_Equals:
          {
            PushT.Type = CTokenType_DivEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          default:
          {
            Advance(&Code);
          } break;
        }
      } break;

      case CTokenType_LT:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_LessEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_LT)
        {
          PushT.Type = CTokenType_LeftShift;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_GT:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_GreaterEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_GT)
        {
          PushT.Type = CTokenType_RightShift;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_Equals:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_AreEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_Dot:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Dot &&
            PeekToken(&Code, 2).Type == CTokenType_Dot)
        {
          PushT.Type = CTokenType_Ellipsis;
          PushT.Value = CS(Code.At, 3);
          Advance(&Code);
          Advance(&Code);
          Advance(&Code);
        }
        else if (Remaining(&Code) > 1 && IsNumeric(*(Code.At+1)))
        {
          PushT = ParseNumericToken(&Code);
        }
        else
        {
          Advance(&Code);
        }

      } break;

      case CTokenType_Bang:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_NotEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;


      case CTokenType_Hat:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_XorEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Pipe:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Pipe)
        {
          PushT.Type = CTokenType_LogicalOr;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_OrEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Ampersand:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Ampersand)
        {
          PushT.Type = CTokenType_LogicalAnd;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_AndEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Percent:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_ModEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Minus:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Minus)
        {
          PushT.Type = CTokenType_Decrement;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_MinusEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_GT)
        {
          PushT.Type = CTokenType_Arrow;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Plus:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_PlusEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Plus)
        {
          PushT.Type = CTokenType_Increment;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Star:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_TimesEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_FSlash)
        {
          ParsingMultiLineComment = False;
          PushT.Type = CTokenType_CommentMultiLineEnd;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_Star;
          PushT.Value = CS(Code.At, 1);
        }
        Advance(&Code);
      } break;

      case CTokenType_SingleQuote:
      {
        if (IgnoreQuotes || ParsingSingleLineComment || ParsingMultiLineComment)
        {
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_CharLiteral;
          PushT.Value = PopQuotedCharLiteral(&Code, True);

          if(PushT.Value.Count > 2)
          {
            Assert(PushT.Value.Start[0] == '\'');
            Assert(PushT.Value.Start[PushT.Value.Count-1] == '\'');

            for ( u32 CharIndex = 1;
                  CharIndex < PushT.Value.Count-1;
                  ++CharIndex )
            {
              PushT.UnsignedValue += (umm)PushT.Value.Start[CharIndex];
            }
          }
          else
          {
            Error("Quoted char literal with length %u .. ???", (u32)PushT.Value.Count);
          }
        }
      } break;

      case CTokenType_DoubleQuote:
      {
        if (IgnoreQuotes || ParsingSingleLineComment || ParsingMultiLineComment)
        {
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_StringLiteral;
          PushT.Value = PopQuotedString(&Code, True);
        }
      } break;

      case CTokenType_BSlash:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Newline)
        {
          PushT.Type = CTokenType_EscapedNewline;
          PushT.Value.Count = 2;
          Advance(&Code);
          ++LineNumber;
        }
        Advance(&Code);
      } break;

      case CTokenType_Newline:
      {
        ++LineNumber;
        ParsingSingleLineComment = False;
        Advance(&Code);
      } break;

      case CTokenType_Colon:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Colon)
        {
          Advance(&Code);
          Advance(&Code);
          PushT.Type = CT_ScopeResolutionOperator;
          PushT.Value.Count = 2;

          if (LastTokenPushed && LastTokenPushed->Type == CTokenType_Identifier)
          {
            LastTokenPushed->Type = CT_NameQualifier;
            PushT.QualifierName = LastTokenPushed;
          }
        }
        else
        {
          Advance(&Code);
        }

      } break;

      case CTokenType_Hash:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Hash)
        {
          Advance(&Code);
          Advance(&Code);
          PushT.Type = CT_PreprocessorPaste;
          PushT.Value.Count = 2;
        }
        else
        {
          if (Ctx)
          {
            const char* HashCharacter = Code.At;
            Advance(&Code);
            const char* FirstAfterHash = Code.At;

            LineNumber += EatSpacesTabsAndEscapedNewlines(&Code);

            counted_string TempValue = PopIdentifier(&Code);

            if ( StringsMatch(TempValue, CSz("define")) )
            {
              const char* FirstAfterDefineKeyword = Code.At;

              LineNumber += EatSpacesTabsAndEscapedNewlines(&Code);

              PushT.Type = CT_PreprocessorDefine;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);

              macro_def Macro = {
                .Name = PopIdentifier(&Code),
                .Body = {
                  .Tokens = {
                    .Start = Result->Tokens.At,
                    .At    = Result->Tokens.At,
                  },
                  .Filename = Code.Filename,
                  .LineNumber = LineNumber,
                }
              };

              Code.At = FirstAfterDefineKeyword;

              if ( !ParsingSingleLineComment &&
                   !ParsingMultiLineComment   )
              {
                Push(&MacrosThatNeedToBeParsedOut, Macro, Memory);
              }
            }
            else if ( StringsMatch(TempValue, CSz("if")) )
            {
              PushT.Type = CT_PreprocessorIf;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("else")) )
            {
              PushT.Type = CT_PreprocessorElse;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("elif")) )
            {
              PushT.Type = CT_PreprocessorElif;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("endif")) )
            {
              PushT.Type = CT_PreprocessorEndif;;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("ifndef")) )
            {
              PushT.Type = CT_PreprocessorIfNotDefined;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("ifdef")) )
            {
              PushT.Type = CT_PreprocessorIfDefined;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("undef")) )
            {
              PushT.Type = CT_PreprocessorUndef;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("include")) )
            {
              PushT.Type = CT_PreprocessorInclude;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("error")) )
            {
              PushT.Type = CT_PreprocessorError;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("warning")) )
            {
              PushT.Type = CT_PreprocessorWarning;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("pragma")) )
            {
              PushT.Type = CT_PreprocessorPragma;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else
            {
              // The token is just a regular hash .. roll back our parsing to the start.
              Code.At = FirstAfterHash;
            }
          }
          else
          {
            Advance(&Code);
          }
        }

      } break;

      case CTokenType_Unknown:
      {
        if (IsNumeric(*Code.At))
        {
          PushT = ParseNumericToken(&Code);
        }
        else
        {
          PushT.Type = CTokenType_Identifier;
          PushT.Value = PopIdentifier(&Code);

          if ( StringsMatch(PushT.Value, CSz("if")) )
          {
            PushT.Type = CTokenType_If;
          }
          else if ( StringsMatch(PushT.Value, CSz("else")) )
          {
            PushT.Type = CTokenType_Else;
          }
          else if ( StringsMatch(PushT.Value, CSz("break")) )
          {
            PushT.Type = CTokenType_Break;
          }
          else if ( StringsMatch(PushT.Value, CSz("switch")) )
          {
            PushT.Type = CTokenType_Switch;
          }
          else if ( StringsMatch(PushT.Value, CSz("case")) )
          {
            PushT.Type = CTokenType_Case;
          }
          else if ( StringsMatch(PushT.Value, CSz("default")) )
          {
            PushT.Type = CTokenType_Default;
          }
          else if ( StringsMatch(PushT.Value, CSz("for")) )
          {
            PushT.Type = CTokenType_For;
          }
          else if ( StringsMatch(PushT.Value, CSz("while")) )
          {
            PushT.Type = CTokenType_While;
          }
          else if ( StringsMatch(PushT.Value, CSz("continue")) )
          {
            PushT.Type = CTokenType_Continue;
          }
          else if ( StringsMatch(PushT.Value, CSz("return")) )
          {
            PushT.Type = CTokenType_Return;
          }
          else if ( StringsMatch(PushT.Value, CSz("thread_local")) )
          {
            PushT.Type = CTokenType_ThreadLocal;
          }
          else if ( StringsMatch(PushT.Value, CSz("const")) )
          {
            PushT.Type = CTokenType_Const;
          }
          else if ( StringsMatch(PushT.Value, CSz("static")) )
          {
            PushT.Type = CTokenType_Static;
          }
          else if ( StringsMatch(PushT.Value, CSz("__volatile__")) )
          {
            PushT.Type = CTokenType_Volatile;
          }
          else if ( StringsMatch(PushT.Value, CSz("volatile")) )
          {
            PushT.Type = CTokenType_Volatile;
          }
          else if ( StringsMatch(PushT.Value, CSz("void")) )
          {
            PushT.Type = CTokenType_Void;
          }
          else if ( StringsMatch(PushT.Value, CSz("long")) )
          {
            PushT.Type = CTokenType_Long;
          }
          else if ( StringsMatch(PushT.Value, CSz("float")) )
          {
            PushT.Type = CTokenType_Float;
          }
          else if ( StringsMatch(PushT.Value, CSz("char")) )
          {
            PushT.Type = CTokenType_Char;
          }
          else if ( StringsMatch(PushT.Value, CSz("double")) )
          {
            PushT.Type = CTokenType_Double;
          }
          else if ( StringsMatch(PushT.Value, CSz("short")) )
          {
            PushT.Type = CTokenType_Short;
          }
          else if ( StringsMatch(PushT.Value, CSz("int")) )
          {
            PushT.Type = CTokenType_Int;
          }
          else if ( StringsMatch(PushT.Value, CSz("bool")) )
          {
            PushT.Type = CTokenType_Bool;
          }
          else if ( StringsMatch(PushT.Value, CSz("__m128")) )
          {
            PushT.Type = CTokenType_M128;
          }
          else if ( StringsMatch(PushT.Value, CSz("auto")) )
          {
            PushT.Type = CTokenType_Auto;
          }
          else if ( StringsMatch(PushT.Value, CSz("signed")) )
          {
            PushT.Type = CTokenType_Signed;
          }
          else if ( StringsMatch(PushT.Value, CSz("unsigned")) )
          {
            PushT.Type = CTokenType_Unsigned;
          }
          else if ( StringsMatch(PushT.Value, CSz("struct")) )
          {
            PushT.Type = CTokenType_Struct;
          }
          else if ( StringsMatch(PushT.Value, CSz("typedef")) )
          {
            PushT.Type = CTokenType_Typedef;
          }
          else if ( StringsMatch(PushT.Value, CSz("__asm__")) )
          {
            PushT.Type = CTokenType_Asm;
          }
          else if ( StringsMatch(PushT.Value, CSz("asm")) )
          {
            PushT.Type = CTokenType_Asm;
          }
          else if ( StringsMatch(PushT.Value, CSz("meta")) )
          {
            PushT.Type = CTokenType_Meta;
          }
          else if ( StringsMatch(PushT.Value, CSz("union")) )
          {
            PushT.Type = CTokenType_Union;
          }
          else if ( StringsMatch(PushT.Value, CSz("using")) )
          {
            PushT.Type = CTokenType_Using;
          }
          else if ( StringsMatch(PushT.Value, CSz("enum")) )
          {
            PushT.Type = CTokenType_Enum;
          }
          else if ( StringsMatch(PushT.Value, CSz("goto")) )
          {
            PushT.Type = CTokenType_Goto;
          }
          else if ( StringsMatch(PushT.Value, CSz("template")) )
          {
            PushT.Type = CTokenType_TemplateKeyword;
          }
          else if ( StringsMatch(PushT.Value, CSz("inline")) )
          {
            PushT.Type = CTokenType_Inline;
          }
          else if ( StringsMatch(PushT.Value, CSz("operator")) )
          {
            PushT.Type = CTokenType_OperatorKeyword;
          }
          else if ( StringsMatch(PushT.Value, CSz("_Pragma")) )
          {
            PushT.Type = CT_Pragma;
          }
          else if ( StringsMatch(PushT.Value, CSz("__pragma")) )
          {
            PushT.Type = CT_Pragma;
          }
          else if ( StringsMatch(PushT.Value, CSz("extern")) )
          {
            PushT.Type = CTokenType_Extern;
          }
          else if ( StringsMatch(PushT.Value, CSz("__VA_ARGS__")) )
          {
            PushT.Type = CT_Preprocessor__VA_ARGS__;
          }
          else
          {

            if ( !ParsingSingleLineComment &&
                 !ParsingMultiLineComment   )
            {
              if (Ctx)
              {
                macro_def *Macro1 = GetByName(&Ctx->Datatypes.Macros, PushT.Value);
                macro_def *Macro2 = GetByName(&MacrosThatNeedToBeParsedOut, PushT.Value);

                if (Macro1)
                {
                  PushT.Type = CT_MacroLiteral;
                  PushT.Macro = Macro1;
                }
                else if (Macro2)
                {
                  PushT.Type = CT_MacroLiteral;
                  PushT.Macro = Macro2;
                }
              }
            }

            if (LastTokenPushed && LastTokenPushed->Type == CT_ScopeResolutionOperator)
            {
              PushT.QualifierName = LastTokenPushed->QualifierName;
            }
          }
        }

      } break;

      default: { Advance(&Code); } break;
    }

    Assert(PushT.Type);
    LastTokenPushed = Push(PushT, &Result->Tokens);

    continue;
  }

  TruncateToCurrentSize(&Result->Tokens);
  Rewind(Result);
  Result->Valid = True;

  if (Ctx)
  {
    ITERATE_OVER(&MacrosThatNeedToBeParsedOut)
    {
      macro_def *Macro = GET_ELEMENT(Iter);
      if (StringsMatch(Macro->Name, CSz("meta"))) { continue; }

      parser *MacroBody = &Macro->Body;
      MacroBody->Tokens.End = Result->Tokens.End;

      RequireToken(MacroBody, CT_PreprocessorDefine);
      RequireToken(MacroBody, CToken(CT_MacroLiteral, Macro->Name));

      if (OptionalTokenRaw(MacroBody, CTokenType_OpenParen))
      {
        Macro->Type = type_macro_function;
        if (OptionalToken(MacroBody, CTokenType_CloseParen))
        {
          // No arguments
        }
        else
        {
          u32 ArgCount = 1 + CountTokensBeforeNext(MacroBody, CTokenType_Comma, CTokenType_CloseParen);
          Macro->NamedArguments = CountedStringBuffer(ArgCount, Memory);

          for ( u32 ArgIndex = 0;
                ArgIndex < ArgCount-1;
                ++ArgIndex)
          {
            counted_string ArgName = RequireToken(MacroBody, CTokenType_Identifier).Value;
            Macro->NamedArguments.Start[ArgIndex] = ArgName;
            RequireToken(MacroBody, CTokenType_Comma);
          }

          if (PeekToken(MacroBody).Type == CTokenType_Identifier)
          {
            counted_string ArgName = RequireToken(MacroBody, CTokenType_Identifier).Value;
            Macro->NamedArguments.Start[Macro->NamedArguments.Count-1] = ArgName;
          }

          if (OptionalToken(MacroBody, CTokenType_Ellipsis))
          {
            Macro->Variadic = True;
            Macro->NamedArguments.Count -= 1;
          }

          RequireToken(MacroBody, CTokenType_CloseParen);
        }
      }
      else
      {
        // TrimLeadingWhitespace(MacroBody);

        Macro->Type = type_macro_keyword;
      }

      MacroBody->Tokens.Start = MacroBody->Tokens.At;

      b32 Done = False;
      while (!Done && TokensRemain(MacroBody))
      {
        switch (PeekTokenRaw(MacroBody).Type)
        {
          case CTokenType_Newline:
          {
            Done = True;
          } break;

          default: { PopTokenRaw(MacroBody); } break;
        }
      }

      TruncateToCurrentSize(&MacroBody->Tokens);

      Assert(MacroBody->Tokens.End);
      Assert(MacroBody->Tokens.End);

      Rewind(&Macro->Body);
    }

    ConcatStreams(&Ctx->Datatypes.Macros, &MacrosThatNeedToBeParsedOut);
  }


  Rewind(Result);

  // Go through and do macro/include expansion as necessary
  parser *Current = Result;
  c_token *LastT = 0;

  while (TokensRemain(Current))
  {
    c_token *T = PeekTokenPointer(Current);
    switch (T->Type)
    {
      case CT_PreprocessorInclude:
      {
        parser *IncludeParser = ResolveInclude(Ctx, Current);
        Current = SplitAndInsertParserInto(Current, T, IncludeParser, Current->Tokens.At, Memory);

        if (!IncludeParser)
        {
          ParseError(Current, T, CSz("Unable to resolve include."));
        }

      } break;

      case CT_PreprocessorDefine:
      {
        RequireToken(Current, T->Type);
        EatUntilIncluding(Current, CTokenType_Newline);
      } break;

      case CT_MacroLiteral:
      {
        Assert(T->Macro);

        parser *Expanded = ExpandMacro(Current, T->Macro, Memory);
        Current = SplitAndInsertParserInto(Current, T, Expanded, Current->Tokens.At, Memory);
      } break;

      case CT_PreprocessorIf:
      {
        NotImplemented;
      } break;

      default:
      {
        RequireToken(Current, T->Type);
      } break;
    }
  }

  Rewind(Result);
  return Result;
}

bonsai_function parser *
TokenizeAnsiStream(parse_context *Ctx, ansi_stream Code)
{
  parser *Result = TokenizeAnsiStream(Code, Ctx->Memory, False, Ctx);
  return Result;
}

bonsai_function parser *
ParserForFile(counted_string Filename, memory_arena* Memory)
{
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Memory);
  parser *Result = TokenizeAnsiStream(SourceFileStream, Memory, False, 0);
  return Result;
}

bonsai_function parser *
ParserForFile(parse_context *Ctx, counted_string Filename)
{
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Ctx->Memory);
  parser *Result = TokenizeAnsiStream(Ctx, SourceFileStream);
  return Result;
}





// TODO(Jesse id: 302, tags: id_301)
bonsai_function parser*
GetByFilename(parser_stream* Stream, counted_string Filename)
{
  TIMED_FUNCTION();

  parser* Result = 0;
  ITERATE_OVER(Stream)
  {
    parser *Parser = GET_ELEMENT(Iter);
    if (StringsMatch(Parser->Filename, Filename))
    {
      Result = Parser;
      break;
    }
  }

  return Result;
}

bonsai_function parser *
ResolveInclude(parse_context *Ctx, parser *Parser)
{
  parser *Result = {};

  c_token *ErrorToken = PeekTokenPointer(Parser);

  RequireToken(Parser, CT_PreprocessorInclude);

  counted_string PartialPath = {};
  if (PeekToken(Parser).Type == CTokenType_StringLiteral)
  {
    PartialPath = RequireToken(Parser, CTokenType_StringLiteral).Value;
    Error("Relative includes NOT SUPPORTED (%.*s)", (u32)PartialPath.Count, PartialPath.Start);
  }
  else
  {
    RequireToken(Parser, CTokenType_LT);
    PartialPath = EatUntilExcluding(Parser, CTokenType_GT);
    if (PartialPath.Count == 0)
    {
      ParseError(Parser, CSz("Include path must be specified."));
    }
    else
    {
      RequireToken(Parser, CTokenType_GT);
    }
  }

  counted_string_cursor *IncludePaths = Ctx->IncludePaths;
  if (IncludePaths)
  {
    for ( u32 PrefixIndex = 0;
          PrefixIndex < Count(IncludePaths);
          ++PrefixIndex )
    {
      counted_string PrefixPath = IncludePaths->Start[PrefixIndex];
      counted_string FullPath = Concat(PrefixPath, PartialPath, TranArena);

      Info("Searching cached parsed files for (%S)", FullPath);

      parser *Got = GetByFilename(&Ctx->AllParsers, FullPath);
      if (Got)
      {
        Result = Allocate(parser, Ctx->Memory, 1);
        *Result = *Got;
        Success("Found cached parser for (%S)", FullPath);
        break;
      }
    }

    if (!Result)
    {
      Info("File not yet parsed (%.*s).", (u32)PartialPath.Count, PartialPath.Start);
      for ( u32 PrefixIndex = 0;
            PrefixIndex < Count(IncludePaths);
            ++PrefixIndex )
      {
        counted_string PrefixPath = IncludePaths->Start[PrefixIndex];
        counted_string FullPath = Concat(PrefixPath, PartialPath, TranArena); // TODO(Jesse id: 304): only do this work once

        if (FileExists(FullPath))
        {
          Success("Found included file at (%S)", FullPath);
          Result = ParserForFile(Ctx, FullPath);
          break;
        }
      }

    }
  }

  if (!Result)
  {
    parser *Got = GetByFilename(&Ctx->AllParsers, PartialPath);
    if (Got)
    {
      Result = Allocate(parser, Ctx->Memory, 1);
      *Result = *Got;
      Success("Found cached parser for (%S)", PartialPath);
    }
  }

  if (!Result)
  {
    if (FileExists(PartialPath))
    {
      Success("Found included file at (%S)", PartialPath);
      Result = ParserForFile(Ctx, PartialPath);
    }
  }

  if (!Result)
  {
    Error("Unable to resolve include for : %S", PartialPath);
  }

  return Result;
}







d_union_member
DUnionMember(counted_string Name, counted_string Type, d_union_flags Flags)
{
  d_union_member Result = {};
  Result.Name = Name;
  Result.Type = Type;
  Result.Flags = Flags;
  return Result;
}

void
PushMember(d_union_decl* dUnion, c_token MemberIdentifierToken, d_union_flags Flags, memory_arena* Memory)
{
  Assert(MemberIdentifierToken.Type == CTokenType_Identifier);
  d_union_member Member = DUnionMember(MemberIdentifierToken.Value, MemberIdentifierToken.Value, Flags);
  Push(&dUnion->Members, Member, Memory);
}

bonsai_function counted_string
GenerateEnumDef(d_union_decl* dUnion, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string Result = FormatCountedString(Memory, CSz("enum %.*s_type\n{\n  type_%.*s_noop,\n"), dUnion->Name.Count, dUnion->Name.Start, dUnion->Name.Count, dUnion->Name.Start);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    Result = Concat(Result, FormatCountedString(Memory, CSz("  type_%S,\n"), Member->Type), Memory);
  }

  Result = Concat(Result, CS("};\n\n"), Memory);
  return Result;
}

bonsai_function counted_string
GenerateStructDef(d_union_decl* dUnion, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string UnionName = dUnion->Name;
  counted_string TagType = dUnion->CustomEnumType.Count ?
    dUnion->CustomEnumType :
    FormatCountedString(Memory, CSz("%S_type"), UnionName);

  counted_string Result = FormatCountedString(Memory, CSz("struct %S\n{\n  %S Type;\n"),
      UnionName, TagType);

  ITERATE_OVER(&dUnion->CommonMembers)
  {
    struct_member* Member = GET_ELEMENT(Iter);
    Assert(Member->Type == type_variable_decl);
    Result =
      Concat(Result,
        FormatCountedString(Memory, CSz("  %S %S;\n"),
          Member->variable_decl.Type.Token.Value,
          Member->variable_decl.Name),
      Memory);
  }
  Result = Concat(Result, CS("\n  union\n  {\n"), Memory);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    if (Member->Flags != d_union_flag_enum_only)
    {
      Result = Concat(Result, FormatCountedString(Memory, CSz("    %.*s %.*s;\n"), Member->Type.Count, Member->Type.Start, Member->Name.Count, Member->Name.Start), Memory);
    }
  }

  Result = Concat(Result, CS("  };\n};\n\n"), Memory);

  return Result;
}

bonsai_function stl_container_def*
GetStlContainer(stl_container_def_stream* Stream, counted_string Name)
{
  stl_container_def *Result = 0;
  ITERATE_OVER(Stream)
  {
    auto* T = GET_ELEMENT(Iter);
    if (StringsMatch(T->Name, Name))
    {
      Result = T;
      break;
    }
  }

  return Result;
}

bonsai_function type_def*
GetTypedefByAlias(type_def_stream* Typedefs, counted_string Alias)
{
  type_def *Result = 0;
  ITERATE_OVER(Typedefs)
  {
    type_def* T = GET_ELEMENT(Iter);
    if (StringsMatch(T->Alias, Alias))
    {
      Result = T;
      break;
    }
  }

  return Result;
}


// TODO(Jesse id: 301, tags: metaprogramming):  These functions are super repetitive, generate them!
bonsai_function enum_def*
GetEnumByType(enum_def_stream* ProgramEnums, counted_string EnumType)
{
  TIMED_FUNCTION();

  enum_def* Result = 0;
  for (enum_def_iterator Iter = Iterator(ProgramEnums);
      IsValid(&Iter);
      Advance(&Iter))
  {
    enum_def* Enum = &Iter.At->Element;
    if (StringsMatch(Enum->Name, EnumType))
    {
      Result = Enum;
      break;
    }
  }

  return Result;
}

bonsai_function struct_def*
GetStructByType(struct_def_stream* ProgramStructs, counted_string StructType)
{
  TIMED_FUNCTION();

  struct_def* Result = 0;
  for (struct_def_iterator Iter = Iterator(ProgramStructs);
      IsValid(&Iter);
      Advance(&Iter))
  {
    struct_def* Struct = &Iter.At->Element;
    if (StringsMatch(Struct->Type, StructType))
    {
      Result = Struct;
      break;
    }
  }

  return Result;
}

bonsai_function datatype
GetDatatypeByName(program_datatypes* Datatypes, counted_string Name)
{
  TIMED_FUNCTION();

  // TODO(Jesse id: 295, tags: speed): This could be optimized significantly by shuffling the logic around, not to mention using hashtables.
  struct_def *S = GetStructByType(&Datatypes->Structs, Name);
  enum_def   *E = GetEnumByType(&Datatypes->Enums, Name);
  type_def   *T = GetTypedefByAlias(&Datatypes->Typedefs, Name);

  stl_container_def   *StlContainer = GetStlContainer(&Datatypes->StlContainers, Name);

  datatype Result = {};

  if (S)
  {
    Assert(!T && !E && !StlContainer);
    Result = Datatype(S);
  }
  else if (E)
  {
    Assert(!T && !S && !StlContainer);
    Result = Datatype(E);
  }
  else if (T)
  {
    Assert(!E && !S && !StlContainer);
    Result = Datatype(T);
  }
  else if (StlContainer)
  {
    Assert(!E && !S && !T);
    Result = Datatype(StlContainer);
  }

  return Result;
}

d_union_decl
ParseDiscriminatedUnion(parser* Parser, program_datatypes* Datatypes, counted_string Name, memory_arena* Memory)
{
  TIMED_FUNCTION();

  d_union_decl dUnion = {};

  dUnion.Name = Name;

  if (OptionalToken(Parser, CTokenType_OpenBrace))
  {
    b32 Complete = False;
    while (!Complete && Remaining(&Parser->Tokens))
    {
      c_token Interior = PeekToken(Parser);

      switch (Interior.Type)
      {
        case CTokenType_Identifier:
        {
          RequireToken(Parser, Interior);

          d_union_flags Flags = {};
          if ( OptionalToken(Parser, CToken(ToString(enum_only))) )
          {
            Flags = d_union_flag_enum_only;
          }

          PushMember(&dUnion, Interior, Flags, Memory);
        } break;

        case CTokenType_CloseBrace:
        {
          RequireToken(Parser, CTokenType_CloseBrace);
          Complete = True;
        } break;

        default:
        {
          Parser->Valid = False;
          Complete = True;
        } break;
      }
    }
  }
  else
  {
    dUnion.CustomEnumType = RequireToken(Parser, CTokenType_Identifier).Value;;

    enum_def* EnumDef = GetEnumByType(&Datatypes->Enums, dUnion.CustomEnumType);
    if (EnumDef)
    {
      ITERATE_OVER(&EnumDef->Members)
      {
        enum_member* Field = GET_ELEMENT(Iter);
        counted_string MemberName = Concat(Concat(dUnion.Name, CS("_"), Memory), Field->Name, Memory);
        PushMember(&dUnion, CToken(MemberName), d_union_flag_none, Memory);
      }
    }
    else
    {
      Error("Couldn't find enum %.*s", (s32)dUnion.CustomEnumType.Count, dUnion.CustomEnumType.Start);
    }
  }

  if (OptionalToken(Parser, CTokenType_Comma))
  {
    RequireToken(Parser, CTokenType_OpenBrace);
    while (!OptionalToken(Parser, CTokenType_CloseBrace))
    {
      struct_member Decl = {
        .Type = type_variable_decl,
        .variable_decl = {
          .Type.Token = RequireToken(Parser, CTokenType_Identifier),
          .Name       = RequireToken(Parser, CTokenType_Identifier).Value,
        }
      };
      Push(&dUnion.CommonMembers, Decl, Memory);
    }
  }

  return dUnion;
}

bonsai_function b32
DoDebugWindow(const char** ArgStrings, u32 ArgCount)
{
  b32 Result = False;
  for (u32 ArgIndex = 1;
      ArgIndex < ArgCount;
      ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result = True;
    }
  }

  return Result;
}

bonsai_function counted_string
PopArgString(const char** ArgStrings, u32 ArgStringCount, u32* ArgIndex)
{
  counted_string Result = {};

  if (*ArgIndex+1 < ArgStringCount)
  {
    *ArgIndex = *ArgIndex + 1;
    Result = CS(ArgStrings[*ArgIndex]);
  }
  else
  {
    Assert(*ArgIndex == ArgStringCount-1);
    Error("Argument required for '%s'.", ArgStrings[*ArgIndex]);
  }

  return Result;
}

bonsai_function arguments
ParseArgs(const char** ArgStrings, u32 ArgCount, memory_arena* Memory)
{
  arguments Result = {
    .Outpath      = CS("src/metaprogramming/output"),
    .Files        = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
    .IncludePaths = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
  };

  for ( u32 ArgIndex = 1;
        ArgIndex < ArgCount;
        ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);

    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result.DoDebugWindow = True;
    }
    else if ( StringsMatch(CS("-I"), Arg) ||
              StringsMatch(CS("--include-path"), Arg) )
    {
      counted_string Include = PopArgString(ArgStrings, ArgCount, &ArgIndex);

      if (Include.Count)
      {
        if (Include.Start[Include.Count-1] == '/')
        {
        }
        else
        {
          Include = Concat(Include, CSz("/"), TranArena ); // Make sure we end with a '/'
        }

        Info("Include path added : (%.*s)", (u32)Include.Count, Include.Start);
        Push(Include, &Result.IncludePaths);
      }
    }
    else if ( StringsMatch(CS("-o"), Arg) ||
              StringsMatch(CS("--output-path"), Arg) )
    {
      Result.Outpath = PopArgString(ArgStrings, ArgCount, &ArgIndex);
      Error("Output path _NOT_CURRENTLY_SUPPORTED_ : (%.*s)", (u32)Result.Outpath.Count, Result.Outpath.Start);
    }
    else
    {
      Push(Arg, &Result.Files);
    }
  }

  TruncateToCurrentSize(&Result.Files);
  Rewind(&Result.Files);

  TruncateToCurrentSize(&Result.IncludePaths);
  Rewind(&Result.IncludePaths);

  Info("%d total include paths added", (u32)Count(&Result.IncludePaths));

  return Result;
}

global_variable random_series TempFileEntropy = {3215432};

bonsai_function b32
Output(c_token_cursor Code, counted_string Filename, memory_arena* Memory)
{
  TIMED_FUNCTION();
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    Rewind(&Code);
    b32 FileWritesSucceeded = True;
    while(Remaining(&Code))
    {
      if (Code.At->Value.Count)
      {
        FileWritesSucceeded &= WriteToFile(&TempFile, Code.At->Value);
      }
      else
      {
        FileWritesSucceeded &= WriteToFile(&TempFile, CS((const char*)&Code.At->Type, 1));
      }

      Advance(&Code);
    }
    FileWritesSucceeded &= CloseFile(&TempFile);

    if (FileWritesSucceeded)
    {
      if (Rename(TempFile, Filename))
      {
        Result = True;
      }
      else
      {
        Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)Filename.Count, Filename.Start);
      }
    }
    else
    {
      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
    }
  }
  else
  {
    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
  }

  return Result;
}

bonsai_function b32
Output(counted_string Code, counted_string OutputFilename, memory_arena* Memory, output_mode Mode = Output_NoOverwrite)
{
  TIMED_FUNCTION();
  Mode = Output_Unsafe;
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    b32 FileWritesSucceeded = WriteToFile(&TempFile, Code);
    FileWritesSucceeded &= WriteToFile(&TempFile, CS("\n"));
    FileWritesSucceeded &= CloseFile(&TempFile);

    if (FileWritesSucceeded)
    {
      if (Mode == Output_NoOverwrite)
      {
        if (FileExists(OutputFilename))
        {
          counted_string FileContents = ReadEntireFileIntoString(OutputFilename, Memory);
          if (StringsMatch(Trim(Code), Trim(FileContents)))
          {
            Info("File contents matched output for %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          }
          else
          {
            Error("File contents didn't match for %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
            Error("TODO(Jesse): Should probably emit to a similar filname with an incremented extension or something..");
          }
        }
        else if (Rename(TempFile, OutputFilename))
        {
          Info("Generated and output %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)OutputFilename.Count, OutputFilename.Start);
        }
      }
      else
      {
        if (Rename(TempFile, OutputFilename))
        {
          Info("Generated and output %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)OutputFilename.Count, OutputFilename.Start);
        }
      }
    }
    else
    {
      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
    }
  }
  else
  {
    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
  }

  return Result;
}

bonsai_function void
DumpStringStreamToConsole(counted_string_stream* Stream)
{
  for (counted_string_iterator Iter = Iterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    counted_string Message = Iter.At->Element;
    Log("%.*s\n", Message.Count, Message.Start);
  }
}

bonsai_function void
EatUntil_TrackingDepth(parser *Parser, c_token_type Open, c_token_type Close, c_token *StartToken)
{
  u32 Depth = 0;
  b32 Success = False;
  while ( Remaining(Parser) )
  {
    c_token T = PopToken(Parser);

    if (T.Type == Open)
    {
      ++Depth;
    }

    if (T.Type == Close)
    {
      if (Depth == 0)
      {
        Success = True;
        break;
      }
      --Depth;
    }
  }

  if (!Success)
  {
    RuntimeBreak();
    ParseError(Parser, StartToken, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)));
  }

  return;
}

bonsai_function counted_string
EatBetween(parser* Parser, c_token_type Open, c_token_type Close)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  c_token *StartToken = PeekTokenPointer(Parser);

  u32 Depth = 0;
  RequireToken(Parser, Open);

  b32 Success = False;
  while (Remaining(&Parser->Tokens))
  {
    c_token T = PopToken(Parser);

    if (T.Type == Open)
    {
      ++Depth;
    }

    if (T.Type == Close)
    {
      if (Depth == 0)
      {
        Success = True;
        break;
      }
      --Depth;
    }
  }

  if (!Success)
  {
    RuntimeBreak();
    ParseError(Parser, StartToken, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)));
    Parser->Valid = False;
  }

  counted_string Result = FinalizeStringFromParser(&Builder);
  return Result;
}

bonsai_function struct_def
StructDef(counted_string Type, counted_string Sourcefile)
{
  struct_def Result = {
    .Type = Type,
    .DefinedInFile = Sourcefile
  };

  return Result;
}

bonsai_function b32
EatStar(parser* Parser)
{
  b32 Result = OptionalToken(Parser, CTokenType_Star);
  return Result;
}

bonsai_function b32
NextTokenIsSpaceOrTab(parser *Parser)
{
  b32 Result = PeekTokenRaw(Parser).Type == CTokenType_Space ||
               PeekTokenRaw(Parser).Type == CTokenType_Tab;
  return Result;
}

bonsai_function void
TrimUntilNewline(parser* Parser)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);

  while (NextTokenIsSpaceOrTab(Parser)) { PopTokenRaw(Parser); }
  OptionalTokenRaw(Parser, CTokenType_Newline);
  Parser->Tokens.Start = Parser->Tokens.At;
}

bonsai_function void
TrimFirstToken(parser* Parser, c_token_type TokenType)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);
  RequireToken(Parser, TokenType);
  Parser->Tokens.Start = Parser->Tokens.At;
}

bonsai_function void
TrimLastToken(parser* Parser, c_token_type TokenType)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken >= Parser->Tokens.Start)
  {
    if (CurrentToken->Type == TokenType)
    {
      Parser->Tokens.End = CurrentToken;
      break;
    }

    --CurrentToken;
  }
}

bonsai_function parser
GetBodyTextForNextScope(parser* Parser)
{
  parser BodyText = *Parser;
  BodyText.OutputTokens = {};

  BodyText.Tokens.Start = BodyText.Tokens.At;
  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
  BodyText.Tokens.End = Parser->Tokens.At;

  TrimFirstToken(&BodyText, CTokenType_OpenBrace);
  TrimUntilNewline(&BodyText);
  TrimLastToken(&BodyText, CTokenType_CloseBrace);
  TrimTrailingWhitespace(&BodyText);
  Rewind(&BodyText.Tokens);

  return BodyText;
}

bonsai_function void
ParseReferencesIndirectionAndPossibleFunctionPointerness(parser *Parser, type_spec *Result)
{
  b32 Done = False;
  while (!Done)
  {
    c_token T = PeekToken(Parser);

    switch (T.Type)
    {
      case CTokenType_OpenParen:
      {
        RequireToken(Parser, CTokenType_OpenParen);
        RequireToken(Parser, CTokenType_Star);
        Result->FunctionPointerTypeName = RequireToken(Parser, CTokenType_Identifier).Value;
        if (PeekToken(Parser).Type == CTokenType_OpenParen)
           { EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen); }
        RequireToken(Parser, CTokenType_CloseParen);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        Result->IsFunctionPointer = True;
        Done = True;
      } break;

      case CTokenType_Ampersand:
      {
        RequireToken(Parser, CTokenType_Ampersand);
        ++Result->ReferenceLevel;
      } break;

      case CTokenType_Star:
      {
        RequireToken(Parser, CTokenType_Star);
        ++Result->IndirectionLevel;
      } break;

      case CTokenType_Const:
      {
        // Ignore const .. for now .. probably forever
        RequireToken(Parser, CTokenType_Const);
      } break;

      case CTokenType_Arrow: // Structs can have members with the same name as a type,
      case CTokenType_Dot:   // so these are valid

      case CTokenType_Comma: // This gets called on macro-functions too, so this is valid
      case CTokenType_CloseParen: // Valid closing token during a cast
      case CTokenType_Identifier:
      case CTokenType_OperatorKeyword: // Finish parsing the return type of an operator funciton
      {
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token while parsing indirection and reference levels."));
    }
  }

  return;
}

bonsai_function b32
IsTypeIdentifier(counted_string TypeName, program_datatypes *Datatypes)
{
  b32 Result = GetDatatypeByName(Datatypes, TypeName).Type != type_datatype_noop;
  return Result;
}

bonsai_function b32
TryAndEatTemplateParameterList(parser *Parser, program_datatypes *Datatypes)
{
  b32 ValidTemplateList = PeekToken(Parser).Type == CTokenType_LT;
  b32 Done = False;
  u32 Lookahead = 1; // Skip the initial <

  while (ValidTemplateList && !Done)
  {
    c_token TemplateParamListTestT = PeekToken(Parser, Lookahead);

    switch (TemplateParamListTestT.Type)
    {
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Long:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Bool:
      case CTokenType_M128:
      case CTokenType_Auto:
      case CTokenType_Void:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Comma:
      case CTokenType_Star:
      case CTokenType_Ampersand:
      {
      } break;

      case CTokenType_Identifier:
      {
        if (!IsTypeIdentifier(TemplateParamListTestT.Value, Datatypes))
        {
          ValidTemplateList = False;
          break;
        }
      } break;

      case CTokenType_GT:
      {
        Done = True;
      } break;

      default:
      {
        ValidTemplateList = False;
      } break;

    }

    ++Lookahead;
  }

  if (ValidTemplateList)
  {
    EatBetween(Parser, CTokenType_LT, CTokenType_GT);
  }

  return ValidTemplateList;
}

bonsai_function b32
IsConstructorFunctionName(c_token T)
{
  b32 Result = T.QualifierName && StringsMatch(T.QualifierName->Value, T.Value);
  return Result;
}

bonsai_function type_spec
ParseTypeSpecifier(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;

  type_spec Result = {};

  b32 Done = False;
  while (!Done)
  {
    c_token T = PeekToken(Parser);

    switch (T.Type)
    {
      case CTokenType_At:
      {
        RequireToken(Parser, CTokenType_At);
        Result.IsMetaTemplateVar = True;
      } break;

      case CTokenType_ThreadLocal:
      {
        RequireToken(Parser, CTokenType_ThreadLocal);
        Result.ThreadLocal = True;
      } break;

      case CTokenType_Const:
      {
        RequireToken(Parser, CTokenType_Const);
        Result.Const = True;
      } break;

      case CTokenType_Static:
      {
        RequireToken(Parser, CTokenType_Static);
        Result.Static = True;
      } break;

      case CTokenType_Volatile:
      {
        RequireToken(Parser, CTokenType_Volatile);
        Result.Volatile = True;
      } break;

      case CTokenType_Signed:
      {
        RequireToken(Parser, CTokenType_Signed);
        Result.Signed = True;
      } break;

      case CTokenType_Unsigned:
      {
        RequireToken(Parser, CTokenType_Unsigned);
        Result.Unsigned = True;
      } break;

      case CTokenType_Enum:
      {
        RequireToken(Parser, CTokenType_Enum);
        Result.Enum = True;
      } break;

      case CTokenType_Struct:
      {
        RequireToken(Parser, CTokenType_Struct);
        Result.Struct = True;
      } break;

      case CTokenType_TemplateKeyword:
      {
        RequireToken(Parser, CTokenType_TemplateKeyword);
        Result.TemplateSource = EatBetween(Parser, CTokenType_LT, CTokenType_GT);
      } break;

      case CTokenType_Extern:
      {
        RequireToken(Parser, T.Type);
        Result.Linkage = linkage_extern;
        if ( OptionalToken(Parser, CToken(CTokenType_StringLiteral, CSz("\"C\""))) )
        {
          Result.Linkage = linkage_extern_c;
        }

      } break;

      case CTokenType_Inline:
      {
        // TODO(Jesse id: 282): Should we parse out the bonsai_function def explicitly here?
        RequireToken(Parser, T.Type);
        Result.Inline = True;
      } break;

      case CTokenType_Short:
      {
        RequireToken(Parser, T.Type);
        Result.Short = True;
        if (PeekToken(Parser).Type == CTokenType_Int)
        {
          Result.Token = RequireToken(Parser, CTokenType_Int);
        }
        Done = True;
      } break;

      case CTokenType_Long:
      {
        RequireToken(Parser, T.Type);

        Result.Long = True;

        if (OptionalToken(Parser, CTokenType_Long))
        {
          Result.LongLong = True;
        }

        if (PeekToken(Parser).Type == CTokenType_Int)
        {
          Result.Token = RequireToken(Parser, CTokenType_Int);
        }

        Done = True;
      } break;

      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Void:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Identifier:
      {
        if (T.Type == CTokenType_Identifier)
        {
          Result.Datatype = GetDatatypeByName(&Ctx->Datatypes, T.Value);
          // TODO(Jesse, id: 296, tags: immediate): When we properly traverse include graphs, this assert should not fail.
          // Assert(Result.Datatype.Type != type_datatype_noop);
        }

        Result.Token = RequireToken(Parser, T.Type);
        Result.HasTemplateArguments = TryAndEatTemplateParameterList(Parser, &Ctx->Datatypes);

        if (IsConstructorFunctionName(Result.Token))
        {
        }
        else
        {
          ParseReferencesIndirectionAndPossibleFunctionPointerness(Parser, &Result);
        }

        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing a type specifier"));
    }

    EatWhitespaceAndComments(Parser);
    continue;
  }

  return Result;
}

bonsai_function ast_node*
ParseInitializerList(parser *Parser, memory_arena *Memory)
{
  ast_node* Result = {};
  ast_node_initializer_list *Node = AllocateAndCastTo(ast_node_initializer_list, &Result, Memory);
  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
  return Result;
}

bonsai_function ast_node_expression*
ParseExpression(parse_context *Ctx);

bonsai_function void
ParseExpression(parse_context *Ctx, ast_node** Result);

bonsai_function variable_decl
ParseVariableDecl(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;

  variable_decl Result = {
    .Type = ParseTypeSpecifier(Ctx),
    .Name = RequireToken(Parser, CTokenType_Identifier).Value,
  };

  if ( PeekToken(Parser).Type == CTokenType_OpenBracket )
  {
    ParseExpression(Ctx, &Result.StaticBufferSize);
  }

  if (OptionalToken(Parser, CTokenType_Equals))
  {
    if (PeekToken(Parser).Type == CTokenType_OpenBrace)
    {
      Result.Value = ParseInitializerList(&Ctx->CurrentParser, Ctx->Memory);
    }
    else
    {
      ParseExpression(Ctx, &Result.Value);
    }
  }

  return Result;
}

bonsai_function function_decl
ParseAndPushFunctionPrototype(parse_context *Ctx, type_spec *ReturnType, counted_string *Name, function_type Type)
{
  parser *Parser = &Ctx->CurrentParser;

  function_decl Result = {
    .Type = Type,
    .ReturnType = *ReturnType,
    .Name = *Name,
  };

  // Function definition args
  b32 DoneParsingArguments = PeekToken(Parser) == CToken(CTokenType_CloseParen);

  if ( PeekToken(Parser).Type == CTokenType_Void &&
       PeekToken(Parser, 1).Type == CTokenType_CloseParen )
  {
    RequireToken(Parser, CTokenType_Void);
    DoneParsingArguments = True;
  }

  while ( !DoneParsingArguments && TokensRemain(Parser) )
  {
    variable_decl Arg = ParseVariableDecl(Ctx);

    if (!OptionalToken(Parser, CTokenType_Comma))
    {
      DoneParsingArguments = True;
    }

    if (OptionalToken(Parser, CTokenType_Ellipsis))
    {
      Result.IsVariadic = True;
      DoneParsingArguments = True;
    }

    Push(&Result.Args, Arg, Ctx->Memory);
    continue;
  }
  RequireToken(Parser, CTokenType_CloseParen); // Ending paren for arguments

  if (PeekToken(Parser).Type == CTokenType_OpenBrace)
  {
    // void FunctionName( arg A1, arg, A2) { .. body text .. }
    Result.Body = GetBodyTextForNextScope(Parser);
    Push(&Ctx->Datatypes.Functions, Result, Ctx->Memory);
  }
  else
  {
    // void FunctionName( arg A1, arg, A2);
    RequireToken(Parser, CTokenType_Semicolon);
  }

  return Result;
}

bonsai_function struct_def
ParseStructBody(parse_context *Ctx, counted_string StructName);

bonsai_function declaration
ParseFunctionOrVariableDecl(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;

  declaration Result = {};

  type_spec DeclType = ParseTypeSpecifier(Ctx);

  if (DeclType.IsFunctionPointer)
  {
    RequireToken(Parser, CTokenType_Semicolon);
  }
  else
  {
    if ( OptionalToken(Parser, CTokenType_OperatorKeyword) )
    {
      counted_string OperatorName = RequireOperatorToken(Parser);
      if ( OptionalToken(Parser, CTokenType_OpenParen) )
      {
        Result.Type = type_declaration_function_decl;
        Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &DeclType, &OperatorName, function_type_operator);
      }
    }
    else if (IsConstructorFunctionName(DeclType.Token))
    {
      Result.Type = type_declaration_function_decl;
      RequireToken(Parser, CTokenType_OpenParen);
      Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &DeclType, &DeclType.Token.Value, function_type_constructor);
    }
    else
    {
      c_token DeclNameToken = RequireToken(Parser, CTokenType_Identifier);

      macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, DeclNameToken.Value);
      if ((!DeclType.Token.Type) && Macro && Macro->Type == type_macro_function)
      {
        // This is a random hack that'll go away once we have macro-bonsai_function expansion
        // TODO(Jesse id: 321 tags: id_320): Once this path goes away, the assertion with the label associated with this todo id should be put back in.
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        OptionalToken(Parser, CTokenType_Semicolon);
      }
      else if ( OptionalToken(Parser, CTokenType_OpenParen) )
      {
        Result.Type = type_declaration_function_decl;
        Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &DeclType, &DeclNameToken.Value, function_type_normal);
      }
      else
      {
        Result.Type = type_declaration_variable_decl;
        Result.variable_decl.Type = DeclType;
        Result.variable_decl.Name = DeclNameToken.Value;

        if ( OptionalToken(Parser, CTokenType_OpenBracket) )
        {
          ParseExpression(Ctx, &Result.variable_decl.StaticBufferSize );
          RequireToken(Parser, CTokenType_CloseBracket);
        }

        if ( OptionalToken(Parser, CTokenType_Equals) )
        {
          if (PeekToken(Parser).Type == CTokenType_OpenBrace)
          {
            Result.variable_decl.Value = ParseInitializerList(Parser, Ctx->Memory);
          }
          else
          {
            ParseExpression(Ctx, &Result.variable_decl.Value);
          }
        }
      }
    }
  }

  // TODO(Jesse id: 323, tags: id_321)
  // Assert(Result.Type);
  return Result;
}

#define TEST__ 'a'

#if TEST__

#endif

bonsai_function u64
ApplyOperator(parser *Parser, u64 LHS, c_token_type OperatorType, u64 RHS)
{
  u64 Result = 0;
  switch (OperatorType)
  {
    case CTokenType_LT:
    {
      Result = LHS < RHS;
    } break;

    case CTokenType_GT:
    {
      Result = LHS > RHS;
    } break;

    case CTokenType_Plus:
    {
      Result = LHS + RHS;
    } break;

    case CTokenType_Minus:
    {
      Result = LHS - RHS;
    } break;

    case CTokenType_Star:
    {
      Result = LHS * RHS;
    } break;

    case CTokenType_FSlash:
    {
      Result = LHS / RHS;
    } break;

    // TODO(Jesse id: 345): How do we differentiate between arithmetic/logical shifts here?
    case CTokenType_LeftShift:
    {
      if (RHS < 64)
      {
        Result = LHS << RHS;
      }
      else
      {
        ParseError(Parser, CSz("Attempted to left-shift by more than 64 bits!"));
      }
    } break;

    // TODO(Jesse id: 346, tags: id_345)
    case CTokenType_RightShift:
    {
      Result = LHS >> RHS;
    } break;

    case CTokenType_LessEqual:
    {
      Result = LHS <= RHS;
    } break;

    case CTokenType_GreaterEqual:
    {
      Result = LHS >= RHS;
    } break;

    case CTokenType_AreEqual:
    {
      Result = LHS == RHS;
    } break;

    case CTokenType_NotEqual:
    {
      Result = LHS != RHS;
    } break;

    case CTokenType_PlusEquals:
    {
      Result = LHS + RHS;
    } break;

    case CTokenType_MinusEquals:
    {
      Result = LHS - RHS;
    } break;

    case CTokenType_TimesEquals:
    {
      Result = LHS * RHS;
    } break;

    case CTokenType_DivEquals:
    {
      Result = LHS / RHS;
    } break;

    case CTokenType_ModEquals:
    {
      Result = LHS % RHS;
    } break;

    case CTokenType_AndEquals:
    {
      Result = LHS && RHS;
    } break;

    case CTokenType_OrEquals:
    {
      Result = LHS || RHS;
    } break;

    case CTokenType_XorEquals:
    {
      Result = LHS ^ RHS;
    } break;

    case CTokenType_Increment:
    {
      Assert(RHS == 0);
      Result = LHS+1;
    } break;

    case CTokenType_Decrement:
    {
      Assert(RHS == 0);
      Result = LHS - 1;
    } break;

    case CTokenType_LogicalAnd:
    {
      Result = LHS && RHS;
    } break;

    case CTokenType_LogicalOr:
    {
      Result = LHS || RHS;
    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Invalid operator type passed to ApplyOperator."));
  }


  return Result;
}

bonsai_function b32
IsOfHigherPrecedenceThan(c_token_type O1, c_token_type O2)
{
  Warn("IsOfHigherPrecedenceThan is left un-implemented.");
  return False;
}

bonsai_function u64
ResolveMacroConstantExpression(parser *Parser)
{
  u64 Result = 0;

  b32 Done = False;
  c_token_type OperatorToApply = {};

  while ( !Done && TokensRemain(Parser) )
  {
    c_token T = PeekTokenRaw(Parser);
    switch (T.Type)
    {
      case CTokenType_CharLiteral:
      case CTokenType_IntLiteral:
      {
        if (OperatorToApply)
        {
          c_token_type NextTokenType = PeekToken(Parser).Type;
          b32 NextIsOperator = TokenIsOperator(NextTokenType);
          if ( NextIsOperator && IsOfHigherPrecedenceThan(OperatorToApply, NextTokenType) )
          {
            u64 NextValue = ResolveMacroConstantExpression(Parser);
            Result = ApplyOperator(Parser, Result, OperatorToApply, NextValue);
          }
          else
          {
            Result = ApplyOperator(Parser, Result, OperatorToApply, T.UnsignedValue);
          }
        }
        else
        {
          Result = T.UnsignedValue;
        }

        RequireTokenRaw(Parser, T.Type);
      } break;

      case CTokenType_OpenParen:
      {
        RequireTokenRaw(Parser, T.Type);
        Result += ResolveMacroConstantExpression(Parser);
      } break;

      case CTokenType_CloseParen:
      case CTokenType_Newline:
      {
        RequireTokenRaw(Parser, T.Type);
        Done = True;
      } break;

      case CTokenType_Space:
      case CTokenType_Tab:
      {
        RequireTokenRaw(Parser, T.Type);
      } break;


      case CTokenType_Plus:
      case CTokenType_Minus:
      case CTokenType_Star:
      case CTokenType_FSlash:
      case CTokenType_LT:
      case CTokenType_GT:
      case CTokenType_LeftShift:
      case CTokenType_RightShift:
      case CTokenType_LessEqual:
      case CTokenType_GreaterEqual:
      case CTokenType_AreEqual:
      case CTokenType_NotEqual:
      case CTokenType_PlusEquals:
      case CTokenType_MinusEquals:
      case CTokenType_TimesEquals:
      case CTokenType_DivEquals:
      case CTokenType_ModEquals:
      case CTokenType_AndEquals:
      case CTokenType_OrEquals:
      case CTokenType_XorEquals:
      case CTokenType_Increment:
      case CTokenType_Decrement:
      case CTokenType_LogicalAnd:
      case CTokenType_LogicalOr:
      {
        RequireTokenRaw(Parser, T.Type);
        OperatorToApply = T.Type;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Now, that's some crap during ResolveMacroConstantExpression"));
    }
  }

  return Result;
}

bonsai_function void
EatIfBlock(parser *Parser)
{
  c_token *StartToken = PeekTokenPointer(Parser);

  u32 Depth = 0;
  b32 Success = False;
  while ( RawTokensRemain(Parser) )
  {
    c_token T = PopTokenRaw(Parser);

    if (T.Type == CT_PreprocessorIf)
    {
      ++Depth;
    }

    if ( T.Type == CT_PreprocessorElse ||
         T.Type == CT_PreprocessorElif ||
         T.Type == CT_PreprocessorEndif  )
    {
      if (Depth == 0)
      {
        Success = True;
        break;
      }
      --Depth;
    }
  }

  if (!Success)
  {
    ParseError(Parser, StartToken, FormatCountedString(TranArena, CSz("Unable to find closing token for "), ToString(CT_PreprocessorIf)));
  }

  return;
}

bonsai_function struct_member
ParseStructMember(parse_context *Ctx, counted_string StructName)
{
  TIMED_FUNCTION();
  parser *Parser = &Ctx->CurrentParser;

  struct_member Result = {};

  b32 Continue = False;
  do {
    Continue = False;

    c_token T = PeekToken(Parser);
    switch(T.Type)
    {
      case CT_PreprocessorIf:
      case CT_PreprocessorElif:
      {
        RequireToken(Parser, T.Type);
        if (ResolveMacroConstantExpression(Parser) == 0)
        {
          EatIfBlock(Parser);
        }
      } break;

      case CT_PreprocessorDefine:
      case CT_PreprocessorIfDefined:
      case CT_PreprocessorIfNotDefined:
      case CT_PreprocessorElse:
      case CT_PreprocessorEndif:
      case CT_PreprocessorUndef:
      case CT_PreprocessorPragma:
      case CT_PreprocessorError:
      case CT_PreprocessorWarning:
      {
        Continue = True;
        RequireToken(Parser, T.Type);
        EatUntilIncluding(Parser, CTokenType_Newline);
      } break;

      case CTokenType_Tilde:
      {
        RequireToken(Parser, CTokenType_Tilde);

        if (StringsMatch(StructName, PeekToken(Parser).Value) &&
             PeekToken(Parser, 1).Type == CTokenType_OpenParen)
        {
          RequireToken(Parser, CTokenType_Identifier);
          RequireToken(Parser, CTokenType_OpenParen);

          Result.Type = type_function_decl;
          type_spec ReturnType = {};
          Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &ReturnType, &StructName, function_type_destructor);
        }
        else
        {
          ParseError(Parser, PeekTokenPointer(Parser), CSz("Destructor name must match the struct name."));
        }
      } break;

      case CTokenType_Union:
      {
        RequireToken(Parser, CTokenType_Union);
        Result.Type = type_struct_member_anonymous;
        Result.struct_member_anonymous.Body = ParseStructBody(Ctx, CS("anonymous_union"));
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_Struct:
      {
        RequireToken(Parser, CTokenType_Struct);
        Result.Type = type_struct_member_anonymous;
        Result.struct_member_anonymous.Body = ParseStructBody(Ctx, CS("anonymous_struct"));
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Identifier:
      {
        u32 DefKeywordsEncountered = 0;
        if ( StringsMatch(StructName, T.Value) && PeekToken(Parser, 1).Type == CTokenType_OpenParen)
        {
          // Constructor bonsai_function
          RequireToken(Parser, CTokenType_Identifier);
          RequireToken(Parser, CTokenType_OpenParen);

          Result.Type = type_function_decl;
          type_spec ReturnType = {};
          Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &ReturnType, &StructName, function_type_constructor);
        }
        else
        {
          declaration Decl = ParseFunctionOrVariableDecl(Ctx);
          switch (Decl.Type)
          {
            case type_declaration_variable_decl:
            {
              Result.Type = type_variable_decl;
              Result.variable_decl = Decl.variable_decl;
              RequireToken(Parser, CTokenType_Semicolon);
            } break;

            case type_declaration_function_decl:
            {
              Result.Type = type_function_decl;
              Result.function_decl = Decl.function_decl;
            } break;

            case type_declaration_noop:
            {
              InvalidCodePath();
            } break;
          }
        }

      } break;

      case CTokenType_CloseBrace:
      {
        // Done parsing struct members
      } break;;

      InvalidDefaultWhileParsing(Parser, CS("While parsing struct member."));
    }
  } while (Continue);

  return Result;
}

bonsai_function counted_string
ConcatTokensUntil(parser* Parser, c_token_type Close, memory_arena* Memory)
{
  // TODO(Jesse  id: 225, tags: todos, easy): Rewrite with string_from_parser
  string_builder Builder = {};
  while (Remaining(&Parser->Tokens) && PeekTokenRaw(Parser).Type != Close)
  {
    Append(&Builder, PopTokenRaw(Parser).Value);
  }
  counted_string Result = Finalize(&Builder, Memory);
  return Result;
}

bonsai_function counted_string
ConcatTokensUntilNewline(parser* Parser, memory_arena* Memory)
{
  counted_string Result = ConcatTokensUntil(Parser, CTokenType_Newline, Memory);
  return Result;
}

bonsai_function struct_member_stream
MembersOfType(struct_def* Struct, counted_string MemberType, memory_arena *Memory)
{
  struct_member_stream Result = {};
  if (MemberType.Start)
  {
    ITERATE_OVER(&Struct->Members)
    {
      struct_member *Member = GET_ELEMENT(Iter);
      switch (Member->Type)
      {
        case type_variable_decl:
        {
          if (StringsMatch(Member->variable_decl.Type.Token.Value, MemberType))
          {
            Push(&Result, *Member, Memory);
          }
        } break;

        InvalidDefaultCase;
      }
    }
  }

  return Result;
}

// TODO(Jesse id: 299): This could be improved by not taking the StructName, and
// filling it out internally.  It would have to check where the struct name is
//
// ie. at the start 'struct foo { ... };'
//
// or at the end 'typedef struct { ... } foo;
//
// This would clean up the calling code quite a bit and get rid of a
// bunch of redundant RequireTokens on Semicolons.

bonsai_function struct_def
ParseStructBody(parse_context *Ctx, counted_string StructName)
{
  TIMED_FUNCTION();
  parser *Parser = &Ctx->CurrentParser;
  struct_def Result = StructDef(StructName, Parser->Filename);

  RequireToken(Parser, CTokenType_OpenBrace);

  struct_member Declaration = {};
  for (;;)
  {
    Declaration = ParseStructMember(Ctx, Result.Type);
    if (Declaration.Type == type_struct_member_noop)
    {
      break;
    }
    else
    {
      Push(&Result.Members, Declaration, Ctx->Memory);
    }
  }

  RequireToken(Parser, CTokenType_CloseBrace);

  return Result;
}

bonsai_function counted_string
ParseIntegerConstant(parser* Parser)
{
  c_token T = PeekToken(Parser);

  const char* Start = T.Value.Start;
  u32 Count = 0;

  if (T.Type == CTokenType_Minus)
  {
    ++Count;
    RequireToken(Parser, CTokenType_Minus);
  }

  c_token NumberT = RequireToken(Parser, CTokenType_Identifier);
  if (IsNumeric(NumberT.Value))
  {
    Count += NumberT.Value.Count;
  }
  else
  {
    ParseError(Parser, CSz("Expected integer constant expression."));
  }

  counted_string Result = {
    .Count = Count,
    .Start = Start,
  };

  return Result;
}

bonsai_function enum_def
ParseEnum(parse_context *Ctx)
{
  TIMED_FUNCTION();

  parser *Parser = &Ctx->CurrentParser;
  program_datatypes *Datatypes = &Ctx->Datatypes;

  counted_string EnumName = RequireToken(Parser, CTokenType_Identifier).Value;

  enum_def Enum = {
    .Name = EnumName
  };

  RequireToken(Parser, CTokenType_OpenBrace);

  b32 Done = False;
  while (!Done && TokensRemain(Parser))
  {
    enum_member Field = {};
    Field.Name = RequireToken(Parser, CTokenType_Identifier).Value;

    if (OptionalToken(Parser, CTokenType_Equals))
    {
      Field.Value = ParseExpression(Ctx);
    }

    Push(&Enum.Members, Field, Ctx->Memory);

    if(OptionalToken(Parser, CTokenType_Comma))
    {
      if (OptionalToken(Parser, CTokenType_CloseBrace))
      {
        Done = True;
      }
    }
    else
    {
      RequireToken(Parser, CTokenType_CloseBrace);
      Done = True;
    }
  }

  return Enum;
}

bonsai_function b32
IsTypeQualifier(c_token T)
{
  b32 Result = False;
  switch (T.Type)
  {
    case CTokenType_LT:
    case CTokenType_Ampersand:
    case CTokenType_Star:
    { Result = True; } break;

    default : {} break;
  }

  return Result;
}

bonsai_function void
OptionalPrefixOperator(parser *Parser)
{
  c_token T = PeekToken(Parser);
  if ( T.Type == CTokenType_Increment ||
       T.Type == CTokenType_Decrement ||
       T.Type == CTokenType_Ampersand ||
       T.Type == CTokenType_Star
       )
  {
    PopToken(Parser);
  }
}

bonsai_function void
OptionalPostfixOperator(parser *Parser)
{
  c_token T = PeekToken(Parser);
  if ( T.Type == CTokenType_Increment ||
       T.Type == CTokenType_Decrement)
  {
    PopToken(Parser);
  }
}

bonsai_function void
ParseDatatypeDef(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;
  c_token TypeSpecifier = PopToken(Parser);

  switch(TypeSpecifier.Type)
  {
    case CTokenType_Enum:
    {
      enum_def Enum = ParseEnum(Ctx);
      Push(&Ctx->Datatypes.Enums, Enum, Ctx->Memory);
    } break;

    case CTokenType_Struct:
    {
      counted_string StructName = RequireToken(Parser, CTokenType_Identifier).Value;
      if ( PeekToken(Parser).Type == CTokenType_OpenBrace )
      {
        struct_def S = ParseStructBody(Ctx, StructName);
        Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
      }
    } break;

    case CTokenType_Union:
    {
      counted_string UnionName = RequireToken(Parser, CTokenType_Identifier).Value;
      struct_def S = ParseStructBody(Ctx, UnionName);
      S.IsUnion = True;
      Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Tried parsing a datatypes that wasn't a struct, enum or union!"));
  }

  RequireToken(Parser, CTokenType_Semicolon);
}

bonsai_function void
ParseAndPushTypedef(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;

  type_spec Type = ParseTypeSpecifier(Ctx);
  counted_string  Alias = {};

  if (Type.IsFunctionPointer)
  {
    Alias = Type.FunctionPointerTypeName;
  }
  else
  {
    Alias = RequireToken(Parser, CTokenType_Identifier).Value;
  }
  RequireToken(Parser, CTokenType_Semicolon);

  type_def Typedef = {
    .Type = Type,
    .Alias = Alias,
  };

  Push(&Ctx->Datatypes.Typedefs, Typedef, Ctx->Memory);
}

bonsai_function void
ParseTypedef(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;

  RequireToken(Parser, CTokenType_Typedef);

  if ( OptionalToken(Parser, CTokenType_Struct) )
  {
    if (PeekToken(Parser).Type == CTokenType_OpenBrace)
    {
      struct_def S = ParseStructBody(Ctx, CS(""));
      S.Type = RequireToken(Parser, CTokenType_Identifier).Value;
      RequireToken(Parser, CTokenType_Semicolon);
      Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
    }
    else
    {
      ParseAndPushTypedef(Ctx);
    }
  }
  else
  {
    ParseAndPushTypedef(Ctx);
  }

  return;
}

bonsai_function function_decl*
GetByName(counted_string Name, function_decl_stream* Stream)
{
  function_decl *Result = {};
  ITERATE_OVER(Stream)
  {
    auto *Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function ast_node_variable_def*
GetByTypeName(counted_string Name, ast_node_variable_def_stream* Stream)
{
  ast_node_variable_def* Result = {};
  ITERATE_OVER(Stream)
  {
    ast_node_variable_def* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Decl.Name, Name))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function ast_node *
ParseFunctionArgument(parser *Parser, memory_arena *Memory, function_decl_stream *FunctionPrototypes);

bonsai_function ast_node_statement *
ParseAllStatements(parse_context *Ctx);

bonsai_function void
ParseUndefine(parse_context *Ctx, parser *Parser)
{
  NotImplemented;
}

bonsai_function counted_string
ParseIfDefinedValue(parser *Parser)
{
  u32 NumOpenParens = 0;
  while (OptionalToken(Parser, CTokenType_OpenParen))
  {
    ++NumOpenParens;
  }

  counted_string Result = RequireToken(Parser, CTokenType_Identifier).Value;

  while (NumOpenParens--)
  {
    RequireToken(Parser, CTokenType_CloseParen);
  }

  return Result;
}

bonsai_function b32
IsDefined(parse_context *Ctx, counted_string DefineValue) 
{
  b32 Result = GetByName(&Ctx->Datatypes.Macros, DefineValue) != 0;
  return Result;
}

bonsai_function void
ParseSingleStatement(parse_context *Ctx, ast_node_statement *Result)
{
  parser *Parser = &Ctx->CurrentParser;

  b32 Done = False;
  while (!Done && TokensRemain(Parser))
  {
    const c_token T = PeekToken(Parser);
    switch (T.Type)
    {
      case CT_PreprocessorIf:
      case CT_PreprocessorElif:
      {
        RequireToken(Parser, T.Type);
        if (ResolveMacroConstantExpression(Parser) == 0)
        {
          EatIfBlock(Parser);
        }
      } break;

      case CT_PreprocessorDefine:
      {
        RequireToken(Parser, T.Type);
        EatUntilIncluding(Parser, CTokenType_Newline);
      } break;

      case CT_PreprocessorUndef:
      {
        NotImplemented;
        RequireToken(Parser, T.Type);
        EatUntilIncluding(Parser, CTokenType_Newline);
      } break;

      case CT_PreprocessorIfDefined:
      {
        RequireToken(Parser, T.Type);
        counted_string DefineValue = ParseIfDefinedValue(Parser);
        if (!IsDefined(Ctx, DefineValue) )
        {
          EatIfBlock(Parser);
        }

      } break;

      case CT_PreprocessorIfNotDefined:
      {
        RequireToken(Parser, T.Type);
        counted_string DefineValue = ParseIfDefinedValue(Parser);
        if ( IsDefined(Ctx, DefineValue) )
        {
          // TODO(Jesse): @preprocessor_if_defined
          EatIfBlock(Parser);
        }

      } break;

      case CT_PreprocessorElse:
      case CT_PreprocessorEndif:

      case CT_PreprocessorPragma:
      case CT_PreprocessorError:
      case CT_PreprocessorWarning:
      {
        RequireToken(Parser, T.Type);
        EatUntilIncluding(Parser, CTokenType_Newline);
      } break;

      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        Result->LHS = ParseExpression(Ctx);
      } break;

      case CTokenType_Identifier:
      {
        if (Result->LHS)
        {
          DebugPrint(Result->LHS);
          Assert(False);
        }
        else
        {
          Result->LHS = ParseExpression(Ctx);
        }

      } break;

      case CTokenType_Equals:
      {
        RequireToken(Parser, T.Type);
        if (PeekToken(Parser).Type == CTokenType_OpenBrace)
        {
          // Initializer List
          EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
        }
        else
        {
          Assert(!Result->RHS);
          Result->RHS = AllocateProtection(ast_node_statement, Ctx->Memory, 1, False);
          ParseSingleStatement(Ctx, Result->RHS);
        }
        Done = True;
      } break;

      case CTokenType_For:
      {
        RequireToken(Parser, T.Type);
#if 0
        RequireToken(Parser, CTokenType_OpenParen);
        Result->Next = AllocateProtection(ast_node_statement, Memory, 1, False);
        ParseSingleStatement(Parser, Memory, Datatypes, Result->Next);
        ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_Semicolon);
        ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_CloseParen);
#else
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
#endif
        Done = True;
      } break;

      case CTokenType_If:
      case CTokenType_While:
      case CTokenType_Switch:
      {
        RequireToken(Parser, T.Type);
        Result->LHS = ParseExpression(Ctx);
        Done = True;
      } break;

      case CTokenType_Case:
      {
        RequireToken(Parser, CTokenType_Case);
        Result->LHS = ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_Colon);
        Done = True;
      } break;

      case CTokenType_Default:
      {
        RequireToken(Parser, CTokenType_Default);
        RequireToken(Parser, CTokenType_Colon);
      } break;

      // What do we do with these?
      case CTokenType_Else:
      case CTokenType_Continue:
      {
        RequireToken(Parser, T.Type);
      } break;

      case CTokenType_Return:
      {
        RequireToken(Parser, T.Type);
        Result->LHS = ParseExpression(Ctx);
      } break;

      case CTokenType_OpenBrace:
      {
        RequireToken(Parser, CTokenType_OpenBrace);
        Result->RHS = ParseAllStatements(Ctx);
        Done = True;
      } break;

      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:

      case CTokenType_CharLiteral:
      case CTokenType_StringLiteral:

      case CTokenType_Minus:
      case CTokenType_LeftShift:
      case CTokenType_RightShift:
      case CTokenType_Increment:
      case CTokenType_Decrement:

      case CTokenType_Tilde:
      case CTokenType_Bang:
      case CTokenType_Ampersand:
      case CTokenType_Star:
      case CTokenType_OpenParen:
      {
        if (Result->LHS)
        {
          DebugPrint(Result);
          ParseError(Parser, CSz(""));
          RuntimeBreak();
        }

        Result->LHS = ParseExpression(Ctx);
        Done = True;
      } break;

      case CTokenType_Colon:
      case CTokenType_CloseParen:
      case CTokenType_Break:
      case CTokenType_CloseBrace:
      case CTokenType_Semicolon:
      {
        RequireToken(Parser, T.Type);
        Done = True;
      } break;

      case CTokenType_Goto:
      {
        RequireToken(Parser, T.Type);
        RequireToken(Parser, CTokenType_Identifier);
      } break;

      case CTokenType_Meta:
      {
        RequireToken(Parser, T.Type);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_Asm:
      {
        RequireToken(Parser, T.Type);
        OptionalToken(Parser, CTokenType_Volatile);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing statement."));
    }
  }

}

bonsai_function ast_node_statement*
ParseSingleStatement(parse_context *Ctx)
{
  ast_node_statement *Result = AllocateProtection(ast_node_statement, Ctx->Memory, 1, False);
  ParseSingleStatement(Ctx, Result);
  return Result;
}

bonsai_function ast_node_statement*
ParseAllStatements(parse_context *Ctx)
{
  ast_node_statement *Result = 0;

  ast_node_statement **Current = &Result;

  while ( TokensRemain(&Ctx->CurrentParser) )
  {
    Assert(*Current == 0);

    *Current = ParseSingleStatement(Ctx);
    while (*Current) { Current = &(*Current)->Next; } // Walk to the end of the statement chain.
  }

  return Result;
}

bonsai_function ast_node*
ParseFunctionCall(parse_context *Ctx, counted_string FunctionName);

bonsai_function void
ParseTypeSpecifierNode(parse_context *Ctx, ast_node_expression *Result, datatype *Data = 0)
{
  ast_node_type_specifier *Node = AllocateAndCastTo(ast_node_type_specifier, &Result->Value, Ctx->Memory);

  Node->TypeSpec = ParseTypeSpecifier(Ctx);

  if (Data)
  {
    Node->Datatype = *Data;
    Assert(Node->Datatype.Type != type_datatype_noop);
  }
  else
  {
    if (Node->TypeSpec.Token.Type == CTokenType_Identifier)
    {
      Node->Datatype = GetDatatypeByName(&Ctx->Datatypes, Node->TypeSpec.Token.Value);
      if (Node->Datatype.Type == type_datatype_noop)
      {
        // TODO(Jesse id: 319, tags: id_320): Type-checking failed.
        //
        // This currently actually fails for stuff like std::vector that we
        // don't parse out definitions for .. what should be the strategy for
        // handling this stuff?  There's also a minor hack that can be cleaned
        // up at id: 320 when this is decided
      }
    }
    else
    {
      //
      // Primitive type .. whadda we do?
      //

      /* primitive_def TempDatatype = GetPrimitiveType(&Datatypes->Primitives, Node->TypeSpec); */
    }
  }

  Node->Name = ParseExpression(Ctx);

  if (Node->Name->Value)
  {
    switch (Node->Name->Value->Type)
    {
      case type_ast_node_function_call: // Constructor bonsai_function
      case type_ast_node_symbol:        // Regular variable definition
      {
      } break;

      // This case should go away once we can check what local varaibles are defined for the scope we're parsing
      case type_ast_node_access:
      {
        Error("BUG during ParseTypeSpecifierNode");
      } break;

      InvalidDefaultWhileParsing(&Ctx->CurrentParser, CSz("Invalid syntax following type-specifier."));
    }
  }
  else
  {
    // Cast or macro bonsai_function call
  }
}

bonsai_function b32
ParseSymbol(parse_context *Ctx, ast_node_expression* Result, b32 SymbolIsNeverTypeSpecifier = True)
{
  parser *Parser = &Ctx->CurrentParser;

  b32 PushedParser = False;

  c_token T = PeekToken(Parser);
  Assert(T.Type == CTokenType_Identifier);

#if 0
  macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, T.Value);
  if (Macro)
  {
    c_token MacroNameToken = RequireToken(Parser, CTokenType_Identifier);
    switch (Macro->Type)
    {
      case type_macro_keyword:
      {
        PushParser(Parser, Macro->Parser, parser_push_type_macro);
        PushedParser = True;
      } break;

      case type_macro_function:
      {
        if (PeekToken(Parser).Type == CTokenType_OpenParen)
        {
          Assert(Result->Value == 0);
          Result->Value = ParseFunctionCall(Ctx, MacroNameToken.Value);
        }
        else
        {
          ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result->Value, Ctx->Memory);
          Node->Token = MacroNameToken;
        }
      } break;

      case type_macro_noop: { InvalidCodePath(); } break;
    }

    return PushedParser;
  }
#endif

  if ( PeekToken(Parser, 1).Type  == CTokenType_OpenParen )
  {
    c_token SymbolToken = RequireToken(Parser, CTokenType_Identifier);
    Assert(!Result->Value);
    Result->Value = ParseFunctionCall(Ctx, SymbolToken.Value);
  }
  else
  {
    datatype TestDatatype = GetDatatypeByName(&Ctx->Datatypes, T.Value);
    if ( SymbolIsNeverTypeSpecifier || TestDatatype.Type == type_datatype_noop )
    {
      ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result->Value, Ctx->Memory);
      Node->Token = RequireToken(Parser, CTokenType_Identifier);
    }
    else
    {
      ParseTypeSpecifierNode(Ctx, Result, &TestDatatype);
    }
  }

  return PushedParser;
}

bonsai_function void
ParseExpression(parse_context *Ctx, ast_node_expression* Result)
{
  parser *Parser = &Ctx->CurrentParser;

  b32 ParsePushedParser = False;
  do
  {
    ParsePushedParser = False;
    const c_token T = PeekToken(Parser);
    switch (T.Type)
    {
      case CTokenType_Meta:
      {
        RequireToken(Parser, CTokenType_Meta);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        ast_node_literal *Node = AllocateAndCastTo(ast_node_literal, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);
      } break;

      case CTokenType_Identifier:
      {
        ParsePushedParser = ParseSymbol(Ctx, Result, False);
      } break;

      case CTokenType_Arrow:
      case CTokenType_Dot:
      {
        ast_node_access *Node = AllocateAndCastTo(ast_node_access, &Result->Value, Ctx->Memory);
        Node->AccessType = RequireToken(Parser, T.Type);

        Node->Symbol = AllocateProtection(ast_node_expression, Ctx->Memory, 1, False);
        ParsePushedParser = ParseSymbol(Ctx, Node->Symbol);
      } break;

      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      {
        ParseTypeSpecifierNode(Ctx, Result);
      } break;

      case CTokenType_OpenParen:
      {
        RequireToken(Parser, T.Type);
        ast_node_parenthesized *Node = AllocateAndCastTo(ast_node_parenthesized, &Result->Value, Ctx->Memory);
        Node->Expr = ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_CloseParen);

        if (Node->Expr->Value->Type == type_ast_node_type_specifier)
        {
          Node->IsCast = True;
          Node->CastValue = ParseExpression(Ctx);
        }

      } break;

      case CTokenType_Ampersand:
      case CTokenType_Tilde:
      case CTokenType_Hat:
      case CTokenType_LT:
      case CTokenType_GT:
      case CTokenType_Bang:
      case CTokenType_LeftShift:
      case CTokenType_RightShift:
      case CTokenType_LessEqual:
      case CTokenType_GreaterEqual:
      case CTokenType_AreEqual:
      case CTokenType_NotEqual:
      case CTokenType_PlusEquals:
      case CTokenType_MinusEquals:
      case CTokenType_TimesEquals:
      case CTokenType_DivEquals:
      case CTokenType_ModEquals:
      case CTokenType_AndEquals:
      case CTokenType_OrEquals:
      case CTokenType_XorEquals:
      case CTokenType_Increment:
      case CTokenType_Decrement:
      case CTokenType_LogicalAnd:
      case CTokenType_LogicalOr:
      case CTokenType_Percent:
      case CTokenType_Pipe:
      case CTokenType_Plus:
      case CTokenType_Minus:
      case CTokenType_FSlash:
      case CTokenType_Star:
      case CTokenType_OpenBracket:
      {
        Assert(!Result->Value);
        ast_node_operator* Node = AllocateAndCastTo(ast_node_operator, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);
        Node->Operand = ParseExpression(Ctx);
        if (T.Type == CTokenType_OpenBracket)
          { RequireToken(Parser, CTokenType_CloseBracket); }
      } break;

      case CTokenType_Question:
      {
        ast_node_operator* Node = AllocateAndCastTo(ast_node_operator, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);
        Node->Operand = ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_Colon);
        ParseExpression(Ctx); // TODO(Jesse id: 260): Currently ignoring the second half of a ternary .. we should probably not do this
      } break;

      case CTokenType_CharLiteral:
      case CTokenType_StringLiteral:
      {
        Assert(!Result->Value);
        ast_node_literal *Node = AllocateAndCastTo(ast_node_literal, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);

        while ( PeekToken(Parser).Type == CTokenType_StringLiteral ||
                PeekToken(Parser).Type == CTokenType_Identifier )
        {
          c_token NextT = PopToken(Parser);
          switch (NextT.Type)
          {
            case CTokenType_StringLiteral:
            {
              Node->Token = CToken(CTokenType_StringLiteral, Concat(Node->Token.Value, NextT.Value, Ctx->Memory)); // TODO(Jesse, id: 264, tags: memory_leak): This should use a string builder
            } break;

            case CTokenType_Identifier:
            {
              // TODO(Jesse id: 264): Once we have proper macro expansion, this can be expanded and concatenated to the string as well.
              macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, NextT.Value);
              switch(Macro->Type)
              {
                case type_macro_keyword:
                {
                } break;

                case type_macro_function:
                {
                  EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
                } break;

                case type_macro_noop: { InvalidCodePath(); } break;
              }
            } break;

            InvalidDefaultWhileParsing(Parser, CSz("Expected macro identifier or string literal."));
          }
        }
      } break;

      case CTokenType_Colon:
      case CTokenType_Semicolon:
      case CTokenType_Comma:
      case CTokenType_CloseBracket:
      case CTokenType_CloseParen:
      case CTokenType_Equals:
      {
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token while parsing expression."));
    }
  } while (ParsePushedParser);

  if (NextTokenIsOperator(Parser))
  {
    Assert(!Result->Next);
    Result->Next = ParseExpression(Ctx);
  }
}

// TODO(Jesse id: 316): These paths are exactly the same .. they should probably use the same API ..?
bonsai_function ast_node_expression*
ParseExpression(parse_context *Ctx)
{
  ast_node_expression *Result = AllocateProtection(ast_node_expression, Ctx->Memory, 1, False);
  ParseExpression(Ctx, Result);
  return Result;
}

bonsai_function void
ParseExpression(parse_context *Ctx, ast_node **Result)
{
  ast_node_expression *Node = AllocateAndCastTo(ast_node_expression, Result, Ctx->Memory);
  ParseExpression(Ctx, Node);
}

#if 0
bonsai_function type_spec
ReduceToTypeSpec(ast_node* InputNode, ast_node_variable_def_stream *Locals)
{
  type_spec Result = {};

  ast_node* Current = InputNode;

  while (Current)
  {
    switch (Current->Type)
    {
      InvalidDefaultCase;

      case type_ast_node_symbol:
      {
        ast_node_symbol *Symbol = SafeCast(ast_node_symbol, Current);
        Assert(Symbol->Token.Type == CTokenType_Identifier);
        ast_node_variable_def *LocalVarDecl = GetByTypeName(Symbol->Token.Value, Locals);

        if (ResultType->Name.Count)
        {
          if (LocalVarDecl)
          {
            if (StringsMatch(ResultType->Name, LocalVarDecl->Decl.Type.Name))
            {
            }
            else
            {
              Error("Type mismatch");
              RuntimeBreak();
            }
          }
          else
          {
            Error("Unable to find local variable decl");
          }
        }
        else
        {
          if (LocalVarDecl)
          {
            ResultType->Name = LocalVarDecl->Decl.Type.Name;
          }
          else
          {
            Error("Unable to find local variable decl");
          }
        }

      } break;

      case type_ast_node_function_call:
      {
        ast_node_function_call *Node = SafeCast(ast_node_function_call, Current);
      } return;

      case type_ast_node_scope:
      {
      } break;

      case type_ast_node_ignored:
      {
      } break;
    }
  }

  return Result;
}
#endif

bonsai_function ast_node*
ParseFunctionCall(parse_context *Ctx, counted_string FunctionName)
{
  parser *Parser = &Ctx->CurrentParser;

  ast_node *Result = {};
  ast_node_function_call *Node = AllocateAndCastTo(ast_node_function_call, &Result, Ctx->Memory);

  Node->Name = FunctionName;
  Node->Prototype = GetByName(Node->Name, &Ctx->Datatypes.Functions);

  RequireToken(Parser, CTokenType_OpenParen);

  b32 Done = False;
  while (!Done)
  {
    ast_node_expression Arg = {};
    ParseExpression(Ctx, &Arg);
    Push(&Node->Args, Arg, Ctx->Memory);

    if(OptionalToken(Parser, CTokenType_Comma))
    {
    }
    else
    {
      RequireToken(Parser, CTokenType_CloseParen);
      Done = True;
    }
  }


  return Result;
}

bonsai_function void
ParseDatatypes(parse_context *Ctx)
{
  parser *Parser = &Ctx->CurrentParser;
  program_datatypes* Datatypes = &Ctx->Datatypes;
  memory_arena* Memory = Ctx->Memory;

  while (TokensRemain(Parser))
  {
    c_token T = PeekToken(Parser);

    switch(T.Type)
    {
      case CT_Pragma:
      {
        RequireToken(Parser, T);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CT_PreprocessorIf:
      case CT_PreprocessorElif:
      {
        RequireToken(Parser, T.Type);
        if (ResolveMacroConstantExpression(Parser) == 0)
        {
          EatIfBlock(Parser);
        }
      } break;

      case CT_PreprocessorDefine:
      case CT_PreprocessorIfDefined:
      case CT_PreprocessorIfNotDefined:
      case CT_PreprocessorElse:
      case CT_PreprocessorEndif:
      case CT_PreprocessorUndef:
      case CT_PreprocessorPragma:
      case CT_PreprocessorError:
      case CT_PreprocessorWarning:
      {
        RequireToken(Parser, T.Type);
        EatUntilIncluding(Parser, CTokenType_Newline);
      } break;

      case CTokenType_Meta:
      {
        RequireToken(Parser, CTokenType_Meta);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_Semicolon:
      {
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_Struct:
      case CTokenType_Enum:
      case CTokenType_Union:
      {
        ParseDatatypeDef(Ctx);
      } break;

      case CTokenType_Typedef:
      {
        ParseTypedef(Ctx);
      } break;

      case CTokenType_Using:
      {
        EatUntilIncluding(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_TemplateKeyword:
      case CTokenType_Extern:
      case CTokenType_Inline:
      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Identifier:
      {
        // We ignore the result of this .. because we're just looking to push
        // functions and push them onto the program_datatypes stream
        ParseFunctionOrVariableDecl(Ctx);

      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing a datatype."));
    }

    EatWhitespaceAndComments(Parser);
    continue;
  }

  return;
}

bonsai_function parser_cursor
AllocateTokenizedFiles(u32 Count, memory_arena* Memory)
{
  parser* Start = Allocate(parser, Memory, Count);
  parser_cursor Result = {
    .Start = Start,
    .At = Start,
    .End = Start + Count,
  };

  return Result;
}

bonsai_function void
GoGoGadgetMetaprogramming(parse_context* Ctx, todo_list_info* TodoInfo);

bonsai_function void
FlushOutputToDisk(parse_context *Ctx, counted_string OutputForThisParser, counted_string NewFilename, todo_list_info* TodoInfo, memory_arena* Memory, b32 IsInlineCode = False)
{
  TIMED_FUNCTION();

  parser *Parser = &Ctx->CurrentParser;

  if (!Parser->Valid)
  {
    ParseError(Parser, CSz("Bad parser state."));
    return;
  }

  counted_string OutputPath = {};

  // TODO(Jesse id: 182, tags: high_priority) This should respect Args.Outpath passed in!
  if ( PeekTokenRaw(Parser).Type == CTokenType_Newline &&
       PeekTokenRaw(Parser, 1).Type == CT_PreprocessorInclude )
  {
    RequireToken(Parser, CToken(CT_PreprocessorInclude));
    RequireToken(Parser, CTokenType_LT);
    RequireToken(Parser, CToken(CS("metaprogramming")));
    RequireToken(Parser, CTokenType_FSlash);
    RequireToken(Parser, CToken(CS("output")));
    RequireToken(Parser, CTokenType_FSlash);
    counted_string IncludeFilename = RequireToken(Parser, CTokenType_Identifier).Value;

    string_builder IncludePathBuilder = {};
    Append(&IncludePathBuilder, CS("src/metaprogramming/output/"));
    Append(&IncludePathBuilder, IncludeFilename);

    if (OptionalToken(Parser, CTokenType_Dot))
    {
      Append(&IncludePathBuilder, CS("."));
      counted_string Extension = RequireToken(Parser, CTokenType_Identifier).Value;
      Append(&IncludePathBuilder, Extension);
    }

    RequireToken(Parser, CTokenType_GT);

    OutputPath = Finalize(&IncludePathBuilder, Memory);
    Output(OutputForThisParser, OutputPath, Memory);
  }
  else
  {
    // TODO(Jesse id: 183, tags: high_priority) This should respect Args.OutPath passed in!
    OutputPath = Concat(CS("src/metaprogramming/output/"), NewFilename, Memory);
    Output(OutputForThisParser, OutputPath, Memory);

    Push(CToken(CTokenType_Newline),     &Parser->OutputTokens);
    Push(CToken(CTokenType_Hash),        &Parser->OutputTokens);
    Push(CToken(CS("include")),          &Parser->OutputTokens);
    Push(CToken(CTokenType_Space),       &Parser->OutputTokens);

    Push(CToken(CTokenType_LT),          &Parser->OutputTokens);
    Push(CToken(CS("metaprogramming")),  &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash),      &Parser->OutputTokens);
    Push(CToken(CS("output")),           &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash),      &Parser->OutputTokens);
    Push(CToken(NewFilename),            &Parser->OutputTokens);
    Push(CToken(CTokenType_GT),          &Parser->OutputTokens);
  }

  parser *OutputParse = TokenizeAnsiStream(Ctx, AnsiStream(OutputForThisParser, OutputPath));

  if (IsInlineCode)
  {
    // TODO(Jesse, id: 226, tags: metaprogramming, output): Should we handle this differently?
    Warn("Not parsing datatypes in inlined code for %.*s", (u32)OutputPath.Count, OutputPath.Start);
  }
  else
  {
    // What do we do here?
    NotImplemented;
    /* ParseDatatypes(Ctx); */
    /* Push(&Ctx->AllParsers, OutputParse, Memory); */
  }

  /* PushParser(&Ctx->CurrentParser, OutputParse, parser_push_type_include); */
  /* GoGoGadgetMetaprogramming(Ctx, TodoInfo); */

  return;
}

// TODO(Jesse, id: 113, tags: cleanup): Remove these?
debug_global platform Plat = {};
debug_global os Os = {};

/* TODO(Jesse, id: 112, tags: bootstrap_debug_system, copy_paste): This is
 * copy-pasted from the callgraph tests .. should we be able to call this from
 * anywhere?  It's also in the platform layer
 */
bonsai_function b32
BootstrapDebugSystem(b32 OpenDebugWindow)
{
  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  if (OpenDebugWindow)
  {
    s32 DebugFlags = GLX_CONTEXT_DEBUG_BIT_ARB;
    b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat, DebugFlags);
    if (!WindowSuccess) { Error("Initializing Window :( "); return False; }
    Assert(Os.Window);
    AssertNoGlErrors;

    InitializeOpenGlExtensions(&Os);

    b32 ShadingLanguageIsRecentEnough = CheckShadingLanguageVersion();
    if (!ShadingLanguageIsRecentEnough) { return False; }
  }

  debug_init_debug_system_proc InitDebugSystem = (debug_init_debug_system_proc)GetProcFromLib(DebugLib, "InitDebugSystem");
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }
  InitDebugSystem(OpenDebugWindow);

  debug_state* DebugState = GetDebugState();
  DebugState->DebugDoScopeProfiling = True;
  DebugState->Plat = &Plat;

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClearDepth(1.0f);

  glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return True;
}

bonsai_function meta_func_arg*
StreamContains(meta_func_arg_stream* Stream, counted_string Match)
{
  meta_func_arg* Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func_arg* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Match, Match))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function meta_func*
StreamContains(meta_func_stream* Stream, counted_string Name)
{
  meta_func* Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function counted_string*
StreamContains(counted_string_stream* Stream, counted_string Name)
{
  counted_string* Result = {};
  ITERATE_OVER(Stream)
  {
    counted_string* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function person*
StreamContains(person_stream* People, counted_string Name)
{
  person* Result = {};
  ITERATE_OVER(People)
  {
    person* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function tagged_counted_string_stream*
StreamContains(tagged_counted_string_stream_stream* Stream, counted_string Tag)
{
  tagged_counted_string_stream* Result = {};
  ITERATE_OVER(Stream)
  {
    tagged_counted_string_stream* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Tag, Tag))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function todo*
StreamContains(todo_stream* Todos, counted_string TodoId)
{
  todo* Result = {};
  ITERATE_OVER(Todos)
  {
    todo* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Id, TodoId))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function tag*
StreamContains(tag_stream* TodoLists, counted_string Tag)
{
  tag* Result = {};
  ITERATE_OVER(TodoLists)
  {
    tag* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Tag, Tag))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function todo*
UpdateTodo(todo_stream* Stream, todo Todo, memory_arena* Memory)
{
  todo* Result = StreamContains(Stream, Todo.Id);
  if (Result)
  {
    Result->Value = Todo.Value;
    Result->FoundInCodebase = Todo.FoundInCodebase;
  }
  else
  {
    Push(Stream, Todo, Memory);
    Result = StreamContains(Stream, Todo.Id);
  }
  return Result;
}

bonsai_function tag*
GetExistingOrCreate(tag_stream* Stream, counted_string Tag, memory_arena* Memory)
{
  tag* Result = StreamContains(Stream, Tag);
  if (!Result)
  {
    tag NewTag = { .Tag = Tag };
    Push(Stream, NewTag, Memory);
    Result = StreamContains(Stream, Tag);
  }
  return Result;
}

bonsai_function person*
GetExistingOrCreate(person_stream* People, counted_string PersonName, memory_arena* Memory)
{
  person* Person = StreamContains(People, PersonName);
  if (!Person)
  {
    person NewPerson = { .Name = PersonName };
    Push(People, NewPerson, Memory);
    Person = StreamContains(People, PersonName);
  }
  return Person;
}

static u32 LargestIdFoundInFile = 0;

bonsai_function todo
Todo(counted_string Id, counted_string Value, b32 FoundInCodebase)
{
  todo Result = { .Id = Id, .Value = Value, .FoundInCodebase = FoundInCodebase };
  return Result;
}

bonsai_function person_stream
ParseAllTodosFromFile(counted_string Filename, memory_arena* Memory)
{
  person_stream People = {};

  parser *Parser = TokenizeAnsiStream(AnsiStreamFromFile(Filename, Memory), Memory, True, 0);

  while (Remaining(&Parser->Tokens))
  {
    RequireToken(Parser, CTokenType_Hash);
    counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;

    person* Person = GetExistingOrCreate(&People, PersonName, Memory);
    while (OptionalToken(Parser, CT_PreprocessorPaste))
    {
      counted_string TagName = RequireToken(Parser, CTokenType_Identifier).Value;

      tag* Tag = GetExistingOrCreate(&Person->Tags, TagName, Memory);
      while (OptionalToken(Parser, CTokenType_Minus))
      {
        RequireToken(Parser, CTokenType_Hash);
        c_token TodoId = RequireToken(Parser, CTokenType_IntLiteral);

        LargestIdFoundInFile = Max(LargestIdFoundInFile, (u32)TodoId.UnsignedValue);
        counted_string TodoValue = Trim(ConcatTokensUntilNewline(Parser, Memory));
        todo NewTodo = Todo(TodoId.Value, TodoValue, False);
        Push(&Tag->Todos, NewTodo, Memory);
        EatWhitespace(Parser);
      }

      EatWhitespace(Parser);
    }

    EatWhitespace(Parser);
  }

  return People;
}

bonsai_function counted_string
GenerateOutfileNameFor(counted_string Name, counted_string DatatypeName, memory_arena* Memory, counted_string Modifier = {})
{
  string_builder OutfileBuilder = {};
  Append(&OutfileBuilder, Name);
  Append(&OutfileBuilder, CSz("_"));
  Append(&OutfileBuilder, DatatypeName);
  if (Modifier.Count)
  {
    Append(&OutfileBuilder, CSz("_"));
    Append(&OutfileBuilder, Modifier);
  }
  Append(&OutfileBuilder, CSz(".h"));
  counted_string Result = Finalize(&OutfileBuilder, Memory);

  return Result;
}

bonsai_function counted_string
GetNameForStructMember(struct_member* Decl)
{
  counted_string Result = {};

  switch (Decl->Type)
  {
    case type_function_decl:
    {
      Result = Decl->function_decl.Name;
    } break;

    case type_variable_decl:
    {
      Result = Decl->variable_decl.Name;
    } break;

    case type_struct_member_anonymous:
    {
      Result = CSz("(anonymous struct or union)");
    } break;

    InvalidDefaultCase;
  }

  return Result;
}

bonsai_function counted_string
Transform(meta_transform_op Transformations, counted_string Input, memory_arena* Memory)
{
  counted_string Result = Input;
  while ( Transformations )
  {
    if ( Transformations & to_capital_case )
    {
      UnsetBitfield(meta_transform_op, Transformations, to_capital_case );
      Result = ToCapitalCase(Result, Memory);
    }
  }

  return Result;
}

bonsai_function counted_string
EscapeDoubleQuotes(counted_string S, memory_arena* Memory)
{
  string_builder B = {};
  for (u32 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    if (S.Start[CharIndex] == '"')
    {
      Append(&B, CSz("\\"));
    }
    Append(&B, CS(S.Start+CharIndex, 1));
  }

  counted_string Result = Finalize(&B, Memory);
  return Result;
}

bonsai_function void
AppendAndEscapeInteriorOfDoubleQuotedString(string_builder* Builder, counted_string S)
{
  if (S.Count >= 2 &&
      S.Start[0] == '"' &&
      S.Start[S.Count-1] == '"')
  {
    S.Count -= 2;
    ++S.Start;
    Append(Builder, CSz("\""));
    Append(Builder, EscapeDoubleQuotes(S, Builder->Memory));
    Append(Builder, CSz("\""));
  }
  else
  {
    Append(Builder, EscapeDoubleQuotes(S, Builder->Memory));
  }
}

bonsai_function meta_transform_op
ParseTransformations(parser* Scope)
{
  meta_transform_op Result = {};

  while (OptionalToken(Scope, CTokenType_Dot))
  {
    meta_transform_op NextOp = MetaTransformOp(RequireToken(Scope, CTokenType_Identifier).Value);
    if (NextOp != meta_transform_op_noop)
    {
      Result = (meta_transform_op)(Result | NextOp);
    }
    else
    {
      Error("Parsing transform ops.");
    }
  }

  return Result;
}

bonsai_function meta_func_arg
ReplacementPattern(counted_string Match, datatype Data)
{
  meta_func_arg Result = {
    .Match = Match,
    .Data = Data
  };

  return Result;
}

bonsai_function meta_func_arg_stream
CopyStream(meta_func_arg_stream* Stream, memory_arena* Memory)
{
  meta_func_arg_stream Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func_arg* Element = GET_ELEMENT(Iter);
    Push(&Result, *Element, Memory);
  }
  return Result;
}

bonsai_function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, parse_context* Ctx, memory_arena* Memory);

// TODO(Jesse id: 222, tags: immediate, parsing, metaprogramming) : Re-add [[nodiscard]] here
bonsai_function counted_string
Execute(counted_string FuncName, parser Scope, meta_func_arg_stream* ReplacePatterns, parse_context* Ctx, memory_arena* Memory)
{
  program_datatypes* Datatypes = &Ctx->Datatypes;
  meta_func_stream* FunctionDefs = &Ctx->MetaFunctions;

  Rewind(&Scope.Tokens);

  string_builder OutputBuilder = {};
  while (Remaining(&Scope.Tokens))
  {
    c_token BodyToken = PopTokenRaw(&Scope);

    if ( BodyToken.Type == CTokenType_StringLiteral )
    {
      parser *StringParse = TokenizeAnsiStream(AnsiStream(BodyToken.Value, Scope.Filename), Memory, True, Ctx);

      counted_string Code = Execute(FuncName, *StringParse, ReplacePatterns, Ctx, Memory);
      AppendAndEscapeInteriorOfDoubleQuotedString(&OutputBuilder, Code);
    }
    else if ( BodyToken.Type == CTokenType_OpenParen )
    {
      b32 ExecutedChildFunc = False;
      ITERATE_OVER_AS(Replace, ReplacePatterns)
      {
        meta_func_arg* Replace = GET_ELEMENT(ReplaceIter);
        if ( OptionalToken(&Scope, CToken(Replace->Match)) )
        {
          ExecutedChildFunc = True;
          RequireToken(&Scope, CTokenType_Dot);
          meta_arg_operator Operator = MetaArgOperator( RequireToken(&Scope, CTokenType_Identifier).Value);

          switch (Operator)
          {
            case meta_arg_operator_noop:
            {
              ParseError(&Scope, CSz("Invalid operator encountered."));
            } break;

            case is_struct:
            {
              RequireToken(&Scope, CTokenType_Question);
              parser StructScope = GetBodyTextForNextScope(&Scope);
              if (Replace->Data.Type == type_struct_def)
              {
                counted_string Code = Execute(FuncName, StructScope, ReplacePatterns, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            } break;

            case is_enum:
            {
              RequireToken(&Scope, CTokenType_Question);
              parser EnumScope = GetBodyTextForNextScope(&Scope);
              switch (Replace->Data.Type)
              {
                case type_enum_member:
                case type_enum_def:
                {
                  counted_string Code = Execute(FuncName, EnumScope, ReplacePatterns, Ctx, Memory);
                  Append(&OutputBuilder, Code);
                } break;

                case type_struct_member:
                {
                  switch (Replace->Data.struct_member->Type)
                  {
                    case type_variable_decl:
                    {
                      enum_def *E = GetEnumByType(&Datatypes->Enums, Replace->Data.struct_member->variable_decl.Type.Token.Value);
                      if (E)
                      {
                        meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                        ITERATE_OVER_AS(NewReplace, &NewArgs)
                        {
                          meta_func_arg* NewReplace = GET_ELEMENT(NewReplaceIter);
                          if (StringsMatch(Replace->Match, NewReplace->Match))
                          {
                            NewReplace->Data = Datatype(E);
                          }
                        }

                        counted_string Code = Execute(FuncName, EnumScope, &NewArgs, Ctx, Memory);
                        Append(&OutputBuilder, Code);
                      }
                    } break;

                    case type_struct_member_noop:
                    case type_struct_member_anonymous:
                    case type_function_decl:
                    {
                    }
                  }
                } break;

                case type_primitive_def:
                case type_struct_def:
                {
                } break;

                case type_stl_container_def:
                case type_type_def:
                {
                  NotImplemented;
                } break;

                case type_datatype_noop:
                {
                  InvalidCodePath();
                } break;
              }

            } break;

            case value:
            {
              if (Replace->Data.Type == type_enum_member)
              {
                // Changed enum_member::Value from counted_string to
                // ast_node_expression, and don't have a string representation
                // so this is going away for a bit..
                //

                NotImplemented;

                /* meta_transform_op Transformations = ParseTransformations(&Scope); */
                /* counted_string Name = Transform(Transformations, Replace->Data.enum_member->Value, Memory); */
                /* Append(&OutputBuilder, Name); */
              }
              else
              {
                Error("Called .value on non-enum_member type.");
              }
            } break;

            case type:
            {
              meta_transform_op Transformations = ParseTransformations(&Scope);
              counted_string TypeName = {};
              switch (Replace->Data.Type)
              {
                case type_datatype_noop: { InvalidCodePath(); } break;

                case  type_type_def:
                {
                  NotImplemented;
                } break;

                case type_stl_container_def:
                {
                  NotImplemented;
                } break;

                case type_primitive_def:
                {
                  NotImplemented;
                  // Does this actually work?
                  // TypeName = Replace->Data.primitive_def->Type.SourceText;
                } break;

                case type_struct_def:
                {
                  TypeName = Replace->Data.struct_def->Type;
                } break;

                case type_enum_def:
                {
                  TypeName = Replace->Data.enum_def->Name;
                } break;

                case type_enum_member:
                {
                  TypeName = Replace->Data.enum_member->Name;
                } break;

                case type_struct_member:
                {
                  switch (Replace->Data.struct_member->Type)
                  {
                    case type_struct_member_noop: { InvalidCodePath(); } break;

                    case type_variable_decl:
                    {
                      TypeName = Replace->Data.struct_member->variable_decl.Type.Token.Value;
                    } break;

                    case type_function_decl:
                    {
                      TypeName = Replace->Data.struct_member->function_decl.Name;
                    } break;

                    case type_struct_member_anonymous:
                    {
                      TypeName = CSz("(anonymous struct/union)");
                    } break;
                  }

                } break;

              }

              counted_string Name = Transform(Transformations, TypeName, Memory);
              Append(&OutputBuilder, Name);
            } break;

            case name:
            {
              switch (Replace->Data.Type)
              {
                case type_struct_member:
                {
                  counted_string MemberName = GetNameForStructMember(Replace->Data.struct_member);
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, MemberName, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_enum_member:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, Replace->Data.enum_member->Name, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_enum_def:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, Replace->Data.enum_def->Name, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_struct_def:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, Replace->Data.struct_def->Type, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                InvalidDefaultCase;
              }

            } break;

            case map_members:
            {
              RequireToken(&Scope, CTokenType_OpenParen);
              counted_string MatchPattern  = RequireToken(&Scope, CTokenType_Identifier).Value;
              RequireToken(&Scope, CTokenType_CloseParen);

              counted_string ContainingConstraint = {};
              counted_string ChildName = {};
              if ( OptionalToken(&Scope, CTokenType_Dot) )
              {
                RequireToken(&Scope, CToken(CSz("containing")));
                RequireToken(&Scope, CTokenType_OpenParen);
                ContainingConstraint = RequireToken(&Scope, CTokenType_Identifier).Value;

                if ( OptionalToken(&Scope, CToken(CSz("as"))) )
                {
                  ChildName = RequireToken(&Scope, CTokenType_Identifier).Value;
                }

                RequireToken(&Scope, CTokenType_CloseParen);
              }

              parser MapMemberScope = GetBodyTextForNextScope(&Scope);

              if (Replace->Data.Type == type_struct_def)
              {
                ITERATE_OVER_AS(Member, &Replace->Data.struct_def->Members)
                {
                  struct_member* Member = GET_ELEMENT(MemberIter);

                  switch (Member->Type)
                  {
                    case type_struct_member_noop:
                    {
                      InvalidCodePath();
                    } break;

                    case type_function_decl:
                    {
                    } break;

                    case type_variable_decl:
                    {
                      if ( ContainingConstraint.Count &&
                           !StringsMatch(Member->variable_decl.Type.Token.Value, ContainingConstraint) )
                      {
                        // Containing constraint failed
                      }
                      else
                      {
                        meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                        Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Member)), Memory);
                        counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, Ctx, Memory);
                        Append(&OutputBuilder, StructFieldOutput);
                      }

                    } break;

                    case type_struct_member_anonymous:
                    {
                      for (struct_member_iterator UnionMemberIter = Iterator(&Member->struct_member_anonymous.Body.Members);
                          IsValid(&UnionMemberIter);
                          Advance(&UnionMemberIter))
                      {
                        struct_member* UnionMember = GET_ELEMENT(UnionMemberIter);
                        if (UnionMember->Type == type_variable_decl)
                        {
                          struct_def* Struct = GetStructByType(&Datatypes->Structs, UnionMember->variable_decl.Type.Token.Value);
                          if (Struct)
                          {
                            struct_member_stream ContainedMembers = MembersOfType(Struct, ContainingConstraint, Memory);
                            if (ContainedMembers.FirstChunk)
                            {
                              meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                              Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Struct)), Memory);
                              if (ChildName.Count) {
                                struct_def SyntheticStruct = {
                                  .Type = CSz("synthetically_created_struct"),
                                  .DefinedInFile = Struct->DefinedInFile,
                                  .Members = ContainedMembers,
                                };
                                Push(&NewArgs, ReplacementPattern(ChildName, Datatype(&SyntheticStruct)), Memory);
                              }
                              counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, Ctx, Memory);
                              Append(&OutputBuilder, StructFieldOutput);
                            }
                          }
                          else
                          {
                            counted_string Name = UnionMember->variable_decl.Type.Token.Value;
                            counted_string ParentStructName = Replace->Data.struct_def->Type;
                            Warn("Couldn't find struct type '%.*s' in union parent '%.*s'.",
                                (u32)Name.Count, Name.Start,
                                (u32)ParentStructName.Count, ParentStructName.Start);
                          }

                        }
                        else
                        {
                          Error("Nested structs/unions and bonsai_function pointers unsupported.");
                        }
                      }
                    } break;
                  }

                  continue;
                }
              }
              else
              {
                Error("Called map_members on a datatype that wasn't a struct - %.*s", (u32)Replace->Data.enum_def->Name.Count, Replace->Data.enum_def->Name.Start);
              }

            } break;

            case map_values:
            {
              RequireToken(&Scope, CTokenType_OpenParen);
              counted_string EnumValueMatch  = RequireToken(&Scope, CTokenType_Identifier).Value;
              RequireToken(&Scope, CTokenType_CloseParen);
              parser NextScope = GetBodyTextForNextScope(&Scope);

              if (Replace->Data.Type == type_enum_def)
              {
                ITERATE_OVER(&Replace->Data.enum_def->Members)
                {
                  enum_member* EnumMember = GET_ELEMENT(Iter);
                  meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                  Push(&NewArgs, ReplacementPattern(EnumValueMatch, Datatype(EnumMember)), Memory);
                  counted_string EnumFieldOutput = Execute(FuncName, NextScope, &NewArgs, Ctx, Memory);
                  Append(&OutputBuilder, EnumFieldOutput);
                  continue;
                }

              }
              else
              {
                Error("Called map_values on a datatype that wasn't an enum - %.*s", (u32)Replace->Data.struct_def->Type.Count, Replace->Data.struct_def->Type.Start);
              }

            } break;
          }

          RequireToken(&Scope, CTokenType_CloseParen);
        }
      }

      meta_func* NestedFunc = StreamContains( FunctionDefs, PeekToken(&Scope).Value );
      if (NestedFunc)
      {
        RequireToken(&Scope, CToken(NestedFunc->Name));

        RequireToken(&Scope, CTokenType_OpenParen);

        counted_string ArgName = RequireToken(&Scope, CTokenType_Identifier).Value;
        meta_func_arg* Arg = StreamContains(ReplacePatterns, ArgName);

        if (Arg)
        {
          RequireToken(&Scope, CTokenType_CloseParen);
          RequireToken(&Scope, CTokenType_CloseParen);

          if (!StringsMatch(NestedFunc->Name, FuncName))
          {
            meta_func_arg_stream NewArgs = {};
            Push(&NewArgs, ReplacementPattern(NestedFunc->ArgName, Arg->Data), Memory);
            counted_string NestedCode = Execute(NestedFunc, &NewArgs, Ctx, Memory);
            Append(&OutputBuilder, NestedCode);
          }
          else
          {
            Error("Recursive bonsai_function calls not supported.");
          }
        }
        else
        {
          ParseError(&Scope, CSz("Unable to resolve bonsai_function argument."));
        }

      }
      else if (ExecutedChildFunc)
      {
      }
      else
      {
        Append(&OutputBuilder, BodyToken.Value);
      }
    }
    else
    {
      Append(&OutputBuilder, BodyToken.Value);
    }

    continue;
  }

  counted_string Result = Finalize(&OutputBuilder, Memory);
  return Result;
}

bonsai_function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, parse_context* Ctx, memory_arena* Memory)
{
  counted_string Result = Execute(Func->Name, Func->Body, Args, Ctx, Memory);
  return Result;
}

bonsai_function void
ConcatStreams(counted_string_stream* S1, counted_string_stream* S2, memory_arena* Memory)
{
  ITERATE_OVER(S2)
  {
    counted_string* Element = GET_ELEMENT(Iter);
    Push(S1, *Element, Memory);
  }
  return;
}

bonsai_function b32
IsMetaprogrammingOutput(counted_string Filename, counted_string OutputDirectory)
{
  b32 Result = Contains(Filename, OutputDirectory);
  return Result;
}

bonsai_function counted_string_stream
ParseDatatypeList(parser* Parser, program_datatypes* Datatypes, tagged_counted_string_stream_stream* NameLists, memory_arena* Memory)
{
  counted_string_stream Result = {};
  while (PeekToken(Parser).Type == CTokenType_Identifier)
  {
    counted_string DatatypeName    = RequireToken(Parser, CTokenType_Identifier).Value;

    struct_def* Struct                 = GetStructByType(&Datatypes->Structs, DatatypeName);
    enum_def* Enum                     = GetEnumByType(&Datatypes->Enums, DatatypeName);
    tagged_counted_string_stream* List = StreamContains(NameLists, DatatypeName);

    if (Struct || Enum)
    {
      Push(&Result, DatatypeName, Memory);
    }
    else if (List)
    {
      ConcatStreams(&Result, &List->Stream, Memory);
    }
    else
    {
      Warn("Type (%.*s) could not be resolved to a struct or named_list, ignoring it.", (u32)DatatypeName.Count, DatatypeName.Start);
    }

    OptionalToken(Parser, CTokenType_Comma);
  }

  return Result;
}

bonsai_function meta_func
ParseMetaFunctionDef(parser* Parser, counted_string FuncName)
{
  RequireToken(Parser, CTokenType_OpenParen);
  counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
  RequireToken(Parser, CTokenType_CloseParen);
  parser Body = GetBodyTextForNextScope(Parser);

  meta_func Func = {
    .Name = FuncName,
    .ArgName = ArgName,
    .Body = Body,
  };

  return Func;
}

#if 0
bonsai_function void
RemoveAllMetaprogrammingOutput(parser_cursor* ParsedFiles, arguments* Args)
{

  for (u32 ParserIndex = 0;
      ParserIndex < Count(ParsedFiles);
      ++ParserIndex)
  {
    parser* Parser = ParsedFiles->Start+ParserIndex;
    if (IsMetaprogrammingOutput(Parser->Filename, Args->Outpath))
    {
      Info("Removing %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
      Remove(Parser->Filename);
      continue;
    }
  }

}
#endif

bonsai_function counted_string
ParseMultiLineTodoValue(parser* Parser, memory_arena* Memory)
{
  string_builder Builder = {};

  while (PeekTokenRaw(Parser).Type != CTokenType_CommentMultiLineEnd)
  {
    c_token T = PopTokenRaw(Parser);

    if (T.Type == CTokenType_Newline)
    {
      EatWhitespace(Parser);
      if (OptionalToken(Parser, CTokenType_Star))
      {
        EatWhitespace(Parser);
      }
      Append(&Builder, CSz(" "));
    }
    else
    {
      Append(&Builder, T.Value);
    }

    continue;
  }

  Ensure( PopTokenRaw(Parser).Type == CTokenType_CommentMultiLineEnd );

  counted_string Result = Trim(Finalize(&Builder, Memory));
  return Result;
}

bonsai_function void
GoGoGadgetMetaprogramming(parse_context* Ctx, todo_list_info* TodoInfo)
{
  program_datatypes *Datatypes   = &Ctx->Datatypes;
  meta_func_stream *FunctionDefs = &Ctx->MetaFunctions;
  memory_arena *Memory           = Ctx->Memory;

  person_stream* People = &TodoInfo->People;
  tagged_counted_string_stream_stream* NameLists = &TodoInfo->NameLists;

  parser *Parser = &Ctx->CurrentParser;
  while (TokensRemain(Parser))
  {
    const c_token T = PeekTokenRaw(Parser);

    switch( T.Type )
    {
      case CTokenType_CommentMultiLineStart:
      case CTokenType_CommentSingleLine:
      {
        c_token CommentStartToken = PopTokenRaw(Parser);
        c_token FirstInteriorT = PeekToken(Parser);
        if ( StringsMatch(FirstInteriorT.Value, CSz("TODO")) )
        {
          RequireToken(Parser, CToken(CSz("TODO")));

          if (OptionalToken(Parser, CTokenType_OpenParen))
          {
            counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;
            counted_string_stream TodoTags = {};
            b32 GotAnyTags = False;

            counted_string IdValue = {};
            OptionalToken(Parser, CTokenType_Comma);

            b32 GeneratedNewId = False;
            if (OptionalToken(Parser, CToken(CSz("id"))))
            {
              RequireToken(Parser, CTokenType_Colon);
              IdValue = RequireToken(Parser, CTokenType_IntLiteral).Value;
            }
            else
            {
              Assert(!IdValue.Count);
              GeneratedNewId = True;
              IdValue = CS(++LargestIdFoundInFile);
              Push(CToken(CS(" id: ")), &Parser->OutputTokens);
              Push(CToken(IdValue),     &Parser->OutputTokens);
            }

            OptionalToken(Parser, CTokenType_Comma);

            if (StringsMatch(PeekToken(Parser).Value, CSz("tags")))
            {
              if (GeneratedNewId)
              {
                Push(CToken(CTokenType_Comma), &Parser->OutputTokens);
              }

              RequireToken(Parser, CToken(CSz("tags")));
              GotAnyTags = True;
              RequireToken(Parser, CTokenType_Colon);
              Push(&TodoTags, RequireToken(Parser, CTokenType_Identifier).Value, Memory);

              while (OptionalToken(Parser, CTokenType_Comma))
              {
                Push(&TodoTags, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
              }
            }

            if (!GotAnyTags)
            {
              counted_string Tag = CSz("untagged");
              Push(&TodoTags, Tag, Memory);
            }

            RequireToken(Parser, CTokenType_CloseParen);
            OptionalToken(Parser, CTokenType_Colon);

            counted_string TodoValue = T.Type == CTokenType_CommentSingleLine ?
              Trim(EatUntilIncluding(Parser, CTokenType_Newline)) :
              ParseMultiLineTodoValue(Parser, Memory);

            person* Person = GetExistingOrCreate(People, PersonName, Memory);

            ITERATE_OVER(&TodoTags)
            {
              counted_string* TodoTag = GET_ELEMENT(Iter);
              tag* Tag = GetExistingOrCreate(&Person->Tags, *TodoTag, Memory);
              UpdateTodo(&Tag->Todos, Todo(IdValue, TodoValue, True), Memory);
            }

          }
        }
        else
        {
          EatComment(Parser, CommentStartToken.Type);
        }

      } break;

      case CTokenType_Meta:
      {
        c_token MetaToken = RequireToken(Parser, CTokenType_Meta);
        RequireToken(Parser, CTokenType_OpenParen);

        if (OptionalToken(Parser, CTokenType_Ellipsis)) // Just ignore meta tags containing (...) .. It's probably the '#define meta(...)' thing
        {
          RequireToken(Parser, CTokenType_CloseParen);
          break;
        }

        counted_string DirectiveString = RequireToken(Parser, CTokenType_Identifier).Value;
        metaprogramming_directive Directive = MetaprogrammingDirective(DirectiveString);
        switch (Directive)
        {
          case polymorphic_func:
          {
            /* function_decl F = ParseFunctionDef(Parser, Memory); */
            /* DebugPrint(F); */
          } break;

          case func:
          {
            if (OptionalToken(Parser, CTokenType_OpenParen))
            {
              counted_string ArgType = RequireToken(Parser, CTokenType_Identifier).Value;
              counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
              RequireToken(Parser, CTokenType_CloseParen);


              parser Body = GetBodyTextForNextScope(Parser);

              meta_func Func = {
                .Name = CSz("anonymous_function"),
                .ArgName = ArgName,
                .Body = Body,
              };

              datatype Arg = GetDatatypeByName(&Ctx->Datatypes, ArgType);
              meta_func_arg_stream Args = {};
              Push(&Args, ReplacementPattern(ArgName, Arg), Memory);
              counted_string Code = Execute(&Func, &Args, Ctx, Memory);

              RequireToken(Parser, CTokenType_CloseParen);
              if (Code.Count)
              {
                counted_string OutfileName = GenerateOutfileNameFor(Func.Name, ArgType, Memory, GetRandomString(8, Hash(&Code), Memory));
                FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory, True);
              }
              else
              {
                Warn("Unable to generate code for meta_func %.*s", (u32)Func.Name.Count, Func.Name.Start);
              }
            }
            else
            {
              counted_string FuncName = RequireToken(Parser, CTokenType_Identifier).Value;
              meta_func Func = ParseMetaFunctionDef(Parser, FuncName);
              Push(FunctionDefs, Func, Memory);
            }

          } break;

          case named_list:
          {
            RequireToken(Parser, CTokenType_OpenParen);

            tagged_counted_string_stream NameList = {
              .Tag = RequireToken(Parser, CTokenType_Identifier).Value
            };

            RequireToken(Parser, CTokenType_CloseParen);

            RequireToken(Parser, CTokenType_OpenBrace);
            while (PeekToken(Parser).Type == CTokenType_Identifier)
            {
              counted_string Name = RequireToken(Parser, CTokenType_Identifier).Value;
              Push(&NameList.Stream, Name, Memory);
              OptionalToken(Parser, CTokenType_Comma);
            }

            RequireToken(Parser, CTokenType_CloseBrace);

            Push(&TodoInfo->NameLists, NameList, Memory);

          } break;

          case for_datatypes:
          {
            RequireToken(Parser, CTokenType_OpenParen);
            RequireToken(Parser, CToken(CSz("all")));
            RequireToken(Parser, CTokenType_CloseParen);

            counted_string_stream Excludes = {};
            if (OptionalToken(Parser, CTokenType_Dot))
            {
              RequireToken(Parser, CToken(CSz("exclude")));
              RequireToken(Parser, CTokenType_OpenParen);
              Excludes = ParseDatatypeList(Parser, Datatypes, NameLists, Memory);
              RequireToken(Parser, CTokenType_CloseParen);
            }

            RequireToken(Parser, CToken(ToString(func)));
            meta_func StructFunc = ParseMetaFunctionDef(Parser, CSz("for_datatypes_struct_callback"));

            RequireToken(Parser, CToken(ToString(func)));
            meta_func EnumFunc = ParseMetaFunctionDef(Parser, CSz("for_datatypes_enum_callback"));

            RequireToken(Parser, CTokenType_CloseParen);

            string_builder OutputBuilder = {};

            for (struct_def_iterator Iter = Iterator(&Datatypes->Structs);
                IsValid(&Iter);
                Advance(&Iter))
            {
              struct_def* Struct = &Iter.At->Element;

              if (!StreamContains(&Excludes, Struct->Type))
              {
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(StructFunc.ArgName, Datatype(Struct)), Memory);
                counted_string Code = Execute(&StructFunc, &Args, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            }

            for (enum_def_iterator Iter = Iterator(&Datatypes->Enums);
                IsValid(&Iter);
                Advance(&Iter))
            {
              enum_def* Enum = &Iter.At->Element;
              if (!StreamContains(&Excludes, Enum->Name))
              {
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(EnumFunc.ArgName, Datatype(Enum)), Memory);
                counted_string Code = Execute(&EnumFunc, &Args, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            }


            counted_string Code = Finalize(&OutputBuilder, Memory);
            counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), CSz("debug_print"), Memory);

            FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);
          } break;

          case d_union:
          {
            counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
            d_union_decl dUnion = ParseDiscriminatedUnion(Parser, Datatypes, DatatypeName, Memory);
            if (Parser->Valid)
            {
              counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), DatatypeName, Memory);

              string_builder CodeBuilder = {};
              if (!dUnion.CustomEnumType.Count)
              {
                counted_string EnumString = GenerateEnumDef(&dUnion, Memory);
                Append(&CodeBuilder, EnumString);
              }

              {
                counted_string StructString = GenerateStructDef(&dUnion, Memory);
                Append(&CodeBuilder, StructString);
              }

              counted_string Code = Finalize(&CodeBuilder, Memory);

              RequireToken(Parser, CTokenType_CloseParen);
              FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);

            }
            else
            {
              Error("Parsing d_union declaration");
            }
          } break;

          default:
          {
            meta_func* Func = StreamContains(FunctionDefs, DirectiveString);
            if (Func)
            {
              RequireToken(Parser, CTokenType_OpenParen);
              counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
              RequireToken(Parser, CTokenType_CloseParen);
              RequireToken(Parser, CTokenType_CloseParen);

              /* Info("Calling bonsai_function : %.*s(%.*s)", (u32)Func->Name.Count, Func->Name.Start, (u32)DatatypeName.Count, DatatypeName.Start); */
              datatype Arg = GetDatatypeByName(&Ctx->Datatypes, DatatypeName);
              meta_func_arg_stream Args = {};
              Push(&Args, ReplacementPattern(Func->ArgName, Arg), Memory);
              counted_string Code = Execute(Func, &Args, Ctx, Memory);

              if (Code.Count)
              {
                counted_string OutfileName = GenerateOutfileNameFor(Func->Name, DatatypeName, Memory);
                FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);
              }
              else
              {
                Warn("Unable to generate code for meta_func %.*s", (u32)Func->Name.Count, Func->Name.Start);
              }
            }
            else
            {
              Error("Couldn't resolve %.*s to a metaprogramming directive or bonsai_function name", (u32)DirectiveString.Count, DirectiveString.Start);
            }
          }

        }
      } break;

      default:
      {
        PopTokenRaw(Parser);
      } break;
    }

    continue;
  }
}

bonsai_function void
WriteTodosToFile(person_stream* People, memory_arena* Memory)
{
  random_series Rng = {.Seed = 123125};
  native_file TempFile = GetTempFile(&Rng, Memory);
  b32 AllWritesSucceeded = TempFile.Handle ? True : False;
  if (AllWritesSucceeded)
  {
    ITERATE_OVER_AS(person, People)
    {
      person* Person = GET_ELEMENT(personIter);
      AllWritesSucceeded &= WriteToFile(&TempFile, CSz("# "));
      AllWritesSucceeded &= WriteToFile(&TempFile, Person->Name);
      AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
      ITERATE_OVER(&Person->Tags)
      {
        tag* Tag = GET_ELEMENT(Iter);

        todo_iterator InnerIter = Iterator(&Tag->Todos);
        if (IsValid(&InnerIter))
        {
          AllWritesSucceeded &= WriteToFile(&TempFile, CSz("  ## "));
          AllWritesSucceeded &= WriteToFile(&TempFile, Tag->Tag);
          AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));

          for (;
              IsValid(&InnerIter);
              Advance(&InnerIter))
          {
            todo* Todo = GET_ELEMENT(InnerIter);
            if (Todo->FoundInCodebase)
            {
              AllWritesSucceeded &= WriteToFile(&TempFile, CSz("    - #"));
              AllWritesSucceeded &= WriteToFile(&TempFile, Todo->Id);
              AllWritesSucceeded &= WriteToFile(&TempFile, CSz(" "));
              AllWritesSucceeded &= WriteToFile(&TempFile, Todo->Value);
              AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
            }
          }

          AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
        }
      }
    }
  }
  else
  {
    Error("Opening Tempfile");
  }

  Rename(TempFile, CSz("todos.md"));
  CloseFile(&TempFile);

  return;
}

bonsai_function void WalkAst(ast_node* Ast);
bonsai_function void WalkAst(ast_node_statement* Ast);

bonsai_function void
WalkAst(ast_node_expression* Ast)
{
  if (Ast)
  {
    WalkAst(Ast->Value);
    WalkAst(Ast->Next);
  }
}

bonsai_function void
WalkAst(ast_node_statement* Ast)
{
  if (Ast)
  {
    WalkAst(Ast->LHS);
    WalkAst(Ast->RHS);
    WalkAst(Ast->Next);
  }
}

bonsai_function void
WalkAst(ast_node* Ast)
{
  if (Ast)
  {
    switch(Ast->Type)
    {
      case type_ast_node_function_call:
      {
        auto Child = SafeAccess(ast_node_function_call, Ast);
      } break;

      case type_ast_node_statement:
      {
        auto Child = SafeAccess(ast_node_statement, Ast);
        WalkAst(Child);
      } break;

      case type_ast_node_variable_def:
      {
        auto Child = SafeAccess(ast_node_variable_def, Ast);
        WalkAst(Child->Value);
      } break;

      case type_ast_node_access:
      {
        auto Child = SafeAccess(ast_node_access, Ast);
        WalkAst(Child->Symbol);
      } break;

      case type_ast_node_expression:
      {
        auto Child = SafeAccess(ast_node_expression, Ast);
        WalkAst(Child);
      } break;

      case type_ast_node_parenthesized:
      {
        auto Child = SafeAccess(ast_node_parenthesized, Ast);
        WalkAst(Child->Expr);
        if (Child->IsCast)
        {
          Assert(Child->CastValue);
          WalkAst(Child->CastValue);
        }
        else
        {
          Assert(!Child->CastValue);
        }
      } break;

      case type_ast_node_operator:
      {
        auto Child = SafeAccess(ast_node_operator, Ast);
        WalkAst(Child->Operand);
      } break;

      case type_ast_node_return:
      {
        auto Child = SafeAccess(ast_node_return, Ast);
        WalkAst(Child->Value);
      } break;

      case type_ast_node_initializer_list:
      case type_ast_node_symbol:
      case type_ast_node_type_specifier:
      case type_ast_node_literal:
      {
      } break;

      case type_ast_node_noop:
      {
        InvalidCodePath();
      } break;

      InvalidDefaultCase;
    }
  }
}

#if 0
bonsai_function void
RegisterPrimitiveDatatypes(program_datatypes *Datatypes, memory_arena *Memory)
{
  primitive_def P = {};

  type_spec *Type = &P.Type;

  //
  // Integral Types
  //

#if 0
  Type->Name = CSz("short"); Type->SourceText = CSz("short");
  Push(&Datatypes->Primitives, P, Memory); Type->Signed = true;

  Type->SourceText = CSz("short int");
  Push(&Datatypes->Primitives, P, Memory);

  Type->SourceText = CSz("signed short int");
  Push(&Datatypes->Primitives, P, Memory);

  Type->SourceText = CSz("signed short int");
  Push(&Datatypes->Primitives, P, Memory);


  Type->Token.Value = CSz("float"); Type->SourceText = CSz("float");
  Push(&Datatypes->Primitives, P, Memory);

  Type->Name = CSz("double"); Type->SourceText = CSz("double");
  Push(&Datatypes->Primitives, P, Memory);
#endif


}
#endif

bonsai_function void
RegisterUnparsedCxxTypes(program_datatypes *Datatypes, memory_arena *Memory)
{
  stl_container_def Container = {};

  // Got this list of STL containers from : http://www.cplusplus.com/reference/stl/
  // I have no real idea if it's complete, or accurate, and I don't really care.

  Container.Name = CSz("array");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("list");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("forward_list");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("dequeue");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("vector");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("stack");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("queue");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("priority_queue");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("multiset");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("multimap");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_set");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_map");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_multiset");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_multimap");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("map");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("set");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("default_random_engine");
  Push(&Datatypes->StlContainers, Container, Memory);
}

#ifndef EXCLUDE_PREPROCESSOR_MAIN
#define SUCCESS_EXIT_CODE 0
#define FAILURE_EXIT_CODE 1

s32
main(s32 ArgCountInit, const char** ArgStrings)
{
  if (ArgCountInit < 0)
  {
    Error("Invalid arg count of %d", ArgCountInit);
    return FAILURE_EXIT_CODE;
  }

  u32 ArgCount = (u32)ArgCountInit;
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  b32 ShouldOpenDebugWindow = DoDebugWindow(ArgStrings, ArgCount);
  if (ShouldOpenDebugWindow)
  {
    if (!BootstrapDebugSystem(ShouldOpenDebugWindow))
    {
      Error("Booting debug system");
      return FAILURE_EXIT_CODE;
    }
  }

  b32 Success = True;
  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;

    arguments Args = ParseArgs(ArgStrings, ArgCount, Memory);
    Assert(Args.Files.Start == Args.Files.At);
    Assert(Args.DoDebugWindow == ShouldOpenDebugWindow);

    parse_context Ctx = {
      .Memory = Memory,
      .IncludePaths = &Args.IncludePaths,
    };

    todo_list_info TodoInfo = {
      .People = ParseAllTodosFromFile(CSz("todos.md"), Memory),
    };

    while ( Args.Files.At < Args.Files.End )
    {
      counted_string CurrentFileName = *Args.Files.At;

      parser *Parser = ParserForFile(&Ctx, CurrentFileName);
      if (!Parser->Valid)
      {
        Error("Tokenizing File: %.*s", (s32)CurrentFileName.Count, CurrentFileName.Start);
      }

      ++Args.Files.At;
    }

    Rewind(&Args.Files);

    /* RemoveAllMetaprogrammingOutput(&Ctx.AllParsers, &Args); */

    /* RegisterPrimitiveDatatypes(&Ctx.Datatypes, Memory); */

    RegisterUnparsedCxxTypes(&Ctx.Datatypes, Memory);

    ITERATE_OVER(&Ctx.AllParsers)
    {
      parser* Parser = GET_ELEMENT(Iter);
      Ctx.CurrentParser = *Parser;

      ParseDatatypes(&Ctx);

      continue;
    }

    ITERATE_OVER(&Ctx.AllParsers)
    {
      // TODO(Jesse id: 340): We should only traverse files that were passed to us on the CLI

      parser* Parser = GET_ELEMENT(Iter);
      Rewind(Parser);

      Ctx.CurrentParser = *Parser;

      if (IsMetaprogrammingOutput(Parser->Filename, Args.Outpath))
      {
        Info("Skipping %S.", Parser->Filename);
      }
      else
      {
        GoGoGadgetMetaprogramming(&Ctx, &TodoInfo);

        if (Parser->Valid)
        {
          if (Parser->OutputTokens.At != Parser->OutputTokens.Start)
          {
            TruncateToCurrentSize(&Parser->OutputTokens);
            Output(Parser->OutputTokens, Parser->Filename, Memory);
            Success("Output '%S'.", Parser->Filename);
          }
          else
          {
            Error("Tried to output an OutputTokens stream of 0 length!");
          }
        }
      }

      continue;
    }

#if 0
    ITERATE_OVER(&Ctx.Datatypes.Functions)
    {
      function_decl *Func = GET_ELEMENT(Iter);

      if (Func->ReturnType.TemplateSource.Count) { continue; }

      PushParser(&Ctx.Parser, Func->Body, parser_push_type_root);
      Func->Ast = ParseAllStatements(&Ctx);
      WalkAst(Func->Ast);

      Ctx.Parser.Depth = 0;
    }
#endif

    WriteTodosToFile(&TodoInfo.People, Memory);
  }
  else
  {
    Warn("No files passed, exiting.");
  }


  if (ShouldOpenDebugWindow)
  {
    debug_state* DebugState = GetDebugState();

    DebugState->UIType = DebugUIType_CallGraph;
    DebugState->DisplayDebugMenu = True;

    DebugState->MainThreadAdvanceDebugSystem();

    while (Os.ContinueRunning)
    {
      v2 LastMouseP = Plat.MouseP;
      while ( ProcessOsMessages(&Os, &Plat) );
      Plat.MouseDP = LastMouseP - Plat.MouseP;

      DebugState->OpenDebugWindowAndLetUsDoStuff();
      BonsaiSwapBuffers(&Os);

      /* glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID); */
      /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
  }

  s32 Result = Success ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
#endif
