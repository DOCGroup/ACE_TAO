/**
 * @file
 *
 * @brief Smoke test (basically just compile) the unbounded sequences
 *        for octets.
 *
 * $Id$
 *
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
#include "tao/Basic_Types.h"
#include "tao/Unbounded_Octet_Sequence_T.h"
#include "ace/Log_Msg.h"

typedef TAO::unbounded_value_sequence <CORBA::Octet> s_sequence;

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
      if (x != 0)
      {
        s_sequence::freebuf(a.get_buffer(true));
      }
      x = b.get_buffer();

      if (d.length())
      {
        s_sequence::freebuf(s_sequence::allocbuf(64));
      }

      s_sequence e(c);
    }
  catch (const ::CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR, "Caught unexpected exception\n"));
      return 1;
    }

  return 0;
}
