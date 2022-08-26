
enum d_union_flags
{
  d_union_flag_none,
  d_union_flag_enum_only
};

enum metaprogramming_directive
{
  meta_directive_noop,
  enum_only,
  d_union,
  named_list,
  for_datatypes,

  func,
  polymorphic_func,
};
meta( string_and_value_tables(metaprogramming_directive) )
#include <poof/generated/string_and_value_tables_metaprogramming_directive.h>

enum meta_arg_operator
{
  meta_arg_operator_noop,

  name,
  type,
  value,
  map_values,
  map_members,
  is_enum,
  is_struct,
  is_union,
  is_defined,
  is_compound,
};
meta( generate_value_table(meta_arg_operator) )
#include <poof/generated/generate_value_table_meta_arg_operator.h>

enum meta_transform_op
{
  meta_transform_op_noop,

  to_capital_case     = (1 << 0),
  to_lowercase        = (1 << 1),
  strip_single_prefix = (1 << 2),
  strip_all_prefix    = (1 << 3), // TODO(Jesse): Change name to strip_all_prefixes
};
meta(generate_value_table(meta_transform_op))
#include <poof/generated/generate_value_table_meta_transform_op.h>






enum c_token_type
{
  CTokenType_Unknown = 0,

  CT_ControlChar_Start_of_Heading          = 1,
  CT_ControlChar_Start_of_Text             = 2,
  CT_ControlChar_End_of_Text               = 3,
  CT_ControlChar_End_of_Transmission       = 4,
  CT_ControlChar_Enquiry                   = 5,
  CT_ControlChar_Acknowledgement           = 6,
  CT_ControlChar_Bell                      = 7,
  CT_ControlChar_Backspace                 = 8,
  /* CT_ControlChar_Horizontal_Tab            = 9, */
  /* CT_ControlChar_Line_Feed                 = 10, */
  CT_ControlChar_Vertical_Tab              = 11,
  CT_ControlChar_Form_Feed                 = 12,
  /* CT_ControlChar_Carriage_Return           = 13, */
  CT_ControlChar_Shift_Out                 = 14,
  CT_ControlChar_Shift_In                  = 15,
  CT_ControlChar_Data_Link_Escape          = 16,
  CT_ControlChar_Device_Control_1          = 17,
  CT_ControlChar_Device_Control_2          = 18,
  CT_ControlChar_Device_Control_3          = 19,
  CT_ControlChar_Device_Control_4          = 20,
  CT_ControlChar_Negative_Acknowledgement  = 21,
  CT_ControlChar_Synchronous_Idle          = 22,
  CT_ControlChar_End_of_Transmission_Block = 23,
  CT_ControlChar_Cancel                    = 24,
  CT_ControlChar_End_of_Medium             = 25,
  CT_ControlChar_Substitute                = 26,
  CT_ControlChar_Escape                    = 27,
  CT_ControlChar_File_Separator            = 28,
  CT_ControlChar_Group_Separator           = 29,
  CT_ControlChar_Record_Separator          = 30,
  CT_ControlChar_Unit_Separator            = 31,
  CT_ControlChar_Delete                    = 127,

  CTokenType_OpenBracket   = '[',
  CTokenType_CloseBracket  = ']',
  CTokenType_OpenBrace     = '{',
  CTokenType_CloseBrace    = '}',
  CTokenType_OpenParen     = '(',
  CTokenType_CloseParen    = ')',
  CTokenType_Dot           = '.',
  CTokenType_Comma         = ',',
  CTokenType_Semicolon     = ';',
  CTokenType_Colon         = ':',
  CTokenType_Hash          = '#',
  CTokenType_At            = '@',
  CTokenType_Dollar        = '$',
  CTokenType_Space         = ' ',
  CTokenType_Tab           = '\t',
  CTokenType_Star          = '*',
  CTokenType_Ampersand     = '&', // TODO(Jesse id: 238, tags: immediate, cleanup): Change name to BitwiseAnd
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
  CTokenType_Pipe          = '|', // TODO(Jesse id: 239, tags: immediate, cleanup): Change name to BitwiseOr
  CTokenType_Newline       = '\n',
  CTokenType_CarrigeReturn = '\r',
  CTokenType_EOF           = EOF,


  CTokenType_CommentSingleLine     = 256, // Making the assumption we'll never parse anything that's not ascii, so start the non-ascii tokens at 256
  CTokenType_CommentMultiLine,

  CTokenType_EscapedNewline,

  CTokenType_Identifier,

  CTokenType_StringLiteral,
  CTokenType_CharLiteral,
  CTokenType_IntLiteral,
  CTokenType_DoubleLiteral,
  CTokenType_FloatLiteral,

  CTokenType_Meta,

