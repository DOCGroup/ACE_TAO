// -*- C++ -*-

/**
 * @file PCRE_Regex.h
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */

#ifndef ACE_PCRE_REGEX_H
#define ACE_PCRE_REGEX_H

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Pull in includes and define types needed for PCRE
#if defined (ACE_HAS_PCRE)
//# include "pcre/pcre.h"
# include "pcre.h"
  typedef ::pcre ACE_pcre;
  typedef ::pcre_extra ACE_pcre_extra;
#else
  struct ACE_pcre {};
  struct ACE_pcre_extra {};
#endif /*ACE_HAS_PCRE*/

/**
 * @class ACE_Regex
 *
 * @brief Provides a wrapper for the PCRE regex library.
 *
 * This class provides an interface for perl compatible regular
 * expressions using the PCRE library. If you want a POSIX interface,
 * use OS_NS_regex.h
 *
 * Please see documentation included with PCRE for full usage
 * explanations.
 */
class ACE_Export ACE_Regex
{
public:
  static ACE_pcre *compile (const char *pattern,
                            int options,
                            const char **errptr,
                            int *erroffset,
                            const unsigned char *tableptr);
  
  static int config (int what, void *where);
  
  static int copy_named_substring (const ACE_pcre *code,
                                   const char *subject,
                                   int *ovector,
                                   int stringcount,
                                   const char *stringname,
                                   char *buffer,
                                   int buffersize);
  
  static int copy_substring (const char *subject,
                             int *ovector,
                             int stringcount,
                             int stringnumber,
                             char *buffer,
                             int buffersize);
  
  static int exec (const ACE_pcre *code,
                   const ACE_pcre_extra *extra,
                   const char *subject,
                   int length, int startoffset,
                   int options, int *ovector, int ovecsize);
  
  static void free_substring (const char *stringptr);
  
  static void free_substring_list (const char **stringptr);
  
  static int fullinfo (const ACE_pcre *code,
                       const ACE_pcre_extra *extra,
                       int what, void *where);
  
  static int get_named_substring (const ACE_pcre *code,
                                  const char *subject,
                                  int *ovector, int stringcount,
                                  const char *stringname,
                                  const char **stringptr);
  
  static int get_stringnumber (const ACE_pcre *code,
                               const char *name);
  
  static int get_substring (const char *subject,
                            int *ovector, int stringcount, int stringnumber,
                            const char **stringptr);
  
  static int get_substring_list (const char *subject,
                                 int *ovector, int stringcount,
                                 const char ***listptr);
  
  static int fullinfo (const ACE_pcre *,
                       int *optptr, int *firstcharptr);
  
  static const unsigned char *maketables (void);
  
  static ACE_pcre_extra *study (const ACE_pcre *code,
                                int options,
                                const char **errptr);
  
  static const char *version (void);
  
protected:
  ACE_Regex ();
  
};

                     

#if defined (__ACE_INLINE__)
#include "ace/PCRE_Regex.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACE_PCRE_REGEX_H */
