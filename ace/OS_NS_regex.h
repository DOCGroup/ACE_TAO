// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_regex.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_REGEX_H
# define ACE_OS_NS_REGEX_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

#include "ace/os_include/os_regex.h"

// @@ Don - Not sure if this is proper.
#ifdef ACE_HAS_POSIX_REGEX
typedef ::regex_t ACE_regex_t;
typedef ::regmatch_t ACE_regmatch_t;
#else
struct ACE_regex_t {};
struct ACE_regmatch_t {};
#endif

namespace ACE_OS {

  // non=standard..
  //@{ @name A set of wrappers for regular expressions.
  ACE_NAMESPACE_INLINE_FUNCTION
  char *compile (const char *instring,
                 char *expbuf,
                 char *endbuf);

  ACE_NAMESPACE_INLINE_FUNCTION
  int step (const char *str,
            char *expbuf);
  //@}


  // POSIX regex functions  
  ACE_NAMESPACE_INLINE_FUNCTION
  int regcomp (ACE_regex_t *preg,
               const char *pattern,
               int cflags);
  
  ACE_NAMESPACE_INLINE_FUNCTION
  int regexec (const ACE_regex_t *preg,
               const char *string,
               size_t nmatch,
               ACE_regmatch_t *pmatch,
               int eflags);
  
  ACE_NAMESPACE_INLINE_FUNCTION
  int regerror (int errcode,
                const ACE_regex_t *preg,
                char *errbuf,
                size_t errbuf_size);
  
  ACE_NAMESPACE_INLINE_FUNCTION
  void regfree (ACE_regex_t *preg);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_regex.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_REGEX_H */