  CT_Keyword_Override,
  CT_Keyword_Virtual,
  CT_Keyword_Noexcept,
  CT_Keyword_Explicit,
  CT_Keyword_Constexpr,
  CT_Keyword_Namespace,
  CT_Keyword_Class,
  CTokenType_Struct,
  CTokenType_Enum,
  CTokenType_Union,
  CTokenType_Typedef,

  CTokenType_Using,

  CTokenType_ThreadLocal,
  CTokenType_Const,
  CTokenType_Static,
  CTokenType_Volatile,
  CTokenType_Long,
  CTokenType_Unsigned,
  CTokenType_Signed,

  CTokenType_Bool,
  CTokenType_Auto,
  CTokenType_Void,
  CTokenType_Double,
  CTokenType_Float,
  CTokenType_Char,
  CTokenType_Int,
  CTokenType_Short,

  CTokenType_Inline,
  CTokenType_TemplateKeyword,
  CTokenType_OperatorKeyword,

  CT_Keyword_Public,
  CT_Keyword_Private,
  CT_Keyword_Protected,

  CT_KeywordPragma,
  CT_KeywordAttribute,

  CT_StaticAssert,

  CTokenType_Extern,
  CTokenType_Asm,
  CTokenType_Goto,

  CTokenType_Ellipsis,

  CTokenType_If,
  CTokenType_Else,
  CTokenType_Break,
  CTokenType_Switch,
  CTokenType_Case,
  CT_Keyword_Delete,
  CTokenType_Default,
  CTokenType_For,
  CTokenType_While,
  CTokenType_Continue,
  CTokenType_Return,

  CTokenType_LeftShift,
  CTokenType_RightShift,

  CTokenType_LessEqual,
  CTokenType_GreaterEqual,
  CTokenType_AreEqual,
  CTokenType_NotEqual,

  CTokenType_PlusEquals,
  CTokenType_MinusEquals,
  CTokenType_TimesEquals,
  CTokenType_DivEquals,
  CTokenType_ModEquals,
  CTokenType_AndEquals,
  CTokenType_OrEquals,
  CTokenType_XorEquals,

  CTokenType_Increment,
  CTokenType_Decrement,
  CTokenType_LogicalAnd,
  CTokenType_LogicalOr,

  CTokenType_Arrow,

  CT_ScopeResolutionOperator,
  CT_NameQualifier,

  CT_MacroLiteral,
  CT_MacroLiteral_SelfRefExpansion,

  CT_PreprocessorPaste,
  CT_PreprocessorPaste_InvalidToken,

  CT_PreprocessorInclude,
  CT_PreprocessorIncludeNext,

  CT_PreprocessorIf,
  CT_PreprocessorElse,
  CT_PreprocessorElif,
  CT_PreprocessorEndif,
  CT_PreprocessorIfDefined,
  CT_PreprocessorIfNotDefined,
  CT_PreprocessorDefine,
  CT_PreprocessorUndef,
  CT_PreprocessorPragma,
  CT_PreprocessorError,
  CT_PreprocessorWarning,
  CT_Preprocessor__VA_ARGS__,
  CT_Preprocessor_Nuked,

  CT_InsertedCode,
};
meta(generate_string_table(c_token_type))
#include <poof/generated/generate_string_table_c_token_type.h>

enum c_token_flags
{
  CTFlags_None = 0,

  CTFlags_RelativeInclude = 1 << 0,
};

struct c_token_cursor;
#if 0
struct c_token;
struct up_info
{
  c_token_cursor *Tokens;
  c_token *At;
};

up_info
UpInfo(c_token_cursor *Tokens, c_token *At)
{
  up_info Result = {
    .Tokens = Tokens,
    .At = At,
  };
  return Result;
}
#endif


struct macro_def;
struct macro_expansion
{
  c_token_cursor *Expansion;
  macro_def *Def;
};


enum va_args_flags
{
  va_args_flags_none = 0,
  va_args_flags_expand_with_commas = (1 << 0),
};

struct parser;
struct c_token
{
  c_token_type Type;
  counted_string Value;

  counted_string Filename;
  u32 LineNumber;
  b32 Erased; // TODO(Jesse): Pack this into Flags

  u8 Flags;

  union
  {
     /* s64         SignedValue; */ // TODO(Jesse id: 272): Fold `-` sign into this value at tokenization time?
     u64            UnsignedValue;
     r64            FloatValue;
     c_token        *QualifierName;

     // NOTE(Jesse): I ordered the 'macro_expansion' struct such that the
     // pointer to the expanded macro will be at the same place as the `Down`
     // poninter.  This is sketchy as fuck, but it'll work, and this the
     // assertions at @janky-macro-expansion-struct-ordering should catch the
     // bug if we reorder the pointers.
     c_token_cursor  *Down;
     macro_expansion Macro;

