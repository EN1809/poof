#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

#include <poof/functions.h>
#include <poof/poof.h>

global_variable memory_arena Global_PermMemory = {};

#define InvalidDefaultWhileParsing(P, ErrorMessage) \
    default: { ParseError(P, ErrorMessage, PeekTokenPointer(P)); } break;



#define DEBUG_PRINT (0)
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



bonsai_function peek_result PeekTokenRawCursor(peek_result *Peek, s32 TokenLookahead = 0);
bonsai_function peek_result PeekTokenRawCursor(c_token_cursor *Tokens, s32 TokenLookahead, b32 CanSearchDown = True);
bonsai_function peek_result PeekTokenRawCursor(parser *Parser, s32 TokenLookahead = 0);

bonsai_function peek_result PeekTokenCursor(peek_result *Peek, s32 TokenLookahead = 0);
bonsai_function peek_result PeekTokenCursor(c_token_cursor *Tokens, s32 TokenLookahead = 0);
bonsai_function peek_result PeekTokenCursor(parser *Parser, s32 TokenLookahead = 0);

bonsai_function c_token* PeekTokenRawPointer(parser *Parser, u32 TokenLookahead);
bonsai_function c_token* PeekTokenRawPointer(parser *Parser, s32 TokenLookahead = 0);

bonsai_function c_token * PeekTokenPointer(c_token_cursor *Tokens, s32 TokenLookahead = 0);
bonsai_function c_token * PeekTokenPointer(parser *Parser, u32 TokenLookahead = 0);

bonsai_function c_token   PeekTokenRaw(parser *Parser, s32 Lookahead = 0);
bonsai_function c_token   PeekToken(parser *Parser, s32 Lookahead = 0);
bonsai_function c_token   PopTokenRaw(parser *Parser);
bonsai_function c_token * PopTokenRawPointer(parser *Parser);
bonsai_function c_token   PopToken(parser *Parser);
bonsai_function b32       OptionalTokenRaw(parser *Parser, c_token_type Type);
bonsai_function c_token * OptionalToken(parser *Parser, c_token T);
bonsai_function c_token * OptionalToken(parser *Parser, c_token_type Type);
bonsai_function c_token   RequireToken(parser *Parser, c_token *ExpectedToken);
bonsai_function c_token   RequireToken(parser *Parser, c_token ExpectedToken);
bonsai_function c_token   RequireToken(parser *Parser, c_token_type ExpectedType);
bonsai_function c_token   RequireTokenRaw(parser *Parser, c_token Expected);
bonsai_function c_token   RequireTokenRaw(parser *Parser, c_token *Expected);
bonsai_function c_token   RequireTokenRaw(parser *Parser, c_token_type ExpectedType);

bonsai_function b32       TokensRemain(parser *Parser, u32 TokenLookahead = 0);
bonsai_function b32       RawTokensRemain(parser *Parser, u32 TokenLookahead = 0);

bonsai_function b32       TokenIsOperator(c_token_type T);
bonsai_function b32       NextTokenIsOperator(parser *Parser);
bonsai_function c_token * RequireOperatorToken(parser *Parser);

bonsai_function void      TrimFirstToken(parser* Parser, c_token_type TokenType);
bonsai_function void      TrimLastToken(parser* Parser, c_token_type TokenType);
bonsai_function void      TrimLeadingWhitespace(parser* Parser);

bonsai_function counted_string EatBetweenExcluding(ansi_stream*, char Open, char Close);
bonsai_function void      EatBetween(parser* Parser, c_token_type Open, c_token_type Close);
bonsai_function b32       EatWhitespace(parser* Parser);
bonsai_function b32       EatSpacesTabsAndEscapedNewlines(parser *Parser);
bonsai_function void      EatWhitespaceAndComments(parser *Parser);

bonsai_function void      FullRewind(parser* Parser);

bonsai_function parser * DuplicateParserTokens(parser *Parser, memory_arena *Memory);
bonsai_function parser * DuplicateParser(parser *Parser, memory_arena *Memory);
bonsai_function c_token_cursor * DuplicateCTokenCursor(c_token_cursor *Tokens, memory_arena *Memory);
bonsai_function parser *         DuplicateCTokenCursor2(c_token_cursor *Tokens, memory_arena *Memory);

//
// Preprocessor stuff
//

bonsai_function b32         TryTransmuteKeywordToken(c_token *T, c_token *LastTokenPushed);
bonsai_function b32         TryTransmuteOperatorToken(c_token *T);
bonsai_function b32         TryTransmuteIdentifierToken(c_token *T);
bonsai_function macro_def * TryTransmuteIdentifierToMacro(parse_context *Ctx, parser *Parser, c_token *T, macro_def *ExpandingMacro);
bonsai_function macro_def * IdentifierShouldBeExpanded(parse_context *Ctx, parser *Parser, c_token *T, macro_def *ExpandingMacro);

bonsai_function c_token_cursor * ResolveInclude(parse_context *Ctx, parser *Parser, c_token *T);
bonsai_function parser * ExpandMacro(parse_context *Ctx, parser *Parser, macro_def *Macro, memory_arena *PermMemory, memory_arena *TempMemory, b32 ScanArgsForAdditionalMacros = False);
bonsai_function u64      ResolveMacroConstantExpression(parse_context *Ctx, parser *Parser, memory_arena *PermMemory, memory_arena *TempMemory, u64 PreviousValue, b32 LogicalNotNextValue);

bonsai_function macro_def * GetMacroDef(parse_context *Ctx, counted_string DefineValue) ;
bonsai_function c_token *   EatIfBlock(parser *Parser, erase_token_mode Erased);
bonsai_function c_token *   EraseAllRemainingIfBlocks(parser *Parser);

bonsai_function void EraseToken(c_token *Token);
bonsai_function void EraseBetweenExcluding(parser *Parser, c_token *StartToken, c_token *OnePastLastToken);

bonsai_function void DumpLocalTokens(parser *Parser);
bonsai_function void PrintTray(char_cursor *Dest, c_token *T, u32 Columns, counted_string Color);
bonsai_function void PrintTraySimple(c_token *T);


bonsai_function void ParseError(parser* Parser, parse_error_code ErrorCode, counted_string ErrorMessage, c_token* ErrorToken = 0);
bonsai_function void ParseError(parser* Parser, counted_string ErrorMessage, c_token* ErrorToken = 0);

bonsai_function ast_node_expression* ParseExpression(parse_context *Ctx);
bonsai_function void                 ParseExpression(parse_context *Ctx, ast_node_expression *Result);
bonsai_function void                 ParseExpression(parse_context *Ctx, ast_node** Result);

bonsai_function struct_def ParseStructBody(parse_context *, c_token *);

bonsai_function parser MaybeParseFunctionBody(parser *Parser, memory_arena *Memory);


inline c_token_cursor *
HasValidDownPointer(c_token *T)
{
  c_token_cursor *Result = (T && T->Down && (T->Type == CT_MacroLiteral || T->Type == CT_InsertedCode)) ? T->Down : 0;
  return Result;
}

#if BONSAI_SLOW
bonsai_function void
SanityCheckCTokenCursor(c_token_cursor *Current)
{
  for (u32 TokenIndex = 0; TokenIndex < TotalElements(Current); ++TokenIndex)
  {
    c_token *T = Current->Start + TokenIndex;

    if (HasValidDownPointer(T))
    {
      Assert(T->Down->Up.Tokens == Current);
      SanityCheckCTokenCursor(T->Down);
    }
  }
}

bonsai_function void
SanityCheckParserChain(parser *Parser)
{
#if 1
  Assert(Parser->Tokens->Up.Tokens != Parser->Tokens);

  c_token_cursor *FirstInChain = Parser->Tokens;

  while (FirstInChain->Up.Tokens) FirstInChain = FirstInChain->Up.Tokens;
  SanityCheckCTokenCursor(FirstInChain);
#endif
}
#else
#define SanityCheckParserChain(...)
#define SanityCheckCTokenCursor(...)
#endif

bonsai_function string_from_parser
StartStringFromParser(parser* Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);

  string_from_parser Result = {};

  if (T)
  {
    Result.Parser = Parser;
    Result.StartToken = T;
  }

  return Result;
}

bonsai_function counted_string
FinalizeStringFromParser(string_from_parser* Builder)
{
  counted_string Result = {};

  parser *Parser = Builder->Parser;
  c_token *StartToken = Builder->StartToken;

  if (Parser)
  {
    if (Contains(Parser, StartToken))
    {
      umm Count = 0;
      // NOTE(Jesse): This would be better if it excluded the At token, but
      // unfortunately we wrote the calling code such that the At token is
      // implicitly included, so we have to have this weird check.
      if (Parser->Tokens->At == Parser->Tokens->End)
      {
        Count = (umm)(Parser->Tokens->At[-1].Value.Start - Builder->StartToken->Value.Start);
      }
      else
      {
        Count = (umm)(Parser->Tokens->At->Value.Start - Builder->StartToken->Value.Start);
      }

      Result = CS(Builder->StartToken->Value.Start, Count);
    }
    else
    {
      Warn(CSz("Unable to call FinalizeStringFromParser due to having spanned a parser chain link."));
    }
  }

  return Result;
}

#if 0
bonsai_function void
SinglyLinkedListSwapInplace(c_token_cursor *P0, c_token_cursor *P1)
{
  NotImplemented;
  Assert(P0->Up != P0);
  Assert(P1->Up != P1);

  Assert(P1->Up == P0);

  auto M0 = *P0; // Mnemonic M0 == Memory0
  auto M1 = *P1;

  *P0 = M1;
  *P1 = M0;

  P0->Up = P0;
  P1->Up = P0;

  Assert(P0->Up != P0);
  Assert(P1->Up != P1);
}
#endif

struct d_list
{
  d_list *Prev;
  d_list *Next;
};

#if 1
bonsai_function void
DoublyLinkedListSwap(d_list *P0, d_list *P1)
{
  Assert(P0 != P1);

  b32 Colocated = P0->Next == P1;
  b32 ColocatedReversed = P1->Next == P0;

  // TODO(Jesse): I'm fairly sure that we don't need this boolean and should
  // be able to just go off the M pointer values .. but I didn't want to sit
  // around and figure it out.  Same goes for ColocatedReversed.  Good news is
  // that this routine is well tested so future-me can fearlessly modify it.
#if 0
  if (Colocated)
  {
    if (P1->Prev != P0)
    {
      Assert(false);

      DumpLocalTokens(P0);
      DebugChars("\n");
      DumpLocalTokens(P1);
      DebugChars("\n");
      DumpLocalTokens(P1->Next);
      DebugChars("\n");
      DumpLocalTokens(P1->Prev);
      DebugChars("\n");
    }
  }
#endif

  if (ColocatedReversed)
  {
    Assert(P0->Prev == P1);
    Assert(!Colocated);

    Colocated = True;
    d_list *Temp = P1;
    P1 = P0;
    P0 = Temp;
  }

  d_list M0 = *P0; // Mnemonic M0 == Memory0
  d_list M1 = *P1;

  *P0 = M1;
  *P1 = M0;

  P0->Next = M1.Next;
  P0->Prev = Colocated ? P1 : M1.Prev;

  P1->Next = Colocated ? P0 : M0.Next;
  P1->Prev = M0.Prev;

  if (M1.Next)
  {
    M1.Next->Prev = P0;
  }

  if (M0.Prev)
  {
    M0.Prev->Next = P1;
  }

  if (!Colocated)
  {
    if (M1.Prev)
    {
      M1.Prev->Next = P0;
    }
    if (M0.Next)
    {
      M0.Next->Prev = P1;
    }
  }

  return;
}
#endif

inline void
Invalidate(peek_result *Peek)
{
  Peek->At = 0;
}

inline void
Invalidate(c_token_cursor *Tokens)
{
  Tokens->At = Tokens->End;
}

inline b32
IsValid(peek_result *Peek)
{
  b32 Result = Peek->At != 0;
  return Result;
}

bonsai_function b32
TokenShouldModifyLineCount(c_token *T, token_cursor_source Source)
{
  b32 Result = False;

  if ( Source == TokenCursorSource_RootFile ||
       Source == TokenCursorSource_Include )
  {
    Result = T->Type == CTokenType_Newline ||
             T->Type == CTokenType_EscapedNewline;
  }

  return Result;
}

bonsai_function c_token *
RewindTo(parser* Parser, c_token *T)
{
  peek_result Current = {};
  if (T)
  {
    Current = PeekTokenRawCursor(Parser->Tokens, -1);
    while (Current.At && Current.At != T)
    {
      Current = PeekTokenRawCursor(&Current, -1);
    }

    if (IsValid(&Current))
    {
      Parser->Tokens = Current.Tokens;
      Parser->Tokens->At = Current.At;
    }
    else
    {
      FullRewind(Parser);
    }
  }
  else
  {
    Warn("ptr(0) passed to RewindTo");
  }

  return Current.At;
}

bonsai_function c_token *
RewindTo(parser* Parser, c_token_type Type)
{
  peek_result Current = {};
  if (Type)
  {
    Current = PeekTokenRawCursor(Parser->Tokens, -1);
    while (Current.At && Current.At->Type != Type)
    {
      Current = PeekTokenRawCursor(&Current, -1);
    }

    if (IsValid(&Current))
    {
      Parser->Tokens = Current.Tokens;
      Parser->Tokens->At = Current.At;
    }
    else
    {
      FullRewind(Parser);
    }
  }
  else
  {
    Warn("CTokenType_Unknown passed to RewindTo");
  }

  return Current.At;
}

// NOTE(Jesse): This function should actually just be able to walk the "Up" chain
// since we never actually modify cursors
bonsai_function void
FullRewind(parser* Parser)
{
  TIMED_FUNCTION();

  SanityCheckParserChain(Parser);

  c_token_cursor *Current = Parser->Tokens;
  Rewind(Current);

  while (Current->Up.Tokens)
  {
    Current = Current->Up.Tokens;
    Rewind(Current);
  }
  Assert(Current->Up.Tokens == 0);

  if (Current != Parser->Tokens)
  {
    Parser->Tokens = Current;
  }
  Assert(Parser->Tokens->Up.Tokens == 0);

  SanityCheckParserChain(Parser);
}

bonsai_function b32
IsNewline(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CT_ControlChar_Form_Feed  ||
               Type == CTokenType_CarrigeReturn;

  return Result;
}

bonsai_function b32
IsNewline(c_token *T)
{
  b32 Result = T && IsNewline(T->Type);
  return Result;
}


bonsai_function b32
IsNBSP(c_token_type Type)
{
  b32 Result = Type == CTokenType_Tab            ||
               Type == CTokenType_Space;

  return Result;
}

bonsai_function b32
IsNBSP(c_token *T)
{
  b32 Result = IsNBSP(T->Type);
  return Result;
}


bonsai_function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CTokenType_CarrigeReturn  ||
               Type == CTokenType_Tab            ||
               Type == CT_ControlChar_Form_Feed  ||
               Type == CTokenType_Space;

  return Result;
}

bonsai_function b32
IsWhitespace(c_token *T)
{
  b32 Result = IsWhitespace(T->Type);
  return Result;
}

bonsai_function b32
IsComment(c_token *T)
{
  b32 Result = (T->Type == CTokenType_CommentSingleLine) || (T->Type == CTokenType_CommentMultiLine);
  return Result;
}

// NOTE(Jesse): This logic seems kinda janky..  It definitely doesn't return
// what a caller would expect it to return, which is how many tokens remain on
// the entire stream.  I think we should actually get rid of this function
// entirely since we use it in places we "should" just use PeekTokenPointer
// instead.
//
// Half-measure might be renaming it to NotEmpty() or something..
#if 0
bonsai_function umm
Remaining(parser* Parser)
{
  umm Result = 0;

  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    Result = Remaining(Parser->Tokens);

    if (!Result && Parser->Tokens->Up.Tokens)
    {
      NotImplemented;
      // NOTE(Jesse): This is going to be buggy because it's not setting the At
      // and then resetting it
      Result = Remaining(Parser->Tokens->Up.Tokens);
    }
  }

  return Result;
}
#endif

#if 0
umm Remaining(c_token_cursor *Tokens)
{
  umm Result = 0;
  if (Tokens->At < Tokens->End)
  {
    Result = (umm)(Tokens->End-Tokens->At);
    Assert(Result > 0);
    Result -= 1;
  }
  return Result;
}
#endif


#if 0
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
#endif

bonsai_function c_token *
AdvanceTo(parser *Parser, peek_result *Peek)
{
  c_token *Result = 0;
  if (Parser->Tokens->At < Parser->Tokens->End)
  {
    // NOTE(Jesse): Yes, it's really weird we return the thing we were on here
    // instead of what we're advancing to.  Yes, we should change it.
    Result = Parser->Tokens->At;

    if (IsValid(Peek))
    {
      Assert(Peek->Tokens);
      Parser->Tokens = Peek->Tokens;
      Parser->Tokens->At = Peek->At;
      Assert(Peek->Tokens->At == Peek->At);
    }
    else
    {
      Parser->Tokens->At = Parser->Tokens->End;
    }
  }

  return Result;
}

bonsai_function void
AdvanceParser(parser* Parser)
{
  NotImplemented;
#if 0
  if (Parser->ErrorCode) { Warn("Advancing an invalid Parser!"); }

  SanityCheckParserChain(Parser);
  Assert(Parser->Tokens->At >= Parser->Tokens->Start);
  Assert(Parser->Tokens->At <= Parser->Tokens->End);
  Assert(Parser->Tokens->Start <= Parser->Tokens->End);

  peek_result Peek = PeekTokenRawCursor(Parser->Tokens, 1);
  if (IsValid(&Peek))
  {
    AdvanceTo(Parser, &Peek);
  }
  else
  {
    Warn("Tried to advance an empty parser.");
  }


  Assert(Parser->Tokens->At >= Parser->Tokens->Start);
  Assert(Parser->Tokens->At <= Parser->Tokens->End);

  // TODO(Jesse): Should we really allow cursors of 0 length?  I don't like that.
  // ie. This check should be < not <=
  Assert(Parser->Tokens->Start <= Parser->Tokens->End);
  SanityCheckParserChain(Parser);

  return;
#endif
}

#if 0
bonsai_function b32
Scan(parser *Parser, c_token *Needle, sign Direction)
{
  b32 Result = False;

  s32 TokenIndex = 0;
  while (c_token *T = PeekTokenRawPointer(Parser, TokenIndex))
  {
    if (T == Needle) { Result = True; break; }

    TokenIndex += Direction;
  }

  return Result;
}
#endif

#if 0
bonsai_function c_token *
AdvanceTo(parser* Parser, c_token_cursor *T)
{
  c_token *Result = IsValid(Parser->Tokens) ? Parser->Tokens->At : 0;
  SanityCheckParserChain(Parser);
  parser OldParser = *Parser;
  c_token_cursor OldTokens = *Parser->Tokens;
  *Parser->Tokens = *T;
  SanityCheckParserChain(Parser);
  return Result;
}
#endif

bonsai_function c_token *
AdvanceTo(parser* Parser, c_token* T)
{
  SanityCheckParserChain(Parser);
  peek_result Peek = PeekTokenRawCursor(Parser);

  while (IsValid(&Peek) && Peek.At != T)
  {
    Peek = PeekTokenRawCursor(&Peek, 1);
  }

  if (IsValid(&Peek))
  {
    Assert(Peek.At == T);
    AdvanceTo(Parser, &Peek);
    Assert(Parser->Tokens->At == T);
  }

  SanityCheckParserChain(Parser);
  return Peek.At;
}

bonsai_function void
ParseMacroArgument(parser* Parser, c_token_buffer *Result)
{
  Result->Start = Parser->Tokens->At;

  u32 Depth = 0;
  b32 Done = False;
  while (c_token *T = PeekTokenRawPointer(Parser) )
  {
    if (Done) break;

    if (Depth == 0 && T->Type == CTokenType_Comma)
    {
      Done = True;
    }
    else if (Depth > 0 && T->Type == CTokenType_CloseParen)
    {
      --Depth;
    }
    else if (T->Type == CTokenType_OpenParen)
    {
      ++Depth;
    }

    if (!Done) PopTokenRaw(Parser);
  }

  Result->Count = (umm)(Parser->Tokens->At - Result->Start);

  OptionalToken(Parser, CTokenType_Comma);

  return;
}

bonsai_function void
EatUntilExcluding(parser* Parser, c_token_type Close)
{
  // TODO(Jesse, performance, slow): This is slow AF
  while (PeekTokenRawPointer(Parser))
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
  return;
}

bonsai_function void
EatUntilIncluding(parser* Parser, c_token_type Close)
{
  while (c_token *T = PopTokenRawPointer(Parser))
  {
    if(T->Type == Close)
    {
      break;
    }
  }
  return;
}




/*****************************                ********************************/
/*****************************  Error Output  ********************************/
/*****************************                ********************************/


bonsai_function void
DumpSingle(c_token_cursor *Cursor, c_token *At)
{
  u32 TokenCount = (u32)TotalElements(Cursor);

  for ( u32 TokenIndex = 0;
        TokenIndex  < TokenCount;
        ++TokenIndex)
  {
    c_token *T = Cursor->Start+TokenIndex;
    if (T == At)
    {
      DebugChars("%S>%S", TerminalColors.Green, TerminalColors.White);
    }

    // NOTE(Jesse): Handle cursors that haven't been completely filled
    if (T->Type)
    {
      PrintToken(T);
    }
    else
    {
      break;
    }
  }

  return;
}

#if 0
bonsai_function void
DumpChain(parser* Parser, u32 LinesToDump = u32_MAX)
{
  c_token_cursor *Cursor = Parser->Tokens;
  c_token *At = Cursor->At;

  while(Cursor->Up) Cursor = Cursor->Up;

  DebugLine("-------------------- Dumping Cursor -- At Line (%d) ----------------------------", At->LineNumber);

  u32 Index = 0;
  /* while (Cursor) */
  {
    DumpSingle(Cursor, At);
    DebugLine("\n%S^%S Link(%d) %S^%S",
        TerminalColors.Blue, TerminalColors.White,
        Index,
        TerminalColors.Blue, TerminalColors.White);
    /* Cursor = Cursor->Next; */
    ++Index;
  }
}
#endif

bonsai_function void
PrintTokenVerbose(c_token_cursor *Tokens, c_token *T, c_token *AbsAt, u32 Depth)
{
  counted_string AtMarker = CSz(" ");
  if (AbsAt == T )
  {
    AtMarker = CSz(GREEN_TERMINAL ">" WHITE_TERMINAL);
  }
  else if (Tokens->At == T)
  {
    AtMarker = CSz(">");
  }

  counted_string ErasedString = T->Erased ? CSz("e") : CSz(" ");
  DebugChars("%*s(%S) %d %S %S %S ", Depth*2, "",  Basename(T->Filename), T->LineNumber, ErasedString, AtMarker, ToString(T->Type));
  PrintToken(*T);

  if (T->Type != CTokenType_Newline && T->Type != CTokenType_EscapedNewline)
  {
    DebugChars("\n");
  }
}

void
DumpCursorSlice(c_token_cursor* Tokens, umm PrevTokens, umm NextTokens)
{
  DebugLine("\n >> Dumping Cursor %S", Tokens->Filename);

  PrintTraySimple(Tokens->Start);

  c_token *StartT = Tokens->At - Min(PrevTokens, AtElements(Tokens));
  Assert(IsValidForCursor(Tokens, StartT));

  c_token *EndT = Tokens->At + Min(NextTokens, Remaining(Tokens));
  Assert(IsValidForCursor(Tokens, EndT) || EndT == Tokens->End);


  c_token *At = StartT;
  while (At < EndT)
  {
    PrintToken(At);
    PrintTraySimple(At);
    /* PrintTokenVerbose(Tokens, T, AbsoluteAt, Depth); */

    ++At;
  }

  DebugLine("\n");
}

void
DumpCursorSimple(c_token_cursor* Tokens, c_token *AbsoluteAt = 0, u32 Depth = 0)
{
  DebugLine("\n%*s>> Dumping Cursor %S", Depth*4, "", Tokens->Filename);

  PrintTraySimple(Tokens->Start);

  for (umm TIndex = 0; TIndex < TotalElements(Tokens); ++TIndex)
  {
    c_token *T = Tokens->Start + TIndex;

    PrintToken(T);
    PrintTraySimple(T);
    /* PrintTokenVerbose(Tokens, T, AbsoluteAt, Depth); */

    switch (T->Type)
    {
      case CT_MacroLiteral:
      case CT_InsertedCode:
      {
        if (T->Down)
        {
          Assert(T->Down->Up.Tokens == Tokens);
          DumpCursorSimple(T->Down, AbsoluteAt, Depth+1);
        }
      } break;

      default: {} break;;
    }
  }

  if (AbsoluteAt == Tokens->End)
  {
    c_token UnknownMarkerToken = {};
    UnknownMarkerToken.Filename = Tokens->Start->Filename;
    PrintTokenVerbose(Tokens, &UnknownMarkerToken, &UnknownMarkerToken, Depth);
  }

  DebugLine("\n%*s>> done", Depth*4, "");
}

#if 0
bonsai_function void
DumpEntireParser(parser* Parser, u32 LinesToDump = u32_MAX, b32 Verbose = False)
{
  DebugLine("%S---%S", TerminalColors.Purple, TerminalColors.White);

  c_token *WasAt = PeekTokenRawPointer(Parser);

  u32 StartingLineNumber = WasAt->LineNumber;
  parse_error_code PrevError = Parser->ErrorCode;
  Parser->ErrorCode = ParseErrorCode_None;

  FullRewind(Parser);

  char TrayBuffer[32];
  counted_string TempString = CSz(TrayBuffer);
  char_cursor TrayCursor = CharCursor(TempString);


  /* b32 DoTray = true;//!Verbose; */
  b32 DoTray = !Verbose;
  if (DoTray)
  {
    PrintTray(&TrayCursor, PeekTokenRawPointer(Parser), 5);
    DebugChars(CS(&TrayCursor));
    TrayCursor.At = TrayCursor.Start;
  }

  while(c_token *T = PeekTokenRawPointer(Parser))
  {
    if (T == WasAt)
    {
      DebugChars("%S>%S", TerminalColors.Green, TerminalColors.White);
    }

    if (Verbose)
    {
      counted_string ErasedString = T->Erased ? CSz("(e)") : CSz("   ");
      DebugChars("(%d) %S %S", T->LineNumber, ErasedString, ToString(T->Type));
    }

    if ( Verbose && !IsNewline(T) )
    {
      DebugChars("(");
    }

    PrintToken(T);

    if ( Verbose && !IsNewline(T) )
    {
      DebugChars(")");
    }

    if (IsNewline(T))
    {
      if (LinesToDump-- == 0) break;

      if (DoTray)
      {
        PrintTray(&TrayCursor, PeekTokenRawPointer(Parser), 5);
        DebugChars(CS(&TrayCursor));
        TrayCursor.At = TrayCursor.Start;
      }
    }
    else
    {
      if (Verbose)
      {
        DebugChars("\n");
      }
    }

    // TODO(Jesse): require token should accept pointers
    RequireTokenRaw(Parser, *T);
  }

  if (WasAt)
  {
    RewindTo(Parser, WasAt);
    if (PeekTokenRawPointer(Parser) != WasAt)
    {
      AdvanceTo(Parser, WasAt);
    }
    Assert(PeekTokenRawPointer(Parser) == WasAt);
  }

  DebugLine("\n%S---%S", TerminalColors.Purple, TerminalColors.White);
  Parser->ErrorCode = PrevError;

  Assert(PeekTokenRawPointer(Parser)->LineNumber == StartingLineNumber);
}
#endif

#if 0
bonsai_function void
TruncateAtNextLineEnd(c_token_cursor *Tokens, u32 Count)
{
  while (Remaining(Tokens))
  {
    if(PopTokenRaw(Tokens).Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        break;
      }
      --Count;
    }
  }

  Tokens->End = Tokens->At;
  Tokens->Next = 0;
}

bonsai_function void
TruncateAtPreviousLineStart(parser* Parser, u32 Count )
{
  while (Parser->Tokens->At > Parser->Tokens->Start)
  {
    if (Parser->Tokens->At->Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        ++Parser->Tokens->At;
        break;
      }
      --Count;
    }
    --Parser->Tokens->At;
  }

  Parser->Tokens->Start = Parser->Tokens->At;
  Parser->Prev = 0;
}
#endif




/*****************************                 *******************************/
/*****************************   Parse Error   *******************************/
/*****************************                 *******************************/



