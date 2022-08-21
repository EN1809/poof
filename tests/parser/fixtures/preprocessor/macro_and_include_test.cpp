
#include <tests/parser/bug_defines.h>

#if 1

// 0 \
   1 \
   2 \
   3

/* 0
 * 1
 * 2
   3 */

#endif


#if 1

#define foo valid_path

foo

#if 1
  foo
#else
  invalid_path
#endif

foo

// Include Test

#undef foo
#include <tests/parser/fixtures/preprocessor/include_test.cpp> // \
this is actually commented
#include <tests/parser/fixtures/preprocessor/include_test_2.cpp>

#include <tests/parser/fixtures/preprocessor/empty.cpp>
#include <tests/parser/fixtures/preprocessor/empty.cpp>

#include <tests/parser/fixtures/preprocessor/random_define.cpp> /* this should be fine */

#include <tests/parser/fixtures/preprocessor/random_define.cpp> // this should be fine \
This line is actually commented
#include <tests/parser/fixtures/preprocessor/random_define.cpp> /* this should be fine */
#include <tests/parser/fixtures/preprocessor/random_define.cpp> /* this should be fine */  \

#include <tests/parser/fixtures/preprocessor/empty.cpp> /*
                                                       */   
#include <tests/parser/fixtures/preprocessor/random_define.cpp> // this should be fine \\\\
This line is actually commented


#define MacroKeyword    this_is_a_variable_name

#define IndirectMacroKeyword MacroKeyword

#define PastedMacroKeyword Macro ## Keyword

#define MacroFunction(a) a

#define MacroFunction2(a) int MacroKeyword = a

#define MacroFunction3(a, b, c, d) a b c d

#define MacroFunction4(a, ...) a

#define MacroFunction5(a, b, ...) a b __VA_ARGS__

#define MacroFunction6(...) __VA_ARGS__

#define MacroFunction7(a, b) a b

#define MacroFunction8(a, b) \
  a                          \
  b

#define MacroFunction9(a, b) a ## b

#define MacroFunction10(a, b, c) a ## b ## c



int MacroKeyword = 42;
int IndirectMacroKeyword = 42;
int PastedMacroKeyword = 42;


// MacroFunction


MacroFunction(int this_is_a_variable_name = 42);

MacroFunction(int MacroKeyword = 42);

MacroFunction(SomeRegularFunctionCall(42));


// MacroFunction2


MacroFunction2(42);

MacroFunction(MacroFunction2(42));

MacroFunction(MacroFunction(MacroFunction2(42)));


// MacroFunction3


MacroFunction3(int, this_is_a_variable_name, =, 42);

MacroFunction3(int, MacroKeyword, =, 42);

MacroFunction3(MacroFunction2(42), , , );


// MacroFunction4


MacroFunction4(int this_is_a_variable_name = 42, , , );

MacroFunction4(int this_is_a_variable_name = 42, other, crap here, should just disappear, including MacroKeyword, MacroFunction2(666));


// MacroFunction5


MacroFunction5(int, this_is_a_variable_name, =, 42);


// MacroFunction6


MacroFunction6(int, this_is_a_variable_name, =, 42);

MacroFunction6(int, MacroKeyword, =, 42);

MacroFunction6(MacroFunction(int MacroKeyword = 42));

MacroFunction6(MacroFunction2(42));


// MacroFunction7


MacroFunction7(SomeRegularFunctionCall(4, 2);, SomeRegularFunctionCall(42));


// MacroFunction8


MacroFunction8(4, 2)
MacroFunction8 (4, 2)
MacroFunction8
MacroFunction8(MacroFunction8, 0)


// MacroFunction9


#endif

MacroFunction9(,this_is_a_variable_name)
MacroFunction9(this_is_a_variable_name,)
MacroFunction9(this_is_a_, variable_name)

MacroFunction9(Macro, Keyword)
MacroFunction9(IndirectMacro, Keyword)
MacroFunction9(PastedMacro, Keyword) // ??? this_is_a_variable_name ???

#define some_thing this_is_a_variable_name
#define some fooberdoober

MacroFunction9(some, _thing) // fooberdoober_thing

#undef some
MacroFunction9(some, _thing) // this_is_a_variable_name

#undef some_thing
MacroFunction9(some, _thing) // some_thing

MacroFunction9(in, t) // CTokenType_Int(int)