     counted_string IncludePath; // TODO(Jesse): We probably care that this (and Macro) increase struct size by 8.  Heap allocate to fix?
  };

  // TODO(Jesse)(correctness): The preprocessor doesn't support this for some reason..
  operator bool()
  {
    b32 Result = (b32)((u64)Type | Value.Count);
    return Result;
  }

};

global_variable c_token NullToken = {};

struct peek_result
{
  c_token_cursor *Tokens;

  c_token *At;
  b32 DoNotDescend;
};

enum token_cursor_source
{
  TokenCursorSource_Unknown,

  TokenCursorSource_RootFile,
  TokenCursorSource_Include,
  TokenCursorSource_MacroExpansion,
  TokenCursorSource_MetaprogrammingExpansion,
  TokenCursorSource_PasteOperator,
  TokenCursorSource_CommandLineOption,
  TokenCursorSource_BodyText,

  // NOTE(Jesse): These should be created temporarily and discarded.  This type
  // should never be seen in "user" code.. it's just used in parsing.
  TokenCursorSource_IntermediateRepresentaton,

  TokenCursorSource_Count,
};
meta(generate_string_table(token_cursor_source));
#include <poof/generated/generate_string_table_token_cursor_source.h>

// TODO(Jesse): Add a way to append additional members to generated datatypes
// then reenable this.
/* meta(generate_cursor(c_token)) */
#include <poof/generated/generate_cursor_c_token.h>


meta(buffer(c_token))
#include <poof/generated/buffer_c_token.h>

meta(buffer(c_token_buffer))
#include <poof/generated/buffer_c_token_buffer.h>

meta(generate_stream(c_token_buffer))
#include <poof/generated/generate_stream_c_token_buffer.h>

enum parse_error_code
{
  ParseErrorCode_None,

  // C parsing errors
  ParseErrorCode_ExpectedSemicolonOrEquals,
  ParseErrorCode_StreamEndedUnexpectedly,
  ParseErrorCode_RequireTokenFailed,
  ParseErrorCode_InvalidTokenGenerated,


  // Poof errors
  ParseErrorCode_DUnionParse,
  ParseErrorCode_UndefinedDatatype,
  ParseErrorCode_PoofTypeError,


  // General errors
  ParseErrorCode_InputStreamNull,


  // Well, shit.
  ParseErrorCode_InternalCompilerError,


  // We hit an error, but didn't classify it.
  ParseErrorCode_Unknown,
};
meta(generate_string_table(parse_error_code))
#include <poof/generated/generate_string_table_parse_error_code.h>

struct parser
{
  parse_error_code ErrorCode;
  c_token_cursor *Tokens;
};
meta(generate_cursor(parser))
#include <poof/generated/generate_cursor_parser.h>

meta(generate_stream(parser))
#include <poof/generated/generate_stream_parser.h>

bonsai_function parser
MakeParser(c_token_cursor *Tokens)
{
  parser Result = {
    .Tokens = Tokens,
  };
  return Result;
}

struct declaration_stream_chunk;

/* TODO(Jesse id: 191, tags: metaprogramming): This can be generated, but it
 * requires lazily evaluating functions once their dependant types have been
 * generated.  Not sure if this is a good idea or not, but we could do it.
 *
 * meta( generate_stream_struct(declaration) )
 *
 * Currently the invariant of being able to remove and re-generate all meta
 * output in a single run of the metaprogramming system is broken by this.
 */

struct declaration_stream
{
  declaration_stream_chunk *FirstChunk;
  declaration_stream_chunk *LastChunk;
};

struct compound_decl // structs and unions
{
  c_token *Type;
  declaration_stream Members;
  b32 IsUnion;
};
meta(stream_and_cursor(compound_decl))
#include <poof/generated/stream_and_cursor_struct_def.h> // TODO(Jesse): Change this name

enum linkage_type
{
  linkage_noop,
  linkage_extern,
  linkage_extern_c,
  linkage_extern_cpp,
};

struct type_indirection_info
{
  b32 ConstPointer;
  b32 VolatileValue;

  u32 ReferenceLevel;
  u32 IndirectionLevel;

  b32 IsFunction;
  b32 IsFunctionPtr;

  c_token *FunctionNameT;
};

global_variable type_indirection_info NullIndirection = {};

