// $Id$

#include "ace/OS.h"
#if !defined (__ACE_INLINE__)
# include "ace/Basic_Types.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, Basic_Types, "$Id$")

#if defined (ACE_LACKS_LONGLONG_T)
#include "ace/SString.h"
#include <math.h>
#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
# include "ace/streams.h"
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

void
ACE_U_LongLong::output (FILE *file) const
{
  if (h_ () > 0)
    ACE_OS::fprintf (file, "0x%lx%0*lx", h_ (), 2 * sizeof l_ (), l_ ());
  else
    ACE_OS::fprintf (file, "0x%lx", l_ ());
}


ACE_CString
ACE_U_LongLong::as_string (u_int base,
                           u_int uppercase) const
{
  ACE_TCHAR output[32];
  u_int index = 0;

  if (*this == 0)
    {
      output[index] = '0';
      index++;
    }
  else
    {
      double val = *this / 1.0;
      while (val > 0)
        {
          double orig = val;
          val = floor(val / base);
          u_int digit = u_int(orig - (val * base));
          output[index] = (digit < 10 ? '0' + digit :
                              (uppercase ? 'A' : 'a') + digit - 10);
          index++;
        }
      for (u_int i = 0; i < (index / 2); i++)
        {
          ACE_TCHAR t = output[i];
          output[i] = output[(index - 1) - i];
          output[(index - 1) - i] = t;
        }
    }
  output[index] = '\0';
  return ACE_CString (output);
}


#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ostream&
operator<< (ostream& os, const ACE_U_LongLong& ll)
{
  ios::fmtflags flags = os.setf(0);

  if ((flags & ios::oct) != 0)
    {
      os << ll.as_string (8).c_str ();
    }
  else if ((flags & ios::hex) != 0)
    {
      os << ll.as_string (16, (flags & ios::uppercase)).c_str ();
    }
  else
    {
      os << ll.as_string ().c_str ();
    }
  return os;
}
#endif


#endif /* ACE_LACKS_LONGLONG_T */

// Explicit template instantiation file
#include "ace/Template_Instantiations.cpp"
