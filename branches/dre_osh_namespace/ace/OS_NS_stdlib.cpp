// -*- C++ -*-
// $Id$

#include "ace/OS_NS_stdlib.h"

ACE_RCSID(ace, OS_NS_stdlib, "$Id$")

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_stdlib.inl"
#endif /* ACE_HAS_INLINED_OS_CALLS */

ACE_EXIT_HOOK ACE_OS::exit_hook_ = 0;

void *
ACE_OS::calloc (size_t elements, size_t sizeof_elements)
{
#if !defined (ACE_HAS_WINCE)
  return ACE_CALLOC_FUNC (elements, sizeof_elements);
#else
  // @@ This will probably not work since it doesn't consider
  // alignment properly.
  return ACE_MALLOC_FUNC (elements * sizeof_elements);
#endif /* ACE_HAS_WINCE */
}

void
ACE_OS::exit (int status)
{
  ACE_OS_TRACE ("ACE_OS::exit");

#if defined (ACE_HAS_NONSTATIC_OBJECT_MANAGER) && !defined (ACE_HAS_WINCE) && !defined (ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER)
  // Shut down the ACE_Object_Manager, if it had registered its exit_hook.
  // With ACE_HAS_NONSTATIC_OBJECT_MANAGER, the ACE_Object_Manager is
  // instantiated on the main's stack.  ::exit () doesn't destroy it.
  if (exit_hook_)
    (*exit_hook_) ();
#endif /* ACE_HAS_NONSTATIC_OBJECT_MANAGER && !ACE_HAS_WINCE && !ACE_DOESNT_INSTANTIATE_NONSTATIC_OBJECT_MANAGER */

#if !defined (ACE_HAS_WINCE)
# if defined (ACE_WIN32)
  ::ExitProcess ((UINT) status);
# elif defined (ACE_PSOSIM)
  ::u_exit (status);
# else
  ::exit (status);
# endif /* ACE_WIN32 */
#else
  // @@ This is not exactly the same as ExitProcess.  But this is the
  // closest one I can get.
  ::TerminateProcess (::GetCurrentProcess (), status);
#endif /* ACE_HAS_WINCE */
}

void
ACE_OS::free (void *ptr)
{
  ACE_FREE_FUNC (ACE_MALLOC_T (ptr));
}

// You may be asking yourself, why are we doing this?  Well, in winbase.h,
// MS didn't follow their normal Api_FunctionA and Api_FunctionW style,
// so we have to #undef their define to get access to the unicode version.
// And because we don't want to #undef this for the users code, we keep
// this method in the .cpp file.
#if defined (ACE_WIN32) && defined (UNICODE) && !defined (ACE_USES_TCHAR)
#undef GetEnvironmentStrings
#endif /* ACE_WIN32 && UNICODE !ACE_USES_TCHAR */

ACE_TCHAR *
ACE_OS::getenvstrings (void)
{
#if defined (ACE_LACKS_ENV)
  ACE_NOTSUP_RETURN (0);
#elif defined (ACE_WIN32)
# if defined (ACE_USES_WCHAR)
  return ::GetEnvironmentStringsW ();
# else /* ACE_USES_WCHAR */
  return ::GetEnvironmentStrings ();
# endif /* ACE_USES_WCHAR */
#else /* ACE_WIN32 */
  ACE_NOTSUP_RETURN (0);
#endif /* ACE_WIN32 */
}

#if !defined (ACE_HAS_ITOA)
char *
ACE_OS::itoa_emulation (int value, char *string, int radix)
{
  char *e = string;
  char *b = string;

  // Short circuit if 0

  if (value == 0)
    {
      string[0] = '0';
      string[1] = 0;
      return string;
    }

  // If negative and base 10, print a - and then do the
  // number.

  if (value < 0 && radix == 10)
    {
      string[0] = '-';
      ++b;
      ++e; // Don't overwrite the negative sign.
      value = -value; // Drop negative sign so character selection is correct.
    }

  // Convert to base <radix>, but in reverse order

  while (value != 0)
    {
      int mod = value % radix;
      value = value / radix;

      *e++ = (mod < 10) ? '0' + mod : 'a' + mod - 10;
    }

  *e-- = 0;

  // Now reverse the string to get the correct result

  while (e > b)
    {
      char temp = *e;
      *e = *b;
      *b = temp;
      ++b;
      --e;
    }

  return string;
}
#endif /* !ACE_HAS_ITOA */

#if defined (ACE_HAS_WCHAR) && defined (ACE_LACKS_ITOW)
wchar_t *
ACE_OS::itow_emulation (int value, wchar_t *string, int radix)
{
  wchar_t *e = string;
  wchar_t *b = string;

  // Short circuit if 0

  if (value == 0)
    {
      string[0] = '0';
      string[1] = 0;
      return string;
    }

  // If negative and base 10, print a - and then do the
  // number.

  if (value < 0 && radix == 10)
    {
      string[0] = '-';
      b++;
    }

  // Convert to base <radix>, but in reverse order

  while (value != 0)
    {
      int mod = value % radix;
      value = value / radix;

      *e++ = (mod < 10) ? '0' + mod : 'a' + mod - 10;
    }

  *e-- = 0;

  // Now reverse the string to get the correct result

  while (e > b)
  {
    wchar_t temp = *e;
    *e = *b;
    *b = temp;
    ++b;
    --e;
  }

  return string;
}
#endif /* ACE_HAS_WCHAR && ACE_LACKS_ITOW */

void *
ACE_OS::malloc (size_t nbytes)
{
  return ACE_MALLOC_FUNC (nbytes);
}

