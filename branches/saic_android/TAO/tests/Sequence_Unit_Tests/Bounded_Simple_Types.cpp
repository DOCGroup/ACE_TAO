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
#include "tao/Bounded_Value_Sequence_T.h"

struct Foo
{
  int x;
};

CORBA::ULong const TEST_INT_MAX = 32;
CORBA::ULong const TEST_FOO_MAX = 64;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  typedef TAO::bounded_value_sequence<int,TEST_INT_MAX> int_sequence;

  int_sequence a;
  int_sequence b(a);

  int_sequence c(0, int_sequence::allocbuf(), true);

  a = b;

  a.length(c.maximum());
  if (a.release())
  {
    b.length(a.length());
  }

  a[0] = 0;
  b[0] = a[0];

  int_sequence const & d = a;
  c[0] = d[0];

  b.replace(0, int_sequence::allocbuf(), true);

  int const * x = d.get_buffer();
  if (x != 0)
  {
    int_sequence::freebuf(a.get_buffer(true));
  }
  x = b.get_buffer();

  int_sequence e(c);

  typedef TAO::bounded_value_sequence<Foo,TEST_FOO_MAX> Foo_sequence;

  Foo_sequence u;
  Foo_sequence v(u);
  u = v;

  return 0;
}