// TODO(Jesse): Remove this?  Maybe put it onto the parser?   I'm not crazy
// about that because it' bloats that struct and we create those things like
// crazy.. but I don't really like that it's a global either.
//
// UPDATE(Jesse): Actually now we create parsers pretty sparingly so we should
// be able to do one per parser without too much bloat..
static const u32 Global_ParseErrorBufferSize = 64*1024;
static char Global_ParseErrorBuffer[Global_ParseErrorBufferSize] = {};

bonsai_function void
Highlight(char_cursor *Dest, counted_string Str, counted_string Color)
{
  CopyToDest(Dest, Color);
  CopyToDest(Dest, Str);
  CopyToDest(Dest, TerminalColors.White);
}

bonsai_function void
Highlight(char_cursor *Dest, char C, counted_string Color)
{
  CopyToDest(Dest, Color);
  CopyToDest(Dest, C);
  CopyToDest(Dest, TerminalColors.White);
}

bonsai_function void
Indent(char_cursor *Dest, u32 TabCount, u32 SpaceCount)
{
  for (u32 ColumnIndex = 0;
      ColumnIndex < TabCount;
      ++ColumnIndex)
  {
    CopyToDest(Dest, '\t');
  }

  for (u32 ColumnIndex = 0;
      ColumnIndex < SpaceCount;
      ++ColumnIndex)
  {
    CopyToDest(Dest, ' ');
  }
}

bonsai_function void
OutputIdentifierUnderline(char_cursor *Dest, u32 IdentifierLength, counted_string Color)
{
  u32 IdentifierLengthSubOne = IdentifierLength > 0 ? IdentifierLength-1 : IdentifierLength;
  b32 DoPipes = IdentifierLength > 3;

  CopyToDest(Dest, Color);

  for (u32 ColumnIndex = 0;
      ColumnIndex < IdentifierLength;
      ++ColumnIndex)
  {
    if (DoPipes)
    {
      if (ColumnIndex == 0)
      {
        CopyToDest(Dest, '|');
      }
      else if (ColumnIndex == IdentifierLengthSubOne)
      {
        CopyToDest(Dest, '|');
      }
      else
      {
        CopyToDest(Dest, '~');
      }
    }
    else
    {
      CopyToDest(Dest, '^');
    }
  }

  CopyToDest(Dest, TerminalColors.White);
}

bonsai_function void
PrintTraySimple(c_token *T)
{
  if (T)
  {
    if (T->Type == CTokenType_Newline || T->Type == CTokenType_EscapedNewline)
    {
      DebugChars(CSz("%*d |"), 6, T->LineNumber);
    }
  }
  else
  {
    DebugChars(CSz("%*c |"), 6, ' ');
  }
}

bonsai_function void
PrintTray(char_cursor *Dest, c_token *T, u32 Columns, counted_string Color = TerminalColors.White)
{
  if (T)
  {
    FormatCountedString_(Dest, CSz("%*d |"), Columns, T->LineNumber);
  }
  else
  {
    FormatCountedString_(Dest, CSz("%*c %S|%S"), Columns, ' ', Color, TerminalColors.White);
  }
}

bonsai_function u64
GetLongestLineInCursor(char_cursor *Cursor)
{
  Assert(Cursor->At == Cursor->Start);
  u64 Result = 0;

  u64 CurrentLineLength = 0;
  while (Remaining(Cursor))
  {
    if (Cursor->At[0] == '\n')
    {
      if (CurrentLineLength > Result)
      {
        Result = CurrentLineLength;
      }
      CurrentLineLength = 0;
    }
    else if (Cursor->At[0] == '\x1b')
    {
      while (Remaining(Cursor))
      {
        Advance(Cursor);

        if (Cursor->At[0] == 'm')
        {
          break;
        }
      }
    }
    else
    {
      CurrentLineLength++;
    }

    Advance(Cursor);
  }

  if (CurrentLineLength > Result)
  {
    Result = CurrentLineLength;
  }

  return Result;
}

bonsai_function void
PrintContext(c_token_cursor_up *Up)
{
  if (Up->Tokens)
  {
    if (Up->Tokens->Up.Tokens)
    {
      PrintContext(&Up->Tokens->Up);
    }

    c_token_cursor Tokens = *Up->Tokens;
                   Tokens.At = Up->At;


    u32 LinesOfContext = 5;

/*     u32 LinesReversed = 0; */
/*     while (LinesReversed <= LinesOfContext) */
/*     { */
/*       RewindTo(Parser, CTokenType_Newline); */
/*       LinesReversed += 1; */
/*     } */

    /* peek_result Current = PeekTokenCursor(Parser, 0, False); */
    /* while (IsValid(&Current)) */

    DumpCursorSlice(&Tokens, 35, 35);
  }
}

bonsai_function void
ParseError(parser* Parser, parse_error_code ErrorCode, counted_string ErrorMessage, c_token* ErrorToken)
{
  parse_error_code PrevErrorCode = Parser->ErrorCode;
  Parser->ErrorCode = ParseErrorCode_None;

  if (Parser->Tokens->Up.Tokens)
  {
    PrintContext(&Parser->Tokens->Up);
  }

  // @optimize_call_advance_instead_of_being_dumb
  if (!ErrorToken)
  {
    ErrorToken = PeekTokenRawPointer(Parser);
    AdvanceTo(Parser, ErrorToken);
  }

  char_cursor ParseErrorCursor_ = {};
  char_cursor *ParseErrorCursor = &ParseErrorCursor_;
  ParseErrorCursor->Start = Global_ParseErrorBuffer;
  ParseErrorCursor->At = Global_ParseErrorBuffer;
  ParseErrorCursor->End = Global_ParseErrorBuffer+Global_ParseErrorBufferSize;

  u32 LinesOfContext = 10;

  counted_string ParserName = {};

  if (AdvanceTo(Parser, ErrorToken))
  {
  }
  else if (RewindTo(Parser, ErrorToken))
  {
  }
  else
  {
    Error("Couldn't find specified token in parser chain.");
    Parser = 0;
  }


  if (Parser)
  {
    u32 ErrorLineNumber = ErrorToken->LineNumber;
    u32 MaxTrayWidth = 1 + GetColumnsFor(ErrorLineNumber + LinesOfContext);
    ParserName = ErrorToken->Filename;

    Assert(PeekTokenRawPointer(Parser) == ErrorToken);

    u32 LinesReversed = 0;
    while (LinesReversed <= LinesOfContext)
    {
      RewindTo(Parser, CTokenType_Newline);
      LinesReversed += 1;
    }

    u32 SpaceCount = 0;
    u32 TabCount = 0;

    u32 ErrorIdentifierLength = (u32)ErrorToken->Value.Count;

    //
    // Print context until we hit the error token.  Also keep track of spaces
    // and tabs while we go such that we know how much to indent error message
    //
    PrintTray(ParseErrorCursor, PeekTokenRawPointer(Parser), MaxTrayWidth);
    while (c_token *T = PopTokenRawPointer(Parser))
    {
      PrintToken(T, ParseErrorCursor);

      // TODO(Jesse, easy): These serial ifs could probably be cleaned up with a switch.
      if (T == ErrorToken)
      {
        break;
      }
      else if ( IsNewline(T->Type) )
      {
        TabCount = 0;
        SpaceCount = 0;

        // TODO(Jesse, easy): Pretty sure this can go away and just call PrintTray w/ Peek
        if (TokenShouldModifyLineCount(T, Parser->Tokens->Source))
        {
          PrintTray(ParseErrorCursor, PeekTokenRawPointer(Parser), MaxTrayWidth);
        }
        else
        {
          PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
        }

      }
      else if (T->Type == CTokenType_Tab)
      {
        ++TabCount;
      }
      else if (T->Value.Count)
      {
        SpaceCount += T->Value.Count;
      }
      else
      {
        ++SpaceCount;
      }

      continue;
    }

    //
    // Print the rest of the line the error token was found on.
    //
    {
      c_token *T = PopTokenRawPointer(Parser);
      while (T)
      {
        PrintToken(T, ParseErrorCursor);

        if (IsNewline(T))
        {
          break;
        }

        T = PopTokenRawPointer(Parser);
      }
    }

    //
    // Output the error message
    //
    {
      c_token *NextT = PeekTokenRawPointer(Parser);
      c_token *PrevT = PeekTokenRawPointer(Parser, -1);
      if (NextT == 0 && PrevT && PrevT->Type != CTokenType_Newline)
      {
        CopyToDest(ParseErrorCursor, '\n');
      }

      Indent(ParseErrorCursor, 0, MaxTrayWidth);
      Highlight(ParseErrorCursor, CSz(" >"), TerminalColors.DarkRed);
      Indent(ParseErrorCursor, TabCount, SpaceCount);
      OutputIdentifierUnderline(ParseErrorCursor, ErrorIdentifierLength, TerminalColors.DarkRed);
      CopyToDest(ParseErrorCursor, '\n');

      PrintTray(ParseErrorCursor, 0, MaxTrayWidth, TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, '\n');

      PrintTray(ParseErrorCursor, 0, MaxTrayWidth, TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, ' ');
      CopyToDest(ParseErrorCursor, ' ');

      Highlight(ParseErrorCursor, CSz("Parse Error"), TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, CSz(" :: "));
      CopyToDest(ParseErrorCursor, ToString(ErrorCode));
      CopyToDest(ParseErrorCursor, '\n');


      PrintTray(ParseErrorCursor, 0, MaxTrayWidth, TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, '\n');


      PrintTray(ParseErrorCursor, 0, MaxTrayWidth, TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, ' ');
      CopyToDest(ParseErrorCursor, ' ');

      for (u32 ECharIndex = 0;
          ECharIndex < ErrorMessage.Count;
          ++ECharIndex)
      {
        char C = ErrorMessage.Start[ECharIndex];
        CopyToDest(ParseErrorCursor, C);

        if (C == '\n')
        {
          PrintTray(ParseErrorCursor, 0, MaxTrayWidth, TerminalColors.Yellow);
          CopyToDest(ParseErrorCursor, ' ');
          CopyToDest(ParseErrorCursor, ' ');
        }
      }

      CopyToDest(ParseErrorCursor, '\n');


      { // Output the final underline
        char_cursor ErrorCursor = CharCursor(ErrorMessage);
        u64 LongestLine = GetLongestLineInCursor(&ErrorCursor);

        PrintTray(ParseErrorCursor, 0, MaxTrayWidth, TerminalColors.Yellow);
        CopyToDest(ParseErrorCursor, TerminalColors.Yellow);
        for (u32 DashIndex = 0;
            DashIndex < LongestLine + 2;
            ++DashIndex)
        {
          CopyToDest(ParseErrorCursor, '_');
        }
        CopyToDest(ParseErrorCursor, '\n');
        CopyToDest(ParseErrorCursor, TerminalColors.White);
      }
    }









    {
      c_token *NextT = PeekTokenRawPointer(Parser);
      if (NextT) { PrintTray(ParseErrorCursor, NextT, MaxTrayWidth); }
    }

    u32 LinesToPrint = LinesOfContext;
    while ( c_token *T = PopTokenRawPointer(Parser) )
    {
      PrintToken(T, ParseErrorCursor);

      if ( IsNewline(T->Type) )
      {
        c_token *NextT = PeekTokenRawPointer(Parser);
        if (NextT) { PrintTray(ParseErrorCursor, NextT, MaxTrayWidth); }
        if (LinesToPrint-- == 0) break;
      }
    }

    char *End = ParseErrorCursor->End;
    ParseErrorCursor->End = ParseErrorCursor->At;
    ParseErrorCursor->At = ParseErrorCursor->Start;

    // TODO(Jesse, tags: bug): This isn't working for some reason.  I think
    // GetLongestLineInCursor is busted here.
    counted_string NameLine = FormatCountedString(TranArena, CSz("  %S:%u  "), ParserName, ErrorLineNumber);
    umm MinLineLen = 80;
    umm LongestLine = Max(MinLineLen, GetLongestLineInCursor(ParseErrorCursor));
    LongestLine = Max(MinLineLen, NameLine.Count+4);

    ParseErrorCursor->End = End;

    if (Global_LogLevel <= LogLevel_Error)
    {
      u64 HalfDashes = (LongestLine-NameLine.Count)/2;
      for (u32 DashIndex = 0; DashIndex < HalfDashes; ++DashIndex) { LogDirect(CSz("-")); }
      LogDirect(NameLine);
      for (u32 DashIndex = 0; DashIndex < HalfDashes; ++DashIndex) { LogDirect(CSz("-")); }

      LogDirect(CSz("\n"));
    }

    CopyToDest(ParseErrorCursor, '\n');
    for (u32 DashIndex = 0;
        DashIndex < LongestLine;
        ++DashIndex)
    {
      CopyToDest(ParseErrorCursor, '-');
    }

    CopyToDest(ParseErrorCursor, '\n');

    if (Global_LogLevel <= LogLevel_Error)
    {
      LogDirect("%S", CS(ParseErrorCursor));
    }


    RewindTo(Parser, ErrorToken);
    Assert(Parser->Tokens->At == ErrorToken);
    Assert(Parser->Tokens->At->LineNumber == ErrorLineNumber);

    Parser->ErrorMessage = ErrorMessage;
    Parser->ErrorCode = ErrorCode;
    Parser->ErrorToken = ErrorToken;
  }
  else
  {
    FormatCountedString_(ParseErrorCursor, CSz("Error determining where the error occurred\n"));
    FormatCountedString_(ParseErrorCursor, CSz("Error messsage was : %S\n"), ErrorMessage);
    LogDirect("%S", CS(ParseErrorCursor));
  }

  return;
}

bonsai_function void
ParseError(parser* Parser, counted_string ErrorMessage, c_token* ErrorToken)
{
  ParseError(Parser, ParseErrorCode_Unknown, ErrorMessage, ErrorToken);
}


#if 0
bonsai_function counted_string
ParseError_StreamEndedUnexpectedly(parser *Parser)
{
  counted_string Result = FormatCountedString(TranArena, CSz("Stream ended unexpectedly in file : %S"), ???->Filename);
  ParseError(Parser, ParseErrorCode_StreamEndedUnexpectedly, Result);
  return Result;
}
#endif

#define ParseErrorTokenHelper(T) \
  (T) ? ToString((T)->Type) : CSz(""), (T) ? (T)->Value : CSz("null")

bonsai_function counted_string
ParseError_ExpectedSemicolonEqualsCommaOrOpenBrace(parser *Parser, c_token *T)
{
  counted_string Result =
    FormatCountedString_(TranArena, CSz("Got %S(%S)\n\nExpected %S(%c), %S(%c), %S(%c) or %S(%c) while parsing variable declaration."),
        ParseErrorTokenHelper(T),
        ToString(CTokenType_Semicolon), CTokenType_Semicolon,
        ToString(CTokenType_Equals), CTokenType_Equals,
        ToString(CTokenType_Comma), CTokenType_Comma,
        ToString(CTokenType_OpenBrace), CTokenType_OpenBrace
        );

  ParseError(Parser, ParseErrorCode_ExpectedSemicolonOrEquals, Result, PeekTokenPointer(Parser) );
  return Result;
}

bonsai_function counted_string
ParseError_RequireTokenFailed(parser *Parser, counted_string FuncName, c_token *Got, c_token *Expected)
{
  counted_string Result = FormatCountedString( TranArena,
                                               CSz("%S Failed \n\nExpected %S(%S)\nGot      %S(%S)"),
                                               FuncName,
                                               ParseErrorTokenHelper(Expected),
                                               ParseErrorTokenHelper(Got));

  ParseError(Parser, ParseErrorCode_RequireTokenFailed, Result, Got);
  return Result;
}

/*****************************                 *******************************/
/*****************************  Token Control  *******************************/
/*****************************                 *******************************/



#define DEBUG_CHECK_FOR_BREAK_HERE(Result) \
  ( (Result) && ((Result)->Type == CTokenType_Identifier && StringsMatch(CSz("break_here"), (Result)->Value)) )


inline u32
RemainingForDir(c_token_cursor *Tokens, s32 Direction)
{
  u32 TokensRemaining = Direction > -1 ? (u32)Remaining(Tokens) : (u32)AtElements(Tokens);
  return TokensRemaining;
}

bonsai_function c_token *
GetNext(c_token_cursor *Tokens, s32 Direction)
{
  u32 TokensRemaining = RemainingForDir(Tokens, Direction);

  c_token *Result = 0;

  // TODO(Jesse)(speed optimize): These serially-dependant if-s should be able
  // to be written independantlly such that the CPU can execute them
  // out-of-order.  We might actually care about that.

  if (Direction == 0 && TokensRemaining > 0)
  {
    Result = Tokens->At;
  }
  else if ( Direction == -1 && TokensRemaining > 0 )
  {
    Tokens->At += Direction;
    Assert(IsValidForCursor(Tokens, Tokens->At));
    Result = Tokens->At;
  }
  else if ( Direction == 1 && TokensRemaining > 1 )
  {
    Tokens->At += Direction;
    Assert(IsValidForCursor(Tokens, Tokens->At));
    Result = Tokens->At;
  }
  else
  {
    Invalidate(Tokens);
  }

  return Result;
}

#if 0
bonsai_function c_token*
GetToken(c_token_cursor *Tokens, s32 PeekIndex)
{
  c_token* Result = 0;

  if (Tokens)
  {
    s32 Direction = GetSign(PeekIndex);
    Direction = Direction ? Direction : 1;
    u32 TokensRemaining = RemainingForDir(Tokens, Direction);
    if (TokensRemaining > Abs(PeekIndex))
    {
      Result = Tokens->At + PeekIndex;
      Assert(IsValidForCursor(Tokens, Result));
    }
    else
    {
      /* s32 Next = PeekIndex - (TokensRemaining*Direction); */
      /* Assert(GetSign(Next) == GetSign(PeekIndex) || Next == 0); */
      /* Result = GetToken(Tokens->Up, Next); */
    }
  }

  return Result;
}
#endif

#if 0
bonsai_function c_token*
PeekTokenRawPointer(c_token_cursor *Tokens, s32 MaxPeek)
{
  Assert(Tokens->At >= Tokens->Start);
  Assert(Tokens->At <= Tokens->End);
  Assert(Tokens->Start <= Tokens->End);

  c_token_cursor Tmp = *Tokens;

  s32 Direction = GetSign(MaxPeek);
  s32 PeekIndex = Direction;

  if (Direction == 0) Direction = 1;

  c_token* Result = 0;
  while ( (Result = GetToken(&Tmp, PeekIndex)) )
  {
    // We need to break out of the loop here if we've hit the last token we're
    // going to peek because if we wait till the end of the loop we'd peek into
    // CT_InsertedCode->Down.  We also have to check at the end of the loop
    // such that when we do peek down we don't overwrite the Result when we hit
    // the while() at the top.  Pretty crap, I know.. probably a better way of
    // phrasing this function.
    if (PeekIndex == MaxPeek)
    {
      break;
    }

    switch (Result->Type)
    {
      case CT_MacroLiteral:
      case CT_InsertedCode:
      {
        if (Result->Down)
        {
          // NOTE(Jesse): I ordered the 'macro_expansion' struct such that the
          // pointer to the expanded macro will be at the same place as the
          // `Down` poninter.  This is sketchy as fuck, but it'll work, and
          // this assertion should catch the bug if we reorder the pointers.
          Assert(Result->Down == Result->Macro.Expansion);

          Assert(Result->Down->At == Result->Down->Start);
          c_token_cursor TmpDown = *Result->Down;
          TmpDown.Up = &Tmp;
          Result = PeekTokenRawPointer(&TmpDown, MaxPeek - (PeekIndex + Direction));

          // NOTE(Jesse): This loop is gettin pretty tortured .. probably time
          // to rewrite it and clean up.
          /* PeekIndex = MaxPeek; */
        }
      } break;

      default: { } break;
    }

    if (PeekIndex == MaxPeek)
    {
      break;
    }
    else
    {
      PeekIndex += Direction;
    }
  }

#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    Result = PeekTokenRawPointer(&Tmp, MaxPeek + 1);
  }

  if (Result && Result->Type == CTokenType_Identifier) { Assert(Result->Value.Start); Assert(Result->Value.Count);  }
  if (Result) { Assert(Result->Type); }
#endif

  Assert(Tmp.At    >= Tmp.Start);
  Assert(Tmp.At    <= Tmp.End);
  Assert(Tmp.Start <= Tmp.End);

  return Result;
}

#else

bonsai_function peek_result
PeekTokenRawCursor(c_token_cursor *Tokens, s32 Direction, b32 CanSearchDown)
{
  Assert(Direction > -2 && Direction < 2);

  c_token_cursor Cached = *Tokens;

  peek_result Result = {};
  Result.Tokens = Tokens;

  if (Tokens->At < Tokens->End)
  {
    Result.At = Tokens->At;
  }

  switch  (Direction)
  {
    case 0: {} break;

    case 1:
    {
      c_token_cursor *Down = HasValidDownPointer(Result.At);
      b32 SearchDown = CanSearchDown && Down && RemainingForDir(Down, Direction) > 0;
      if (SearchDown)
      {
        Assert(Result.At->Down == Result.At->Macro.Expansion); // @janky-macro-expansion-struct-ordering

        c_token *PrevDownAt = Down->At;
        Down->At = Down->Start;
        Result = PeekTokenRawCursor(Down, Min(0, Direction));
        Down->At = PrevDownAt;

        // NOTE(Jesse): This is not strictly valid, but for well-formed code it
        // should always pass
        Assert(GetNext(Tokens, 0) != 0);
      }

      if ( (SearchDown && IsValid(&Result) == False) || // Didn't get a token peeking down
            SearchDown == False                       )
      {
        Result.At = GetNext(Tokens, Direction);
      }

      if (!IsValid(&Result) && Tokens->Up.Tokens) // Down buffer(s) and current buffer had nothing, pop up
      {
        c_token *UpAt = Tokens->Up.Tokens->At;
        Tokens->Up.Tokens->At = Tokens->Up.At;
        Assert(Tokens->Up.At->Type == CT_InsertedCode ||
               Tokens->Up.At->Type == CT_MacroLiteral);
        Result = PeekTokenRawCursor(Tokens->Up.Tokens, Direction);
        Tokens->Up.Tokens->At = UpAt;
        Result.DoNotDescend = True;
      }
    } break;

    case -1:
    {
      Result.At = GetNext(Tokens, Direction);

      c_token_cursor *Down = HasValidDownPointer(Result.At);
      b32 SearchDown = CanSearchDown && Down && RemainingForDir(Down, Direction) > 0;
      if (SearchDown)
      {
        Assert(Result.At->Down == Result.At->Macro.Expansion); // @janky-macro-expansion-struct-ordering

        c_token *PrevDownAt = Down->At;
        Down->At = Down->End;
        Result = PeekTokenRawCursor(Down, Min(0, Direction));
        Down->At = PrevDownAt;

        // NOTE(Jesse): This is not strictly valid, but for well-formed code it
        // should always pass
        Assert(GetNext(Tokens, 0) != 0);
      }

      if (!IsValid(&Result) && Tokens->Up.Tokens) // Down buffer(s) and current buffer had nothing, pop up
      {
        c_token *UpAt = Tokens->Up.Tokens->At;
        Tokens->Up.Tokens->At = Tokens->Up.At;
        Assert(Tokens->Up.At->Type == CT_InsertedCode ||
               Tokens->Up.At->Type == CT_MacroLiteral);
        Result = PeekTokenRawCursor(Tokens->Up.Tokens, 0, False);
        Tokens->Up.Tokens->At = UpAt;
        Result.DoNotDescend = True;
      }

    } break;

    InvalidDefaultCase;
  }

  *Tokens = Cached;

  if (IsValid(&Result))
  {
    Assert(Result.At);
    Assert(Result.Tokens);
    Assert(IsValidForCursor(Result.Tokens, Result.At));

    Assert(Result.Tokens->Start);
    Assert(Result.Tokens->At);
    Assert(Result.Tokens->End);
  }


#if 0
#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    Result = PeekTokenPointer(Tokens, Direction);
  }

  if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif
#endif

  return Result;
}

bonsai_function peek_result
PeekTokenRawCursor(peek_result *Peek, s32 Direction)
{
  peek_result Result = {};

  if (IsValid(Peek))
  {
    c_token *PrevAt = Peek->Tokens->At;
    Peek->Tokens->At = Peek->At;
    peek_result TmpResult = PeekTokenRawCursor(Peek->Tokens, Direction, Peek->DoNotDescend == False);
    Peek->Tokens->At = PrevAt;

    if (IsValid(&TmpResult))
    {
      Result = TmpResult;
    }
  }
  return Result;
}

bonsai_function peek_result
PeekTokenRawCursor(parser *Parser, s32 Direction)
{
  peek_result Result = {};
  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    Result = PeekTokenRawCursor(Parser->Tokens, Direction);
  }
  return Result;
}

#endif

bonsai_function c_token*
PeekTokenRawPointer(parser* Parser, s32 Lookahead)
{
  c_token *T = {};

  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    s32 Direction = GetSign(Lookahead);
    s32 Count = Direction;

    peek_result Current = PeekTokenRawCursor(Parser->Tokens, Direction);
    while (Count != Lookahead)
    {
      Current = PeekTokenRawCursor(&Current, Direction);
      Count += Direction;
    }

    if (IsValid(&Current))
    {
      T = Current.At;
    }
  }
  return T;
}

bonsai_function c_token*
PeekTokenRawPointer(parser* Parser, u32 Lookahead)
{
  c_token *T = PeekTokenRawPointer(Parser, (s32)Lookahead);
  return T;
}

bonsai_function c_token
PeekTokenRaw(parser* Parser, s32 Direction)
{
  c_token *T = PeekTokenRawPointer(Parser, Direction);
  c_token Result = {};
  if (T) Result = *T;
  return Result;
}

bonsai_function peek_result
PeekTokenCursor(c_token_cursor *Tokens, s32 Skip)
{
  SanityCheckCTokenCursor(Tokens);

  c_token *Result = {};

  s32 Direction = GetSign(Skip);
  if (Direction == 0) Direction = 1;
  Assert(Direction == 1 || Direction == -1);


  s32 Hits = Min(0, Direction);
  peek_result Current = PeekTokenRawCursor(Tokens, Hits);
  while ( IsValid(&Current) )
  {
    if ( Current.At->Erased )
    {
    }
    else if ( Current.At->Type == CTokenType_CommentSingleLine)
    {
    }
    else if ( Current.At->Type == CTokenType_CommentMultiLine)
    {
    }
    else if (IsWhitespace(Current.At))
    {
    }
    else
    {
      if (Hits == Skip)
      {
        break;
      }
      else
      {
        Hits += Direction;
      }
    }

    Current = PeekTokenRawCursor(&Current, Direction);
  }

  // TODO(Jesse): Pack the conditional up above into a function we can call
  // here.. ? 
  if (IsValid(&Current) && (Current.At->Erased || IsWhitespace(Current.At) || IsComment(Current.At))) { Invalidate(&Current); } // Fires if the stream ends with whitespace/comment


#if 0
#if BONSAI_INTERNAL
  if (Current.At && DEBUG_CHECK_FOR_BREAK_HERE(*Current.At))
  {
    Current.At = PeekTokenPointer(Tokens, Lookahead+1);
  }

  if (Current.At) { Assert(!StringsMatch(Current.At->Value, CSz("break_here"))); }
#endif
#endif

  SanityCheckCTokenCursor(Tokens);
  return Current;
}


bonsai_function peek_result
PeekTokenCursor(parser *Parser, s32 Skip)
{
  peek_result Result = {};
  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    Result = PeekTokenCursor(Parser->Tokens, Skip);
  }
  return Result;
}

bonsai_function c_token*
PeekTokenPointer(parser* Parser, s32 Skip)
{
  c_token *Result = 0;
  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    peek_result C = PeekTokenCursor(Parser->Tokens, Skip);
    if (IsValid(&C)) Result = C.At;
  }
  return Result;
}

bonsai_function c_token*
PeekTokenPointer(parser* Parser, u32 Skip)
{
  c_token *Result = PeekTokenPointer(Parser, (s32)Skip);
  return Result;
}