#if defined (ACE_LACKS_MKTEMP)
ACE_TCHAR *
ACE_OS::mktemp (ACE_TCHAR *s)
{
  ACE_OS_TRACE ("ACE_OS::mktemp");
  if (s == 0)
    // check for null template string failed!
    return 0;
  else
    {
      ACE_TCHAR *xxxxxx = ACE_OS::strstr (s, ACE_LIB_TEXT ("XXXXXX"));

      if (xxxxxx == 0)
        // the template string doesn't contain "XXXXXX"!
        return s;
      else
        {
          ACE_TCHAR unique_letter = ACE_LIB_TEXT ('a');
          ACE_stat sb;

          // Find an unused filename for this process.  It is assumed
          // that the user will open the file immediately after
          // getting this filename back (so, yes, there is a race
          // condition if multiple threads in a process use the same
          // template).  This appears to match the behavior of the
          // SunOS 5.5 mktemp().
          ACE_OS::sprintf (xxxxxx,
                           ACE_LIB_TEXT ("%05d%c"),
                           ACE_OS::getpid (),
                           unique_letter);
          while (ACE_OS::stat (s, &sb) >= 0)
            {
              if (++unique_letter <= ACE_LIB_TEXT ('z'))
                ACE_OS::sprintf (xxxxxx,
                                 ACE_LIB_TEXT ("%05d%c"),
                                 ACE_OS::getpid (),
                                 unique_letter);
              else
                {
                  // maximum of 26 unique files per template, per process
                  ACE_OS::sprintf (xxxxxx, ACE_LIB_TEXT ("%s"), ACE_LIB_TEXT (""));
                  return s;
                }
            }
        }
      return s;
    }
}
#endif /* ACE_LACKS_MKTEMP */

void *
ACE_OS::realloc (void *ptr, size_t nbytes)
{
  return ACE_REALLOC_FUNC (ACE_MALLOC_T (ptr), nbytes);
}

#if defined (ACE_LACKS_STRTOL)
long
ACE_OS::strtol_emulation (const char *nptr, char **endptr, int base)
{
  register const char *s = nptr;
  register unsigned long acc;
  register int c;
  register unsigned long cutoff;
  register int neg = 0, any, cutlim;

  /*
   * Skip white space and pick up leading +/- sign if any.
   * If base is 0, allow 0x for hex and 0 for octal, else
   * assume decimal; if base is already 16, allow 0x.
   */
  do {
    c = *s++;
  } while (isspace(c));
  if (c == '-') {
    neg = 1;
    c = *s++;
  } else if (c == '+')
    c = *s++;
  if ((base == 0 || base == 16) &&
    c == '0' && (*s == 'x' || *s == 'X')) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0)
    base = c == '0' ? 8 : 10;

  /*
   * Compute the cutoff value between legal numbers and illegal
   * numbers.  That is the largest legal value, divided by the
   * base.  An input number that is greater than this value, if
   * followed by a legal input character, is too big.  One that
   * is equal to this value may be valid or not; the limit
   * between valid and invalid numbers is then based on the last
   * digit.  For instance, if the range for longs is
   * [-2147483648..2147483647] and the input base is 10,
   * cutoff will be set to 214748364 and cutlim to either
   * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
   * a value > 214748364, or equal but the next digit is > 7 (or 8),
   * the number is too big, and we will return a range error.
   *
   * Set any if any `digits' consumed; make it negative to indicate
   * overflow.
   */
  cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
  cutlim = cutoff % (unsigned long)base;
  cutoff /= (unsigned long)base;
  for (acc = 0, any = 0;; c = *s++) {
    if (isdigit(c))
      c -= '0';
    else if (isalpha(c))
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    else
      break;
    if (c >= base)
      break;
    if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
      any = -1;
    else {
      any = 1;
      acc *= base;
      acc += c;
    }
  }
  if (any < 0) {
    acc = neg ? LONG_MIN : LONG_MAX;
    errno = ERANGE;
  } else if (neg)
    acc = -acc;
  if (endptr != 0)
    *endptr = any ? (char *)s - 1 : (char *)nptr;
  return (acc);
}
#endif /* ACE_LACKS_STRTOL */

#if defined (ACE_LACKS_STRTOUL)
unsigned long
ACE_OS::strtoul_emulation (const char *nptr,
                           char **endptr,
                           register int base)
{
  register const char *s = nptr;
  register unsigned long acc;
  register int c;
  register unsigned long cutoff;
  register int neg = 0, any, cutlim;

  /*
   * See strtol for comments as to the logic used.
   */
  do
    c = *s++;
  while (isspace(c));
  if (c == '-')
    {
      neg = 1;
      c = *s++;
    }
  else if (c == '+')
    c = *s++;
  if ((base == 0 || base == 16) &&
      c == '0' && (*s == 'x' || *s == 'X'))
    {
      c = s[1];
      s += 2;
      base = 16;
    }
  if (base == 0)
    base = c == '0' ? 8 : 10;
  cutoff = (unsigned long) ULONG_MAX / (unsigned long) base;
  cutlim = (unsigned long) ULONG_MAX % (unsigned long) base;

  for (acc = 0, any = 0;; c = *s++)
    {
      if (isdigit(c))
        c -= '0';
      else if (isalpha(c))
        c -= isupper(c) ? 'A' - 10 : 'a' - 10;
      else
        break;
      if (c >= base)
        break;
      if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
        any = -1;
      else
        {
          any = 1;
          acc *= base;
          acc += c;
        }
    }
  if (any < 0)
    {
      acc = ULONG_MAX;
      errno = ERANGE;
    } else if (neg)
    acc = -acc;
  if (endptr != 0)
    *endptr = any ? (char *) s - 1 : (char *) nptr;
  return (acc);
}
#endif /* ACE_LACKS_STRTOUL */

