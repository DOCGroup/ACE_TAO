/* -*- C++ -*- */
// $Id$

// Get_Opt.i

ACE_INLINE 
ACE_Get_Opt::ACE_Get_Opt (int argc, 
		  char **argv,
		  char *optstring, 
		  int skip, 
		  int report_errors)
  : opterr (report_errors), 
    nextchar (0), 
    optarg (0), 
    first_nonopt (skip), 
    last_nonopt (skip), 
    optind (skip),
    noptstring (optstring), 
    nargc (argc), 
    nargv (argv)
{
  ACE_TRACE ("ACE_Get_Opt::ACE_Get_Opt");
}