// TODO(Jesse): Adding an index feature to map_values would let us generate
// bitfields like this.  We could also do stuff like statically ensure a
// particular width, or quickly re-order fields without having to manually
// re-jig the shifts.
//
enum type_qualifier
{
  TypeQual_None        = 0,
  TypeQual_Void        = (1 << 0),
  TypeQual_Auto        = (1 << 1),
  TypeQual_Class       = (1 << 2),
  TypeQual_Struct      = (1 << 3),
  TypeQual_Union       = (1 << 4),
  TypeQual_Enum        = (1 << 5),
  TypeQual_Bool        = (1 << 6),
  TypeQual_Inline      = (1 << 7),
  TypeQual_ThreadLocal = (1 << 8),
  TypeQual_Static      = (1 << 9),
  TypeQual_Extern      = (1 << 10),
  TypeQual_Const       = (1 << 11),
  TypeQual_Volatile    = (1 << 12),
  TypeQual_Signed      = (1 << 13),
  TypeQual_Unsigned    = (1 << 14),
  TypeQual_Char        = (1 << 15),
  TypeQual_Long        = (1 << 16),
  TypeQual_Int         = (1 << 17),
  TypeQual_Long_Long   = (1 << 18),
  TypeQual_Double      = (1 << 19),
  TypeQual_Short       = (1 << 20),
  TypeQual_Float       = (1 << 21),
  TypeQual_Constexpr   = (1 << 22),
  TypeQual_Explicit    = (1 << 23),
  TypeQual_Operator    = (1 << 24),
  TypeQual_Virtual     = (1 << 25),
};
meta(string_and_value_tables(type_qualifier))
#include <poof/generated/string_and_value_tables_type_qualifier.h>

struct type_spec
{
  c_token *QualifierNameT;
  c_token *DatatypeToken;
  // datatype Datatype;

  type_qualifier Qualifier;

  b32 HasTemplateArguments;
  counted_string TemplateSource;

  linkage_type Linkage;
};

struct ast_node;
struct variable_decl
{
  type_spec Type;
  counted_string Name;
  ast_node *StaticBufferSize;
  ast_node *Value;

  u32 StrictBitWidth; // For declarations in structs like : `int foo_bool : 1`
  type_indirection_info Indirection;
};
meta(generate_stream(variable_decl))
#include <poof/generated/generate_stream_variable_decl.h>

enum function_type
{
  function_type_noop,
  function_type_constructor,
  function_type_destructor,
  function_type_operator,
  function_type_normal,
};
meta(string_and_value_tables(function_type))
#include <poof/generated/string_and_value_tables_function_type.h>

struct ast_node_statement;
struct function_decl
{
  function_type Type;

  type_spec ReturnType;
  c_token *NameT;
  variable_decl_stream Args;

  // TODO(Jesse): Pack these into a u8 flags
  b32 IsVariadic;
  b32 ImplIsDefault;
  b32 ImplIsDeleted;

  parser Body;
  ast_node *Ast;
};

meta(generate_stream(function_decl))
#include <poof/generated/generate_stream_function_decl.h>

struct ast_node_expression;
struct enum_member
{
  counted_string Name;
  ast_node_expression *Expr;
};
meta(generate_stream(enum_member))
#include <poof/generated/generate_stream_enum_member.h>

struct enum_decl
{
  counted_string Name;
  enum_member_stream Members;
};
meta(stream_and_cursor(enum_decl))
#include <poof/generated/stream_and_cursor_enum_def.h>

meta(
  d_union declaration
  {
    enum_decl
    function_decl
    compound_decl
    variable_decl
  }
)
#include <poof/generated/d_union_declaration.h>

meta(string_and_value_tables(declaration_type))
#include <poof/generated/string_and_value_tables_declaration_type.h>

meta(generate_cursor(declaration))
#include <poof/generated/generate_cursor_declaration.h>

meta(generate_stream_chunk_struct(declaration))
#include <poof/generated/generate_stream_chunk_declaration.h>

// TODO(Jesse): Should type_spec actually be representative of primitive types?
struct primitive_def
{
  type_spec TypeSpec;
};

enum datatype_type
{
  type_datatype_noop,
  type_declaration,
  type_enum_member,
  type_type_def,
  type_primitive_def,
};
meta(generate_string_table(datatype_type))
#include <poof/generated/generate_string_table_datatype_type.h>

struct enum_member;
struct type_def;

struct datatype
{
  datatype_type Type;

  union
  {
    declaration    declaration;
    primitive_def  primitive_def;
    enum_member   *enum_member;
    type_def      *type_def;
  };
};

struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
};
meta(generate_stream(d_union_member))
#include <poof/generated/generate_stream_d_union_member.h>

struct type_def
{
  type_spec Type;
  counted_string Alias;
  b32 IsFunction;
};
meta(generate_stream(type_def))
#include <poof/generated/generate_stream_type_def.h>


bonsai_function datatype
Datatype(declaration* M)
{
  datatype Result = {
    .Type = type_declaration,
    .declaration = *M,
  };
  return Result;
}