#if 0
bonsai_function c_token*
PeekTokenPointer(parser* Parser, u32 Lookahead)
{
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;

  c_token* Result = PeekTokenRawPointer(Parser, LocalLookahead);
  while (Result)
  {
    if ( Result->Erased )
    {
    }
    else if ( Result->Type == CTokenType_CommentSingleLine)
    {
    }
    else if ( Result->Type == CTokenType_CommentMultiLine)
    {
    }
    else if (IsWhitespace(Result))
    {
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

  if (Result && (IsWhitespace(Result) || IsComment(Result))) { Result = 0; } // Fires if the stream ends with whitespace/comment

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
#endif

bonsai_function c_token
PeekToken(parser* Parser, s32 Lookahead)
{
  c_token* Pointer = PeekTokenPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

bonsai_function c_token *
PopTokenRawPointer(parser* Parser)
{
  peek_result This = PeekTokenRawCursor(Parser);
  c_token *Result = 0;

  if (IsValid(&This))
  {
    Result = AdvanceTo(Parser, &This);
  }

  peek_result Next = PeekTokenRawCursor(Parser, 1);
  if (IsValid(&Next))
  {
    Result = AdvanceTo(Parser, &Next);
  }
  else
  {
    Invalidate(Parser->Tokens);
  }

#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    AdvanceParser(Parser);
    Result = PopTokenRawPointer(Parser);
  }

  if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif

  return Result;
}

bonsai_function c_token
PopTokenRaw(parser* Parser)
{
  c_token *T = 0;

  peek_result Peek = PeekTokenRawCursor(Parser, 1);
  T = AdvanceTo(Parser, &Peek);

  c_token Result = {};
  if (T) Result = *T;

#if BONSAI_INTERNAL
  if (DEBUG_CHECK_FOR_BREAK_HERE(&Result))
  {
    RuntimeBreak();
    if (PeekTokenRawPointer(Parser)) { AdvanceParser(Parser); }
    Result = PopTokenRaw(Parser);
  }

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

bonsai_function c_token *
PopTokenPointer(parser* Parser)
{
  peek_result NextT = PeekTokenCursor(Parser);

  // TODO(Jesse): This is kinda tortured .. should probably work on the API
  // here. In particular it's not obvious what AdvanceTo returns, and it
  // actually returns what I wouldn't expect if I guessed.
  //
  // Furthermore the first AdvanceTo is necessary because of the return value
  // weirdness, while it shouldn't be necessary if the API was better.
  //
  // UPDATE(Jesse): Now we `should` be able to just do `AdvanceTo(NextRawT)`
  // without the intermediate AdvanceTo .. I think
  //

  AdvanceTo(Parser, &NextT);
  peek_result NextRawT = PeekTokenRawCursor(&NextT, 1);
  c_token *Result = AdvanceTo(Parser, &NextRawT);

#if BONSAI_INTERNAL
  if (DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    if (PeekTokenRawPointer(Parser)) { AdvanceParser(Parser); }
    Result = PopTokenPointer(Parser);
  }
  else if (Result)
  {
    Assert(!StringsMatch(Result->Value, CSz("break_here")));
  }

#endif

  return Result;
}
bonsai_function c_token
PopToken(parser* Parser)
{
  c_token *Pop = PopTokenPointer(Parser);

  c_token Result = {};
  if (Pop) Result = *Pop;

  return Result;
}

bonsai_function b32
OptionalTokenRaw(parser* Parser, c_token_type Type)
{
  b32 Result = False;
  c_token Peeked = PeekTokenRaw(Parser);
  if (Peeked.Type == Type)
  {
    Result = True;
    c_token Popped = PopTokenRaw(Parser);
    Assert(Popped.Type == Peeked.Type);
    Assert(Popped.Type == Type);
  }

  return Result;
}

bonsai_function b32
OptionalTokenRaw(parser* Parser, c_token T)
{
  b32 Result = False;
  c_token Peeked = PeekTokenRaw(Parser);
  if (Peeked == T)
  {
    Result = True;
    c_token Popped = PopTokenRaw(Parser);
    Assert(Popped == Peeked);
  }

  return Result;
}

// TODO(Jesse): Optimize by calling Advance() instead of RequireToken()
// @optimize_call_advance_instead_of_being_dumb
bonsai_function c_token *
OptionalToken(parser* Parser, c_token T)
{
  c_token *Result = PeekTokenPointer(Parser);
  if (Result && *Result == T) { RequireToken(Parser, *Result); } else { Result = 0;}
  return Result;
}

// TODO(Jesse): Optimize by calling Advance() instead of RequireToken()
// @optimize_call_advance_instead_of_being_dumb
bonsai_function c_token *
OptionalToken(parser* Parser, c_token_type Type)
{
  c_token *Result = PeekTokenPointer(Parser);
  if (Result && Result->Type == Type) { RequireToken(Parser, *Result); } else { Result = 0; }
  return Result;
}

// TODO(Jesse): Could be optimized significanly utilizing new routines.
// @optimize_call_advance_instead_of_being_dumb
bonsai_function c_token
RequireToken(parser* Parser, c_token ExpectedToken)
{
  c_token Result = {};

  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    c_token* PeekedToken = PeekTokenPointer(Parser);
    if (PeekedToken) Result = *PeekedToken;

    // TODO(Jesse, id: 348, tags: immediate, id_347) : This should go into an AreEqual bonsai_function I think..
    if ( Result.Type != ExpectedToken.Type ||
         (ExpectedToken.Value.Count > 0 && !StringsMatch(ExpectedToken.Value, Result.Value) ))
    {
      ParseError_RequireTokenFailed( Parser, CSz("RequireToken"), PeekedToken, &ExpectedToken );
    }
    else
    {
      Result = PopToken(Parser);
    }

#if BONSAI_INTERNAL
    Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif
  }

  return Result;
}

// TODO(Jesse): This function should likely be primal for the sake of efficiency
bonsai_function c_token
RequireToken(parser* Parser, c_token *ExpectedToken)
{
  c_token Result = {};
  if (ExpectedToken)
  {
    Result = RequireToken(Parser, *ExpectedToken);
  }
  else
  {
    Error("Internal error : RequireToken was passed ptr(0)");
  }
  return Result;
}


bonsai_function c_token
RequireToken(parser* Parser, c_token_type ExpectedType)
{
  c_token Result = RequireToken(Parser, CToken(ExpectedType));
  return Result;
}

bonsai_function c_token *
RequireTokenPointer(parser* Parser, c_token *ExpectedToken)
{
  c_token *Result = {};

  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    c_token* PeekedToken = PeekTokenPointer(Parser);

    // TODO(Jesse, id: 348, tags: immediate, id_347) : This should go into an AreEqual bonsai_function I think..
    if (PeekedToken && 
         (PeekedToken->Type != ExpectedToken->Type ||
         (ExpectedToken->Value.Count > 0 && !StringsMatch(ExpectedToken->Value, PeekedToken->Value) ))
       )
    {
      ParseError_RequireTokenFailed( Parser, CSz("RequireToken"), PeekedToken, ExpectedToken );
    }
    else
    {
      Result = PopTokenPointer(Parser);
      Assert(Result == PeekedToken);
    }

#if BONSAI_INTERNAL
    if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif
  }

  return Result;
}

bonsai_function c_token*
RequireTokenPointer(parser* Parser, c_token Expected)
{
  c_token *Result = RequireTokenPointer(Parser, &Expected);
  return Result;
}

bonsai_function c_token*
RequireTokenPointer(parser* Parser, c_token_type ExpectedType)
{
  c_token *Result = RequireTokenPointer(Parser, CToken(ExpectedType));
  return Result;
}


// @duplicated_require_token_raw
// @optimize_call_advance_instead_of_being_dumb
bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token Expected )
{
  c_token *Peek = PeekTokenRawPointer(Parser);
  if (Peek && *Peek == Expected)  // TODO(Jesse id: 349, tags: id_347) : Change to a function call instead of == operator
  {
    PopTokenRaw(Parser);
  }
  else
  {
    ParseError_RequireTokenFailed(Parser, CSz("RequireTokenRaw"), Peek, &Expected);
  }

  c_token Result = {};
  if (Peek) Result = *Peek;
  return Result;
}

bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token *Expected )
{
  Assert(Expected);
  c_token Result = RequireTokenRaw(Parser, *Expected);
  return Result;
}

// @duplicated_require_token_raw
// @optimize_call_advance_instead_of_being_dumb
bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token_type Expected )
{
  c_token *Peek = PeekTokenRawPointer(Parser);
  if (Peek->Type == Expected)  // TODO(Jesse tags:id_347) : Change to a function call instead of == operator
  {
    PopTokenRaw(Parser);
  }
  else
  {
    c_token Temp = CToken(Expected);
    ParseError_RequireTokenFailed(Parser, CSz("RequireTokenRaw"), Peek, &Temp);
  }

  c_token Result = {};
  if (Peek) Result = *Peek;
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

bonsai_function c_token *
RequireOperatorToken(parser* Parser)
{
  c_token *Result = PeekTokenPointer(Parser);
  if (Result)
  {
    switch (Result->Type)
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
      case CTokenType_Bool:
      case CTokenType_Equals: // Assignment
      {
        RequireToken(Parser, Result->Type);
      } break;

      case CTokenType_OpenBracket:
      {
        RequireToken(Parser, Result->Type);
        RequireToken(Parser, CTokenType_CloseBracket);
      } break;

      default:
      {
        ParseError(Parser, FormatCountedString(TranArena, CSz("Expected operator, got %S(%S)"), ToString(Result->Type), Result->Value), Result);
        Result = 0;
      } break;
    }
  }

  return Result;
}






bonsai_function void
CopyRemainingIntoCursor(c_token_cursor *Src, c_token_cursor *Dest)
{
  u32 SrcElements = (u32)Remaining(Src);
  u32 DestElements = (u32)Remaining(Dest);

  /* DebugLine(FormatCountedString(TranArena, CSz("copy %lu -> %lu"), SrcElements, DestElements)); */
  /* DumpSingle(Src, Src->Start); */
  /* DumpSingle(Dest, Dest->Start); */

  if ( SrcElements <= DestElements )
  {
    for ( u32 TokenIndex = 0;
          TokenIndex < SrcElements;
          ++TokenIndex)
    {
      Push(Src->At[TokenIndex], Dest);
    }
  }
  else
  {
    Error("Dest c_token_cursor was too full to hold entire source buffer!" );
  }
}

