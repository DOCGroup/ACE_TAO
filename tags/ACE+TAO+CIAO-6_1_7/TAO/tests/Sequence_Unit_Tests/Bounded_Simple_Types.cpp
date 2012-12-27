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
#include "ace/Log_Msg.h"

struct Foo
{
  int x;
};

CORBA::ULong const TEST_INT_MAX = 32;
CORBA::ULong const TEST_FOO_MAX = 64;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
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
      try
        {
          c[0] = d[0];
#if defined (TAO_CHECKED_SEQUENCE_INDEXING) && (TAO_CHECKED_SEQUENCE_INDEXING == 1)
          return 1;
#endif
        }
      catch (const ::CORBA::BAD_PARAM &)
        {
          // c has length = 0, so there is an exception when we try
          // to access element above length.
        }

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

    }
  catch (const ::CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR, "Caught unexpected exception\n"));
      return 1;
    }
  return 0;
}
