// $Id$
#include "Options.h"
#include "ace/Get_Opt.h"
 
// Default Constructor
Options::Options ()
: debug_ (1),
  ior_output_file_ (0)
{
  // Nothing
}

int
Options::parse_args (int argc, ASYS_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "d:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        this->debug_ = ACE_OS::atoi (get_opts.optarg);
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Error: Unable to open %s for writing: %p\n",
                             get_opts.optarg), -1);
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s"
                           " [-d] <debug_level>"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Returns the debug level for the IR.  

int 
Options::debug (void) const
{
  return this->debug_;
}

// Returns the file where the IOR should be stored.

FILE *
Options::output_file (void) const
{
  return this->ior_output_file_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragme instantiate ACE_Singleton <Options, ACE_Null_Mutex>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