bonsai_function void
CopyBufferIntoCursor(c_token_buffer *Src, c_token_cursor *Dest, u32 Skip = 0)
{
  if ( Src->Count <= Remaining(Dest) )
  {
    for ( u32 TokenIndex = Skip;
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

bonsai_function c_token_buffer *
TryMacroArgSubstitution(c_token *T, counted_string_buffer *NamedArguments, c_token_buffer_buffer *ArgInstanceValues)
{
  c_token_buffer *Result = {};

  u32 ArgIndex = (u32)IndexOf(NamedArguments, T->Value);
  if (ArgIndex < NamedArguments->Count && ArgIndex < ArgInstanceValues->Count)
  {
    Result = ArgInstanceValues->Start + ArgIndex;
  }

  return Result;
}

bonsai_function void
CopyVaArgsInto(c_token_buffer_stream *VarArgs, c_token_cursor *Dest, b32 DoCommas)
{
  ITERATE_OVER(VarArgs)
  {
    c_token_buffer* Arg = GET_ELEMENT(Iter);
    CopyBufferIntoCursor(Arg, Dest);

    if (DoCommas)
    {
      if (IsLastElement(&Iter) == False)
      {
        Push(CToken(CTokenType_Comma, CSz(",")), Dest);
      }
    }
  }
}

// TODO(Jesse, tags: immediate, memory):  The memory allocation in this function
// is super bad.  What we need is Begin/End TemporaryMemory such that we can
// allocate a bunch of temp shit on the TempMemory arena and roll it all back
// at the end of the function.  The only thing that goes on the PermMemory is
// the Result.
bonsai_function parser *
ExpandMacro( parse_context *Ctx,
             parser *Parser,
             macro_def *Macro,
             memory_arena *PermMemory,
             memory_arena *TempMemory,
             b32 ScanArgsForAdditionalMacros
           )
{
  TIMED_FUNCTION();

  parser *FirstPass = AllocateParserPtr(
                        CSz("macro_expansion TODO(Jesse): What do we say here?"),
                        0,
                        (u32)Kilobytes(32),
                        TokenCursorSource_MacroExpansion,
                        0,
                        {0, 0},
                        TranArena
                      );

  Assert(Macro->Body.At == Macro->Body.Start);
  Assert(Macro->Body.Up.Tokens == 0);

  EatWhitespaceAndComments(Parser);

  // NOTE(Jesse): Because of how the main preprocessor loop works (the part
  // that does #if #else evaluation) this routine must be able to require the
  // macro name token without calling RequireTokenRaw().
  //
  // TODO(Jesse, correctness): Is this a bug, or is the above comment wrong?
  //
  c_token MacroNameT = RequireTokenRaw(Parser, CToken(CT_MacroLiteral, Macro->Name));

  // NOTE(Jesse): These get filled out in the case where we've got a macro
  // function to expand.  The expansion loop works for both function and
  // keyword style macros if they're left empty
  //
  c_token_buffer_stream VarArgs = {};
  c_token_buffer_buffer ArgInstanceValues = {};

  TIMED_BLOCK("Parse Instance Args");
  switch (Macro->Type)
  {
    case type_macro_keyword:
    {
      // Keyword macros don't need any special treatment
    } break;

    case type_macro_function:
    {
      // Parse instance args

      c_token *Start = PeekTokenPointer(Parser);
      if (Start && Start->Type == CTokenType_OpenParen)
      {
        c_token_cursor Tokens = CTokenCursor(Start, Start, CSz(DEFAULT_FILE_IDENTIFIER), TokenCursorSource_IntermediateRepresentaton, {0,0} );
        parser InstanceArgs_ = { .Tokens = &Tokens };
        parser *InstanceArgs = &InstanceArgs_;

        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        InstanceArgs->Tokens->End = Parser->Tokens->At;

        TrimFirstToken(InstanceArgs, CTokenType_OpenParen);
        TrimLastToken(InstanceArgs, CTokenType_CloseParen);

        Assert(Ctx);

        if (ScanArgsForAdditionalMacros)
        {
          // TODO(Jesse): Can call PopTokenRawPointer() instead?
          // @optimize_call_advance_instead_of_being_dumb
          while (c_token *T = PeekTokenRawPointer(InstanceArgs))
          {
            if (T->Type == CTokenType_Identifier)
            {
              TryTransmuteIdentifierToMacro(Ctx, InstanceArgs, T, Macro);
            }
            PopTokenRaw(InstanceArgs);
          }
        }
        FullRewind(InstanceArgs);

        if ( TotalElements(InstanceArgs->Tokens) == 1 &&
              Macro->NamedArguments.Count == 0 )

        {
          OptionalToken(InstanceArgs, CToken(CTokenType_Void, CSz("void")));
        }

        //
        // Parse the instance args into their buffer
        //

        if (Macro->NamedArguments.Count)
        {
          ArgInstanceValues = CTokenBufferBuffer(Macro->NamedArguments.Count, TempMemory);
          for ( u32 ArgIndex = 0;
                ArgIndex < ArgInstanceValues.Count;
                ++ArgIndex)
          {
            ParseMacroArgument(InstanceArgs, ArgInstanceValues.Start+ArgIndex);
          }
        }

        if (Macro->Variadic)
        {
          while (TokensRemain(InstanceArgs))
          {
            c_token_buffer *Arg = Push(&VarArgs, {}, TempMemory);
            ParseMacroArgument(InstanceArgs, Arg);
          }
        }
        Assert(TokensRemain(InstanceArgs) == 0);
      }

    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Invalid Macro Type detected"));
  }
  END_BLOCK();

  /* c_token *ResetToken = PeekTokenRawPointer(Parser); */



  TIMED_BLOCK("Parameter Sub && Pasting");

  parser *MacroBody = DuplicateCTokenCursor2(&Macro->Body, TempMemory);

  //
  // Pre-scan for any macros such that we can mark __VA_ARGS__ to expand with
  // comma separators.  This is to support @va_args_paste_into_another_macro
  //

  while (c_token *T = PeekTokenRawPointer(MacroBody))
  {
    Assert(T->Type != CT_MacroLiteral);

    // NOTE(Jesse): Can't do this because IdentifierShouldBeExpanded needs to
    // be able to peek the next token.  I suppose we could make it just blindly
    // trust we're on a macro, but I'd much rather have assertions in that
    // function.  Maybe this is indicative that that function is weird and we
    // should delete it?
    //
    // RequireTokenRaw(MacroBody, T);


    if ( T->Type == CTokenType_Identifier )
    {
      macro_def *ThisMacro = IdentifierShouldBeExpanded(Ctx, MacroBody, T, Macro);
      if (ThisMacro && ThisMacro->Type == type_macro_function)
      {
        RequireTokenRaw(MacroBody, T);

        u32 Depth = 0;

        for (;;)
        {
          T = PopTokenRawPointer(MacroBody);

          if (T->Type == CT_Preprocessor__VA_ARGS__)
          {
            Assert(T->Flags == 0);
            T->Flags |= va_args_flags_expand_with_commas;
          }

          if (T->Type == CTokenType_OpenParen)
          {
            ++Depth;
          }

          if (T->Type == CTokenType_CloseParen)
          {
            // NOTE(Jesse): Decrementing first is kinda wonky, but we start on
            // the macro token and always hit an open paren
            Assert(Depth > 0);
            if (--Depth == 0)
            {
              break;
            }
          }

          continue;
        }

      }
      else
      {
        RequireTokenRaw(MacroBody, T);
      }
    }
    else
    {
      RequireTokenRaw(MacroBody, T);
    }
  }

  FullRewind(MacroBody);

  //
  // Do parameter substitution and pasting
  //

  // @optimize_call_advance_instead_of_being_dumb
  while (c_token *T = PeekTokenRawPointer(MacroBody))
  {
    if (MacroBody->ErrorCode) break;

    switch (T->Type)
    {
      case CT_Preprocessor__VA_ARGS__:
      {
        RequireToken(MacroBody, T->Type);
        CopyVaArgsInto(&VarArgs, FirstPass->Tokens, T->Flags & va_args_flags_expand_with_commas);
      } break;

      case CTokenType_Identifier:
      {
        RequireToken(MacroBody, T);

        if (c_token_buffer *ArgBuffer = TryMacroArgSubstitution(T, &Macro->NamedArguments, &ArgInstanceValues ))
        {
          c_token_cursor ArgTokens = CTokenCursor(ArgBuffer, CSz(DEFAULT_FILE_IDENTIFIER), TokenCursorSource_IntermediateRepresentaton, {0,0} );
          parser Arguments = MakeParser(&ArgTokens);
          while (c_token* ArgToken = PeekTokenRawPointer(&Arguments))
          {
            if (ArgToken->Type == CT_MacroLiteral ||
               (ArgToken->Type == CTokenType_Identifier && TryTransmuteIdentifierToMacro(Ctx, &Arguments, ArgToken, Macro)) )
            {
              parser *Expanded = ExpandMacro(Ctx, &Arguments, ArgToken->Macro.Def, PermMemory, TempMemory);
              if (Expanded->ErrorCode == ParseErrorCode_None)
              {
                // Copy expanded buffer into FirstPass, marking self-referring
                // keyword instances such that we don't expand them again.
                while (c_token *ExpandedT = PopTokenRawPointer(Expanded))
                {
                  // TODO(Jesse, tags: metaprogramming, correctness): Test this
                  // path actually does what I intended.  At the moment I
                  // observed self-refe'd macros not getting expanded due to
                  // MacroShouldBeExpanded()
                  //
                  // @mark_keyword_macros_self_referential
                  //
                  if (Macro->Type == type_macro_keyword && StringsMatch(ExpandedT->Value, Macro->Name))
                  {
                    ExpandedT->Type = CT_MacroLiteral_SelfRefExpansion;
                  }
                  Push(*ExpandedT, FirstPass->Tokens);
                }
              }
              else
              {
                ParseError( FirstPass,
                            Expanded->ErrorCode,
                            FormatCountedString(TranArena, CSz("While Expanding %S"), ArgToken->Value),
                            ArgToken);
              }
            }
            else
            {
              // @token_control_pointers
              Ensure(PopTokenRawPointer(&Arguments) == ArgToken);
              Push(*ArgToken, FirstPass->Tokens);
            }

          }
        }
        else
        {
          Push(*T, FirstPass->Tokens);
        }
      } break;

      case CT_PreprocessorPaste:
      {
        c_token *Prev = PeekTokenPointer(FirstPass, -1);
        c_token *Next = PeekTokenPointer(MacroBody, 1);

        // TODO(Jesse): Once token control functions return pointers we can
        // rewrite this such that Next gets assigned here instead of in the
        // PeekTokenPointer call above
        //
        // @token_control_pointers
        RequireToken(MacroBody, CT_PreprocessorPaste);
        RequireToken(MacroBody, Next->Type);

        if (c_token_buffer *ArgBuffer = TryMacroArgSubstitution(Next, &Macro->NamedArguments, &ArgInstanceValues ))
        {
          c_token_cursor Tokens = CTokenCursor(ArgBuffer, CSz(DEFAULT_FILE_IDENTIFIER), TokenCursorSource_IntermediateRepresentaton, {0,0} );
          parser Temp = MakeParser(&Tokens);
          Next = PeekTokenPointer(&Temp);

          if (Next && Next->Type == CT_MacroLiteral)
          {
            // NOTE(Jesse, tags: begin_temporary_memory): We need BeginTemporaryMemory here
            parser *Expanded = ExpandMacro(Ctx, &Temp, Next->Macro.Def, TranArena, TempMemory);
            if (Expanded->ErrorCode)
            {
              // TOOD(Jesse, tags: immediate): Write a test that exercises this
              // path.  Right now there's a bug blocking it
              ParseError( &Temp,
                          Expanded->ErrorCode,
                          FormatCountedString(TranArena, CSz("0 While Expanding %S"), Next->Value),
                          Next);
            }
            else
            {
              Next = PopTokenPointer(Expanded);
              CopyRemainingIntoCursor(Expanded->Tokens, FirstPass->Tokens);
              /* DumpChain(FirstPass); */
            }
            // NOTE(Jesse, tags: end_temporary_memory begin_temporary_memory):
            // We would roll-back the temp-mem here
          }
          else
          {
            // If we ever hit this assertion (I think) it means that Next was
            // declared as a macro _after_ the currently expanding macro.  I
            // was not sure if this case would ever happen, but it might.
            //
            // TODO(Jesse, correctness, immediate): Prove/Deny this can ever be
            // hit, and the above statement is correct.
            if (Next && Next->Type == CTokenType_Identifier && TryTransmuteIdentifierToMacro(Ctx, &Temp, Next, Macro) )
            {
              Assert(False);
            }

            // @token_control_pointers
            Ensure(PopTokenPointer(&Temp) == Next);
            CopyRemainingIntoCursor(Temp.Tokens, FirstPass->Tokens);
          }

        }


        if (Prev && Next)
        {
          // This is a rediculous special case.
          //
          // @non_portable_va_args_paste for more information on wtf is going
          // on.  Somebody is definitely fired.
          if ( Prev->Type == CTokenType_Comma &&
               Next->Type == CT_Preprocessor__VA_ARGS__ &&
               VarArgs.FirstChunk == 0 )
          {
            Prev->Type = CT_Preprocessor_Nuked;
            Prev->Erased = True;
            Prev->Value.Start = 0;
            Prev->Value.Count = 0;
          }
          else if ( Next->Type == CT_Preprocessor__VA_ARGS__ )
          {
            // Don't paste __VA_ARGS__ onto stuff
            //
            // TODO(Jesse, correctness): What do real compilers do?  No idea.
            //
            CopyVaArgsInto(&VarArgs, FirstPass->Tokens, True);
          }
          else
          {
            Prev->Type = CT_PreprocessorPaste_InvalidToken;
            Prev->Value = Concat(Prev->Value, Next->Value, &Global_PermMemory);

            // TODO(Jesse, correctness): Pasting numeric tokens together should
            // result in new numeric tokens .. yes..?  Write TryTransmuteNumber()
            //
            Prev->UnsignedValue = 0;

            if (TryTransmuteKeywordToken(Prev, 0))
            {
            }
            else if (TryTransmuteOperatorToken(Prev))
            {
            }
            else if (TryTransmuteIdentifierToken(Prev))
            {
            }
            else
            {
              umm CurrentSize = TotalSize(FirstPass->Tokens);
              TruncateToCurrentElements(FirstPass->Tokens);
              /* umm NewSize = TotalSize(&FirstPass->Tokens); */
              /* Reallocate((u8*)FirstPass->Tokens->Start, TranArena, CurrentSize, NewSize); */

              ParseError(FirstPass,
                         ParseErrorCode_InvalidTokenGenerated,
                         FormatCountedString(TranArena, CSz("Invalid token generated during paste (%S)"), Prev->Value),
                         Prev);
            }

          }
        }
        else if (Next) // No Prev pointer .. macro was called like : CONCAT(,only_passed_one_thing)
        {
          Push(*Next, FirstPass->Tokens);
        }
      } break;

      default:
      {
        PopTokenRaw(MacroBody);
        Push(*T, FirstPass->Tokens);
      } break;
    }

  }
  END_BLOCK();


  parser *Result = AllocateParserPtr(
                      CSz("macro_expansion TODO(Jesse): What do we say here?"),
                      0,
                      (u32)Kilobytes(32),
                      TokenCursorSource_MacroExpansion,
                      0,
                      {0,0},
                      PermMemory
                    );


  { // NOTE(Jesse): Unnecessary to reallocate here .. blow whole allocation
    // away at the end of the function
    umm CurrentSize = TotalSize(FirstPass->Tokens);
    TruncateToCurrentElements(FirstPass->Tokens);
  }

  Rewind(FirstPass->Tokens);

#if 0
    umm CurrentSize = TotalSize(Result->Tokens);
    TruncateToCurrentElements(Result->Tokens);
#else
  if (FirstPass->ErrorCode == ParseErrorCode_None)
  {
    TIMED_BLOCK("Expand Body");

    while (c_token *T = PeekTokenRawPointer(FirstPass))
    {
      T->LineNumber = MacroNameT.LineNumber;
      T->Filename = MacroNameT.Filename;

      switch(T->Type)
      {
        case CTokenType_Identifier:
        {
          // TODO(Jesse): What is specified to happen if a named argument
          // has the same value as a defined macro?  Is this correct?

          if (TryTransmuteIdentifierToMacro(Ctx, FirstPass, T, Macro))
          {
            parser *Expanded = ExpandMacro(Ctx, FirstPass, T->Macro.Def, PermMemory, TempMemory);
            if (Expanded->ErrorCode)
            {
              ParseError( FirstPass,
                          Expanded->ErrorCode,
                          FormatCountedString(TranArena, CSz("While Expanding %S"), T->Value),
                          T);
            }
            else
            {
              CopyRemainingIntoCursor(Expanded->Tokens, Result->Tokens);
            }
          }
          else
          {
            // @token_control_pointers
            Ensure(PopTokenRawPointer(FirstPass) == T);
            Push(*T, Result->Tokens);
          }

        } break;

        case CT_MacroLiteral:
        {
          parser *Expanded = ExpandMacro(Ctx, FirstPass, T->Macro.Def, PermMemory, TempMemory);
          if (Expanded->ErrorCode)
          {
            ParseError( FirstPass,
                        Expanded->ErrorCode,
                        FormatCountedString(TranArena, CSz("While Expanding %S"), T->Value),
                        T);
          }
          else
          {
            CopyRemainingIntoCursor(Expanded->Tokens, Result->Tokens);
          }
        } break;


        default:
        {
          // @token_control_pointers
          Ensure(PopTokenRawPointer(FirstPass) == T);
          Push(*T, Result->Tokens);
        } break;
      }

    }

    END_BLOCK();
  }
#endif

  if (FirstPass->ErrorCode == ParseErrorCode_None)
  {
    umm CurrentSize = TotalSize(FirstPass->Tokens);
    TruncateToCurrentElements(FirstPass->Tokens);
    /* umm NewSize = TotalSize(FirstPass->Tokens); */
    /* Reallocate((u8*)FirstPass->Tokens->Start, TranArena, CurrentSize, NewSize); */
  }

  {
    umm CurrentSize = TotalSize(Result->Tokens);
    TruncateToCurrentElements(Result->Tokens);
    /* umm NewSize = TotalSize(Result->Tokens); */
    /* Reallocate((u8*)Result->Tokens->Start, PermMemory, CurrentSize, NewSize); */
  }

  Rewind(Result->Tokens);
  /* LogDirect("\n\n -- %S\n", Macro->Name); */
  /* DumpChain(FirstPass); */
  /* DumpChain(Result); */
  /* LogDirect("\n"); */

  Result->ErrorCode = FirstPass->ErrorCode;
  Result->ErrorToken = FirstPass->ErrorToken;
  Result->ErrorMessage = FirstPass->ErrorMessage;

  return Result;
}



/*************************                         ***************************/
/*************************  Comments & Whitespace  ***************************/
/*************************                         ***************************/


bonsai_function b32
EatComment(parser* Parser, c_token_type CommentT)
{
  b32 Result = False;
  switch (CommentT)
  {
    case CTokenType_CommentSingleLine:
    {
      Result = OptionalTokenRaw(Parser, CTokenType_CommentSingleLine);
    } break;

    case CTokenType_CommentMultiLine:
    {
      Result = OptionalTokenRaw(Parser, CTokenType_CommentMultiLine);
    } break;

    default:
    {
    } break;
  }

  return Result;
}

bonsai_function b32
EatComment(parser* Parser)
{
  b32 Result = EatComment(Parser, PeekTokenRaw(Parser).Type);
  return Result;
}

bonsai_function void
EatSpacesTabsEscapedNewlinesAndComments(parser *Parser)
{
  b32 Continue = true;
  while (Continue)
  {
    b32 AteWhitespace = EatSpacesTabsAndEscapedNewlines(Parser);
    b32 AteComment = EatComment(Parser);
    Continue = AteWhitespace || AteComment;
  }
}

// @optimize_call_advance_instead_of_being_dumb
bonsai_function void
EatWhitespaceAndComments(parser *Parser)
{
  while ( c_token *T = PeekTokenRawPointer(Parser) )
  {
    if (IsWhitespace(T))
    {
      PopTokenRawPointer(Parser);
    }
    else if (IsComment(T))
    {
      EatComment(Parser);
    }
    else
    {
      break;
    }
  }
}

// NOTE(Jesse): This is duplicated @duplicate_EatSpacesTabsAndEscapedNewlines
// NOTE(Jesse): This could also be optimized using PeekTokenRawPointer now
bonsai_function b32
EatSpacesTabsAndEscapedNewlines(parser *Parser)
{
  b32 Result = False;

  c_token_type Type = PeekTokenRaw(Parser).Type;
  while ( RawTokensRemain(Parser) &&
          ( Type == CTokenType_Space            ||
            Type == CTokenType_Tab              ||
            Type == CTokenType_EscapedNewline   ||
            Type == CTokenType_FSlash ) )
  {
    Result = True;
    if ( Type == CTokenType_FSlash )
    {
      // NOTE(Jesse): This is garbage and should actually never happen.  An excaped newline is '\n' .. which is a backslash!!!!
      Assert(False);
#if 0
      if ( PeekTokenRaw(Parser, 1).Type == CTokenType_Newline )
      {
        RequireTokenRaw(Parser, CTokenType_FSlash);
        RequireTokenRaw(Parser, CTokenType_Newline);
      }
      else
      {
        break;
      }
#endif
    }
    else
    {
      RequireTokenRaw(Parser, Type);
    }

    Type = PeekTokenRaw(Parser).Type;
  }

  return Result;
}


// NOTE(Jesse): We could metapgrogram these routines if we had a feature for it
// @meta_similar_whitespace_routines
//
// @optimize_call_advance_instead_of_being_dumb
bonsai_function b32
EatNBSP(parser* Parser)
{
  b32 Result = False;

  c_token *T = PeekTokenRawPointer(Parser);
  while (T && IsNBSP(T))
  {
    Result = True;
    PopTokenRawPointer(Parser);
    T = PeekTokenRawPointer(Parser);
  }

  return Result;
}


// @meta_similar_whitespace_routines
//
// @optimize_call_advance_instead_of_being_dumb
bonsai_function b32
EatWhitespace(parser* Parser)
{
  b32 Result = False;

  c_token *T = PeekTokenRawPointer(Parser);
  while (T && IsWhitespace(T))
  {
    Result = True;
    PopTokenRawPointer(Parser);
    T = PeekTokenRawPointer(Parser);
  }

  return Result;
}

// NOTE(Jesse): This is the same as EatWhitespace but it doesn't return a bool
bonsai_function void
TrimLeadingWhitespace(parser* Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);
  while (T && IsWhitespace(T))
  {
    PopTokenRawPointer(Parser);
    T = PeekTokenRawPointer(Parser);
  }
}

bonsai_function void
TrimTrailingWhitespace(parser* Parser)
{
  c_token* CurrentToken = Parser->Tokens->End-1;

  while (CurrentToken > Parser->Tokens->Start)
  {
    // TODO(Jesse, correctness) This function fails if we hit one of these!
    // Rewrite it such that we properly traverse "Down" pointers.
    Assert( ! (Parser->Tokens->At->Type == CT_InsertedCode ||
               Parser->Tokens->At->Type == CT_MacroLiteral) );

    if ( CurrentToken->Type == CTokenType_Space ||
         CurrentToken->Type == CTokenType_Tab )
    {
      Parser->Tokens->End = CurrentToken;
      if (Parser->Tokens->At > CurrentToken)
      {
        Parser->Tokens->At = CurrentToken;
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

// @duplicate_EatSpacesTabsAndEscapedNewlines
bonsai_function u32
EatSpacesTabsAndEscapedNewlines(ansi_stream *Code)
{
  u32 LinesEaten = 0;
  c_token_type Type = PeekToken(Code).Type;
  while ( Type == CTokenType_Space ||
          Type == CTokenType_Tab   ||
          Type == CTokenType_FSlash )
  {
    if ( Type == CTokenType_FSlash )
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

    Type = PeekToken(Code).Type;
  }
  return LinesEaten;
}

bonsai_function counted_string*
GetByValue(counted_string_hashtable *Table, counted_string Value)
{
  counted_string *Result = {};

  counted_string_linked_list_node *Bucket = GetHashBucket(Hash(&Value), Table);
  while (Bucket)
  {
    counted_string *Element = &Bucket->Element;

    if (StringsMatch(Element, Value))
    {
      Result = Element;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

bonsai_function macro_def*
GetByName(macro_def_hashtable *Table, counted_string Name)
{
  macro_def *Result = {};

  macro_def_linked_list_node *Bucket = GetHashBucket(Hash(&Name), Table);
  while (Bucket)
  {
    macro_def *M = &Bucket->Element;

    if (!M->Undefed &&
        StringsMatch(M->Name, Name))
    {
      Result = M;
      break;
    }
    else
    {
      Bucket = Bucket->Next;
    }
  }

  return Result;
}

bonsai_function u32
CountTokensBeforeNext(parser *Parser, c_token_type T1, c_token_type T2)
{
  Assert(T1 != T2);
  u32 Result = 0;

  c_token *ResetToStart = Parser->Tokens->At;

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

  Parser->Tokens->At = ResetToStart;

  return Result;
}







bonsai_function c_token_cursor *
SplitAndInsertTokenCursor(c_token_cursor *CursorToSplit, c_token* SplitStart, c_token_cursor *CursorToInsert, c_token* SplitEnd, memory_arena *Memory)
{
  InvalidCodePath();
#if 0
  c_token_cursor *Result = CursorToInsert;

  SanityCheckParserChain(CursorToSplit);
  SanityCheckParserChain(CursorToInsert);

  Assert(SplitStart <= SplitEnd);
  Assert(SplitStart >= CursorToSplit->Start);
  Assert(SplitStart <= CursorToSplit->End);
  Assert(SplitEnd >= CursorToSplit->Start);
  Assert(SplitEnd <= CursorToSplit->End);


  s64 SecondHalfLength = CursorToSplit->End - SplitEnd;
  Assert(SecondHalfLength >= 0);

  c_token_cursor *SecondHalfOfSplit = 0;
  if (SecondHalfLength)
  {
    SecondHalfOfSplit = AllocateProtection(c_token_cursor, Memory, 1, False);
    SecondHalfOfSplit->Source = CursorToSplit->Source;
    SecondHalfOfSplit->Filename = CursorToSplit->Filename;

    SecondHalfOfSplit->Start = SplitEnd;
    SecondHalfOfSplit->At = SplitEnd;
    SecondHalfOfSplit->End = CursorToSplit->End;
  }

  CursorToSplit->At = SplitStart;
  CursorToSplit->End = SplitStart;

  Assert(CursorToSplit->At == CursorToSplit->End);
  Assert(CursorToSplit->At == SplitStart);
  Assert(CursorToSplit->End == SplitStart);


  Assert(CursorToInsert->Up == 0);
  Rewind(CursorToInsert);
  Assert(CursorToInsert->Up == 0);

  c_token_cursor *ParserToInsertLastNext = CursorToInsert;
  while (ParserToInsertLastNext->Next) { ParserToInsertLastNext = ParserToInsertLastNext->Next; }
  ParserToInsertLastNext->Next = SecondHalfOfSplit;

  if (SecondHalfOfSplit)
  {
    SecondHalfOfSplit->Next = CursorToSplit->Next;

    if (SecondHalfOfSplit->Next) {SecondHalfOfSplit->Next->Prev = SecondHalfOfSplit;}

    SecondHalfOfSplit->Prev = ParserToInsertLastNext;
    Assert(SecondHalfOfSplit->At == SecondHalfOfSplit->Start);
  }

  CursorToInsert->Prev = CursorToSplit;
  CursorToSplit->Next = CursorToInsert; // Don't move me

#if BONSAI_INTERNAL
  { // Sanity checks
    Assert(CursorToInsert->At == CursorToInsert->Start);

    if (CursorToSplit->Next) { Assert(CursorToSplit->Next->Prev == CursorToSplit); }
    if (CursorToSplit->Prev) { Assert(CursorToSplit->Prev->Next == CursorToSplit); }

    if (CursorToInsert->Next) { Assert(CursorToInsert->Next->Prev == CursorToInsert); }
    if (CursorToInsert->Prev) { Assert(CursorToInsert->Prev->Next == CursorToInsert); }

    if (SecondHalfOfSplit && SecondHalfOfSplit->Next) { Assert(SecondHalfOfSplit->Next->Prev == SecondHalfOfSplit); }
    if (SecondHalfOfSplit && SecondHalfOfSplit->Prev) { Assert(SecondHalfOfSplit->Prev->Next == SecondHalfOfSplit); }

    Assert(CursorToSplit->Next == CursorToInsert);
    Assert(CursorToInsert->Prev == CursorToSplit);
    Assert(ParserToInsertLastNext->Next == SecondHalfOfSplit);

    if (SecondHalfOfSplit)
    {
      Assert(SecondHalfOfSplit->Prev == ParserToInsertLastNext);
    }
  }
#endif

  SanityCheckParserChain(CursorToSplit);
  SanityCheckParserChain(CursorToInsert);
  if (SecondHalfOfSplit)
  {
    SanityCheckParserChain(SecondHalfOfSplit);
  }

  return Result;
#endif
  return 0;
}

bonsai_function c_token_cursor *
SplitAndInsertTokenCursor(c_token_cursor *CursorToSplit, c_token_cursor *CursorToInsert, memory_arena *Memory)
{
  return SplitAndInsertTokenCursor(CursorToSplit, CursorToSplit->At, CursorToInsert, CursorToSplit->At, Memory);
}

bonsai_function c_token_cursor *
DuplicateCTokenCursor(c_token_cursor *Tokens, memory_arena *Memory)
{
  Assert(Tokens->At == Tokens->Start);

  parser Parser_ = MakeParser(Tokens);
  parser *Parser = &Parser_;


  umm TokenCount = (umm)(Tokens->End - Tokens->Start);
  u32 LineNumber = 0;
  counted_string Filename = CSz("TODO(Jesse): add filename here?");
  c_token_cursor *Result = AllocateTokenCursor(Memory, Filename, TokenCount, TokenCursorSource_Unknown, LineNumber, {});

  c_token *At = Result->Start;
  u32 AtIndex = 0;
  while (c_token *T = PopTokenRawPointer(Parser))
  {
    // NOTE(Jesse): This routine only supports copying buffers with no down pointers
    Assert(HasValidDownPointer(T) == False);

    Assert(T->Type != CTokenType_CommentMultiLine || T->Type != CTokenType_CommentSingleLine || !T->Erased);
    At[AtIndex++] = *T;
  }

  Assert(AtIndex == TokenCount);

  Rewind(Tokens);

  return Result;
}

bonsai_function parser *
DuplicateCTokenCursor2(c_token_cursor *Tokens, memory_arena *Memory)
{
  Assert(Tokens->At == Tokens->Start);

  parser *Result = Allocate(parser, Memory, 1);
  Result->Tokens = DuplicateCTokenCursor(Tokens, Memory);
  return Result;
}

// NOTE(Jesse): This function is pretty sketch .. it modifies the parser you
// pass in which kinda goes against convention in this codebase.
bonsai_function parser *
DuplicateParserTokens(parser *Parser, memory_arena *Memory)
{
  Assert(Parser->Tokens->At == Parser->Tokens->Start);
  Parser->Tokens = DuplicateCTokenCursor(Parser->Tokens, Memory);
  return Parser;
}

bonsai_function b32
TokenCursorsMatch(c_token_cursor *C1, c_token_cursor *C2)
{
  umm C1Elements = TotalElements(C1);
  umm C2Elements = TotalElements(C2);

  b32 Result = C1Elements == C2Elements;

  if (Result)
  {
    for(u32 TokenIndex = 0;
        TokenIndex < C1Elements;
        ++TokenIndex)
    {
      if (C1->Start[TokenIndex] != C2->Start[TokenIndex])
      {
        Result = False;
        break;
      }
    }
  }

  return Result;
}

bonsai_function void
DefineMacro(parse_context *Ctx, parser *Parser, macro_def *Macro)
{
  TIMED_FUNCTION();

  memory_arena *Memory = Ctx->Memory;

  RequireToken(Parser, CT_PreprocessorDefine);

  c_token_cursor TmpTokens = {};
  parser InstanceBody = MakeParser(&TmpTokens);

  InstanceBody.Tokens->Start = Parser->Tokens->At;
  InstanceBody.Tokens->At = Parser->Tokens->At;
  EatUntilExcluding(Parser, CTokenType_Newline);
  InstanceBody.Tokens->End = Parser->Tokens->At;

  // We have to copy tokens because ExpandMacro is written with the assumption
  // that the valid macro body tokens will not be erased and will not have
  // undergone macro expansion. That assumption is invalidated if we don't copy
  // tokens.
  parser *InstanceParser = DuplicateParserTokens(&InstanceBody, Memory);

  RequireToken(InstanceParser, CToken(CT_MacroLiteral, Macro->Name));

  //
  // Classify as keyword or function
  //

  if (OptionalTokenRaw(InstanceParser, CTokenType_OpenParen))
  {
    Macro->Type = type_macro_function;

    if (OptionalToken(InstanceParser, CTokenType_CloseParen))
    {
      // No arguments
    }
    else
    {
      // Parse named arguments

      u32 ArgCount = 1 + CountTokensBeforeNext(InstanceParser, CTokenType_Comma, CTokenType_CloseParen);
      Macro->NamedArguments = CountedStringBuffer(ArgCount, Memory);

      for ( u32 ArgIndex = 0;
            ArgIndex < ArgCount-1;
            ++ArgIndex)
      {
        counted_string ArgName = RequireToken(InstanceParser, CTokenType_Identifier).Value;
        Macro->NamedArguments.Start[ArgIndex] = ArgName;
        RequireToken(InstanceParser, CTokenType_Comma);
      }

      if (PeekToken(InstanceParser).Type == CTokenType_Identifier)
      {
        counted_string ArgName = RequireToken(InstanceParser, CTokenType_Identifier).Value;
        Macro->NamedArguments.Start[Macro->NamedArguments.Count-1] = ArgName;
      }

      if (OptionalToken(InstanceParser, CTokenType_Ellipsis))
      {
        Macro->Variadic = True;
        Macro->NamedArguments.Count -= 1;
      }

      RequireToken(InstanceParser, CTokenType_CloseParen);
    }
  }
  else
  {
    Macro->Type = type_macro_keyword;
  }

  InstanceParser->Tokens->Start = InstanceParser->Tokens->At;

#if 0
  // Sanity check
  while (c_token *T = PeekTokenRawPointer(InstanceParser))
  {
    switch (T->Type)
    {

      case CT_MacroLiteral:
      case CTokenType_Newline: { InvalidCodePath(); } break;
      default: { PopTokenRaw(InstanceParser); } break;
    }
  }
  Assert(InstanceParser->Tokens->At == InstanceParser->Tokens->End);
#endif

  if (Macro->Body.Start && TokenCursorsMatch(&Macro->Body, InstanceParser->Tokens))
  {
    // Redefining a macro with the same body is valid and has no effect from
    // what I can tell.
    //
    // TODO(Jesse, memory, memory-leak): When we have streaming arena allocations rewind the arena here
    /* Info("Macro redef'd with matching body."); */
  }
  else
  {
    Macro->Body = *InstanceParser->Tokens;
  }

  Rewind(&Macro->Body);
}

bonsai_function void
SkipToEndOfCursor(c_token_cursor *At, c_token_cursor *ToSkip)
{
  InvalidCodePath();
#if 0
  if (ToSkip->Next)
  {
    SinglyLinkedListSwapInplace(At, ToSkip->Next);
  }
  else
  {
    ToSkip->At = ToSkip->End;
    SinglyLinkedListSwapInplace(At, ToSkip);
  }
#endif
}

bonsai_function b32
MacroShouldBeExpanded(parser *Parser, c_token *T, macro_def *ThisMacro, macro_def *ExpandingMacro)
{
  Assert(PeekTokenPointer(Parser) == T);
  Assert(T->Type == CTokenType_Identifier);
  Assert(StringsMatch(T->Value, ThisMacro->Name));

  b32 Result = False;

  switch (ThisMacro->Type)
  {
    case type_macro_function:
    {
      if (PeekToken(Parser, 1).Type == CTokenType_OpenParen)
      {
        Result = True;
      }
    } break;

    case type_macro_keyword:
    {
      if (ExpandingMacro && StringsMatch(ExpandingMacro->Name, ThisMacro->Name))
      {
        // TODO(Jesse): This is the path that's preventing self-referential macros
        // from expanding, but we should take this out in favor of the code at
        // @mark_keyword_macros_self_referential.
        /* Info("Prevented recursive macro expansion of (%S)",  ExpandingMacro->Name); */
      }
      else
      {
        Result = True;
      }
    } break;

    InvalidDefaultCase;
  }


  return Result;
}

bonsai_function macro_def *
IdentifierShouldBeExpanded(parse_context *Ctx, parser *Parser, c_token *T, macro_def *ExpandingMacro)
{
  Assert(T->Type == CTokenType_Identifier);

  macro_def *Result = False;

  if (macro_def *ThisMacro = GetMacroDef(Ctx, T->Value))
  {
    if (MacroShouldBeExpanded(Parser, T, ThisMacro, ExpandingMacro))
    {
      Result = ThisMacro;
    }
  }

  return Result;
}

bonsai_function macro_def *
TryTransmuteIdentifierToMacro(parse_context *Ctx, parser *Parser, c_token *T, macro_def *ExpandingMacro)
{
  Assert(T->Type == CTokenType_Identifier);

  macro_def *Result = IdentifierShouldBeExpanded(Ctx, Parser, T, ExpandingMacro);
  if (Result)
  {
    T->Type = CT_MacroLiteral;
    T->Macro.Def = Result;
    Assert(T->Macro.Expansion == 0);
  }

  return Result;
}


bonsai_function b32
TryTransmuteIdentifierToken(c_token *T)
{
  Assert(T->Type == CT_PreprocessorPaste_InvalidToken);

  b32 Result = True;

  for (u32 CharIndex = 0;
      Result && CharIndex < T->Value.Count;
      ++CharIndex)
  {
    char C = T->Value.Start[CharIndex];
    Result &= IsAlphaNumeric(C) || C == '_';;
  }

  if (Result)
  {
    T->Type = CTokenType_Identifier;
  }

  return Result;
}

bonsai_function b32
TryTransmuteOperatorToken(c_token *T)
{
  TIMED_FUNCTION();

  Assert(T->Type == CT_PreprocessorPaste_InvalidToken);

  // TODO(Jesse): Implement me.

  b32 Result = T->Type != CT_PreprocessorPaste_InvalidToken;
  return Result;
}

bonsai_function b32
TryTransmuteKeywordToken(c_token *T, c_token *LastTokenPushed)
{
  TIMED_FUNCTION();

  c_token_type StartType = T->Type;

  Assert(T->Type == CT_PreprocessorPaste_InvalidToken ||
         T->Type == CTokenType_Unknown);


  // TODO(Jesse): A large portion of this can be generated.  It should also
  // probably do a comptime string hash and switch off that instead of doing
  // so many serial comparisons.  It _also_ shouldn't even do serial
  // comparisons but I benched doing it both ways
  //
  // ie `if {} if {}` vs `if {} else if {} else if {}`
  //
  // and there was no measurable difference in runtime.  I decided to keep them
  // serial in case there's a subtle behavior difference between the two that
  // the tests don't pick up on.  I'd rather hit and debug that when this moves
  // to being generated if it doesn't make the program perceptibly faster.
  //

  if ( StringsMatch(T->Value, CSz("if")) )
  {
    T->Type = CTokenType_If;
  }
  else if ( StringsMatch(T->Value, CSz("else")) )
  {
    T->Type = CTokenType_Else;
  }
  else if ( StringsMatch(T->Value, CSz("break")) )
  {
    T->Type = CTokenType_Break;
  }
  else if ( StringsMatch(T->Value, CSz("switch")) )
  {
    T->Type = CTokenType_Switch;
  }
  else if ( StringsMatch(T->Value, CSz("case")) )
  {
    T->Type = CTokenType_Case;
  }
  else if ( StringsMatch(T->Value, CSz("delete")) )
  {
    T->Type = CT_Keyword_Delete;
  }
  else if ( StringsMatch(T->Value, CSz("default")) )
  {
    T->Type = CTokenType_Default;
  }
  else if ( StringsMatch(T->Value, CSz("for")) )
  {
    T->Type = CTokenType_For;
  }
  else if ( StringsMatch(T->Value, CSz("while")) )
  {
    T->Type = CTokenType_While;
  }
  else if ( StringsMatch(T->Value, CSz("continue")) )
  {
    T->Type = CTokenType_Continue;
  }
  else if ( StringsMatch(T->Value, CSz("return")) )
  {
    T->Type = CTokenType_Return;
  }
  else if ( StringsMatch(T->Value, CSz("thread_local")) )
  {
    T->Type = CTokenType_ThreadLocal;
  }
  else if ( StringsMatch(T->Value, CSz("const")) )
  {
    T->Type = CTokenType_Const;
  }
  else if ( StringsMatch(T->Value, CSz("static")) )
  {
    T->Type = CTokenType_Static;
  }
  else if ( StringsMatch(T->Value, CSz("__volatile__")) )
  {
    T->Type = CTokenType_Volatile;
  }
  else if ( StringsMatch(T->Value, CSz("volatile")) )
  {
    T->Type = CTokenType_Volatile;
  }
  else if ( StringsMatch(T->Value, CSz("void")) )
  {
    T->Type = CTokenType_Void;
  }
  else if ( StringsMatch(T->Value, CSz("long")) )
  {
    T->Type = CTokenType_Long;
  }
  else if ( StringsMatch(T->Value, CSz("float")) )
  {
    T->Type = CTokenType_Float;
  }
  else if ( StringsMatch(T->Value, CSz("char")) )
  {
    T->Type = CTokenType_Char;
  }
  else if ( StringsMatch(T->Value, CSz("double")) )
  {
    T->Type = CTokenType_Double;
  }
  else if ( StringsMatch(T->Value, CSz("short")) )
  {
    T->Type = CTokenType_Short;
  }
  else if ( StringsMatch(T->Value, CSz("int")) )
  {
    T->Type = CTokenType_Int;
  }
  else if ( StringsMatch(T->Value, CSz("bool")) )
  {
    T->Type = CTokenType_Bool;
  }
  else if ( StringsMatch(T->Value, CSz("auto")) )
  {
    T->Type = CTokenType_Auto;
  }
  else if ( StringsMatch(T->Value, CSz("signed")) )
  {
    T->Type = CTokenType_Signed;
  }
  else if ( StringsMatch(T->Value, CSz("unsigned")) )
  {
    T->Type = CTokenType_Unsigned;
  }
  else if ( StringsMatch(T->Value, CSz("public")) )
  {
    T->Type = CT_Keyword_Public;
  }
  else if ( StringsMatch(T->Value, CSz("private")) )
  {
    T->Type = CT_Keyword_Private;
  }
  else if ( StringsMatch(T->Value, CSz("protected")) )
  {
    T->Type = CT_Keyword_Protected;
  }
  else if ( StringsMatch(T->Value, CSz("noexcept")) )
  {
    T->Type = CT_Keyword_Constexpr;
  }
  else if ( StringsMatch(T->Value, CSz("constexpr")) )
  {
    T->Type = CT_Keyword_Constexpr;
  }
  else if ( StringsMatch(T->Value, CSz("namespace")) )
  {
    T->Type = CT_Keyword_Namespace;
  }
  else if ( StringsMatch(T->Value, CSz("class")) )
  {
    T->Type = CT_Keyword_Class;
  }
  else if ( StringsMatch(T->Value, CSz("struct")) )
  {
    T->Type = CTokenType_Struct;
  }
  else if ( StringsMatch(T->Value, CSz("typedef")) )
  {
    T->Type = CTokenType_Typedef;
  }
  else if ( StringsMatch(T->Value, CSz("__asm__")) )
  {
    T->Type = CTokenType_Asm;
  }
  else if ( StringsMatch(T->Value, CSz("asm")) )
  {
    T->Type = CTokenType_Asm;
  }
  else if ( StringsMatch(T->Value, CSz("meta")) )
  {
    T->Type = CTokenType_Meta;
  }
  else if ( StringsMatch(T->Value, CSz("union")) )
  {
    T->Type = CTokenType_Union;
  }
  else if ( StringsMatch(T->Value, CSz("using")) )
  {
    T->Type = CTokenType_Using;
  }
  else if ( StringsMatch(T->Value, CSz("enum")) )
  {
    T->Type = CTokenType_Enum;
  }
  else if ( StringsMatch(T->Value, CSz("goto")) )
  {
    T->Type = CTokenType_Goto;
  }
  else if ( StringsMatch(T->Value, CSz("template")) )
  {
    T->Type = CTokenType_TemplateKeyword;
  }
  else if ( StringsMatch(T->Value, CSz("__inline__")) )
  {
    T->Type = CTokenType_Inline;
  }
  else if ( StringsMatch(T->Value, CSz("__inline")) )
  {
    T->Type = CTokenType_Inline;
  }
  else if ( StringsMatch(T->Value, CSz("inline")) )
  {
    T->Type = CTokenType_Inline;
  }
  else if ( StringsMatch(T->Value, CSz("operator")) )
  {
    T->Type = CTokenType_OperatorKeyword;
  }
  else if ( StringsMatch(T->Value, CSz("static_assert")) )
  {
    T->Type = CT_StaticAssert;
  }
  else if ( StringsMatch(T->Value, CSz("_Pragma")) )
  {
    T->Type = CT_KeywordPragma;
  }
  else if ( StringsMatch(T->Value, CSz("__pragma")) )
  {
    T->Type = CT_KeywordPragma;
  }
  else if ( StringsMatch(T->Value, CSz("extern")) )
  {
    T->Type = CTokenType_Extern;
  }
  else if ( StringsMatch(T->Value, CSz("__attribute__")) )
  {
    T->Type = CT_KeywordAttribute;
  }
  else if ( StringsMatch(T->Value, CSz("__VA_ARGS__")) )
  {
    T->Type = CT_Preprocessor__VA_ARGS__;
  }
  else if (LastTokenPushed && LastTokenPushed->Type == CT_ScopeResolutionOperator)
  {
    T->QualifierName = LastTokenPushed->QualifierName;
  }

  b32 Result = T->Type != StartType;
  return Result;
}

bonsai_function c_token_cursor *
TokenizeAnsiStream(ansi_stream Code, memory_arena* Memory, b32 IgnoreQuotes, parse_context *Ctx, token_cursor_source Source)
{
  TIMED_FUNCTION();

  u32 LineNumber = 1;

  // Allocate a huge buffer that gets truncated to the necessary size at the end of the tokenization
  //
  // TODO(Jesse)(hardening): This could now (easily?) expand as we go, but at
  // the moment I don't see a reason for doing that work.  Maybe when I get
  // around to hardening.
  c_token_cursor *Tokens = AllocateTokenCursor(Memory, Code.Filename, (u32)Megabytes(8), Source, LineNumber, {0, 0});

  if (!Tokens->Start)
  {
    Error("Allocating Token Buffer");
    return {};
  }

  if (Code.Start)
  {
    b32 ParsingSingleLineComment = False;
    b32 ParsingMultiLineComment = False;

    c_token *LastTokenPushed = 0;
    c_token *CommentToken = 0;

    TIMED_BLOCK("Lexer");
    while(Remaining(&Code))
    {
      c_token FirstT = PeekToken(&Code);
      c_token SecondT = {};
      if (Remaining(&Code, 1)) SecondT = PeekToken(&Code, 1);

      c_token PushT = {};

      PushT.Type = FirstT.Type;
      PushT.Value = CS(Code.At, 1);

      switch (FirstT.Type)
      {
        case CTokenType_FSlash:
        {
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
              PushT.Type = CTokenType_CommentMultiLine;
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
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_LessEqual;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_LT)
          {
            PushT.Type = CTokenType_LeftShift;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        } break;

        case CTokenType_GT:
        {
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_GreaterEqual;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_GT)
          {
            PushT.Type = CTokenType_RightShift;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        } break;

        case CTokenType_Equals:
        {
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_AreEqual;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        } break;

        case CTokenType_Dot:
        {
          if (SecondT.Type == CTokenType_Dot &&
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
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_NotEqual;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        } break;

        case CTokenType_Hat:
        {
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_XorEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        }break;

        case CTokenType_Pipe:
        {
          if (SecondT.Type == CTokenType_Pipe)
          {
            PushT.Type = CTokenType_LogicalOr;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_OrEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        }break;

        case CTokenType_Ampersand:
        {
          if (SecondT.Type == CTokenType_Ampersand)
          {
            PushT.Type = CTokenType_LogicalAnd;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_AndEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        }break;

        case CTokenType_Percent:
        {
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_ModEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        }break;

        case CTokenType_Minus:
        {
          if (SecondT.Type == CTokenType_Minus)
          {
            PushT.Type = CTokenType_Decrement;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_MinusEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_GT)
          {
            PushT.Type = CTokenType_Arrow;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        }break;

        case CTokenType_Plus:
        {
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_PlusEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_Plus)
          {
            PushT.Type = CTokenType_Increment;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          Advance(&Code);
        }break;

        case CTokenType_Star:
        {
          if (SecondT.Type == CTokenType_Equals)
          {
            PushT.Type = CTokenType_TimesEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
          }
          else if (SecondT.Type == CTokenType_FSlash)
          {
            ParsingMultiLineComment = False;
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
          if (SecondT.Type == CTokenType_CarrigeReturn)
          {
            ++PushT.Value.Count;
            Advance(&Code);
            Assert(PeekToken(&Code, 1).Type == CTokenType_Newline);
          }

          if (PeekToken(&Code, 1).Type == CTokenType_Newline)
          {
            PushT.Type = CTokenType_EscapedNewline;
            ++PushT.Value.Count;
            Advance(&Code);
          }
          Advance(&Code);
        } break;

        case CTokenType_CarrigeReturn:
        {
          Advance(&Code);
          if (PeekToken(&Code).Type == CTokenType_Newline)
          {
            PushT.Type = CTokenType_Newline;
            PushT.Value.Count = 2;
            ParsingSingleLineComment = False;
            Advance(&Code);
          }
        } break;

        case CTokenType_Newline:
        {
          ParsingSingleLineComment = False;
          Advance(&Code);
        } break;

        case CTokenType_Colon:
        {
          if (SecondT.Type == CTokenType_Colon)
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
          if (SecondT.Type == CTokenType_Hash)
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
                PushT.Type = CT_PreprocessorDefine;
                PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
              }
              else if ( StringsMatch(TempValue, CSz("undef")) )
              {
                PushT.Type = CT_PreprocessorUndef;
                PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
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
              else if ( StringsMatch(TempValue, CSz("include")) )
              {
                PushT.Type = CT_PreprocessorInclude;
                PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
              }
              else if ( StringsMatch(TempValue, CSz("include_next")) )
              {
                PushT.Type = CT_PreprocessorIncludeNext;
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
            PushT.Value = PopIdentifier(&Code);

            // This looks kinda dumb but this code has to look like this to
            // pacify the asserts in TryTransmuteKeywordToken().  Not sure if
            // that's actually a good reason for making this code very verbose,
            // but I'm leaving it like this for now.
            //
            // For reference the less-verbose version I had in mind is:
            //
            // PushT.Type = CTokenType_Identifier;
            // TryTransmuteKeywordToken(&PushT, LastTokenPushed);
            //
            // But this triggers the assert in TryTransmuteKeywordToken, which I
            // don't really want to get rid of.

            if (Ctx)
            {
              if (TryTransmuteKeywordToken(&PushT, LastTokenPushed))
              {
              }
              else
              {
                PushT.Type = CTokenType_Identifier;
              }
            }
            else
            {
              PushT.Type = CTokenType_Identifier;
            }
          }

        } break;

        default: { Advance(&Code); } break;
      }

      Assert(PushT.Type);

      PushT.Filename = Code.Filename;
      PushT.LineNumber = LineNumber;

      if (ParsingSingleLineComment || ParsingMultiLineComment)
      {

        // Set CommentToken on the first loop through when we start parsing a comment
        if (!CommentToken)
        {
          PushT.Erased = True;
          CommentToken = Push(PushT, Tokens);
        }

        if ( (PushT.Type == CTokenType_EscapedNewline) ||
             (ParsingMultiLineComment && PushT.Type == CTokenType_Newline) )
        {
          LastTokenPushed = Push(PushT, Tokens);

          Assert(CommentToken->Erased);
          CommentToken->Value.Count = (umm)(Code.At - CommentToken->Value.Start - (u32)PushT.Value.Count);
          CommentToken->LineNumber = LineNumber;
          CommentToken = Push(*CommentToken, Tokens);
          Assert(CommentToken->Erased);

          CommentToken->LineNumber = LineNumber+1;
          CommentToken->Value.Start = Code.At;
        }
      }
      else if ( CommentToken && !(ParsingSingleLineComment || ParsingMultiLineComment) )
      {
        umm Count = (umm)(Code.At - CommentToken->Value.Start);

        // We finished parsing a comment on this token
        if (PushT.Type == CTokenType_Newline || PushT.Type == CTokenType_CarrigeReturn)
        {

          // TODO(Jesse): Is this actually busted for \r\n ?  Seems like we should sub 2 for that case?
          if (Count) { Count -= 1; } // Exclude the \r or \n from single line comments

          LastTokenPushed = Push(PushT, Tokens);
        }

        CommentToken->Value.Count = Count;
        Assert(CommentToken->Erased);
        CommentToken = 0;
      }
      else if ( PushT.Type == CT_PreprocessorInclude ||
                PushT.Type == CT_PreprocessorIncludeNext )
      {
        LineNumber += EatSpacesTabsAndEscapedNewlines(&Code);
        if (*Code.At == '"')
        {
          PushT.Flags |= CTFlags_RelativeInclude;
          PushT.IncludePath = PopQuotedString(&Code, True);
        }
        else if (*Code.At == '<')
        {
          PushT.IncludePath = EatBetweenExcluding(&Code, '<', '>');
        }
        else
        {
          // NOTE(Jesse): This path throws an error during ResolveInclude()
        }

        // TODO(Jesse, correctness): This seems very sus..  Why would we need to eat whitespace?
        LineNumber += EatSpacesTabsAndEscapedNewlines(&Code);
        PushT.Value.Count = (umm)(Code.At - PushT.Value.Start);

        LastTokenPushed = Push(PushT, Tokens);

        c_token InsertedCodePlaceholder {
          .Type = CT_InsertedCode,
          .Filename = Code.Filename,
          .LineNumber = LineNumber,
        };

        LastTokenPushed = Push(InsertedCodePlaceholder, Tokens);
      }
      else
      {
        LastTokenPushed = Push(PushT, Tokens);
      }

      // NOTE(Jesse): This has to come last.
      if ( IsNewline(PushT.Type) ) { ++LineNumber; }

      continue;
    }

    END_BLOCK();
  }
  else
  {
    Warn("Input stream to TokenizeAnsiStreamInput was null");
  }

  umm CurrentSize = TotalSize(Tokens);
  TruncateToCurrentElements(Tokens);
  umm NewSize = TotalSize(Tokens);

  Reallocate((u8*)Tokens->Start, Memory, CurrentSize, NewSize);

  Rewind(Tokens);

  return Tokens;
}

bonsai_function b32
RunPreprocessor(parse_context *Ctx, parser *Parser, memory_arena *Memory)
{
  TIMED_FUNCTION();

  memory_arena TempMemory = {};

  c_token *LastT = 0;
  while (TokensRemain(Parser))
  {
    // TODO(Jesse)(speed): The only thing in this loop that we decides we can't
    // use PopTokenPointer is that DefineMacro wants to require the #define
    // keyword .. surely we're clever enough to get around that and still be
    // safe at runtime.
    c_token *T = PeekTokenPointer(Parser);
    switch (T->Type)
    {
      case CTokenType_Meta:
      {
        RequireToken(Parser, T->Type);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CT_PreprocessorPragma:
      {
        RequireToken(Parser, T->Type);

        if (OptionalToken(Parser, CToken(CSz("once"))))
        {
          if (GetByValue(&Ctx->Datatypes.FilesParsed, T->Filename))
          {
            // TODO(Jesse, tags: memory): Free all memory
            Parser->Tokens->At = Parser->Tokens->Start;
            Parser->Tokens->End = Parser->Tokens->Start;
            T = 0;
          }
          else
          {
            Insert(T->Filename, &Ctx->Datatypes.FilesParsed, Memory);
          }
        }

        if (T)
        {
          EatUntilExcluding(Parser, CTokenType_Newline);
          EraseBetweenExcluding(Parser, T, Parser->Tokens->At);
        }

      } break;

      case CT_PreprocessorInclude:
      case CT_PreprocessorIncludeNext:
      {
        RequireToken(Parser, T->Type);
        c_token *InsertedCodeT = PopTokenPointer(Parser);
        Assert(InsertedCodeT->Type == CT_InsertedCode);
        Assert(InsertedCodeT->Down == 0);

        EraseToken(T);
        EraseToken(InsertedCodeT);

        /* InsertedCodeT->Down = Allocate(c_token_cursor, Memory, 1); */
        /* c_token_cursor *Inc = ResolveInclude(Ctx, Parser, T); */
        /* *InsertedCodeT->Down = *Inc; */

        InsertedCodeT->Down = ResolveInclude(Ctx, Parser, T);
        if (InsertedCodeT->Down)
        {
          /* InsertedCodeT->Down->Up.Up = Allocate(c_token_cursor, Memory, 1); */
          /* *InsertedCodeT->Down->Up.Up = *Parser->Tokens; */
          InsertedCodeT->Down->Up.Tokens = Parser->Tokens;
          InsertedCodeT->Down->Up.At = InsertedCodeT;
          Assert(InsertedCodeT->Down->Up.At->Type == CT_InsertedCode);
          Assert(InsertedCodeT->Down->At == InsertedCodeT->Down->Start);
        }

        /* EatSpacesTabsEscapedNewlinesAndComments(Parser); */

        /* RequireToken(Parser, T->Type); */

        /* EraseBetweenExcluding(Parser, T, Parser->Tokens->At); */

        // TODO(Jesse): Pretty sure this can be EatUntilExcluding(Reuslt, CTokenType_Newline)
        // although when I tried it it failed for some reason.  TBD.
        b32 Continue = True;
        while (Continue)
        {
          c_token *Peek = PeekTokenRawPointer(Parser);

          if (IsWhitespace(Peek) || IsComment(Peek))
          {
            RequireTokenRaw(Parser, Peek->Type); // @optimize_call_advance_instead_of_being_dumb

            if (Peek->Type == CTokenType_CommentSingleLine ||
                Peek->Type == CTokenType_Newline)
            {
              Continue = False;
            }
          }
          else
          {
            Continue = False;
          }
        }

        /* if (IncludeParser) */
        /* { */
        /*   c_token_cursor *LastCursorOfChain = SplitAndInsertTokenCursor(Parser->Tokens, &IncludeParser->Tokens, Memory); */
        /*   SkipToEndOfCursor(Parser->Tokens, LastCursorOfChain); */
        /* } */

      } break;

      case CTokenType_Identifier:
      {
        if (Ctx)
        {
          if (macro_def *Macro = TryTransmuteIdentifierToMacro(Ctx, Parser, T, 0))
          {
            AdvanceTo(Parser, T);
            parser *Expanded = ExpandMacro(Ctx, Parser, Macro, Memory, &TempMemory, True);
            if (Expanded->ErrorCode)
            {
              ParseError( Parser,
                          Expanded->ErrorCode,
                          FormatCountedString(TranArena, CSz("While Expanding %S"), T->Value),
                          T);
            }
            else
            {
              EraseBetweenExcluding(Parser, T, Parser->Tokens->At);
              T->Macro.Expansion = Expanded->Tokens;
              T->Macro.Def = Macro;
              Expanded->Tokens->Up.Tokens = Parser->Tokens;
              Expanded->Tokens->Up.At = T;
              Assert(Parser->Tokens->Source != TokenCursorSource_IntermediateRepresentaton);
              Assert(Expanded->Tokens->Up.At->Type == CT_MacroLiteral);
            }
          }
          else
          {
            RequireToken(Parser, CToken(T->Value));
          }
        }
        else
        {
          RequireToken(Parser, CToken(T->Value));
        }
      } break;

      case CT_MacroLiteral:
      {
        InvalidCodePath();
      } break;

      case CT_PreprocessorDefine:
      {
        c_token *MacroNameToken = PeekTokenPointer(Parser, 1);

        if (Ctx)
        {
          macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, MacroNameToken->Value);

          if (!Macro)
          {
            macro_def_linked_list_node *MacroNode = AllocateProtection(macro_def_linked_list_node, Ctx->Memory, 1, False);
            Macro = &MacroNode->Element;

            Macro->Name = MacroNameToken->Value;
            Insert(MacroNode, &Ctx->Datatypes.Macros);
          }

          MacroNameToken->Type = CT_MacroLiteral;
          MacroNameToken->Macro.Def = Macro;

          DefineMacro(Ctx, Parser, Macro);

          EraseBetweenExcluding(Parser, T, Parser->Tokens->At);
          RequireTokenRaw(Parser, CTokenType_Newline);
        }

      } break;

      case CT_PreprocessorUndef:
      {
        RequireToken(Parser, T->Type);
        counted_string MacroName = PopToken(Parser).Value;
        if (Ctx)
        {
          macro_def *Macro1 = GetMacroDef(Ctx, MacroName);
          if (Macro1) { Macro1->Undefed = True; }
        }
        EatUntilExcluding(Parser, CTokenType_Newline);
        EraseBetweenExcluding(Parser, T, Parser->Tokens->At);
        RequireTokenRaw(Parser, CTokenType_Newline);
      } break;

      case CT_PreprocessorIf:
      case CT_PreprocessorElif:
      {
        RequireToken(Parser, T->Type);
        if (ResolveMacroConstantExpression(Ctx, Parser, Memory, &TempMemory, 0, False))
        {
          // @optimize_call_advance_instead_of_being_dumb
          c_token *RewindT = PeekTokenRawPointer(Parser);
          EraseBetweenExcluding(Parser, T, Parser->Tokens->At);
          Assert(T->Erased);
          EatIfBlock(Parser, PreserveTokens);
          EraseAllRemainingIfBlocks(Parser);
          RewindTo(Parser, RewindT);
        }
        else
        {
          EraseBetweenExcluding(Parser, T, Parser->Tokens->At);
          EatIfBlock(Parser, EraseTokens);
        }
      } break;

      case CT_PreprocessorIfDefined:
      {
        RequireToken(Parser, T->Type);
        c_token *DefineValue = PopTokenPointer(Parser);

        EraseToken(T);
        EraseToken(DefineValue);

        if ( GetMacroDef(Ctx, DefineValue->Value) )
        {
          EatIfBlock(Parser, PreserveTokens);
          EraseAllRemainingIfBlocks(Parser);
          RewindTo(Parser, T);
        }
        else
        {
          EatIfBlock(Parser, EraseTokens);
        }
      } break;

      case CT_PreprocessorIfNotDefined:
      {
        RequireToken(Parser, T->Type);
        c_token *DefineValue = PopTokenPointer(Parser);

        EraseToken(T);
        EraseToken(DefineValue);

        if ( ! GetMacroDef(Ctx, DefineValue->Value) )
        {
          EatIfBlock(Parser, PreserveTokens);
          EraseAllRemainingIfBlocks(Parser);
          RewindTo(Parser, T);
        }
        else
        {
          EatIfBlock(Parser, EraseTokens);
        }
      } break;

      // If we hit this case it must be the last one in an #if #elif chain because when
      // we hit a true clause we eat the rest of the clauses.
      case CT_PreprocessorElse:
      case CT_PreprocessorEndif:
      {
        EraseToken(T);
      } break;

      /* case CTokenType_Extern: */
      /* { */
      /*   RequireToken(Parser, T->Type); */
      /*   RuntimeBreak(); */
      /* } break; */

      default:
      {
        RequireToken(Parser, T->Type);
      } break;
    }
  }

  Assert (Parser->Tokens->Up.Tokens == 0);
  Rewind(Parser->Tokens);

  Ensure(RewindArena(&TempMemory));

  return (Parser->ErrorCode == ParseErrorCode_None);
}

bonsai_function c_token_cursor *
CTokenCursorForAnsiStream(parse_context *Ctx, ansi_stream SourceFileStream, token_cursor_source Source)
{
  c_token_cursor *Result = TokenizeAnsiStream(SourceFileStream, Ctx->Memory, False, Ctx, Source);
  return Result;
}

bonsai_function parser *
ParserForAnsiStream(parse_context *Ctx, ansi_stream SourceFileStream, token_cursor_source Source)
{
  TIMED_FUNCTION();

  parser *Result = Allocate(parser, Ctx->Memory, 1);

  if (Result)
  {
    if (SourceFileStream.Start)
    {
      Result->Tokens = CTokenCursorForAnsiStream(Ctx, SourceFileStream, Source);
    }
    else
    {
      Result->ErrorCode = ParseErrorCode_InputStreamNull;
    }
  }
  else
  {
    Error("Allocating parser during ParserForAnsiStream");
  }

  return Result;

}

bonsai_function parser *
ParserForFile(parse_context *Ctx, counted_string Filename, token_cursor_source Source)
{
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Ctx->Memory);
  parser *Result = ParserForAnsiStream(Ctx, SourceFileStream, Source);
  return Result;
}

bonsai_function parser *
PreprocessedParserForFile(parse_context *Ctx, counted_string Filename, token_cursor_source Source, c_token_cursor *Up)
{
  parser *Result = ParserForFile(Ctx, Filename, Source);
  if (Result && Result->ErrorCode == ParseErrorCode_None)
  {
    if (RunPreprocessor(Ctx, Result, Ctx->Memory))
    {
      /* c_token_cursor Tmp = *Result->Tokens; */
      /* Result->Tokens = Allocate(c_token_cursor, Ctx->Memory, 1); */
      /* *Result->Tokens = Tmp; */
      // All good
    }
    else
    {
      Warn("Error encountered while running preprocessor on file %S", Filename);
    }
  }

  return Result;
}






#if 0
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
#endif

bonsai_function c_token_cursor *
ResolveInclude(parse_context *Ctx, parser *Parser, c_token *T)
{
  TIMED_FUNCTION();

  if (const char *INCLUDE_ENV = PlatformGetEnvironmentVar("INCLUDE"))
  {
    Warn("poof does not support the environemnt variable INCLUDE (%s)", INCLUDE_ENV);
  }

  Assert(IsValidForCursor(Parser->Tokens, T));

  c_token_cursor *Result = {};
  counted_string FinalIncludePath = {};

  b32 SkipFirst = T->Type == CT_PreprocessorIncludeNext ? True : False;

  counted_string PartialPath = T->IncludePath;
  if (PartialPath.Count == 0)
  {
    ParseError(Parser, CSz("Include path must be specified."), T);
    return Result;
  }

  if (T->Flags & CTFlags_RelativeInclude)
  {
    if (SkipFirst != False)
    {
      Error("Relative includes not supported with 'include_next'.");
      return Result;
    }

    c_token_cursor *Current = Parser->Tokens;
    while (Current)
    {
      Assert(Current->Filename.Count);
      PartialPath = StripQuotes(PartialPath);
      counted_string CurrentFilepath = Concat(Dirname(Current->Filename), PartialPath, TranArena);
      if (FileExists(CurrentFilepath))
      {
        FinalIncludePath = CurrentFilepath;
        break;
      }
      else
      {
        Current = Current->Up.Tokens;
      }
    }
  }

  if (FinalIncludePath.Count == 0)
  {
    counted_string_cursor *IncludePaths = Ctx->IncludePaths;
    if (IncludePaths)
    {
      if (!Result)
      {
        for ( u32 PrefixIndex = 0;
              PrefixIndex < Count(IncludePaths);
              ++PrefixIndex )
        {
          counted_string PrefixPath = IncludePaths->Start[PrefixIndex];
          counted_string FullPath = Concat(PrefixPath, PartialPath, TranArena); // TODO(Jesse id: 304): only do this work once

          if (FileExists(FullPath))
          {
            if (SkipFirst)
            {
              SkipFirst = False;
            }
            else
            {
              FinalIncludePath = FullPath;
              break;
            }
          }
        }

      }
    }

    if ( ! FinalIncludePath.Count )
    {
      if (FileExists(PartialPath))
      {
        if (SkipFirst)
        {
          SkipFirst = False;
        }
        else
        {
          FinalIncludePath = PartialPath;
        }
      }
    }
  }

  if (FinalIncludePath.Count)
  {
    LogSuccess("Including (%S)", FinalIncludePath);
    parser *IncludeParser = PreprocessedParserForFile(Ctx, FinalIncludePath, TokenCursorSource_Include, Parser->Tokens);
    if (IncludeParser)
    {
      // TODO(Jesse)(memory_leak) This leaks the parser struct but it's such
      // small overhead I'm not going to worry about it right now.  It's easy
      // to fix but I'm tired and don't feel like it.
      // @leak_parser_struct
      Result = IncludeParser->Tokens;
    }
  }
  else
  {
    const char *FmtMessage = SkipFirst ?
      "Unable to resolve include_next for file : (%S)" :
      "Unable to resolve include for file : (%S)";

    Warn(FmtMessage, PartialPath);
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

  counted_string Result = FormatCountedString(Memory, CSz("enum %S_type\n{\n  type_%S_noop,\n"), dUnion->Name, dUnion->Name);

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

  counted_string Result = FormatCountedString(Memory, CSz("struct %S\n{\n  enum %S Type;\n"),
      UnionName, TagType);

  ITERATE_OVER(&dUnion->CommonMembers)
  {
    struct_member* Member = GET_ELEMENT(Iter);
    Assert(Member->Type == type_variable_decl);
    Result =
      Concat(Result,
        FormatCountedString(Memory, CSz("  %S %S;\n"),
          Member->variable_decl.Type.DatatypeToken->Value,
          Member->variable_decl.Name),
      Memory);
  }

  b32 ValidMemberFound = False;
  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    if (Member->Flags != d_union_flag_enum_only)
    {
      if (!ValidMemberFound)
      {
        Result = Concat(Result, CS("\n  union\n  {\n"), Memory);
        ValidMemberFound = True;
      }
      Result = Concat(Result, FormatCountedString(Memory, CSz("    struct %S %S;\n"), Member->Type, Member->Name), Memory);
    }
  }
  if (ValidMemberFound)
  {
    Result = Concat(Result, CS("  };\n"), Memory);
  }

  Result = Concat(Result, CS("};\n\n"), Memory);

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
    if (StringsMatch(Struct->Type->Value, StructType))
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
    if (T || E ||StlContainer)
    {
      // TODO(Jesse, correctness): Not sure what the solution to this is, but
      // this is definitely incorrect.
      BUG("Multiple datatypes for %S detected!", Name);
    }
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
    while (c_token *Interior = PeekTokenPointer(Parser))
    {
      if (Complete) break;

      switch (Interior->Type)
      {
        case CTokenType_Identifier:
        {
          RequireToken(Parser, Interior);

          d_union_flags Flags = {};
          if ( OptionalToken(Parser, CToken(ToString(enum_only))) )
          {
            Flags = d_union_flag_enum_only;
          }

          PushMember(&dUnion, *Interior, Flags, Memory);
        } break;

        case CTokenType_CloseBrace:
        {
          RequireToken(Parser, CTokenType_CloseBrace);
          Complete = True;
        } break;

        default:
        {
          Parser->ErrorCode = ParseErrorCode_DUnionParse;
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
      Error("Couldn't find enum %S", dUnion.CustomEnumType);
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
          .Type.DatatypeToken = RequireTokenPointer(Parser, CTokenType_Identifier),
          .Name               = RequireToken(Parser, CTokenType_Identifier).Value,
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
ParseArgs(const char** ArgStrings, u32 ArgCount, parse_context *Ctx, memory_arena* Memory)
{
  arguments Result = {
    .Outpath      = CSz("."),
    .Files        = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
    .IncludePaths = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
  };

#if 0
  counted_string PrevArg = {};
  for ( u32 ArgIndex = 0;
        ArgIndex < ArgCount;
        ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    b32 PrevArgWasInclude = StringsMatch(PrevArg, CSz("-I")) || StringsMatch(PrevArg, CSz("--include-path"));

    if (PrevArgWasInclude) { LogDirect("\""); }
    LogDirect("%S", Arg);
    if (PrevArgWasInclude) { LogDirect("\""); }
    LogDirect(" ");

    PrevArg = Arg;
  }
  LogDirect("\n");
#endif

  for ( u32 ArgIndex = 1;
        ArgIndex < ArgCount;
        ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);

    if (StringsMatch(CSz("##"), Arg))
    {
      break;
    }
    else if (StringsMatch(CSz("-d"), Arg) ||
             StringsMatch(CSz("--do-debug-window"), Arg) )
    {
      Result.DoDebugWindow = True;
    }
    else if ( StringsMatch(CSz("-I"), Arg) ||
              StringsMatch(CSz("--include-path"), Arg) )
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

        Info("Include path added : (%S)", Include);
        Push(Include, &Result.IncludePaths);
      }
    }
    else if ( StringsMatch(CS("-D"), Arg) ||
              StringsMatch(CS("--define"), Arg) )
    {
      if (ArgIndex+1 < ArgCount)
      {
        ArgIndex += 1;
        counted_string MacroName = CS(ArgStrings[ArgIndex]);

        if (Contains(CSz("="), MacroName))
        {
          Warn("Currently, custom define values are unsupported.  Please use `(--define/-D) DEFINE_NAME` to set DEFINE_NAME=1.");
        }

        macro_def_linked_list_node *MacroNode = Allocate_macro_def_linked_list_node(Ctx->Memory);
        macro_def *Macro = &MacroNode->Element;

        Macro->Type = type_macro_keyword;
        Macro->Name = MacroName;

        CTokenCursor(&Macro->Body, 1, Memory, CSz("<CLI>"), TokenCursorSource_CommandLineOption, {0,0});

        Macro->Body.Start[0] = CToken(1u);

        Insert(MacroNode, &Ctx->Datatypes.Macros);
      }
      else
      {
        Error("Macro name required when using (--define/-D) switches.");
      }

    }
#if 0
    // NOTE(Jesse): This has to come after the above -D path
    else if ( StartsWith(Arg, CSz("-D")) )
    {
      macro_def *NewMacro = Push(&Ctx->Datatypes.Macros, { .Name = MacroNameToken->Value }, Ctx->Memory);
      NewMacro->Type = type_macro_keyword;
      counted_string Name = Substring(Arg, 2);
      NewMacro->Name = Name;
    }
#endif
    else if ( StringsMatch(CS("--log-level"), Arg) )
    {
      // Handled in SetupStdout
      ArgIndex += 1; // Have to skip the switch value
    }
    else if ( StringsMatch(CS("-c0"), Arg) ||
              StringsMatch(CS("--colors-off"), Arg) )
    {
      // Handled in SetupStdout
    }
    else if ( StringsMatch(CS("-o"), Arg) ||
              StringsMatch(CS("--output-path"), Arg) )
    {
      Result.Outpath = PopArgString(ArgStrings, ArgCount, &ArgIndex);
      if (LastChar(Result.Outpath) == '/')
      {
        Warn("Value passed with (%S) should not end with a '/'.", Arg);
      }
    }
    else if ( StartsWith(Arg, CSz("-")) )
    {
      Error("Unknown Switch %S", Arg);
    }
    else
    {
      Push(Arg, &Result.Files);
    }
  }

  TruncateToCurrentElements(&Result.Files);
  Rewind(&Result.Files);

  TruncateToCurrentElements(&Result.IncludePaths);
  Rewind(&Result.IncludePaths);

  Info("%d total include paths added", (u32)Count(&Result.IncludePaths));

  return Result;
}

#if 0
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
        Error("Renaming tempfile: %S -> %S", TempFile.Path, Filename);
      }
    }
    else
    {
      Error("Writing to tempfile: %S", TempFile.Path);
    }
  }
  else
  {
    Error("Opening tempfile: %S", TempFile.Path);
  }

  return Result;
}
#endif

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
            Info("File contents matched output for %S", OutputFilename);
          }
          else
          {
            Error("File contents didn't match for %S", OutputFilename);
            Error("TODO(Jesse): Should probably emit to a similar filname with an incremented extension or something..");
          }
        }
        else if (Rename(TempFile.Path, OutputFilename))
        {
          Info("Generated and output %S", OutputFilename);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %S -> %S", TempFile.Path, OutputFilename);
        }
      }
      else
      {
        if (Rename(TempFile.Path, OutputFilename))
        {
          Info("Generated and output %S", OutputFilename);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %S -> %S", TempFile.Path, OutputFilename);
        }
      }
    }
    else
    {
      Error("Writing to tempfile: %S", TempFile.Path);
    }
  }
  else
  {
    Error("Opening tempfile: %S", TempFile.Path);
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
    DebugChars("%S\n", Message);
  }
}

bonsai_function void
EatUntil_TrackingDepth(parser *Parser, c_token_type Open, c_token_type Close, c_token *StartToken)
{
  u32 Depth = 0;
  b32 Success = False;
  while ( c_token *T = PopTokenPointer(Parser) )
  {

    if (T->Type == Open)
    {
      ++Depth;
    }

    if (T->Type == Close)
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
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)), StartToken);
  }

  return;
}

bonsai_function void
EatBetweenRaw(parser* Parser, c_token_type Open, c_token_type Close)
{
  u32 Depth = 0;
  RequireTokenRaw(Parser, Open);

  b32 Success = False;
  while (RawTokensRemain(Parser))
  {
    c_token T = PopTokenRaw(Parser);

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
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)));
  }

  return;
}

