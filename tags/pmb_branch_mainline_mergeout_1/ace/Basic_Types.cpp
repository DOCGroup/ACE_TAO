#include "ace/OS.h"

#if !defined (__ACE_INLINE__)
# include "ace/Basic_Types.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (ace,
           Basic_Types,
           "$Id$")


#if defined (ACE_LACKS_LONGLONG_T)
# include "ace/Log_Msg.h"
# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
#  include "ace/streams.h"
# endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

void
ACE_U_LongLong::output (FILE *file) const
{
  if (h_ () > 0)
    ACE_OS::fprintf (file, "0x%lx%0*lx", h_ (), 2 * sizeof l_ (), l_ ());
  else
    ACE_OS::fprintf (file, "0x%lx", l_ ());
}


ACE_TCHAR *
ACE_U_LongLong::as_string (ACE_TCHAR *output,
                           unsigned int base,
                           unsigned int uppercase) const
{
  if (*this == 0)
    {
      ACE_OS::strcpy(output, "0");
    }
  else
    {
      switch(base)
        {
          case 8:
            {
              unsigned int index = 0;
              int bshift = 31;
              while(bshift >= 1)
                {
                  unsigned int sval = (this->h_ () >> bshift) & 7;
                  if (sval > 0 || index != 0)
                    {
                      output[index] = sval + '0';
                      index++;
                    }
                  bshift -= 3;
                }
              bshift = 30;
              while(bshift >= 0)
                {
                  unsigned int sval = (this->l_ () >> bshift) & 7;
                  // Combine the last bit of hi with the first 3-bit digit
                  if (bshift == 30)
                    {
                      sval |= (this->h_ () & 1) << 2;
                    }
                  if (sval > 0 || index != 0)
                    {
                      output[index] = sval + '0';
                      index++;
                    }
                  bshift -= 3;
                }
              output[index] = '\0';
              break;
            }
          case 10:
            {
              ACE_OS::sprintf(output, "%.0f", *this / 1.0);
              break;
            }
          case 16:
            {
              if (this->h_ () != 0)
                {
                  ACE_OS::sprintf(output,
                                  (uppercase ? "%lX%0*lX" : "%lx%0*lx"),
                                  this->h_ (), 2 * sizeof this->l_ (),
                                  this->l_ ());
                }
              else
                {
                  ACE_OS::sprintf(output,
                                  (uppercase ? "%lX" : "%lx"), this->l_ ());

                }
              break;
            }
          default:
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_LIB_TEXT ("Unsupported base = %u\n"), base));
              output[0] = '\0';
            }
        }
    }

  return output;
}


# if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
ostream&
operator<< (ostream& os, const ACE_U_LongLong& ll)
{
  unsigned long flags = os.setf(0);
  char buffer[32];

  if ((flags & ios::oct) != 0)
    os << ll.as_string (buffer, 8);
  else if ((flags & ios::hex) != 0)
    os << ll.as_string (buffer, 16, (flags & ios::uppercase));
  else
    os << ll.as_string (buffer);
  return os;
}
# endif


#endif /* ACE_LACKS_LONGLONG_T */

// Explicit template instantiation file
#include "ace/Template_Instantiations.cpp"