bonsai_function datatype
Datatype(enum_member* E)
{
  datatype Result = {
    .Type = type_enum_member,
    .enum_member = E,
  };
  return Result;
}

bonsai_function declaration
Declaration(compound_decl* S)
{
  declaration Result = {};
  Result.Type = type_compound_decl;
  Result.compound_decl = *S;
  return Result;
}

bonsai_function datatype
Datatype(compound_decl* S)
{
  datatype Result = {};
  Result.Type = type_declaration;
  Result.declaration = Declaration(S);
  return Result;
}

bonsai_function datatype
Datatype(type_def* E)
{
  datatype Result = {
    .Type = type_type_def,
    .type_def = E,
  };
  return Result;
}

bonsai_function declaration
Declaration(enum_decl* E)
{
  declaration Result = {};
  Result.Type = type_enum_decl;
  Result.enum_decl = *E;
  return Result;
}

bonsai_function datatype
Datatype(enum_decl* E)
{
  datatype Result = {};
  Result.Type = type_declaration;
  Result.declaration = Declaration(E);
  return Result;
}

bonsai_function datatype
Datatype(type_spec S)
{
  datatype Result = {};
  Result.Type = type_primitive_def;
  Result.primitive_def.TypeSpec = S;
  return Result;
}


struct meta_func_arg
{
  counted_string Match;
  datatype Data;
};
meta(generate_stream(meta_func_arg))
#include <poof/generated/generate_stream_meta_func_arg.h>










struct d_union_decl
{
  counted_string Name;

  d_union_member_stream Members;
  declaration_stream CommonMembers;

  counted_string CustomEnumType;
};

enum macro_type
{
  type_macro_noop,

  type_macro_keyword,
  type_macro_function,
};

struct macro_def
{
  macro_type Type;
  counted_string Name;
  c_token_cursor Body;

  counted_string_buffer NamedArguments;
  b32 Variadic;
  b32 Undefed; // Gets toggled when we hit an undef
  /* b32 IsExpanding; */
};
meta(generate_stream(macro_def))
#include <poof/generated/generate_stream_macro_def.h>

bonsai_function umm
Hash(macro_def *M)
{
  umm Result = Hash(&M->Name);
  // TODO(Jesse): Hash arguments and incorporate Type & Variadic ..?
  return Result;
}

meta(hashtable(macro_def))
#include <poof/generated/hashtable_macro_def.h>


struct meta_func
{
  counted_string Name;
  counted_string ArgName;
  parser Body;
};
meta(generate_stream(meta_func))
#include <poof/generated/generate_stream_meta_func.h>


struct todo
{
  counted_string Id;
  counted_string Value;
  b32 FoundInCodebase;
};
meta(generate_stream(todo))
#include <poof/generated/generate_stream_todo.h>

struct tag
{
  counted_string Tag;
  todo_stream Todos;
};
meta(generate_stream(tag))
#include <poof/generated/generate_stream_tag.h>

struct person
{
  counted_string Name;
  tag_stream Tags;
};
meta(generate_stream(person))
#include <poof/generated/generate_stream_person.h>

#define SafeAccess(T, Ptr) (&(Ptr)->T); Assert((Ptr)->Type == type_##T)

#define SafeAccessPtr(T, Ptr) ((Ptr)->T); Assert((Ptr)->Type == type_##T)
#define SafeAccessObj(T, Obj) ((Obj).T); Assert((Obj).Type == type_##T)
#define SafeAccessObjPtr(T, Obj) (&(Obj).T); Assert((Obj).Type == type_##T)

struct ast_node_expression
{
  ast_node *Value;
  ast_node_expression *Next;
};
meta(generate_stream(ast_node_expression))
#include <poof/generated/generate_stream_ast_node_expression.h>

struct ast_node_statement
{
  ast_node_expression *LHS;
  ast_node_statement  *RHS;
  ast_node_statement  *Next;
};

struct ast_node_function_call
{
  counted_string Name;
  function_decl *Prototype;
  ast_node_expression_stream Args;
};

struct ast_node_type_specifier
{
  datatype Datatype;
  type_spec TypeSpec;
  type_indirection_info Indirection;
  ast_node_expression *Name;
};

struct ast_node_variable_def
{
  datatype Type;
  variable_decl Decl;
  ast_node *Value;
};
meta(generate_stream(ast_node_variable_def))
#include <poof/generated/generate_stream_ast_node_variable_def.h>

struct ast_node_access
{
  c_token AccessType; // Arrow or Dot
  ast_node_expression *Symbol;
};

struct ast_node_parenthesized
{
  ast_node_expression *Expr;

  // TODO(Jesse id: 262): These should probably be in their own type_ast_node_cast .. ?
  b32 IsCast;
  ast_node_expression *CastValue;
};

