/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 *        for octets.
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#include "tao/Basic_Types.h"
#include "tao/OctetSeqC.h"
#include "tao/Unbounded_Octet_Sequence_T.h"
#include "ace/Log_Msg.h"

#include <utility>

using s_sequence = TAO::unbounded_value_sequence <CORBA::Octet>;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
    {
      s_sequence a;
      s_sequence b(23);

      s_sequence c(32, 0, s_sequence::allocbuf(32), true);
      a = b;

      a.length(c.maximum());
      if (a.release())
      {
        b.length(a.length());
      }

      a[0] = 'a';
      b[0] = a[0];

      s_sequence const & d = a;
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

      b.replace(64, 0, s_sequence::allocbuf(64), true);

      CORBA::Octet const * x = d.get_buffer();
      if (x != nullptr)
      {
        s_sequence::freebuf(a.get_buffer(true));
      }
      x = b.get_buffer();

      if (d.length())
      {
        s_sequence::freebuf(s_sequence::allocbuf(64));
      }

      s_sequence const e(c);

      CORBA::OctetSeq idl_source (4);
      idl_source.length (2);
      idl_source[0] = 3;
      idl_source[1] = 7;
      CORBA::OctetSeq idl_copy (idl_source);
      if (idl_copy[0] != 3 || idl_copy[1] != 7)
        return 1;

      CORBA::OctetSeq idl_copy_assigned;
      idl_copy_assigned = idl_source;
      if (idl_copy_assigned[0] != 3 || idl_copy_assigned[1] != 7)
        return 1;

      CORBA::Octet * const idl_buffer = idl_source.get_buffer ();
      CORBA::OctetSeq idl_moved (std::move (idl_source));
      if (idl_moved.get_buffer () != idl_buffer
          || idl_moved[0] != 3
          || idl_moved[1] != 7)
        return 1;
      if (idl_source.maximum () != 0
          || idl_source.length () != 0
          || idl_source.release ())
        return 1;

      CORBA::OctetSeq idl_move_assigned (3);
      idl_move_assigned.length (1);
      idl_move_assigned[0] = 9;
      CORBA::Octet * const idl_move_buffer = idl_moved.get_buffer ();
      idl_move_assigned = std::move (idl_moved);
      if (idl_move_assigned.get_buffer () != idl_move_buffer
          || idl_move_assigned[0] != 3
          || idl_move_assigned[1] != 7)
        return 1;
      if (idl_moved.maximum () != 0
          || idl_moved.length () != 0
          || idl_moved.release ())
        return 1;
    }
  catch (const ::CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR, "Caught unexpected exception\n"));
      return 1;
    }

  return 0;
}
