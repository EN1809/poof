
#include <tests/integration/src/common_structures.h>

poof(
  func foobaz(type) @omit_include
  {
    foobaZ!
  }
)

struct thing
{
  int foo;
  int baz;
};

poof(foobaz(thing))
// tests/integration/generated/functions/foobaz_.h


poof(
  func func2(type) @omit_include
  {
    @are_equal(type, cs)?
    {
      // type == type
      type.hash == @cs.hash
    }
    {
      // type != type
      type.hash != cs.hash
    }
  }
)

poof(func2(thing))
// tests/integration/generated/functions/func2_thing.h