bonsai_function void
RequireToken(ansi_stream *Code, char C)
{
  Assert(*Code->At == C);
  Advance(Code);
}

bonsai_function counted_string
EatBetweenExcluding(ansi_stream *Code, char Open, char Close)
{
  u32 Depth = 0;
  RequireToken(Code, Open);

  const char* Start = Code->At;

  b32 Success = False;
  while (Remaining(Code))
  {
    char T = Advance(Code);

    if (T == Open)
    {
      ++Depth;
    }

    if (T == Close)
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
    InvalidCodePath();
    /* ParseError(Code, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close))); */
  }

  Assert(Code->At-1 >= Code->Start);
  const char* End = Code->At-1;

  umm Count = (umm)(End-Start);

  counted_string Result = {
    .Count = Count,
    .Start = Start,
  };

  return Result;
}

bonsai_function void
EatBetween(parser* Parser, c_token_type Open, c_token_type Close)
{
  b32 Success = False;

  if (Parser->ErrorCode == ParseErrorCode_None)
  {
    u32 Depth = 0;
    RequireToken(Parser, Open);

    while ( c_token *T = PopTokenPointer(Parser) )
    {

      if (T->Type == Open)
      {
        ++Depth;
      }

      if (T->Type == Close)
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
      ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)));
    }
  }
}