struct ast_node_initializer_list
{
  u32 Ignored;
};

struct ast_node_operator
{
  ast_node_expression *Operand;
  c_token Token;
};

struct ast_node_literal
{
  c_token Token;
};

struct ast_node_symbol
{
  c_token Token;
};

struct ast_node_return
{
  ast_node_expression *Value;
};

struct ast_node_predicated
{
  c_token Token;
  ast_node_expression *Predicate;
};

meta(
  d_union ast_node
  {
    ast_node_statement
    ast_node_access
    ast_node_literal
    ast_node_expression
    ast_node_parenthesized
    ast_node_operator
    ast_node_function_call
    ast_node_return
    ast_node_initializer_list
    ast_node_symbol
    ast_node_variable_def
    ast_node_type_specifier
  }
)
#include <poof/generated/d_union_ast_node.h>

meta(generate_stream(ast_node))
#include <poof/generated/generate_stream_ast_node.h>

bonsai_function ast_node*
AllocateAstNode(ast_node_type T, ast_node **Result, memory_arena* Memory)
{
  Assert(Result && (*Result == 0)); // We got a valid pointer, and it hasn't been allocated yet.

  *Result = AllocateProtection(ast_node, Memory, 1, False);
  (*Result)->Type = T;

  return *Result;
}

#define AllocateAndCastTo(T, NodeDest, Memory) (&AllocateAstNode(type_##T, (NodeDest), (Memory))->T)

struct arguments
{
  counted_string Outpath;
  counted_string_cursor Files;

  counted_string_cursor IncludePaths;

  b32 DoDebugWindow;
};

struct program_datatypes
{
  macro_def_hashtable      Macros;
  counted_string_hashtable FilesParsed;

  compound_decl_stream     Structs;
  enum_decl_stream         Enums;
  function_decl_stream     Functions;
  type_def_stream          Typedefs;
};

struct for_enum_constraints
{
  // Replacement Patterns
  counted_string TypeName;
  counted_string ValueName;
};

struct body_text_constraints
{
  counted_string MemberContains;

  // Replacement Patterns
  counted_string TypeTag;
  counted_string TypeName;
  counted_string ValueName;
};

struct todo_list_info
{
  person_stream People;
  tagged_counted_string_stream_stream NameLists;
};



struct string_from_parser
{
  parser *Parser;
  c_token *StartToken;
};

enum output_mode
{
  Output_NoOverwrite,
  Output_Unsafe,
};


#define WriteTo(Dest, S) if (Dest) { CopyToDest(Dest, S); } else { DebugChars(CSz("%S"), S); }

inline void
PrintToken(c_token *Token, char_cursor *Dest = 0)
{
  if (Token)
  {
    // NOTE(Jesse): Annoyingly, printing out the line tray for multi-line comments
    // requires that we allow printing tokens with Value.Count == 0
    /* Assert(Token->Value.Start && Token->Value.Count); */

    counted_string Color = {};

    switch (Token->Type)
    {
      case CT_PreprocessorInclude:
      case CT_PreprocessorIncludeNext:
      case CT_PreprocessorIf:
      case CT_PreprocessorElse:
      case CT_PreprocessorElif:
      case CT_PreprocessorEndif:
      case CT_PreprocessorIfDefined:
      case CT_PreprocessorIfNotDefined:
      case CT_PreprocessorDefine:
      case CT_PreprocessorUndef:
      case CT_PreprocessorPragma:
      case CT_PreprocessorError:
      case CT_PreprocessorWarning:
      case CT_Preprocessor__VA_ARGS__:
      {
        if (Token->Erased)
        {
          Color = TerminalColors.BrightYellow;
        }
        else
        {
          Color = TerminalColors.Yellow;
        }
      } break;

      case CT_MacroLiteral:
      {
        if (Token->Erased)
        {
          Color = TerminalColors.Purple;
        }
        else
        {
          Color = TerminalColors.Blue;
        }
      } break;

      default:
      {
        if (Token->Erased)
        {
          Color = TerminalColors.Grey;
        }
      } break;
    }


    if (Color.Count) { WriteTo(Dest, Color); }

    /* if (Token->Type == CTokenType_Newline) { WriteTo(Dest, CSz("\\n")); } */

    WriteTo(Dest, Token->Value);

    if (Color.Count) { WriteTo(Dest, TerminalColors.White); }
  }
}
#undef WriteTo

inline void
PrintToken(c_token Token, char_cursor *Dest = 0)
{
  PrintToken(&Token, Dest);
}

