// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
ACE_Get_Opt::ACE_Get_Opt_Long_Option::operator < (ACE_Get_Opt_Long_Option const &rhs)
{
  return this->name_ < rhs.name_;
}

ACE_INLINE int
ACE_Get_Opt::argc () const
{
  return this->argc_;
}

ACE_INLINE ACE_TCHAR **
ACE_Get_Opt::argv () const
{
  return this->argv_;
}

ACE_INLINE ACE_TCHAR*
ACE_Get_Opt::opt_arg () const
{
  return this->optarg;
}

ACE_INLINE int
ACE_Get_Opt::opt_opt ()
{
  return this->optopt_;
}

ACE_INLINE int &
ACE_Get_Opt::opt_ind ()
{
  return this->optind;
}

#ifdef ACE_USES_WCHAR
ACE_INLINE  ACE_Get_Opt::ACE_Get_Opt (int argc,
               ACE_TCHAR **argv,
               ACE_TCHAR const *optstring,
               int skip_args,
               int report_errors,
               int ordering,
               int long_only)
  : argc_ (argc),
    argv_ (argv),
    optind (skip_args),
    opterr (report_errors),
    optarg {},
    long_only_ (long_only != 0),
    has_colon_ (false),
    nextchar_ {},
    optopt_ {},
    ordering_ (ordering),
    nonopt_start_ (optind),
    nonopt_end_ (optind),
    long_option_ {}
{
  ACE_Get_Opt_Init (optstring);
}

ACE_INLINE  ACE_Get_Opt::ACE_Get_Opt (int argc,
               ACE_TCHAR **argv,
               char const *optstring,
               int skip_args,
               int report_errors,
               int ordering,
               int long_only)
  : argc_ (argc),
    argv_ (argv),
    optind (skip_args),
    opterr (report_errors),
    optarg {},
    long_only_ (long_only != 0),
    has_colon_ (false),
    nextchar_ {},
    optopt_ {},
    ordering_ (ordering),
    nonopt_start_ (optind),
    nonopt_end_ (optind),
    long_option_ {}
{
  ACE_Get_Opt_Init (ACE_TEXT_CHAR_TO_TCHAR (optstring));
}
#endif

ACE_END_VERSIONED_NAMESPACE_DECL