bonsai_function struct_def
StructDef(c_token *StructNameT) // , counted_string Sourcefile)
{
  struct_def Result = {
    .Type = StructNameT,
    /* .DefinedInFile = Sourcefile */
  };

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
  Assert(Parser->Tokens->At == Parser->Tokens->Start);

  while (NextTokenIsSpaceOrTab(Parser)) { PopTokenRaw(Parser); }
  OptionalTokenRaw(Parser, CTokenType_Newline);
  Parser->Tokens->Start = Parser->Tokens->At;
}

bonsai_function void
TrimFirstToken(parser* Parser, c_token_type TokenType)
{
  Assert(Parser->Tokens->At == Parser->Tokens->Start);
  RequireToken(Parser, TokenType);
  Parser->Tokens->Start = Parser->Tokens->At;
}

bonsai_function void
TrimLastToken(parser* Parser, c_token_type TokenType)
{
  c_token* CurrentToken = Parser->Tokens->End-1;

  while (CurrentToken >= Parser->Tokens->Start)
  {
    if (CurrentToken->Type == TokenType)
    {
      Parser->Tokens->End = CurrentToken;
      break;
    }

    --CurrentToken;
  }
}

inline b32
TokenValidFor(c_token_cursor *Tokens, c_token *T)
{
  b32 Result = T >= Tokens->Start && T <= Tokens->End;
  return Result;
}

bonsai_function parser
GetBodyTextForNextScope(parser* Parser, memory_arena *Memory)
{
  // TODO(Jesse, immediate): This should return c_token_cursor .. I think ..
  parser BodyText = {};

  c_token_cursor *Tokens = AllocateProtection(c_token_cursor, Memory, 1, False);
  BodyText.Tokens = Tokens;

  c_token_cursor *StartTokens = Parser->Tokens;
  c_token *Start = PeekTokenPointer(Parser);

  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);

  // TODO(Jesse, immediate): This is janky as fuck and should work..
  // differently.  I'm not entirely sure _what_ should be different, but the
  // assertions and -1 +1 business below is a symptom of the return value of
  // this function having to be a slice into an existing c_token_cursor.  This
  // holds true without issue (at the moment) for the interpreter code, but the
  // C++ parsing code can encounter situations where the token after End was on
  // a different cursor, so we do these sketchy hacks.  There are also
  // situations where this fails completely.. ie if a function is closed by a
  // macro.  So..  what we should do in that situation is unclear.
  //
  // This is documented with the define BUG_BODY_SCOPE_ACROSS_C_TOKEN_CURSOR_BOUNDARY
  //
  c_token *End = PeekTokenRawPointer(Parser, -1);

  Assert(End->Type == CTokenType_CloseBrace);
  Assert(TokenValidFor(StartTokens, Start));
  Assert(TokenValidFor(StartTokens, End));

  CTokenCursor(Tokens, Start, (umm)(End-Start) + 1, Start->Filename, TokenCursorSource_BodyText, {});

  TrimFirstToken(&BodyText, CTokenType_OpenBrace);
  TrimUntilNewline(&BodyText);
  TrimLastToken(&BodyText, CTokenType_CloseBrace);
  TrimTrailingWhitespace(&BodyText);
  Rewind(BodyText.Tokens);

  Assert(BodyText.Tokens->At == BodyText.Tokens->Start);

  return BodyText;
}

bonsai_function type_indirection_info
ParseReferencesIndirectionAndPossibleFunctionPointerness(parser *Parser)
{
  type_indirection_info Result = {};

  b32 Done = False;
  if (OptionalToken(Parser, CTokenType_Const))
  {
    Result.ConstPointer = True;
  }

  while (TokensRemain(Parser) && Done == False)
  {
    c_token T = PeekToken(Parser);

    switch (T.Type)
    {
      case CTokenType_OpenParen:
      {
        RequireToken(Parser, CTokenType_OpenParen);
        RequireToken(Parser, CTokenType_Star);

        if (c_token *FunctionPointerTypeNameToken = OptionalToken(Parser, CTokenType_Identifier))
        {
          Result.FunctionPointerTypeName = FunctionPointerTypeNameToken->Value;
        }

        if (PeekToken(Parser).Type == CTokenType_OpenParen)
           { EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen); }
        RequireToken(Parser, CTokenType_CloseParen);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        Result.IsFunctionPointer = True;
        Done = True;
      } break;

      case CTokenType_Volatile:
      {
        RequireToken(Parser, CTokenType_Volatile);
        Result.VolatileValue = True;
      } break;

      case CTokenType_LogicalAnd:
      {
        RequireToken(Parser, CTokenType_LogicalAnd);
        Assert(Result.ReferenceLevel == 0);
        Result.ReferenceLevel += 2;
      } break;

      case CTokenType_Ampersand:
      {
        RequireToken(Parser, CTokenType_Ampersand);
        ++Result.ReferenceLevel;
      } break;

      case CTokenType_Star:
      {
        RequireToken(Parser, CTokenType_Star);
        ++Result.IndirectionLevel;
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
      case CT_NameQualifier:
      case CTokenType_OperatorKeyword: // Finish parsing the return type of an operator funciton
      case CTokenType_OpenBracket: // Parsing an un-named followed by [], such as in `void myfunc(char[42])`
      case CTokenType_Ellipsis:
      case CTokenType_Semicolon:
      {
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token while parsing indirection and reference levels."));
    }
  }

  return Result;
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
    c_token TemplateParamListTestT = PeekToken(Parser, (s32)Lookahead);

    switch (TemplateParamListTestT.Type)
    {
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Long:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Bool:
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
IsConstructorOrDestructorName(counted_string ClassName, counted_string FnName)
{
  b32 Result = StringsMatch(ClassName, FnName);
  return Result;
}

bonsai_function b32
IsConstructorOrDestructorName(c_token *ClassNameT, c_token *FNameT)
{
  b32 Result = False;
  if (ClassNameT && FNameT)
  {
    Assert(ClassNameT != FNameT);
    Result = StringsMatch(ClassNameT->Value, FNameT->Value);
  }

  return Result;
}

bonsai_function b32
IsConstructorOrDestructorName(counted_string *ClassName, counted_string *FnName)
{
  b32 Result = StringsMatch(ClassName, FnName);
  return Result;
}


bonsai_function b32
IsConstructorOrDestructorName(c_token *T)
{
  b32 Result = False;
  if (T)
  {
    if (T->Type) { Assert(T->Type == CTokenType_Identifier); }
    Result = T->QualifierName && StringsMatch(T->QualifierName->Value, T->Value);
  }
  return Result;
}

bonsai_function void
ParseLongness(parser *Parser, type_spec *Result)
{
  RequireToken(Parser, CTokenType_Long);
  Result->Qualifier |= TypeQual_Long;

  if (OptionalToken(Parser, CTokenType_Double)) { Result->Qualifier |= TypeQual_Double; }
  else
  {
    if (OptionalToken(Parser, CTokenType_Long)) { Result->Qualifier |= TypeQual_Long_Long; }
    if (OptionalToken(Parser, CTokenType_Int)) { Result->Qualifier |= TypeQual_Int; }
  }
}

bonsai_function void
EatNameQualifiers(parser *Parser)
{
  while (c_token *T = PeekTokenPointer(Parser))
  {
    if (T->Type == CT_NameQualifier)
    {
      RequireToken(Parser, *T);
      RequireToken(Parser, CT_ScopeResolutionOperator);
    }
    else
    {
      /* Assert(T->Type == CTokenType_Identifier); */
      break;
    }
  }
}

bonsai_function void
TryEatAttributes(parser *Parser)
{
  while (OptionalToken(Parser, CT_KeywordAttribute))
  {
    EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
  }
}

bonsai_function counted_string 
StringFromTokenSpan(parser *Parser, c_token *StartToken, c_token *EndToken, memory_arena *Memory)
{
  Assert(IsValidForCursor(Parser->Tokens, StartToken));
  Assert(IsValidForCursor(Parser->Tokens, EndToken));

  counted_string Result = {};
  return Result;
}

bonsai_function void
ParsePrimitivesAndQualifiers(parser *Parser, type_spec *Result)
{
  b32 Done = False;
  while (!Done)
  {
    c_token T = PeekToken(Parser);

    switch (T.Type)
    {
      case CT_KeywordAttribute:
      {
        // Type attribute
        TryEatAttributes(Parser);
      } break;

      case CTokenType_At:
      {
        NotImplemented;

        /* RequireToken(Parser, CTokenType_At); */
        /* Result->IsMetaTemplateVar = True; */
      } break;

      case CTokenType_ThreadLocal:
      {
        RequireToken(Parser, CTokenType_ThreadLocal);
        Result->Qualifier |= TypeQual_ThreadLocal;
      } break;

      case CTokenType_Const:
      {
        if (Result->Qualifier & TypeQual_Const)
        {
          Done = True;
        }
        else
        {
          RequireToken(Parser, CTokenType_Const);
          Result->Qualifier |= TypeQual_Const;
        }
      } break;

      case CT_Keyword_Constexpr:
      {
        RequireToken(Parser, CT_Keyword_Constexpr);
        Result->Qualifier |= TypeQual_Constexpr;
      } break;
      case CTokenType_Static:
      {
        RequireToken(Parser, CTokenType_Static);
        Result->Qualifier |= TypeQual_Static;
      } break;
      case CTokenType_Volatile:
      {
        RequireToken(Parser, CTokenType_Volatile);
        Result->Qualifier |= TypeQual_Volatile;
      } break;
      case CTokenType_Signed:
      {
        RequireToken(Parser, T);
        Result->Qualifier |= TypeQual_Signed;
      } break;
      case CTokenType_Unsigned:
      {
        RequireToken(Parser, T);
        Result->Qualifier |= TypeQual_Unsigned;
      } break;
      case CTokenType_Enum:
      {
        RequireToken(Parser, CTokenType_Enum);
        Result->Qualifier |= TypeQual_Enum;
      } break;
      case CT_Keyword_Class:
      {
        RequireToken(Parser, CT_Keyword_Class);
        Result->Qualifier |= TypeQual_Class;
      } break;
      case CTokenType_Struct:
      {
        RequireToken(Parser, CTokenType_Struct);
        Result->Qualifier |= TypeQual_Struct;
      } break;
      case CTokenType_Union:
      {
        RequireToken(Parser, CTokenType_Union);
        Result->Qualifier |= TypeQual_Union;
      } break;

      case CTokenType_TemplateKeyword:
      {
        RequireToken(Parser, CTokenType_TemplateKeyword);
        string_from_parser Builder = StartStringFromParser(Parser);
        EatBetween(Parser, CTokenType_LT, CTokenType_GT);
        Result->TemplateSource = FinalizeStringFromParser(&Builder);
      } break;

      case CTokenType_Extern:
      {
        RequireToken(Parser, T.Type);
        Result->Linkage = linkage_extern;
        if ( OptionalToken(Parser, CToken(CTokenType_StringLiteral, CSz("\"C\""))) )
        {
          Result->Linkage = linkage_extern_c;
        }
        else if ( OptionalToken(Parser, CToken(CTokenType_StringLiteral, CSz("\"C++\""))) )
        {
          Result->Linkage = linkage_extern_cpp;
        }
      } break;

      case CTokenType_Inline:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Inline;
      } break;

      case CTokenType_Long:
      {
        RequireToken(Parser, CTokenType_Long);
        if (Result->Qualifier & TypeQual_Long)
        {
          Result->Qualifier |= TypeQual_Long_Long;
        }
        Result->Qualifier |= TypeQual_Long;
      } break;

      case CTokenType_Short:
      {
        RequireToken(Parser, CTokenType_Short);
        Result->Qualifier |= TypeQual_Short;
      } break;

      case CTokenType_Int:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Int;
      } break;

      case CTokenType_Double:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Double;
      } break;
      case CTokenType_Float:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Float;
      } break;
      case CTokenType_Char:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Char;
      } break;
      case CTokenType_Void:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Void;
      } break;
      case CTokenType_Bool:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Bool;
      } break;

      case CTokenType_Auto:
      {
        RequireToken(Parser, T.Type);
        Result->Qualifier |= TypeQual_Auto;
      } break;

      case CT_NameQualifier:
      {
        EatNameQualifiers(Parser);
      } break;

      default:
      {
        Done = True;
      } break;
    }

    continue;
  }
}

bonsai_function b32
IsPrimitiveType(type_spec *Type)
{
  u32 Mask =
    TypeQual_Constexpr |
    TypeQual_Auto      |
    TypeQual_Bool      |
    TypeQual_Signed    |
    TypeQual_Unsigned  |
    TypeQual_Char      |
    TypeQual_Long      |
    TypeQual_Int       |
    TypeQual_Long_Long |
    TypeQual_Double    |
    TypeQual_Short     |
    TypeQual_Void      |
    TypeQual_Float;

  b32 Result = (Type->Qualifier & Mask) > 0;
  return Result;
}

bonsai_function type_spec
ParseTypeSpecifier(parse_context *Ctx, c_token *StructNameT = 0)
{
  type_spec Result = {};
  parser *Parser = Ctx->CurrentParser;

  ParsePrimitivesAndQualifiers(Parser, &Result);

  if (IsPrimitiveType(&Result))
  {
  }
  else if (OptionalToken(Parser, CTokenType_OperatorKeyword))
  {
    // NOTE(Jesse): It's nuts we have to do this check three times but that's
    // just the world we live in when we're trying to tolerate unknown types
    //
    // This case is for a struct member such as
    //
    // operator bool()
    //
    // @operator_qual_triple_check
    Result.Qualifier |= TypeQual_Operator;
  }
  else
  {
    Result.DatatypeToken = RequireTokenPointer(Parser, CTokenType_Identifier);
    Result.Datatype = GetDatatypeByName(&Ctx->Datatypes, Result.DatatypeToken->Value);
    // TODO(Jesse, id: 296, tags: immediate): When we properly traverse
    // include graphs, this assert should not fail.
    //
    // Assert(Result.Datatype.Type != type_datatype_noop);
  }

  if (OptionalToken(Parser, CTokenType_OperatorKeyword))
  {
    // @operator_qual_triple_check
    //
    // This case is for:
    //
    // u32 operator *()
    Result.Qualifier |= TypeQual_Operator;
  }

  TryEatAttributes(Parser); // Value Attribute

  Result.HasTemplateArguments = TryAndEatTemplateParameterList(Parser, &Ctx->Datatypes);

  b32 IsConstructor = (IsConstructorOrDestructorName(Result.DatatypeToken) ||
                          IsConstructorOrDestructorName(Result.DatatypeToken, StructNameT)) && PeekToken(Parser).Type == CTokenType_OpenParen;

  if (IsConstructor)
  {
    Result.Qualifier |= TypeQual_Constructor;
  }
  else if (Result.Qualifier & TypeQual_Operator)
  {
  }
  else
  {
    Result.Indirection = ParseReferencesIndirectionAndPossibleFunctionPointerness(Parser);
  }

  // @operator_qual_triple_check
  //
  // This case is for:
  //
  // foo *operator &()
  if (OptionalToken(Parser, CTokenType_OperatorKeyword))
  {
    Result.Qualifier |= TypeQual_Operator;
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

bonsai_function variable_decl
ParseVariableDecl(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  variable_decl Result = {
    .Type = ParseTypeSpecifier(Ctx),
  };

  if (PeekToken(Parser).Type == CTokenType_Identifier)
  {
    Result.Name = RequireToken(Parser, CTokenType_Identifier).Value;

    TryEatAttributes(Parser);

    if ( PeekToken(Parser).Type == CTokenType_OpenBracket )
    {
      ParseExpression(Ctx, &Result.StaticBufferSize);
    }

    if (OptionalToken(Parser, CTokenType_Equals))
    {
      if (PeekToken(Parser).Type == CTokenType_OpenBrace)
      {
        Result.Value = ParseInitializerList(Ctx->CurrentParser, Ctx->Memory);
      }
      else
      {
        ParseExpression(Ctx, &Result.Value);
      }
    }
  }
  else
  {
    if ( PeekToken(Parser).Type == CTokenType_OpenBracket )
    {
      ParseExpression(Ctx, &Result.StaticBufferSize);
    }
  }

  return Result;
}


bonsai_function void
MaybeParseDeleteOrDefault(parser *Parser, function_decl *Result)
{
  auto Type = Result->Type ;
  Assert(Type == function_type_constructor ||
         Type == function_type_destructor ||
         Type == function_type_operator );

  if (OptionalToken(Parser, CTokenType_Equals))
  {
    Result->ImplIsDefault = (b32)(OptionalToken(Parser, CTokenType_Default) != 0);
    Result->ImplIsDeleted = (b32)(OptionalToken(Parser, CT_Keyword_Delete) != 0);

    if (! (Result->ImplIsDefault || Result->ImplIsDeleted) )
    {
      ParseError(Parser, CSz("Expected either keyword : 'delete' or 'default'") );
    }

    if ( Result->ImplIsDefault && Result->ImplIsDeleted )
    {
      ParseError(Parser,
          FormatCountedString(TranArena, CSz("Cannot delete and default implementation of %S %S"), ToString(Type), Result->NameT->Value) );
    }
  }
}

bonsai_function void
MaybeParseAttributes(parser *Parser)
{
  if (PeekToken(Parser).Type == CT_KeywordAttribute)
  {
    // void FunctionName( arg A1, arg, A2) __attribute__((whatever));
    //
    TryEatAttributes(Parser);
    RequireToken(Parser, CTokenType_Semicolon);
  }
}

bonsai_function function_decl
ParseFunctionParameterList(parse_context *Ctx, type_spec *ReturnType, c_token *FuncNameT, function_type Type, c_token *StructNameT = 0)
{
  parser *Parser = Ctx->CurrentParser;

  RequireToken(Parser, CTokenType_OpenParen);

  function_decl Result = {
    .Type = Type,
    .ReturnType = *ReturnType,
    .NameT = FuncNameT,
  };

  // Function definition args
  b32 DoneParsingArguments = PeekToken(Parser) == CToken(CTokenType_CloseParen);

  switch (Type)
  {
    case function_type_destructor:
    {
      InvalidCodePath();
    } break;

    case function_type_constructor:
    case function_type_operator:
    case function_type_normal:
    {
      // TODO(Jesse): Is this valid for constructors?
      if ( PeekToken(Parser).Type == CTokenType_Void &&
           PeekToken(Parser, 1).Type == CTokenType_CloseParen )
      {
        RequireToken(Parser, CTokenType_Void);
        DoneParsingArguments = True;
      }

      while ( !DoneParsingArguments && TokensRemain(Parser) )
      {
        variable_decl Arg = ParseVariableDecl(Ctx);
        Push(&Result.Args, Arg, Ctx->Memory);

        if (!OptionalToken(Parser, CTokenType_Comma))
        {
          DoneParsingArguments = True;
        }

        if (OptionalToken(Parser, CTokenType_Ellipsis))
        {
          Result.IsVariadic = True;
          DoneParsingArguments = True;
        }

        continue;
      }

      RequireToken(Parser, CTokenType_CloseParen); // Ending paren for arguments

    } break;

    InvalidDefaultWhileParsing(Parser, FormatCountedString(TranArena, CSz("ParseFunctionParameterList called with unsupported function type (%S)."), ToString(Type)) );
  }

  switch (Type)
  {
    case function_type_destructor:
    {
      InvalidCodePath();
    } break;

    case function_type_operator:
    {
    } break;

    case function_type_constructor:
    case function_type_normal: {} break;
    case function_type_noop: { ParseError(Parser, CSz("Got function_type_noop during ParseFunctionParameterList") ); } break;
  }


  MaybeParseAttributes(Parser);

#if 0
  if (PeekToken(Parser).Type == CTokenType_OpenBrace)
  {
    // void FunctionName( arg A1, arg, A2) { .. body text .. }
    Result.Body = GetBodyTextForNextScope(Parser, Ctx->Memory);
  }
#endif


  return Result;
}

bonsai_function void
ParseStaticBuffers(parse_context *Ctx, parser *Parser, ast_node **Dest)
{
  ast_node_expression *Current = {};

  if ( PeekToken(Parser).Type == CTokenType_OpenBracket )
  {
    Current = AllocateAndCastTo(ast_node_expression, Dest, Ctx->Memory);
  }

  while ( OptionalToken(Parser, CTokenType_OpenBracket) )
  {
    ParseExpression(Ctx, Current);
    RequireToken(Parser, CTokenType_CloseBracket);

    if ( PeekToken(Parser).Type == CTokenType_OpenBracket )
    {
      Current->Next = Allocate(ast_node_expression, Ctx->Memory, 1);
    }
    Current = Current->Next;
  }
}

// NOTE(Jesse): This function is not meant to parse struct-member specific
// functions such as constructors and destructors.  It parses variables or free functions
bonsai_function declaration
ParseFunctionOrVariableDecl(parse_context *Ctx, type_spec *DeclType)
{
  parser *Parser = Ctx->CurrentParser;

  declaration Result = {};

  if (DeclType->Indirection.IsFunctionPointer)
  {
    Result.Type = type_declaration_variable_decl;
    Result.variable_decl.Type = *DeclType;
  }
  else
  {
    if ( DeclType->Qualifier & TypeQual_Operator ) // operator==(foo *F1, foo *F2)
    {
      c_token *OperatorNameT = RequireOperatorToken(Parser);

      if ( PeekToken(Parser).Type == CTokenType_OpenParen )
      {
        Result.Type = type_declaration_function_decl;
        Result.function_decl = ParseFunctionParameterList(Ctx, DeclType, OperatorNameT, function_type_operator);
        MaybeParseDeleteOrDefault(Parser, &Result.function_decl);
        Result.function_decl.Body = MaybeParseFunctionBody(Parser, Ctx->Memory);
      }
    }
    else if (DeclType->Qualifier & TypeQual_Constructor) // my_thing::my_thing(...) {...}
    {
      Result.Type = type_declaration_function_decl;
      Result.function_decl = ParseFunctionParameterList(Ctx, DeclType, DeclType->DatatypeToken, function_type_constructor, DeclType->DatatypeToken->QualifierName);
      Result.function_decl.Body = MaybeParseFunctionBody(Parser, Ctx->Memory);
    }
    else if ( OptionalToken(Parser, CTokenType_Semicolon) ) // template<typename T> class foo;
    {
      // TODO(Jesse): Do we actually need this still?
    }
    else // could be variable or fn
    {
      EatNameQualifiers(Parser);

      c_token *DeclNameToken = RequireTokenPointer(Parser, CTokenType_Identifier);

      b32 IsFunction = PeekToken(Parser).Type == CTokenType_OpenParen;
      if ( IsFunction )
      {
        Result.Type = type_declaration_function_decl;
        Result.function_decl = ParseFunctionParameterList(Ctx, DeclType, DeclNameToken, function_type_normal);
        Result.function_decl.Body = MaybeParseFunctionBody(Parser, Ctx->Memory);
      }
      else // Variable decl
      {
        // TODO(Jesse): Call ParseVariableDecl here!!
        Result.Type = type_declaration_variable_decl;
        Result.variable_decl.Type = *DeclType;
        Result.variable_decl.Name = DeclNameToken->Value;

        TryEatAttributes(Parser);

        ParseStaticBuffers(Ctx, Parser, &Result.variable_decl.StaticBufferSize);

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
        else if ( PeekToken(Parser).Type == CTokenType_Semicolon )
        {
        }
        else if ( PeekToken(Parser).Type == CTokenType_Comma )
        {
        }
        else if ( PeekToken(Parser).Type == CTokenType_OpenBrace )
        {
          Result.variable_decl.Value = ParseInitializerList(Parser, Ctx->Memory);
        }
        else
        {
          ParseError_ExpectedSemicolonEqualsCommaOrOpenBrace(Parser, PeekTokenPointer(Parser));
        }
      }
    }
  }

  // TODO(Jesse id: 323, tags: id_321)
  /* Assert(Result.Type); */
  return Result;
}

#define _a_TEST 'a'

#if _a_TEST

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

    case CTokenType_Ampersand:
    {
      Result = LHS & RHS;
    } break;

    case CTokenType_Pipe:
    {
      Result = LHS | RHS;
    } break;

    case CTokenType_Hat:
    {
      Result = LHS ^ RHS;
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

#if 0
bonsai_function b32
IsNextTokenMacro(parse_context *Ctx, parser *Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);
  b32 Result = (GetMacroDef(Ctx, T->Value) != 0);
  return Result;
}
#endif

// This function is confusing as fuck.  Sorry about that.  It's written
// recursively because when we encounter a macro literal we either had to call
// ResolveMacroConstantExpression again on the resulting expanded parser, or
// track each macro expansion and where we're at in each.  That seemed onerous
// and annoying, so I did it the 'lazy' recursive way.
//
// TODO(Jesse): Should this function not return a signed value?!  Pretty sure
// constant expressions can resolve to negative values.. just sayin..
bonsai_function u64
ResolveMacroConstantExpression(parse_context *Ctx, parser *Parser, memory_arena *PermMemory, memory_arena *TempMemory, u64 PreviousValue, b32 LogicalNotNextValue)
{
  TIMED_FUNCTION();

  u64 Result = PreviousValue;

  EatSpacesTabsAndEscapedNewlines(Parser);

  if (RawTokensRemain(Parser))
  {
    c_token T = PeekTokenRaw(Parser); // Has to be raw because newlines delimit the end of a macro expression
    switch (T.Type)
    {
      case CT_MacroLiteral:
      {
        if (T.Erased)
        {
          RequireTokenRaw(Parser, T);

          if (T.Macro.Def->Type == type_macro_function)
          {
            c_token Next = PeekTokenRaw(Parser);
            if (Next.Type == CTokenType_OpenParen)
            {
              EatBetweenRaw(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
            }
          }
          Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue);
        }
        else
        {
          ParseError(Parser, CSz("Invalid MacroLiteral encountered."));
        }
      } break;

      case CTokenType_Question:
      {
        RequireTokenRaw(Parser, T);
        u64 Operator = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue);
        if (Operator)
        {
          Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue);
        }
        else
        {
          EatUntilIncluding(Parser, CTokenType_Colon);
          Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue);
        }
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue);
      } break;

      case CTokenType_Bang:
      {
        RequireTokenRaw(Parser, CTokenType_Bang);
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue ? False : True);
     } break;

      case CTokenType_Identifier:
      {
        if (OptionalTokenRaw(Parser, CToken(CSz("defined"))))
        {
          EatSpacesTabsAndEscapedNewlines(Parser);
          c_token NextToken = PeekTokenRaw(Parser);
          switch (NextToken.Type)
          {
            case CT_MacroLiteral:
            case CTokenType_Identifier:
            {
              // TODO(Jesse, tags: correctness): Should this path has to set the next token to CT_MacroLiteral
              c_token *PotentialMacroToken = PeekTokenRawPointer(Parser);
              macro_def *M = GetMacroDef(Ctx, PotentialMacroToken->Value);
              b64 NextTokenIsMacro = (M && !M->Undefed);
              RequireTokenRaw(Parser, PotentialMacroToken->Type); // @optimize_call_advance_instead_of_being_dumb

              Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, LogicalNotNextValue ? !NextTokenIsMacro : NextTokenIsMacro, False);
            } break;

            case CTokenType_OpenParen:
            {
              u32 ParenCount = 0;
              while (PeekTokenRaw(Parser).Type == CTokenType_OpenParen )
              {
                RequireTokenRaw(Parser, CTokenType_OpenParen);
                EatSpacesTabsAndEscapedNewlines(Parser);
                ++ParenCount;
              }

              // TODO(Jesse, tags: correctness): Should this path has to set the next token to CT_MacroLiteral
              c_token *PotentialMacroToken = PeekTokenRawPointer(Parser);
              macro_def *M = GetMacroDef(Ctx, PotentialMacroToken->Value);
              b64 NextTokenIsMacro = (M && !M->Undefed);
              RequireTokenRaw(Parser, PotentialMacroToken->Type); // @optimize_call_advance_instead_of_being_dumb

              while (ParenCount--)
              {
                EatSpacesTabsAndEscapedNewlines(Parser);
                RequireTokenRaw(Parser, CTokenType_CloseParen);
              }

              Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, LogicalNotNextValue ? !NextTokenIsMacro : NextTokenIsMacro, False);
            } break;

            InvalidDefaultCase;
          }
        }
        else
        {
          // TODO(Jesse)(correctness): Call TryTransmuteIdentifierToMacro() here?
          macro_def *Macro = GetMacroDef(Ctx, T.Value);
          if (Macro)
          {
            c_token *pT = PeekTokenRawPointer(Parser);
            pT->Type = CT_MacroLiteral;
            pT->Macro.Def = Macro;
            Assert(pT->Macro.Expansion == 0);

            // TODO(Jesse): Is it actually even possible for this to fail?
            parser *Expanded = ExpandMacro(Ctx, Parser, Macro, PermMemory, TempMemory);
            if (Expanded->ErrorCode)
            {
              ParseError( Parser,
                          Expanded->ErrorCode,
                          FormatCountedString(TranArena, CSz("While Expanding %S"), pT->Value),
                          pT);
            }
            else
            {
              u64 MacroExpansion = ResolveMacroConstantExpression(Ctx, Expanded, PermMemory, TempMemory, Result, LogicalNotNextValue);
              Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, MacroExpansion, False);
            }
          }
          else if (!Macro || Macro->Undefed)
          {
            RequireTokenRaw(Parser, CTokenType_Identifier);
            Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, LogicalNotNextValue ? 1 : 0, False);
          }
          else
          {
            InvalidCodePath();
          }
        }
      } break;

      case CTokenType_CharLiteral:
      case CTokenType_IntLiteral:
      {
        RequireTokenRaw(Parser, T.Type);

        u64 ThisValue = LogicalNotNextValue ? !T.UnsignedValue : T.UnsignedValue;
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, ThisValue, False);

      } break;

      case CTokenType_OpenParen:
      {
        RequireTokenRaw(Parser, T.Type);
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, False);
        if (LogicalNotNextValue)
        {
          Result = !Result;
        }
        EatSpacesTabsAndEscapedNewlines(Parser);
        RequireTokenRaw(Parser, CTokenType_CloseParen);
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, False);
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
      case CTokenType_Ampersand:
      case CTokenType_Pipe:
      case CTokenType_Hat:
      {
        RequireTokenRaw(Parser, T.Type);
        c_token_type OperatorToApply = T.Type;
        u64 NextValue = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, False);
        u64 OperationResult = ApplyOperator(Parser, Result, OperatorToApply, NextValue);
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, OperationResult, False);
      } break;

      case CTokenType_CommentMultiLine:
      case CTokenType_EscapedNewline:
      {
        RequireTokenRaw(Parser, T.Type);
        Result = ResolveMacroConstantExpression(Ctx, Parser, PermMemory, TempMemory, Result, LogicalNotNextValue);
      } break;

      case CTokenType_CommentSingleLine:
      case CTokenType_Newline:
      case CTokenType_CloseParen:
      case CTokenType_Colon:
      {
        // We're done
      } break;

      default: { ParseError(Parser, FormatCountedString_(TranArena, CSz(" ResolveMacroConstantExpression failed : Invalid %S(%S)"), ToString(T.Type), T.Value )); } break;
    }
  }

  return Result;
}

