/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 * for primitive and other self-managed types.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "sequence.hpp"

struct Foo
{
  int x;
};

CORBA::ULong const TEST_INT_MAX = 32;
CORBA::ULong const TEST_FOO_MAX = 64;

int main()
{
  typedef TAO::bounded_sequence<int,TEST_INT_MAX> int_sequence;

  int_sequence a;
  int_sequence b(a);

  a = b;

  typedef TAO::bounded_sequence<Foo,TEST_FOO_MAX> Foo_sequence;

  Foo_sequence c;
  Foo_sequence d(c);
  d = c;

  return 0;
}

// We use explicit template instantiation to force the instantiation
// of all member function and thus tests the full class.  This should
// work across all platforms, even on platforms that do not *require*
// explicit instantiation of templates.
template class TAO::bounded_sequence<int,TEST_INT_MAX>;
template class TAO::bounded_sequence<Foo,TEST_FOO_MAX>;
