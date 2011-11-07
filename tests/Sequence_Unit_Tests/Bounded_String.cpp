/**
 * @file
 *
 * @brief Smoke test (basically just compile) the bounded sequences
 *        for strings.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Bounded_Basic_String_Sequence_T.h"
#include "tao/CORBA_String.h"
#include "ace/Log_Msg.h"

CORBA::ULong const MAXIMUM = 42;
typedef TAO::bounded_basic_string_sequence<char, MAXIMUM> s_sequence;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  try
    {
      s_sequence a;
      s_sequence b(a);

      s_sequence c(0, s_sequence::allocbuf(), true);
      a = b;

      a.length(c.maximum());
      if (a.release())
      {
        b.length(a.length());
      }
      a[0] = const_cast<char const*>("Hello");
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

      b.replace(0, s_sequence::allocbuf(), true);

      char const * const * x = d.get_buffer();
      if (x != 0)
      {
        s_sequence::freebuf(a.get_buffer(true));
      }
      x = b.get_buffer();

      if (d.length())
      {
        s_sequence::freebuf(s_sequence::allocbuf());
      }

      s_sequence e(c);

      CORBA::String_var w(const_cast<char const*>("World"));
      try
        {
          e[0] = w;
#if defined (TAO_CHECKED_SEQUENCE_INDEXING) && (TAO_CHECKED_SEQUENCE_INDEXING == 1)
          return 1;
#endif
        }
      catch (const ::CORBA::BAD_PARAM &)
        {
          // e has length = 0, so there is an exception when we try
          // to access element above length.
        }
    }
  catch (const ::CORBA::Exception &)
    {
      ACE_ERROR ((LM_ERROR, "Caught unexpected exception\n"));
      return 1;
    }
  return 0;
}