bonsai_function void
EraseToken(c_token *Token)
{
  Token->Erased = True;
}

bonsai_function void
EraseBetweenExcluding(parser *Parser, c_token *FirstToErase, c_token *OnePastLastToErase)
{
  b32 E0 = (FirstToErase > OnePastLastToErase);
  b32 E1 = (FirstToErase < Parser->Tokens->Start);
  b32 E2 = (FirstToErase >= Parser->Tokens->End);
  b32 E3 = (OnePastLastToErase <= Parser->Tokens->Start);
  b32 E4 = (OnePastLastToErase > Parser->Tokens->End);

  if (E0 || E1 || E2 || E3 || E4)
  {
    ParseError(Parser, CSz("Invalid token span passed to EraseBetweenExcluding"));
  }
  else
  {
    c_token *At = FirstToErase;
    while (At < OnePastLastToErase)
    {
      EraseToken(At++);
    }

  }

  return;
}

bonsai_function c_token *
EraseAllRemainingIfBlocks(parser *Parser)
{
  c_token *StartToken = PeekTokenRawPointer(Parser);
  c_token *Result = StartToken;

  u32 Depth = 0;
  while (Result)
  {
    if (Result->Type == CT_PreprocessorIf ||
        Result->Type == CT_PreprocessorIfNotDefined ||
        Result->Type == CT_PreprocessorIfDefined)
    {
      ++Depth;
    }

    if (Result->Type == CT_PreprocessorEndif )
    {
      if (Depth == 0)
      {
        break;
      }

      --Depth;
    }

    Result->Erased = True;

    // @optimize_call_advance_instead_of_being_dumb
    RequireTokenRaw(Parser, *Result);
    Result = PeekTokenRawPointer(Parser);
  }

  if (!Result)
  {
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token for %S."), ToString(CT_PreprocessorIf)), StartToken);
  }

  /* Assert(Result == Parser->Tokens->At); */
  return Result;
}

bonsai_function c_token *
EatIfBlock(parser *Parser, erase_token_mode EraseMode)
{
  c_token *StartToken = PeekTokenRawPointer(Parser);
  c_token *Result = StartToken;

  u32 Depth = 0;
  while (Result)
  {
    if (Result->Type == CT_PreprocessorIf ||
        Result->Type == CT_PreprocessorIfNotDefined ||
        Result->Type == CT_PreprocessorIfDefined)
    {
      ++Depth;
    }

    if ( Result->Type == CT_PreprocessorElse ||
         Result->Type == CT_PreprocessorElif ||
         Result->Type == CT_PreprocessorEndif )
    {
      if (Depth == 0)
      {
        break;
      }

      if (Result->Type == CT_PreprocessorEndif)
      {
        --Depth;
      }
    }

    if (EraseMode == EraseTokens)
    {
      Result->Erased = True;
    }

    // @optimize_call_advance_instead_of_being_dumb
    RequireTokenRaw(Parser, *Result);
    Result = PeekTokenRawPointer(Parser);
  }

  if (!Result)
  {
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token for %S."), ToString(CT_PreprocessorIf)), StartToken);
  }

  /* Assert(Result == Parser->Tokens->At); */
  return Result;
}

#if 0
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
#endif

bonsai_function macro_def *
GetMacroDef(parse_context *Ctx, counted_string DefineValue)
{
  TIMED_FUNCTION();
  macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, DefineValue);

  macro_def *Result = 0;
  if (Macro && !Macro->Undefed)
  {
    Result = Macro;
  }
  return Result;
}

#if 0
bonsai_function macro_def *
GetMacroDef(parse_context *Ctx, counted_string *DefineValue)
{
  macro_def *Result = GetMacroDef(Ctx, *DefineValue);
  return Result;
}
#endif

bonsai_function b32
IsImplementation(function_decl *Func)
{
  b32 Result = Func->Body.Tokens != 0;
  return Result;
}

bonsai_function b32
IsDefaultImplementation(function_decl *Func)
{
  b32 Result = Func->ImplIsDefault;
  return Result;
}

bonsai_function b32
IsDeletedImplementation(function_decl *Func)
{
  b32 Result = Func->ImplIsDeleted;
  return Result;
}

bonsai_function parser
MaybeParseFunctionBody(parser *Parser, memory_arena *Memory)
{
  parser Result = {};
  if (PeekToken(Parser).Type == CTokenType_OpenBrace)
  {
    Result = GetBodyTextForNextScope(Parser, Memory);
  }
  return Result;
}

bonsai_function void
ParseStructMemberOperatorFn(parse_context *Ctx, struct_member *Result, c_token *StructNameT)
{
}

bonsai_function void
ParseStructMemberConstructorFn(parse_context *Ctx, type_spec *DeclType, struct_member *Result, c_token *StructNameT)
{
  parser *Parser = Ctx->CurrentParser;

  c_token *FuncNameT = DeclType->DatatypeToken;

  if (IsConstructorOrDestructorName(StructNameT, FuncNameT))
  {
    Result->Type = type_function_decl;
    type_spec ReturnType = {};
    Result->function_decl = ParseFunctionParameterList(Ctx, &ReturnType, FuncNameT, function_type_constructor, StructNameT);

    if (OptionalToken(Parser, CTokenType_Colon) )
    {
      while (OptionalToken(Parser, CTokenType_Identifier))
      {
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        OptionalToken(Parser, CTokenType_Comma);
      }
    }

    MaybeParseDeleteOrDefault(Parser, &Result->function_decl);
    MaybeParseAttributes(Parser);
    Result->function_decl.Body = MaybeParseFunctionBody(Parser, Ctx->Memory);

    if (IsImplementation(&Result->function_decl) ||
        IsDefaultImplementation(&Result->function_decl) ||
        IsDeletedImplementation(&Result->function_decl))
    {
      /* Push(&Ctx->Datatypes.Functions, Result->function_decl, Ctx->Memory); */
    }
    else
    {
      ParseError(Parser,
          CSz("Constructor function must have a body"),
          FuncNameT);
    }
  }
  else
  {
    ParseError(Parser,
        FormatCountedString(TranArena, CSz("Constructor (%S) name must match the struct name (%S)"), FuncNameT->Value, StructNameT->Value),
        FuncNameT );
  }
}

bonsai_function void
ParseStructMemberDestructorFn(parse_context *Ctx, struct_member *Result, c_token *StructNameT)
{
  Assert(Result->Type == type_struct_member_noop);

  parser *Parser = Ctx->CurrentParser;
  RequireToken(Parser, CTokenType_Tilde);

  c_token *FuncNameT = RequireTokenPointer(Parser, CTokenType_Identifier);
  type_spec ReturnType = {};

  if (IsConstructorOrDestructorName(StructNameT, FuncNameT))
  {
    c_token *GotOpen = OptionalToken(Parser, CTokenType_OpenParen);
    c_token *GotClose = OptionalToken(Parser, CTokenType_CloseParen);
    if ( GotOpen && GotClose )
    {
    }
    else
    {
      ParseError(Parser, FormatCountedString(TranArena, CSz("Destructor (%S) must have an empty parameter list!"), FuncNameT->Value) );
    }
  }
  else
  {
    ParseError(Parser, FormatCountedString(TranArena, CSz("Destructor (%S) name must match the struct name (%S)"), FuncNameT->Value, StructNameT->Value ) );
  }

  Result->Type = type_function_decl;
  Result->function_decl.Type = function_type_destructor;

  MaybeParseDeleteOrDefault(Parser, &Result->function_decl);
  Result->function_decl.Body = MaybeParseFunctionBody(Parser, Ctx->Memory);
}


bonsai_function struct_member
ParseStructMember(parse_context *Ctx, c_token *StructNameT)
{
  TIMED_FUNCTION();
  parser *Parser = Ctx->CurrentParser;

  struct_member Result = {};

  c_token *T = PeekTokenPointer(Parser);
  if (T)
  {
    switch(T->Type)
    {
      case CTokenType_Tilde: // Destructor
      {
        ParseStructMemberDestructorFn(Ctx, &Result, StructNameT);
      } break;

      // NOTE(Jesse): We don't handle inline enum declarations atm
      case CTokenType_Enum:
      {
        RequireToken(Parser, T->Type);
        Result.Type = type_variable_decl;
        Result.variable_decl = ParseVariableDecl(Ctx);
      } break;

      case CTokenType_Union:
      case CTokenType_Struct:
      {
        RequireToken(Parser, T);

        c_token *TypeNameT = {};
        if (PeekToken(Parser).Type == CTokenType_Identifier)
        {
          Result.Type = type_variable_decl;

          if (PeekToken(Parser, 1).Type == CTokenType_OpenBrace)
          {
            TypeNameT = RequireTokenPointer(Parser, CTokenType_Identifier);
          }
          else
          {
            Result.variable_decl = ParseVariableDecl(Ctx);
            Result.variable_decl.Type.Indirection = ParseReferencesIndirectionAndPossibleFunctionPointerness(Parser);
          }
        }

        if (PeekToken(Parser).Type == CTokenType_OpenBrace)
        {
          if (TypeNameT)
          {
            Result.Type = type_struct_decl;
          }
          else
          {
            Result.Type = type_struct_member_anonymous;
            /* TypeName = T->Type == CTokenType_Struct ? CSz("anonymous_struct") :  CSz("anonymous_union"); */
          }

          Result.struct_member_anonymous.Body = ParseStructBody(Ctx, TypeNameT);

          if (c_token *NameT = OptionalToken(Ctx->CurrentParser, CTokenType_Identifier))
          {
            // TODO(Jesse, correctness): This is the actual member name.. what do we do with this?
            /* Result.struct_member_anonymous.Body.Type = NameT->Value; */

            if ( OptionalToken(Parser, CTokenType_OpenBracket) )
            {
              variable_decl Tmp = {};
              ParseExpression(Ctx, &Tmp.StaticBufferSize );
              RequireToken(Parser, CTokenType_CloseBracket);
            }

            Assert(PeekToken(Ctx->CurrentParser).Type == CTokenType_Semicolon ||
                   PeekToken(Ctx->CurrentParser).Type == CTokenType_Comma);
          }
        }

      } break;

      case CT_NameQualifier:
      {
        EatNameQualifiers(Parser);
      } [[fallthrough]];

      case CTokenType_OperatorKeyword:
      case CT_Keyword_Constexpr:
      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
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
        type_spec DeclType = ParseTypeSpecifier(Ctx, StructNameT);

        b32 IsConstructor = IsConstructorOrDestructorName(StructNameT, DeclType.DatatypeToken) &&
                            PeekToken(Parser).Type == CTokenType_OpenParen ;

        Assert( ((DeclType.Qualifier & TypeQual_Constructor)>0) == IsConstructor);

        if (DeclType.Qualifier & TypeQual_Constructor)
        {
          ParseStructMemberConstructorFn(Ctx, &DeclType, &Result, StructNameT);
        }
        else // operator, regular function, or variable decl
        {
          declaration Decl = ParseFunctionOrVariableDecl(Ctx, &DeclType);
          switch (Decl.Type)
          {
            case type_declaration_variable_decl:
            {
              Result.Type = type_variable_decl;
              Result.variable_decl = Decl.variable_decl;
            } break;

            case type_declaration_function_decl:
            {
              Result.Type = type_function_decl;
              Result.function_decl = Decl.function_decl;

              if (IsConstructorOrDestructorName(StructNameT, Result.function_decl.NameT))
              {
                if (OptionalToken(Parser, CTokenType_Colon)) // Member initializers
                {
                  InvalidCodePath();
#if 0
                  while (OptionalToken(Parser, CTokenType_Identifier))
                  {
                    EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
                    OptionalToken(Parser, CTokenType_Comma);
                  }
#endif
                }
              }

              if (PeekToken(Parser).Type == '{')
              {
                /* InvalidCodePath(); */
                Result.function_decl.Body = GetBodyTextForNextScope(Parser, Ctx->Memory);
                /* Push(&Ctx->Datatypes.Functions, Result.function_decl, Ctx->Memory); */
              }
            } break;

            case type_declaration_noop:
            {
              InvalidCodePath();
            } break;
          }

        }
      } break;

      case CTokenType_Semicolon:
      case CTokenType_CloseBrace:
      {
        // Done parsing struct member, or finished with the struct
      } break;

      InvalidDefaultWhileParsing(Parser,
          FormatCountedString(TranArena, CSz("Unexpected token encountered while parsing struct %S"), StructNameT->Value));
    }
  }

  return Result;
}

