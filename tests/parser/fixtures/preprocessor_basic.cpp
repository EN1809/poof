// single line comment's "work"
/* multi line comment's "work" */

bonsai_function void
FunctionName()
{
  counted_string S = CS("thing\n");
  int foo = 3/3;
  return;
}

bonsai_function void
WriteStructTo(d_union_decl* dUnion, native_file* OutFile, memory_arena* Memory)
{
  counted_string UnionName = dUnion->Name;
  counted_string Decl = FormatCountedString(Memory, "\n");
}

//
// Integral contant tests
//

42

42L
42LU
42LL
42LLU

42U
42UL
42ULL

42l
42lu
42ll
42llu

42u
42ul
42ull

//
// Hex Constant tests
//

0x1
0x0000000000000001

0xa

0x7f
0xff

0xffff
0xffFFff
0xffFFffFF

0xffFFffFFffFFffFF

0x123456789abcdef1
0x7fffffffffffffff

//
// Binary constant tests
//

0b1
0b01
0b101
0b111

0b011111111111111111111111111111111111111
0b11011111111111111111111111111111111111111
0b1000000000000000000000000000000000000000000000000000000000000000
0b0000000000000000000000000000000000000000000000000000000000000001


//
// Float/Double tests
//

42.f
42.0f
42.42f
.42f

42.
42.0
42.42
.42










//
// Positive Exponent tests (double)
//

42e1
42e2

42.e1
42.e2

42.0e1
42.0e2

.42e0
.42e1
.42e2

42e+1
42e+2

42.e+1
42.e+2

42.0e+1
42.0e+2

.42e+0
.42e+1
.42e+2


//
// Negative Exponent tests (double)
//

42e-1
42e-2

42.e-1
42.e-2

42.0e-1
42.0e-2

.42e-1
.42e-2

//
// Positive Exponent tests (double)
//

42E1
42E2

42.E1
42.E2

42.0E1
42.0E2

.42E0
.42E1
.42E2

42E+1
42E+2

42.E+1
42.E+2

42.0E+1
42.0E+2

.42E+0
.42E+1
.42E+2


//
// Negative Exponent tests (double)
//

42E-1
42E-2

42.E-1
42.E-2

42.0E-1
42.0E-2

.42E-1
.42E-2








//
// Positive Exponent tests (float)
//

42e1f
42e2f

42.e1f
42.e2f

42.0e1f
42.0e2f

.42e0f
.42e1f
.42e2f

42e+1f
42e+2f

42.e+1f
42.e+2f

42.0e+1f
42.0e+2f

.42e+0f
.42e+1f
.42e+2f


//
// Negative Exponent tests (float)
//

42e-1f
42e-2f

42.e-1f
42.e-2f

42.0e-1f
42.0e-2f

.42e-1f
.42e-2f

//
// Positive Exponent tests (float)
//

42E1f
42E2f

42.E1f
42.E2f

42.0E1f
42.0E2f

.42E0f
.42E1f
.42E2f

42E+1f
42E+2f

42.E+1f
42.E+2f

42.0E+1f
42.0E+2f

.42E+0f
.42E+1f
.42E+2f


//
// Negative Exponent tests (float)
//

42E-1f
42E-2f

42.E-1f
42.E-2f

42.0E-1f
42.0E-2f

.42E-1f
.42E-2f

#include <tests/parser/fixtures/preprocessor/boundary_conditions.cpp>

