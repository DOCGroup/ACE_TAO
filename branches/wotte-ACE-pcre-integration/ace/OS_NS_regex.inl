// -*- C++ -*-
// $Id$

#include "ace/OS_NS_errno.h"
#include "ace/os_include/os_regex.h"

ACE_INLINE char *
ACE_OS::compile (const char *instring, char *expbuf, char *endbuf)
{
  ACE_OS_TRACE ("ACE_OS::compile");
#if defined (ACE_HAS_REGEX)
  ACE_OSCALL_RETURN (::compile (instring, expbuf, endbuf), char *, 0);
#else
  ACE_UNUSED_ARG (instring);
  ACE_UNUSED_ARG (expbuf);
  ACE_UNUSED_ARG (endbuf);

  ACE_NOTSUP_RETURN (0);
#endif /* ACE_HAS_REGEX */
}

ACE_INLINE int
ACE_OS::step (const char *str, char *expbuf)
{
  ACE_OS_TRACE ("ACE_OS::step");
#if defined (ACE_HAS_REGEX)
  ACE_OSCALL_RETURN (::step (str, expbuf), int, -1);
#else
  ACE_UNUSED_ARG (str);
  ACE_UNUSED_ARG (expbuf);

  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_REGEX */
}


ACE_INLINE int
ACE_OS::regcomp (ACE_regex_t *preg,
                 const char *pattern,
                 int cflags)
{
  ACE_OS_TRACE ("ACE_OS::regcomp");
#ifdef ACE_HAS_POSIX_REGEX
  ACE_OSCALL_RETURN (::regcomp (preg, pattern, cflags), int, -1);
#else
  ACE_UNUSED_ARG (preg);
  ACE_UNUSED_ARG (pattern);
  ACE_UNUSED_ARG (cflags);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_POSIX_REGEX*/
}

ACE_INLINE int
ACE_OS::regexec (const ACE_regex_t *preg,
                 const char *the_string,
                 size_t nmatch,
                 ACE_regmatch_t *pmatch,
                 int eflags)
{
  ACE_OS_TRACE ("ACE_OS::regcomp");
#ifdef ACE_HAS_POSIX_REGEX
  ACE_OSCALL_RETURN (::regexec (preg, the_string, nmatch,
                                pmatch, eflags), int, -1);
#else
  ACE_UNUSED_ARG (preg);
  ACE_UNUSED_ARG (the_string);
  ACE_UNUSED_ARG (nmatch);
  ACE_UNUSED_ARG (pmatch);
  ACE_UNUSED_ARG (eflags);

  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_POSIX_REGEX*/
}

ACE_INLINE int
ACE_OS::regerror (int errcode,
                  const ACE_regex_t *preg,
                  char *errbuf,
                  size_t errbuf_size)
{
  ACE_OS_TRACE ("ACE_OS::regerror");
#ifdef ACE_HAS_POSIX_REGEX
  ACE_OSCALL_RETURN (::regerror (errcode, preg, errbuf,
                                 errbuf_size), int, -1);
#else
   ACE_UNUSED_ARG (errcode);
   ACE_UNUSED_ARG (preg);
   ACE_UNUSED_ARG (errbuf);
   ACE_UNUSED_ARG (errbuf_size);

   ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_POSIX_REGEX */
}

ACE_INLINE void
ACE_OS::regfree (ACE_regex_t *preg)
{
  ACE_OS_TRACE ("ACE_OS::regfree");
#ifdef ACE_HAS_POSIX_REGEX
  ACE_OSCALL_RETURN (::regfree (preg), void, -1);
#else
  ACE_UNUSED_ARG (preg);
#endif /*ACE_HAS_POSIX_REGEX*/
}
