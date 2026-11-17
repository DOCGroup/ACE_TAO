/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 * for primitive and other self-managed types.
 *
 * @author Carlos O'Ryan
 */
#include "tao/Unbounded_Value_Sequence_T.h"
#include "tao/LongSeqC.h"
#include "ace/Log_Msg.h"

#include <utility>

struct Foo
{
  int x {};
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
    {
      using int_sequence = TAO::unbounded_value_sequence<int>;

      int_sequence a;
      int_sequence b(23);

      int_sequence c(32, 0, int_sequence::allocbuf(32), true);

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

      b.replace(64, 0, int_sequence::allocbuf(64), true);

      int const * x = d.get_buffer();
      if (x != nullptr)
      {
        int_sequence::freebuf(a.get_buffer(true));
      }
      x = b.get_buffer();

      int_sequence const e(c);

      using Foo_sequence = TAO::unbounded_value_sequence<Foo>;

      Foo_sequence u;
      Foo_sequence const v(32);
      u = v;

      CORBA::LongSeq idl_source (4);
      idl_source.length (2);
      idl_source[0] = 3;
      idl_source[1] = 7;

      CORBA::Long * const idl_buffer = idl_source.get_buffer ();
      CORBA::LongSeq idl_moved (std::move (idl_source));
      if (idl_moved.get_buffer () != idl_buffer
          || idl_moved[0] != 3
          || idl_moved[1] != 7)
      {
        return 1;
      }

      CORBA::LongSeq idl_move_assigned;
      CORBA::Long * const idl_move_buffer = idl_moved.get_buffer ();
      idl_move_assigned = std::move (idl_moved);
      if (idl_move_assigned.get_buffer () != idl_move_buffer
          || idl_move_assigned[0] != 3
          || idl_move_assigned[1] != 7)
      {
        return 1;
      }

      CORBA::Long external_buffer[] {5, 9};
      {
        CORBA::LongSeq borrowed_source (2, 2, external_buffer, false);
        CORBA::LongSeq borrowed_moved (std::move (borrowed_source));
        if (borrowed_moved.get_buffer () == external_buffer
            || !borrowed_moved.release ()
            || borrowed_moved[0] != 5
            || borrowed_moved[1] != 9
            || borrowed_source.get_buffer () != external_buffer
            || borrowed_source.length () != 2
            || borrowed_source.release ())
          {
            return 1;
          }
        external_buffer[0] = 1;
        if (borrowed_moved[0] != 5)
          {
            return 1;
          }

        external_buffer[0] = 5;
        CORBA::LongSeq borrowed_move_assigned (1);
        borrowed_move_assigned.length (1);
        borrowed_move_assigned[0] = 3;
        borrowed_move_assigned = std::move (borrowed_source);
        if (borrowed_move_assigned.get_buffer () == external_buffer
            || !borrowed_move_assigned.release ()
            || borrowed_move_assigned[0] != 5
            || borrowed_move_assigned[1] != 9
            || borrowed_source.get_buffer () != external_buffer
            || borrowed_source.length () != 2
            || borrowed_source.release ())
          {
            return 1;
          }
        external_buffer[0] = 2;
        if (borrowed_move_assigned[0] != 5 || borrowed_source[0] != 2)
          {
            return 1;
          }
      }
    }
  catch (const ::CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR, "Caught unexpected exception\n"));
      return 1;
    }

  return 0;
}
