// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_ctype.h"
#include "ace/Default_Constants.h"
#include "ace/os_include/os_errno.h"
#include "ace/OS_Memory.h"

template <typename CHAR_TYPE> int
ACE_OS::string_to_argv (CHAR_TYPE *buf,
                        int &argc,
                        CHAR_TYPE **&argv,
                        int substitute_env_args)
{
  // Reset the number of arguments
  argc = 0;

  if (buf == 0)
    return -1;

  CHAR_TYPE *cp = buf;

  // First pass: count arguments.

  // '#' is the start-comment token..
  while (*cp != CHAR_TYPE ('\0') && *cp != CHAR_TYPE ('#'))
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*cp))
        cp++;

      // Increment count and move to next whitespace..
      if (*cp != CHAR_TYPE ('\0'))
        argc++;

      while (*cp != CHAR_TYPE ('\0') && !ACE_OS::ace_isspace (*cp))
        {
          // Grok quotes....
          if (*cp == CHAR_TYPE ('\'') || *cp == CHAR_TYPE ('"'))
            {
              CHAR_TYPE quote = *cp;

              // Scan past the string..
              for (cp++; *cp != CHAR_TYPE ('\0') && *cp != quote; cp++)
                continue;

              // '\0' implies unmatched quote..
              if (*cp == CHAR_TYPE ('\0'))
                {
                  argc--;
                  break;
                }
              else
                cp++;
            }
          else
            cp++;
        }
    }

  // Second pass: copy arguments.
  CHAR_TYPE arg[ACE_DEFAULT_ARGV_BUFSIZ];
  CHAR_TYPE *argp = arg;

  // Make sure that the buffer we're copying into is always large
  // enough.
  if (cp - buf >= ACE_DEFAULT_ARGV_BUFSIZ)
    ACE_NEW_RETURN (argp,
                    CHAR_TYPE[cp - buf + 1],
                    -1);

  // Make a new argv vector of argc + 1 elements.
  ACE_NEW_RETURN (argv,
                  CHAR_TYPE *[argc + 1],
                  -1);

  CHAR_TYPE *ptr = buf;

  for (int i = 0; i < argc; i++)
    {
      // Skip whitespace..
      while (ACE_OS::ace_isspace (*ptr))
        ptr++;

      // Copy next argument and move to next whitespace..
      cp = argp;
      while (*ptr != CHAR_TYPE ('\0') && !ACE_OS::ace_isspace (*ptr))
        if (*ptr == CHAR_TYPE ('\'') || *ptr == CHAR_TYPE ('"'))
          {
            CHAR_TYPE quote = *ptr++;

            while (*ptr != CHAR_TYPE ('\0') && *ptr != quote)
              *cp++ = *ptr++;

            if (*ptr == quote)
              ptr++;
          }
        else
          *cp++ = *ptr++;

      *cp = CHAR_TYPE ('\0');

#if !defined (ACE_LACKS_ENV)
      // Check for environment variable substitution here.
      if (substitute_env_args)
      {
          ACE_TCHAR* dup = ACE_OS::strenvdup(ACE_TEXT_TO_TCHAR_IN(argp));
          argv[i] = ACE::String_Conversion::Convert_Out< CHAR_TYPE >(dup).c_str();
          delete[] dup;

          if (argv[i] == 0)
            {
              if (argp != arg)
                delete [] argp;
              errno = ENOMEM;
              return -1;
            }
      }
      else
#endif /* ACE_LACKS_ENV */
        {
          argv[i] = ACE_OS::strdup(argp);

          if (argv[i] == 0)
            {
              if (argp != arg)
                delete [] argp;
              errno = ENOMEM;
              return -1;
            }
        }
    }

  if (argp != arg)
    delete [] argp;

  argv[argc] = 0;
  return 0;
}