bonsai_function counted_string
ConcatTokensUntil(parser* Parser, c_token_type Close, memory_arena* Memory)
{
  NotImplemented;

  counted_string Result = {};
#if 0
  // TODO(Jesse  id: 225, tags: todos, easy): Rewrite with string_from_parser
  string_builder Builder = {};
  while (Remaining(Parser) && PeekTokenRaw(Parser).Type != Close)
  {
    Append(&Builder, PopTokenRaw(Parser).Value);
  }
  counted_string Result = Finalize(&Builder, Memory);
#endif
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
          if (StringsMatch(Member->variable_decl.Type.DatatypeToken->Value, MemberType))
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

struct comma_separated_decl
{
  c_token *NameT;
  type_indirection_info Indirection;
  ast_node *StaticBufferSize;
  ast_node *Value;
};

bonsai_function comma_separated_decl
ParseCommaSeperatedDecl(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;
  comma_separated_decl Result = {};
  Result.Indirection = ParseReferencesIndirectionAndPossibleFunctionPointerness(Parser);

  Result.NameT = RequireTokenPointer(Parser, CTokenType_Identifier);

  if ( OptionalToken(Parser, CTokenType_OpenBracket) )
  {
    ParseExpression(Ctx, &Result.StaticBufferSize );
    RequireToken(Parser, CTokenType_CloseBracket);
  }

  if ( OptionalToken(Parser, CTokenType_Equals) )
  {
    ParseExpression(Ctx, &Result.Value);
  }

  return Result;
}

bonsai_function void
EatAdditionalCommaSeperatedNames(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  while (OptionalToken(Parser, CTokenType_Comma))
  {
    ParseCommaSeperatedDecl(Ctx);
  }
}

bonsai_function void
EatAllVisibilityQualifiers(parser *Parser)
{
  c_token *T = PeekTokenPointer(Parser);
  while (T &&
              ( T->Type == CT_Keyword_Public ||
                T->Type == CT_Keyword_Private ||
                T->Type == CT_Keyword_Protected )
        )
  {
    RequireToken(Parser, T);
    RequireToken(Parser, CTokenType_Colon);
    T = PeekTokenPointer(Parser);
  }
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
ParseStructBody(parse_context *Ctx, c_token *StructNameT)
{
  TIMED_FUNCTION();
  parser *Parser = Ctx->CurrentParser;
  struct_def Result = StructDef(StructNameT); //, Parser->Filename);

  RequireToken(Parser, CTokenType_OpenBrace);

  for (;;)
  {
    EatAllVisibilityQualifiers(Parser);

    struct_member Declaration = ParseStructMember(Ctx, Result.Type);
    if (Declaration.Type == type_struct_member_noop)
    {
      break;
    }
    else
    {
      Push(&Result.Members, Declaration, Ctx->Memory);
    }

    //  Parse comma-separated definitions .. ie `struct fing { int foo, bar, baz; };`
    while (OptionalToken(Parser, CTokenType_Comma))
    {
      switch (Declaration.Type)
      {
        // NOTE(Jesse): This is weird, but we use the variable_decl type for
        // struct declarations..?
        case type_struct_decl:
        case type_variable_decl:
        {
          // NOTE(Jesse): Because we use variable_decl for structs we can't use
          // SafeAccess()
          // auto Var = SafeAccessObj(variable_decl, Declaration);

          auto Var =  Declaration.variable_decl;
          comma_separated_decl Decl = ParseCommaSeperatedDecl(Ctx);

          Var.Name = Decl.NameT->Value;
          Var.Type.Indirection = Decl.Indirection;
          Var.StaticBufferSize = Decl.StaticBufferSize;
          Var.Value = Decl.Value;
        } break;

        case type_function_decl:
        case type_struct_member_anonymous:
        {
          NotImplemented;
        } break;

        case type_struct_member_noop:
        {
          InvalidCodePath();
        } break;

      }
      Push(&Result.Members, Declaration, Ctx->Memory);
    }

    if (Declaration.Type == type_function_decl)
    {
      OptionalToken(Parser, CTokenType_Semicolon);
    }
    else
    {
      RequireToken(Parser, CTokenType_Semicolon);
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

bonsai_function void
ParseEnumBody(parse_context *Ctx, parser *Parser, enum_def *Enum, memory_arena *Memory)
{
  RequireToken(Parser, CTokenType_OpenBrace);

  b32 Done = False;
  while (!Done && TokensRemain(Parser))
  {
    enum_member Field = {};
    Field.Name = RequireToken(Parser, CTokenType_Identifier).Value;

    if (OptionalToken(Parser, CTokenType_Equals))
    {
      Field.Expr = ParseExpression(Ctx);
    }

    Push(&Enum->Members, Field, Ctx->Memory);

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

  return;
}

bonsai_function enum_def
ParseEnum(parse_context *Ctx)
{
  TIMED_FUNCTION();

  parser *Parser = Ctx->CurrentParser;
  program_datatypes *Datatypes = &Ctx->Datatypes;

  c_token *EnumNameT = OptionalToken(Parser, CTokenType_Identifier);
  counted_string EnumName = EnumNameT ? EnumNameT->Value : CSz("(anonymous enum)");

  enum_def Enum = {
    .Name = EnumName
  };

  ParseEnumBody(Ctx, Parser, &Enum, Ctx->Memory);

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
  parser *Parser = Ctx->CurrentParser;
  c_token TypeSpecifier = PopToken(Parser);

  switch(TypeSpecifier.Type)
  {
    case CTokenType_Enum:
    {
      enum_def Enum = ParseEnum(Ctx);
      Push(&Ctx->Datatypes.Enums, Enum, Ctx->Memory);
    } break;

    case CT_Keyword_Class:
    case CTokenType_Struct:
    {
      c_token *StructNameT = RequireTokenPointer(Parser, CTokenType_Identifier);
      if ( PeekToken(Parser).Type == CTokenType_OpenBrace )
      {
        struct_def S = ParseStructBody(Ctx, StructNameT);
        Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
      }
    } break;

    case CTokenType_Union:
    {
      c_token *UnionNameT = RequireTokenPointer(Parser, CTokenType_Identifier);
      struct_def S = ParseStructBody(Ctx, UnionNameT);
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
  parser *Parser = Ctx->CurrentParser;

  type_spec Type = ParseTypeSpecifier(Ctx);
  counted_string Alias = {};
  b32 IsFunction = Type.Indirection.IsFunctionPointer;

  if (Type.Indirection.IsFunctionPointer)
  {
    Alias = Type.Indirection.FunctionPointerTypeName;
  }
  else
  {
    Alias = RequireToken(Parser, CTokenType_Identifier).Value;

    if (PeekToken(Parser).Type == CTokenType_OpenParen)
    {
      // TODO(Jesse): This is pretty half-baked and probably should be represented
      // differently.  I just hacked it in here to get typedef'd funcs to parse.
      //
      // I guess we could just eat the function arguments and discard them like we
      // do for function pointers..
      IsFunction = True;

      EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
    }
  }

  EatAdditionalCommaSeperatedNames(Ctx);

  TryEatAttributes(Parser);

  if ( OptionalToken(Parser, CTokenType_OpenBracket) )
  {
    variable_decl Tmp = {};
    ParseExpression(Ctx, &Tmp.StaticBufferSize );
    RequireToken(Parser, CTokenType_CloseBracket);
  }

  RequireToken(Parser, CTokenType_Semicolon);

  type_def Typedef = {
    .Type = Type,
    .Alias = Alias,
    .IsFunction = IsFunction,
  };

  Push(&Ctx->Datatypes.Typedefs, Typedef, Ctx->Memory);
}

bonsai_function void
ParseTypedef(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  RequireToken(Parser, CTokenType_Typedef);

  // @optimize_call_advance_instead_of_being_dumb
  c_token *Peek = PeekTokenPointer(Parser);
  if ( Peek->Type == CTokenType_Struct ||
       Peek->Type == CTokenType_Union
     )
  {
    RequireToken(Parser, Peek);
    if (PeekToken(Parser).Type == CTokenType_OpenBrace)
    {
      struct_def S = ParseStructBody(Ctx, 0);

      comma_separated_decl Decl = ParseCommaSeperatedDecl(Ctx);
      S.Type = Decl.NameT;
      TryEatAttributes(Parser);

      EatAdditionalCommaSeperatedNames(Ctx);

      RequireToken(Parser, CTokenType_Semicolon);
      Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
    }
    else if ( PeekToken(Parser, 0).Type == CTokenType_Identifier &&
              PeekToken(Parser, 1).Type == CTokenType_OpenBrace )
    {
      comma_separated_decl Decl = ParseCommaSeperatedDecl(Ctx);
      struct_def S = ParseStructBody(Ctx, Decl.NameT);

      /* comma_separated_decl ActualName = */ ParseCommaSeperatedDecl(Ctx);
      EatAdditionalCommaSeperatedNames(Ctx);

      RequireToken(Parser, CTokenType_Semicolon);
      Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
    }
    else
    {
      ParseAndPushTypedef(Ctx);
    }
  }
  else if (Peek->Type == CTokenType_Enum)
  {
    RequireToken(Parser, *Peek);

    enum_def Enum = {};

    {
      c_token Name = PeekToken(Parser);
      if (Name.Type == CTokenType_Identifier)
      {
        RequireToken(Parser, Name);
        Enum.Name = Name.Value;
      }
    }

    ParseEnumBody(Ctx, Parser, &Enum, Ctx->Memory);

    {
      c_token Name = PeekToken(Parser);
      if (Name.Type == CTokenType_Identifier)
      {
        RequireToken(Parser, Name);
        Enum.Name = Name.Value;
      }
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
  TIMED_FUNCTION();

  function_decl *Result = {};
  ITERATE_OVER(Stream)
  {
    auto *Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->NameT->Value, Name))
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
ParseSingleStatement(parse_context *Ctx, ast_node_statement *Result)
{
  parser *Parser = Ctx->CurrentParser;

  b32 Done = False;
  while (!Done && TokensRemain(Parser))
  {
    const c_token T = PeekToken(Parser);
    switch (T.Type)
    {
      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        Result->LHS = ParseExpression(Ctx);
      } break;

      case CTokenType_Identifier:
      {
        Assert(!Result->LHS);
        Result->LHS = ParseExpression(Ctx);
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

      case CTokenType_Bang:
      case CTokenType_Ampersand:
      case CTokenType_Tilde:
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

  while ( TokensRemain(Ctx->CurrentParser) )
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
    if (Node->TypeSpec.DatatypeToken->Type == CTokenType_Identifier)
    {
      Node->Datatype = GetDatatypeByName(&Ctx->Datatypes, Node->TypeSpec.DatatypeToken->Value);
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

      InvalidDefaultWhileParsing(Ctx->CurrentParser, CSz("Invalid syntax following type-specifier."));
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
  parser *Parser = Ctx->CurrentParser;

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
  parser *Parser = Ctx->CurrentParser;

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

      case CTokenType_Struct:
      case CTokenType_Enum:
      case CTokenType_Union:
      {
        RequireToken(Parser, T.Type);
      } [[fallthrough]];

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
  parser *Parser = Ctx->CurrentParser;

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
ParseDatatypes(parse_context *Ctx, parser *Parser)
{
  TIMED_FUNCTION();

  Assert(Parser == Ctx->CurrentParser);

  program_datatypes* Datatypes = &Ctx->Datatypes;
  memory_arena* Memory = Ctx->Memory;

  while ( c_token *T = PeekTokenPointer(Parser) )
  {
    switch(T->Type)
    {
      case CT_KeywordPragma:
      case CTokenType_Meta:
      {
        RequireToken(Parser, T->Type);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CT_StaticAssert:
      {
        RequireToken(Parser, CT_StaticAssert);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        RequireToken(Parser, CTokenType_Semicolon);
        /* EraseBetweenExcluding(Result, T, Result->Tokens->At); */
      } break;


      case CTokenType_Semicolon:
      {
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CT_Keyword_Class:
      case CTokenType_Struct:
      case CTokenType_Enum:
      case CTokenType_Union:
      {
        if (PeekToken(Parser, 1).Type == CTokenType_OpenBrace ||
            PeekToken(Parser, 2).Type == CTokenType_OpenBrace)
        {
          ParseDatatypeDef(Ctx);
        }
        else
        {
          // NOTE(Jesse): Globally-scoped variable : `global_variable struct foo = { .bar = 1 }`
          ParseVariableDecl(Ctx);
        }
      } break;

      case CTokenType_Typedef:
      {
        ParseTypedef(Ctx);
      } break;

      case CTokenType_Using:
      {
        EatUntilIncluding(Parser, CTokenType_Semicolon);
      } break;

      case CT_NameQualifier:
      {
        EatNameQualifiers(Parser);
      } break;

      case CTokenType_OperatorKeyword:
      case CT_Keyword_Constexpr:
      case CTokenType_TemplateKeyword:
      case CTokenType_Extern:
      case CTokenType_Inline:
      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
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
        type_spec DeclType = ParseTypeSpecifier(Ctx);
        declaration Decl = ParseFunctionOrVariableDecl(Ctx, &DeclType);

        if (Decl.Type == type_declaration_function_decl)
        {
          Push(&Ctx->Datatypes.Functions, Decl.function_decl, Ctx->Memory); // Free function
        }

        EatAdditionalCommaSeperatedNames(Ctx);
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

  parser *Parser = Ctx->CurrentParser;

  if (Parser->ErrorCode)
  {
    Error(CSz("Parse Error Encountered, not flushing to disk."));
    return;
  }

  counted_string OutputPath = {};

  EatUntilIncluding(Parser, CTokenType_Newline);
  EatNBSP(Parser);

  b32 FoundValidInclude = False;
  c_token *T = PeekTokenRawPointer(Parser);
  if (T && T->Type == CT_PreprocessorInclude)
  {
    counted_string IncludePath = RequireTokenRaw(Parser, CT_PreprocessorInclude).IncludePath;
    OutputPath = Concat(Ctx->Args->Outpath, Basename(IncludePath), Memory);
    FoundValidInclude = True;
  }

  if (FoundValidInclude == False)
  {
    ParseError(Parser, CSz("A meta() tag must be followed directly by an include tag on the next line."), T);

    // NOTE(Jesse): This path is for inserting the requisite includes when we
    // re-enable inplace editing
    /* NotImplemented; */

    /* Assert(PeekTokenRaw(Parser).Type == CTokenType_Newline); */
    /* OutputPath = Concat(Ctx->Args->Outpath, NewFilename, Memory); */
  }
  else
  {
    Output(OutputForThisParser, OutputPath, Memory);
    parser *OutputParse = ParserForAnsiStream(Ctx, AnsiStream(OutputForThisParser, OutputPath), TokenCursorSource_MetaprogrammingExpansion);

    if (IsInlineCode)
    {
      // TODO(Jesse, id: 226, tags: metaprogramming, output): Should we handle this differently?
      Warn("Not parsing datatypes in inlined code for %S", OutputPath);
    }
    else
    {
      // NOTE(Jesse): This is horribly tortured.. remove Ctx->CurrentParser..
      // it's completely unnecessary cruft.
      parser *OldParser = Ctx->CurrentParser;
      Ctx->CurrentParser = OutputParse;
      RunPreprocessor(Ctx, OutputParse, Memory);
      ParseDatatypes(Ctx, OutputParse);
      Ctx->CurrentParser = OldParser;
    }
  }
  /* PushParser(Ctx->CurrentParser, OutputParse, parser_push_type_include); */
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
BootstrapDebugSystem()
{
  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  /* GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, DebugLibName_GetDebugState); */
  /* if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; } */

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat);
  if (!WindowSuccess) { Error("Initializing Window :( "); return False; }
  Assert(Os.Window);

  InitializeOpengl(&Os);

  init_debug_system_proc InitDebugSystem = (init_debug_system_proc)GetProcFromLib(DebugLib, DebugLibName_InitDebugSystem);
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }

  GetDebugState = InitDebugSystem(&GL);

  debug_state* DebugState = GetDebugState();
  DebugState->DebugDoScopeProfiling = True;
  DebugState->Plat = &Plat;

  GL.ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  GL.ClearDepth(1.0f);

  DebugState->ClearFramebuffers();

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
  TIMED_FUNCTION();

  person_stream People = {};

  parser *Parser = ParserForFile(0, Filename, TokenCursorSource_Unknown);

  if (Parser)
  {
    while (TokensRemain(Parser))
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
  }

  return People;
}

bonsai_function counted_string
GenerateOutfileNameFor(counted_string Name, counted_string DatatypeName, memory_arena* Memory, counted_string Modifier = {})
{
  string_builder OutfileBuilder = {};
  Append(&OutfileBuilder, CSz("/"));
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
      Result = Decl->function_decl.NameT->Value;
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
ApplyTransformations(meta_transform_op Transformations, counted_string Input, memory_arena* Memory)
{
  counted_string Result = Input;
  {
    if ( Transformations & to_capital_case )
    {
      UnsetBitfield(meta_transform_op, Transformations, to_capital_case );
      Result = ToCapitalCase(Result, Memory);
    }

    if ( Transformations & to_lowercase )
    {
      UnsetBitfield(meta_transform_op, Transformations, to_lowercase );
      Result = ToLowerCase(Result, Memory);
    }

    if ( Transformations & strip_single_prefix )
    {
      UnsetBitfield(meta_transform_op, Transformations, strip_single_prefix );
      Result = StripPrefix(Result, Memory, 0);
    }

    if ( Transformations & strip_all_prefix )
    {
      UnsetBitfield(meta_transform_op, Transformations, strip_all_prefix );
      Result = StripPrefix(Result, Memory, u64_MAX);
    }
  }

  Assert(Transformations == meta_transform_op_noop);

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
      Error("Parsing meta_transform_ops.");
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

#include <poof/print_ast_node.h>

bonsai_function counted_string
PrintType(type_spec *Type, memory_arena *Memory)
{
  counted_string Result = {};
  string_builder Builder = {};

  if (Type->Datatype.Type)
  {
    Result = Type->DatatypeToken->Value;
  }
  else
  {
    meta
    (
      func (type_qualifier Enum)
      {
        (Enum.map_values(EnumVal)
        {
          if (Type->Qualifier & (EnumVal.name))
          {
            Append(&Builder, CSz("(EnumVal.name.strip_all_prefix.to_lowercase) "));
          }
        })
      }
    )
#include <poof/generated/anonymous_function_type_qualifier_fPa9h41Z.h>
  }

  if (Result.Count == 0)
  {
    Result = Finalize(&Builder, Memory);
  }

  return Result;
}

// TODO(Jesse id: 222, tags: immediate, parsing, metaprogramming) : Re-add [[nodiscard]] here
bonsai_function counted_string
Execute(counted_string FuncName, parser Scope, meta_func_arg_stream* ReplacePatterns, parse_context* Ctx, memory_arena* Memory)
{
  TIMED_FUNCTION();

  program_datatypes* Datatypes = &Ctx->Datatypes;
  meta_func_stream* FunctionDefs = &Ctx->MetaFunctions;

  Assert(Scope.Tokens->At == Scope.Tokens->Start);

  string_builder OutputBuilder = {};
  while ( c_token *BodyToken = PopTokenRawPointer(&Scope) )
  {
    if ( BodyToken->Type == CTokenType_StringLiteral )
    {
      counted_string TempStr = BodyToken->Value;

      TempStr = StripQuotes(TempStr);

      parser *StringParse = ParserForAnsiStream(Ctx, AnsiStream(TempStr), TokenCursorSource_MetaprogrammingExpansion);
      counted_string Code = Execute(FuncName, *StringParse, ReplacePatterns, Ctx, Memory);

      Append(&OutputBuilder, CSz("\""));
      Append(&OutputBuilder, EscapeDoubleQuotes(Code, OutputBuilder.Memory));
      Append(&OutputBuilder, CSz("\""));

/*       AppendAndEscapeInteriorOfDoubleQuotedString(&OutputBuilder, Code); */
    }
    else if ( BodyToken->Type == CTokenType_OpenParen )
    {
      b32 ExecutedChildFunc = False;
      ITERATE_OVER_AS(Replace, ReplacePatterns)
      {
        meta_func_arg* Replace = GET_ELEMENT(ReplaceIter);
        if ( OptionalToken(&Scope, CToken(Replace->Match)) )
        {
          ExecutedChildFunc = True;
          RequireToken(&Scope, CTokenType_Dot);

          // NOTE(Jesse): RequireTokenPointer
          c_token *MetaOperatorToken = PeekTokenPointer(&Scope);
          RequireToken(&Scope, CTokenType_Identifier);

          meta_arg_operator Operator = MetaArgOperator( MetaOperatorToken->Value );

          switch (Operator)
          {
            case meta_arg_operator_noop:
            {
              ParseError(&Scope, CSz("Invalid operator encountered."));
            } break;

            case is_struct:
            {
              RequireToken(&Scope, CTokenType_Question);
              parser StructScope = GetBodyTextForNextScope(&Scope, Memory);
              if (Replace->Data.Type == type_struct_def)
              {
                counted_string Code = Execute(FuncName, StructScope, ReplacePatterns, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            } break;

            case is_enum:
            {
              RequireToken(&Scope, CTokenType_Question);
              parser EnumScope = GetBodyTextForNextScope(&Scope, Memory);
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
                      enum_def *E = GetEnumByType(&Datatypes->Enums, Replace->Data.struct_member->variable_decl.Type.DatatypeToken->Value);
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
                    case type_struct_decl:
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
              switch (Replace->Data.Type )
              {
                case type_datatype_noop:
                {
                  InvalidCodePath();
                } break;

                case type_struct_def:
                case type_enum_def:
                case type_type_def:
                case type_primitive_def:
                case type_stl_container_def:
                {
                  NotImplemented;
                } break;

                case type_enum_member:
                {
                  enum_member *Datatype = SafeAccessObj(enum_member, Replace->Data);

                  string_builder Builder = {};
                  PrintAstNode(Datatype->Expr->Value, &Builder);

                  counted_string Value = Finalize(&Builder, Memory);
                  Append(&OutputBuilder, Value);
                } break;


                case type_struct_member:
                {
                  struct_member *Member = SafeAccessObj(struct_member, Replace->Data);
                  switch (Member->Type)
                  {
                    case type_struct_member_noop:
                    {
                      InvalidCodePath();
                    } break;

                    case type_struct_decl:
                    case type_function_decl:
                    case type_struct_member_anonymous:
                    {
                      NotImplemented;
                    } break;

                    case type_variable_decl:
                    {
                      variable_decl *Var = SafeAccess(variable_decl, Member);
                      counted_string Value = PrintAstNode(Var->Value, Memory);
                      Append(&OutputBuilder, Value);
                    } break;
                  }

                  auto Datatype = SafeAccessObj(struct_member, Replace->Data);

#if 0
                  string_builder Builder = {};
                  PrintAstNode(Datatype->Expr->Value, &Builder);

                  counted_string Value = Finalize(&Builder, Memory);
                  Append(&OutputBuilder, Value);
#endif

                } break;
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
                  TypeName = Replace->Data.struct_def->Type->Value;
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
                  auto Member = SafeAccessObj(struct_member, Replace->Data);
                  switch (Member->Type)
                  {
                    case type_struct_member_noop: { InvalidCodePath(); } break;

                    case type_variable_decl:
                    {
                      auto Decl = SafeAccess(variable_decl, Member);
                      TypeName = PrintType(&Decl->Type, Memory);
                    } break;

                    case type_function_decl:
                    {
                      TypeName = Replace->Data.struct_member->function_decl.NameT->Value;
                    } break;

                    case type_struct_decl:
                    {
                      TypeName = CSz("TODO(Jesse): Preserve the struct name here.");
                    } break;

                    case type_struct_member_anonymous:
                    {
                      TypeName = CSz("(anonymous struct/union)");
                    } break;
                  }

                } break;

              }

              counted_string Name = ApplyTransformations(Transformations, TypeName, Memory);
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
                  counted_string Name = ApplyTransformations(Transformations, MemberName, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_enum_member:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = ApplyTransformations(Transformations, Replace->Data.enum_member->Name, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_enum_def:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = ApplyTransformations(Transformations, Replace->Data.enum_def->Name, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_struct_def:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = ApplyTransformations(Transformations, Replace->Data.struct_def->Type->Value, Memory);
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

              parser MapMemberScope = GetBodyTextForNextScope(&Scope, Memory);
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
                           !StringsMatch(Member->variable_decl.Type.DatatypeToken->Value, ContainingConstraint) )
                      {
                        // Containing constraint failed
                      }
                      else
                      {
                        meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                        Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Member)), Memory);
                        Rewind(MapMemberScope.Tokens);
                        counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, Ctx, Memory);
                        Append(&OutputBuilder, StructFieldOutput);
                      }

                    } break;

                    case type_struct_decl:
                    case type_struct_member_anonymous:
                    {
                      for (struct_member_iterator UnionMemberIter = Iterator(&Member->struct_member_anonymous.Body.Members);
                          IsValid(&UnionMemberIter);
                          Advance(&UnionMemberIter))
                      {
                        struct_member* UnionMember = GET_ELEMENT(UnionMemberIter);
                        if (UnionMember->Type == type_variable_decl)
                        {
                          struct_def* Struct = GetStructByType(&Datatypes->Structs, UnionMember->variable_decl.Type.DatatypeToken->Value);
                          if (Struct)
                          {
                            struct_member_stream ContainedMembers = MembersOfType(Struct, ContainingConstraint, Memory);
                            if (ContainedMembers.FirstChunk)
                            {
                              meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                              Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Struct)), Memory);
                              if (ChildName.Count) {
                                struct_def SyntheticStruct = {
                                  // TODO(Jesse): If this crashes .. sorry.  What should we actually do here?
                                  .Type = 0, // CToken(CSz("synthetically_created_struct")),
                                  .DefinedInFile = Struct->DefinedInFile,
                                  .Members = ContainedMembers,
                                };
                                Push(&NewArgs, ReplacementPattern(ChildName, Datatype(&SyntheticStruct)), Memory);
                              }
                              Rewind(MapMemberScope.Tokens);
                              counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, Ctx, Memory);
                              Append(&OutputBuilder, StructFieldOutput);
                            }
                          }
                          else
                          {
                            counted_string Name = UnionMember->variable_decl.Type.DatatypeToken->Value;
                            counted_string ParentStructName = Replace->Data.struct_def->Type->Value;
                            Warn("Couldn't find struct type '%S' in union parent '%S'.", Name, ParentStructName);
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
                Assert(Replace->Data.Type);
                Error("Called map_members on a datatype that wasn't a struct");
              }

            } break;

            case map_values:
            {
              RequireToken(&Scope, CTokenType_OpenParen);
              counted_string EnumValueMatch  = RequireToken(&Scope, CTokenType_Identifier).Value;
              RequireToken(&Scope, CTokenType_CloseParen);
              parser NextScope = GetBodyTextForNextScope(&Scope, Memory);

              if (Replace->Data.Type == type_enum_def)
              {
                ITERATE_OVER(&Replace->Data.enum_def->Members)
                {
                  enum_member* EnumMember = GET_ELEMENT(Iter);
                  meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                  Push(&NewArgs, ReplacementPattern(EnumValueMatch, Datatype(EnumMember)), Memory);
                  Rewind(NextScope.Tokens);
                  counted_string EnumFieldOutput = Execute(FuncName, NextScope, &NewArgs, Ctx, Memory);
                  Append(&OutputBuilder, EnumFieldOutput);
                  continue;
                }

              }
              else
              {
                Error("Called map_values on a datatype that wasn't an enum - %S", Replace->Data.struct_def->Type);
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
        Append(&OutputBuilder, BodyToken->Value);
      }
    }
    else
    {
      Append(&OutputBuilder, BodyToken->Value);
    }

    continue;
  }

  counted_string Result = Finalize(&OutputBuilder, Memory);
  return Result;
}

bonsai_function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, parse_context* Ctx, memory_arena* Memory)
{
  Assert(Func->Body.Tokens->At == Func->Body.Tokens->Start);

  counted_string Result = Execute(Func->Name, Func->Body, Args, Ctx, Memory);

  Assert(Func->Body.Tokens->At == Func->Body.Tokens->End);
  Rewind(Func->Body.Tokens);
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
      Warn("Type (%S) could not be resolved to a struct or named_list, ignoring it.", DatatypeName);
    }

    OptionalToken(Parser, CTokenType_Comma);
  }

  return Result;
}

bonsai_function meta_func
ParseMetaFunctionDef(parser* Parser, counted_string FuncName, memory_arena *Memory)
{
  TIMED_FUNCTION();

  RequireToken(Parser, CTokenType_OpenParen);
  counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
  RequireToken(Parser, CTokenType_CloseParen);
  parser Body = GetBodyTextForNextScope(Parser, Memory);

  meta_func Func = {
    .Name = FuncName,
    .ArgName = ArgName,
    .Body = Body,
  };

  return Func;
}

// TODO(Jesse): Rewrite this so it doesn't suck
//
// Or maybe just delete it?
#if 0
bonsai_function void
RemoveAllMetaprogrammingOutputRecursive(const char * OutputPath)
{

  WIN32_FIND_DATA fdFile;
  HANDLE hFind = NULL;

  char sPath[2048];

  //Specify a file mask. *.* = We want everything!
  sprintf(sPath, "%s\\*.*", OutputPath);

  if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
  {
      printf("Path not found: [%s]\n", OutputPath);
  }

  do
  {
      //Find first file will always return "."
      //    and ".." as the first two directories.
      if(strcmp(fdFile.cFileName, ".") != 0
              && strcmp(fdFile.cFileName, "..") != 0)
      {
          //Build up our file path using the passed in
          //  [OutputPath] and the file/foldername we just found:
          sprintf(sPath, "%s\\%s", OutputPath, fdFile.cFileName);

          //Is the entity a File or Folder?
          if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
          {
              printf("Descending into Directory: %s\n", sPath);
              RemoveAllMetaprogrammingOutputRecursive(sPath);
          }
          else
          {
              printf("Removeing File: %s\n", sPath);
              Ensure(Remove(CS(sPath)));
          }
      }
  }
  while(FindNextFile(hFind, &fdFile));

  FindClose(hFind);

#if 0
  for (u32 ParserIndex = 0;
      ParserIndex < Count(ParsedFiles);
      ++ParserIndex)
  {
    parser* Parser = ParsedFiles->Start+ParserIndex;
    if (IsMetaprogrammingOutput(Parser->Filename, Args->Outpath))
    {
      Info("Removing %S", Parser->Filename);
      Remove(Parser->Filename);
      continue;
    }
  }
#endif

}
#endif

#if 0
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
#endif

bonsai_function void
GoGoGadgetMetaprogramming(parse_context* Ctx, todo_list_info* TodoInfo)
{
  TIMED_FUNCTION();

  program_datatypes *Datatypes   = &Ctx->Datatypes;
  meta_func_stream *FunctionDefs = &Ctx->MetaFunctions;
  memory_arena *Memory           = Ctx->Memory;

  person_stream* People = &TodoInfo->People;
  tagged_counted_string_stream_stream* NameLists = &TodoInfo->NameLists;

  parser *Parser = Ctx->CurrentParser;
  while (c_token *T = PeekTokenPointer(Parser))
  {
    switch( T->Type )
    {
      case CTokenType_CommentMultiLine:
      case CTokenType_CommentSingleLine:
      {
        Assert(False);
#if 1
        c_token CommentToken = RequireTokenRaw(Parser, T->Type);
        /* EatComment(Parser, CommentStartToken.Type); */
#else

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
              /* Push(CToken(CS(" id: ")), &Parser->OutputTokens); */
              /* Push(CToken(IdValue),     &Parser->OutputTokens); */
            }

            OptionalToken(Parser, CTokenType_Comma);

            if (StringsMatch(PeekToken(Parser).Value, CSz("tags")))
            {
              if (GeneratedNewId)
              {
                /* Push(CToken(CTokenType_Comma), &Parser->OutputTokens); */
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

            counted_string TodoValue = {};
            if ( T.Type == CTokenType_CommentSingleLine )
            {
              string_from_parser Builder = StartStringFromParser(Parser);
                EatUntilIncluding(Parser, CTokenType_Newline);
              TodoValue = FinalizeStringFromParser(&Builder);

              TodoValue = Trim(TodoValue);
            }
            else
            {
              TodoValue = ParseMultiLineTodoValue(Parser, Memory);
            }

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
#endif

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
              c_token *ArgTypeToken = PeekTokenPointer(Parser);

              counted_string ArgType = RequireToken(Parser, CTokenType_Identifier).Value;
              counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
              RequireToken(Parser, CTokenType_CloseParen);

              parser Body = GetBodyTextForNextScope(Parser, Memory);

              meta_func Func = {
                .Name = CSz("anonymous_function"),
                .ArgName = ArgName,
                .Body = Body,
              };

              datatype Arg = GetDatatypeByName(&Ctx->Datatypes, ArgType);

              if (Arg.Type)
              {
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(ArgName, Arg), Memory);
                counted_string Code = Execute(&Func, &Args, Ctx, Memory);

                RequireToken(Parser, CTokenType_CloseParen);
                while(OptionalToken(Parser, CTokenType_Semicolon));
                if (Code.Count)
                {
                  counted_string OutfileName = GenerateOutfileNameFor(Func.Name, ArgType, Memory, GetRandomString(8, Hash(&Code), Memory));
                  FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory, True);
                }
                else
                {
                  Warn("Unable to generate code for meta_func %S", Func.Name);
                }
              }
              else
              {
                ParseError(Parser,
                    ParseErrorCode_UndefinedDatatype,
                    FormatCountedString(TranArena, CSz("Unable to find definition for datatype (%S)"), ArgTypeToken->Value),
                    ArgTypeToken);
              }
            }
            else
            {
              counted_string FuncName = RequireToken(Parser, CTokenType_Identifier).Value;
              meta_func Func = ParseMetaFunctionDef(Parser, FuncName, Memory);
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
            meta_func StructFunc = ParseMetaFunctionDef(Parser, CSz("for_datatypes_struct_callback"), Memory);

            RequireToken(Parser, CToken(ToString(func)));
            meta_func EnumFunc = ParseMetaFunctionDef(Parser, CSz("for_datatypes_enum_callback"), Memory);

            RequireToken(Parser, CTokenType_CloseParen);

            string_builder OutputBuilder = {};

            for (struct_def_iterator Iter = Iterator(&Datatypes->Structs);
                IsValid(&Iter);
                Advance(&Iter))
            {
              struct_def* Struct = &Iter.At->Element;

              if (!StreamContains(&Excludes, Struct->Type->Value))
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
            if (Parser->ErrorCode == ParseErrorCode_None)
            {
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
              while(OptionalToken(Parser, CTokenType_Semicolon));

              counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), DatatypeName, Memory);
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

              /* Info("Calling bonsai_function : %S(%S)", Func->Name, DatatypeName); */
              datatype Arg = GetDatatypeByName(&Ctx->Datatypes, DatatypeName);
              meta_func_arg_stream Args = {};
              Push(&Args, ReplacementPattern(Func->ArgName, Arg), Memory);
              counted_string Code = Execute(Func, &Args, Ctx, Memory);
              while(OptionalToken(Parser, CTokenType_Semicolon));

              if (Code.Count)
              {
                counted_string OutfileName = GenerateOutfileNameFor(Func->Name, DatatypeName, Memory);
                FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);
              }
              else
              {
                Warn("Unable to generate code for meta_func %S", Func->Name);
              }
            }
            else
            {
              ParseError(Parser, FormatCountedString(TranArena, CSz("Couldn't resolve %S to a metaprogramming directive or bonsai_function name"), DirectiveString));
            }
          }

        }
      } break;

      default:
      {
        PopToken(Parser);
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

  AllWritesSucceeded &= CloseFile(&TempFile);

  if (AllWritesSucceeded)
  {
    Rename(TempFile.Path, CSz("todos.md"));
  }

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
main(s32 ArgCount_, const char** ArgStrings)
{
  memory_arena Memory_ = {};
  memory_arena* Memory = &Memory_;

  Memory->NextBlockSize = Gigabytes(2);

  Assert(ArgCount_ > 0);
  u32 ArgCount = (u32)ArgCount_;

  parse_context Ctx = AllocateParseContext(Memory);

  SetupStdout(ArgCount, ArgStrings);

  arguments Args = ParseArgs(ArgStrings, ArgCount, &Ctx, Memory);
  Ctx.Args = &Args;

  TryCreateDirectory(TMP_DIR_ROOT);
  TryCreateDirectory(Args.Outpath);

  if (Args.DoDebugWindow)
  {
    if (BootstrapDebugSystem() == 1)
    {
      GetDebugState()->MainThreadAdvanceDebugSystem(0);
      DEBUG_REGISTER_ARENA(Memory);
      DEBUG_REGISTER_ARENA(&Global_PermMemory);
    }
    else
    {
      Error("Booting debug system");
      return FAILURE_EXIT_CODE;
    }
  }

  b32 Success = True;
  if (ArgCount > 1)
  {
    // TODO(Jesse): Make ParseArgs operate on the parse context directly?
    Ctx.IncludePaths = &Args.IncludePaths;

    todo_list_info TodoInfo = {};

    /* todo_list_info TodoInfo = { */
    /*   .People = ParseAllTodosFromFile(CSz("todos.md"), Memory), */
    /* }; */

    RegisterUnparsedCxxTypes(&Ctx.Datatypes, Memory);

    Assert(TotalElements(&Args.Files) == 1);
    Assert(Args.Files.Start == Args.Files.At);
    counted_string ParserFilename = Args.Files.Start[0];

    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);

    MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(0.0);

    if (Parser->ErrorCode == ParseErrorCode_None)
    {
      /* RemoveAllMetaprogrammingOutputRecursive(GetNullTerminated(Args.Outpath)); */

      Ctx.CurrentParser = Parser;
      ParseDatatypes(&Ctx, Parser);

      MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(0);

      FullRewind(Ctx.CurrentParser);

      MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(0);
      GoGoGadgetMetaprogramming(&Ctx, &TodoInfo);

/*       if (Parser->Valid) */
/*       { */
/*         if (Parser->OutputTokens->At != Parser->OutputTokens->Start) */
/*         { */
/*           TruncateToCurrentElements(&Parser->OutputTokens); */
/*           Output(Parser->OutputTokens, Parser->Filename, Memory); */
/*           LogSuccess("Output '%S'.", Parser->Filename); */
/*         } */
/*         else */
/*         { */
/*           Error("Tried to output an OutputTokens stream of 0 length!"); */
/*         } */
/*       } */

    }
    else
    {
      Error("Tokenizing File: %S", ParserFilename);
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

    /* WriteTodosToFile(&TodoInfo.People, Memory); */

  }
  else
  {
    Warn("No files passed, exiting.");
  }


  // BootstrapDebugSystem is behind a flag, or it could have failed.
  if (GetDebugState)
  {
    debug_state *DebugState = GetDebugState();
    DebugState->UIType = DebugUIType_Memory;
    DebugState->DisplayDebugMenu = True;
    DebugState->DebugDoScopeProfiling = False;

    DebugState->MainThreadAdvanceDebugSystem(0);

    hotkeys Hotkeys = {};
    while (Os.ContinueRunning)
    {
      ClearClickedFlags(&Plat.Input);

      v2 LastMouseP = Plat.MouseP;
      while ( ProcessOsMessages(&Os, &Plat) );
      Plat.MouseDP = LastMouseP - Plat.MouseP;

      BindHotkeysToInput(&Hotkeys, &Plat.Input);

      DebugState->OpenDebugWindowAndLetUsDoStuff();
      BonsaiSwapBuffers(&Os);

      DebugState->ClearFramebuffers();

      GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
      GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      /* Ensure(RewindArena(TranArena)); */
    }
  }

  TryDeleteDirectory(TMP_DIR_ROOT);

  s32 Result = !Success; // ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
#endif