b32 AreEqual(c_token T1, c_token T2)
{
  b32 Result = (T1.Type == T2.Type);

  if (Result && T1.Type == CTokenType_Newline)
  {
    // NOTE(Jesse): On Windows newline chars can be length 2 (\r\n) so we don't
    // check that the Value strings match for that case
    Assert(T2.Type == CTokenType_Newline);
  }
  else
  {
    if (Result && (T1.Value.Count > 1 || T2.Value.Count > 1) )
    {
      Result &= StringsMatch(T1.Value, T2.Value);
    }
  }
  return Result;
}

b32 AreEqual(c_token *T1, c_token T2)
{
  b32 Result = False;
  if (T1)
  {
    Result = AreEqual(*T1, T2);
  }
  return Result;
}

b32 AreEqual(c_token T1, c_token *T2)
{
  b32 Result = False;
  if (T2)
  {
    Result = AreEqual(T1, *T2);
  }
  return Result;
}

b32 AreEqual(c_token *T1, c_token *T2)
{
  b32 Result = False;
  if (T1 && T2)
  {
    Result = AreEqual(*T1, *T2);
  }
  return Result;
}

b32
operator==(c_token T1, c_token T2)
{
  b32 Result = AreEqual(T1, T2);
  return Result;
}

b32
operator!=(c_token T1, c_token T2)
{
  b32 Result = !(T1==T2);
  return Result;
}

inline c_token
CToken(r32 FloatValue)
{
  c_token Result = {
    .Type = CTokenType_Float,
    .Value = FormatCountedString(TranArena, CSz("%f"), FloatValue), // TODO(Jesse id: 350, tags: memory_leak)
    .FloatValue = FloatValue
  };
  return Result;
}

inline c_token
CToken(u32 UnsignedValue)
{
  c_token Result = {
    .Type = CTokenType_IntLiteral,
    .Value = FormatCountedString(TranArena, CSz("%u"), UnsignedValue), // TODO(Jesse id: 351, tags: memory_leak)
    .UnsignedValue = UnsignedValue,
  };
  return Result;
}

inline c_token
CToken(counted_string Value)
{
  c_token Result = {
    .Type = CTokenType_Identifier,
    .Value = Value
  };
  return Result;
}

inline c_token
CToken(c_token_type Type, counted_string Value = CSz(""))
{
  c_token Result = {};

  Result.Type = Type;
  Result.Value = Value;

  return Result;
}

inline void
CTokenCursor(c_token_cursor *Result, c_token *Buffer, umm Count, counted_string Filename, token_cursor_source Source, c_token_cursor_up Up)
{
  Result->Filename = Filename;
  Result->Source = Source;
  Result->Start = Buffer;
  Result->At = Result->Start;
  Result->End = Result->Start + Count;
  Result->Up = Up;
}

bonsai_function c_token_cursor
CTokenCursor(c_token *Start, c_token *End, counted_string Filename, token_cursor_source Source, c_token_cursor_up Up)
{
  c_token_cursor Result = {
    .Start = Start,
    .End = End,
    .At = Start,
    .Filename = Filename,
    .Source = Source,
    .Up = Up,
  };
  return Result;
}

bonsai_function c_token_cursor
CTokenCursor(c_token_buffer *Buf, counted_string Filename, token_cursor_source Source, c_token_cursor_up Up)
{
  c_token_cursor Result = CTokenCursor(Buf->Start, Buf->Start + Buf->Count, Filename, Source, Up);
  return Result;
}


inline void
CTokenCursor(c_token_cursor *Result, umm Count, memory_arena *Memory, counted_string Filename, token_cursor_source Source, c_token_cursor_up Up)
{
  c_token *Buffer = AllocateProtection(c_token, Memory, Count, False);
  CTokenCursor(Result, Buffer, Count, Filename, Source, Up);
}

c_token_cursor *
AllocateTokenCursor(memory_arena* Memory, counted_string Filename, umm Count, token_cursor_source Source, u32 LineNumber, c_token_cursor_up Up)
{
  c_token_cursor *Result = AllocateProtection(c_token_cursor, Memory, 1, False);
  CTokenCursor(Result, Count, Memory, Filename, Source, Up);
  return Result;
}

bonsai_function parser
AllocateParser(counted_string Filename, u32 LineNumber, u32 TokenCount, token_cursor_source Source, u32 OutputBufferTokenCount, c_token_cursor_up Up, memory_arena *Memory)
{
  TIMED_FUNCTION();

  parser Result = {};

  Result.Tokens = AllocateTokenCursor(Memory, Filename, TokenCount, Source, LineNumber, Up);
  if (!Result.Tokens->Start)
  {
    Error("Allocating Token Buffer");
    return Result;
  }

  return Result;
}