MacroFunction9(Macro, MacroFunction9(,Keyword)) // this_is_a_variable_name
MacroFunction9(MacroKeyword, ) // this_is_a_variable_name
MacroFunction9(MacroFunction9(Macro, Keyword), ) // this_is_a_variable_name
MacroFunction9(, MacroFunction9(Macro, Keyword)) // this_is_a_variable_name
MacroFunction9(MacroFunction9(Macro, ), Keyword) // this_is_a_variable_name

#if BUG_OPERATOR_PASTE
MacroFunction9(+,+) // CTokenType_Increment(++)
#endif

// MacroFunction10


MacroFunction10(i,n,t) // CTokenType_Int(int)
MacroFunction10(this_is, _a_, variable_name) // this_is_a_variable_name
MacroFunction10(Mac, ro, Keyword) // this_is_a_variable_name
MacroFunction10(Macro, , Keyword) // this_is_a_variable_name
MacroFunction10(,Macro,Keyword) // this_is_a_variable_name
MacroFunction10(Macro,Keyword,) // this_is_a_variable_name

#define self_including_macro_keyword self_including_macro_keyword 42
self_including_macro_keyword // should expand to "self_including_macro_keyword 42"
#undef self_including_macro_keyword

#if BUG_RECURSIVE_MACRO_EXPANSION
#define self_including_macro_keyword MacroFunction(self_including_macro_keyword 42)
self_including_macro_keyword // should expand to "self_including_macro_keyword 42"
#undef self_including_macro_keyword

// NOTE(Jesse): I have no idea what this is specified to expand to.  I observed
// Clang not expanding it at all, so that's what I've programmed.
//
// UPDATE(Jesse): This now ends up in an infinite expansion loop.  Fixing the
// following cases should be done at some point, but since it's currently not
// a problem I don't have any reason to want to do it.
//
  #define m1() m2()
  #define m2() m1()
  m2()
#endif


#if BUG_SELF_INCLUDING_MACRO_FUNCTION
#define self_including_macro_function() self_including_macro_function()
self_including_macro_function()
#endif


#ifndef _DEBUG

  valid_path

#else

  invalid_path

  #ifndef _DEBUG
    invalid_path
  #else
    invalid_path
  #endif

  invalid_path

#endif


// NOTE(Jesse): This tests that we properly handle macros declared out-of-order
// during pasting
#define concat(x,y) x##y
#define concat_constant_to(a) concat(this_is_a_, a)
#define concat_4(b, c, _) concat_constant_to( concat_2(b, c) )
#define concat_2(d, r) concat(d, r)
#define whatever double
concat_4(variable_name,, (whatever __x))



// @va_args_paste_into_another_macro
//
// Found out MSVC and GCC have different behavoir for the following.  Currently
// poof emulates MSVC
//
// gcc outputs
// P1 = _foo | P2 = _bar | P3 = _baz
//
// MSVC & poof output
// P1 = _foo _bar _baz | P2 = | P3 =
//
// Supposedly amending MACRO_VA_ARGS to:
// #define MACRO_VA_ARGS(...) MACRO_WITH_3_PARAMS( ## __VA_ARGS__)
//
// convinces MSVC and GCC to emit the same thing, which is the gcc case.
//
// I experimented and clang exhibits the same behavior as GCC, so that's the
// behavior I've emulated for poof.
//
// https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/preprocessor/macros/__VA_ARGS__/index


#define MACRO_WITH_3_PARAMS(p1, p2, p3) P1 p1 P2 p2 P3 p3
#define MACRO_VA_ARGS(...) MACRO_WITH_3_PARAMS(__VA_ARGS__)
MACRO_VA_ARGS(_foo, _bar, _baz)

#define MACRO_VA_ARGS_PASTED(...) MACRO_WITH_3_PARAMS(##__VA_ARGS__)
MACRO_VA_ARGS_PASTED(_foo, _bar, _baz)


// @non_portable_va_args_paste
//
// https://en.cppreference.com/w/cpp/preprocessor/replace
//
// ```
// Note: some compilers offer an extension that allows ## to appear after a
// comma and before __VA_ARGS__, in which case the ## does nothing when the
// variable arguments are present, but removes the comma when the variable
// arguments are not present: this makes it possible to define macros such as
// fprintf (stderr, format, ##__VA_ARGS__)
// ```
//
// FML.

#define macro_with_disappearing_comma(a, ...) a , ## __VA_ARGS__
macro_with_disappearing_comma(_foo)        // _foo        < no comma
macro_with_disappearing_comma(_bar, _baz)  // _bar, _baz  < comma
                                           //
                                           //             .. magic



#undef foo

#define foo f
#define foo_arg_func(a1, foo) a1 foo

foo_arg_func(a1, a2)

#undef foo

foo_arg_func(a1, a2)
