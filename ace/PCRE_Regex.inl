/* -*- C++ -*- */
// $Id$
// PCRE_Regex.inl

ACE_INLINE ACE_pcre *
ACE_Regex::compile (const char *pattern,
                    int options,
                    const char **errptr,
                    int *erroffset,
                    const unsigned char *tableptr)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_compile(pattern, options, errptr, erroffset, tableptr);
#else
  ACE_UNUSED_ARG (pattern);
  ACE_UNUSED_ARG (options);
  ACE_UNUSED_ARG (errptr);
  ACE_UNUSED_ARG (erroffset);
  ACE_UNUSED_ARG (tableptr);
  
  ACE_NOTSUP_RETURN (0);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::config (int what, void *where)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_config (what, where);
#else
  ACE_UNUSED_ARG (what);
  ACE_UNUSED_ARG (where);
  
  ACE_NOTSUP_RETURN (-1);
#endif
}

ACE_INLINE int
ACE_Regex::copy_named_substring (const ACE_pcre *code,
                                 const char *subject,
                                 int *ovector,
                                 int stringcount,
                                 const char *stringname,
                                 char *buffer,
                                 int buffersize)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_copy_named_substring (code, subject, ovector, stringcount,
                                      stringname, buffer, buffersize);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (subject);
  ACE_UNUSED_ARG (ovector);
  ACE_UNUSED_ARG (stringcount);
  ACE_UNUSED_ARG (stringname);
  ACE_UNUSED_ARG (buffer);
  ACE_UNUSED_ARG (buffersize);
  
    
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::copy_substring (const char *subject,
                           int *ovector,
                           int stringcount,
                           int stringnumber,
                           char *buffer,
                           int buffersize)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_copy_substring (subject, ovector, stringcount, stringnumber,
                           buffer, buffersize);
#else
  ACE_UNUSED_ARG (subject);
  ACE_UNUSED_ARG (ovector);
  ACE_UNUSED_ARG (stringcount);
  ACE_UNUSED_ARG (stringnumber);
  ACE_UNUSED_ARG (buffer);
  ACE_UNUSED_ARG (buffersize);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::exec (const ACE_pcre *code,
                 const ACE_pcre_extra *extra,
                 const char *subject,
                 int length, int startoffset,
                 int options, int *ovector, int ovecsize)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_exec (code, extra, subject, length, startoffset,
                      options, ovector, ovecsize);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (extra);
  ACE_UNUSED_ARG (subject);
  ACE_UNUSED_ARG (length);
  ACE_UNUSED_ARG (startoffset);
  ACE_UNUSED_ARG (options);
  ACE_UNUSED_ARG (ovector);
  ACE_UNUSED_ARG (ovecsize);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}


ACE_INLINE void
ACE_Regex::free_substring (const char *stringptr)
{
#ifdef ACE_HAS_PCRE
  ::pcre_free_substring (stringptr);
#else
  ACE_UNUSED_ARG (stringptr);
  
  ACE_NOTSUP;
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE void
ACE_Regex::free_substring_list (const char **stringptr)
{
#ifdef ACE_HAS_PCRE
  ::pcre_free_substring_list (stringptr);
#else
  ACE_UNUSED_ARG (stringptr);
  
  ACE_NOTSUP;
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::fullinfo (const ACE_pcre *code,
                     const ACE_pcre_extra *extra,
                     int what, void *where)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_fullinfo (code, extra, what, where);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (extra);
  ACE_UNUSED_ARG (what);
  ACE_UNUSED_ARG (where);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::get_named_substring (const ACE_pcre *code,
                                const char *subject,
                                int *ovector, int stringcount,
                                const char *stringname,
                                const char **stringptr)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_get_named_substring (code, subject, ovector, stringcount,
                                     stringname, stringptr);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (subject);
  ACE_UNUSED_ARG (ovector);
  ACE_UNUSED_ARG (stringcount);
  ACE_UNUSED_ARG (stringname);
  ACE_UNUSED_ARG (stringptr);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::get_stringnumber (const ACE_pcre *code,
                             const char *name)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_get_stringnumber (code, name);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (name);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::get_substring (const char *subject,
                          int *ovector, int stringcount, int stringnumber,
                          const char **stringptr)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_get_substring (subject, ovector, stringcount, stringnumber,
                               stringptr);
#else
  ACE_UNUSED_ARG (subject);
  ACE_UNUSED_ARG (ovector);
  ACE_UNUSED_ARG (stringcount);
  ACE_UNUSED_ARG (stringnumber);
  ACE_UNUSED_ARG (stringptr);

  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE int
ACE_Regex::fullinfo (const ACE_pcre *code,
                 int *optptr, int *firstcharptr)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_info (code, optptr, firstcharptr);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (optptr);
  ACE_UNUSED_ARG (firstcharptr);
  
  ACE_NOTSUP_RETURN (-1);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE const unsigned char *
ACE_Regex::maketables (void)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_maketables ();
#else
  ACE_NOTSUP_RETURN (0);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE ACE_pcre_extra *
ACE_Regex::study (const ACE_pcre *code,
                  int options,
                  const char **errptr)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_study (code, options, errptr);
#else
  ACE_UNUSED_ARG (code);
  ACE_UNUSED_ARG (options);
  ACE_UNUSED_ARG (errptr);
  
  ACE_NOTSUP_RETURN (0);
#endif /*ACE_HAS_PCRE*/
}

ACE_INLINE const char *
ACE_Regex::version (void)
{
#ifdef ACE_HAS_PCRE
  return ::pcre_version ();
#else
  ACE_NOTSUP_RETURN (0);
#endif /*ACE_HAS_PCRE*/
}