bonsai_function parser*
AllocateParserPtr(counted_string Filename, u32 LineNumber, u32 TokenCount, token_cursor_source Source, u32 OutputBufferTokenCount, c_token_cursor_up Up,  memory_arena *Memory)
{
  Assert(OutputBufferTokenCount == 0);
  parser *Result = AllocateProtection(parser, Memory, 1, False);
  *Result = AllocateParser(Filename, LineNumber, TokenCount, Source, OutputBufferTokenCount, Up, Memory);
  return Result;
}

c_token
PeekToken(ansi_stream* Stream, u32 Lookahead = 0)
{
  c_token Result = {};

  if (Stream->At+Lookahead < Stream->End)
  {
    char At = *(Stream->At+Lookahead);
    // TODO(Jesse, id: 193, tags: metaprogramming): Metaprogram this.  I've had bugs multiple times because of it.
    switch (At)
    {

      case CT_ControlChar_Start_of_Heading:
      case CT_ControlChar_Start_of_Text:
      case CT_ControlChar_End_of_Text:
      case CT_ControlChar_End_of_Transmission:
      case CT_ControlChar_Enquiry:
      case CT_ControlChar_Acknowledgement:
      case CT_ControlChar_Bell:
      case CT_ControlChar_Backspace:
      case CT_ControlChar_Vertical_Tab:
      case CT_ControlChar_Form_Feed:
      case CT_ControlChar_Shift_Out:
      case CT_ControlChar_Shift_In:
      case CT_ControlChar_Data_Link_Escape:
      case CT_ControlChar_Device_Control_1:
      case CT_ControlChar_Device_Control_2:
      case CT_ControlChar_Device_Control_3:
      case CT_ControlChar_Device_Control_4:
      case CT_ControlChar_Negative_Acknowledgement:
      case CT_ControlChar_Synchronous_Idle:
      case CT_ControlChar_End_of_Transmission_Block:
      case CT_ControlChar_Cancel:
      case CT_ControlChar_End_of_Medium:
      case CT_ControlChar_Substitute:
      case CT_ControlChar_Escape:
      case CT_ControlChar_File_Separator:
      case CT_ControlChar_Group_Separator:
      case CT_ControlChar_Record_Separator:
      case CT_ControlChar_Unit_Separator:
      case CT_ControlChar_Delete:

      case CTokenType_OpenBracket:
      case CTokenType_CloseBracket:
      case CTokenType_OpenBrace:
      case CTokenType_CloseBrace:
      case CTokenType_OpenParen:
      case CTokenType_CloseParen:
      case CTokenType_Dot:
      case CTokenType_Comma:
      case CTokenType_Semicolon:
      case CTokenType_Colon:
      case CTokenType_Hash:
      case CTokenType_At:
      case CTokenType_Dollar:
      case CTokenType_Space:
      case CTokenType_Tab:
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
      case CTokenType_Pipe:
      case CTokenType_Newline:
      case CTokenType_CarrigeReturn:
      case CTokenType_EOF:
      {
        Result = { .Type = (c_token_type)At };
      } break;
    }
  }
  else
  {
    Warn("Attempted to get token past end of stream on file : %S", Stream->Filename);
  }

  return Result;
}

meta(generate_stream_iterator(declaration))
#include <poof/generated/generate_stream_iterator_datatype.h>

meta(generate_stream_push(declaration))
#include <poof/generated/generate_stream_push_datatype.h>

bonsai_function b32
Contains(parser *Parser, c_token *T)
{
  b32 Result = False;
  if (T >= Parser->Tokens->Start && T < Parser->Tokens->End)
  {
    Result = True;
  }
  return Result;
}

enum parser_push_type
{
  parser_push_type_noop,

  parser_push_type_root,
  parser_push_type_include,
  parser_push_type_macro,
};

struct parse_context
{
  parser                *CurrentParser;

  program_datatypes      Datatypes;
  counted_string_cursor *IncludePaths;
  meta_func_stream       MetaFunctions;
  memory_arena          *Memory;

  arguments *Args;
};

enum erase_token_mode
{
  PreserveTokens = 0,
  EraseTokens = 1,
};

struct comma_separated_decl
{
  c_token *NameT;
  type_indirection_info Indirection;
  ast_node *StaticBufferSize;
  ast_node *Value;
};

struct d_list
{
  d_list *Prev;
  d_list *Next;
};

bonsai_function parse_context
AllocateParseContext(memory_arena *Memory)
{
  parse_context Ctx = {
    .Memory = Memory
  };
  Ctx.Datatypes.Macros = Allocate_macro_def_hashtable(4096, Memory);
  Ctx.Datatypes.FilesParsed = Allocate_counted_string_hashtable(512, Memory);
  return Ctx;
}

bonsai_function b32
IsValidForCursor(c_token_cursor *Tokens, c_token *T)
{
  b32 Result = T < Tokens->End && T >= Tokens->Start;
  return Result;
}

