// $Id$
#include "Options.h"
#include "ace/Get_Opt.h"
#include "ace/Configuration.h"

// Default Constructor
Options::Options ()
: debug_ (1),
  ior_output_file_ (0),
  config_ (0),
  startup_timeout_ (5)
{
  // Nothing
}

Options::~Options ()
{
  delete config_;
}

int
Options::parse_args (int argc, ASYS_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "d:o:p:rt:");
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
      case 'p': // persistent heap implementation
        {
          ACE_Configuration_Heap* heap = new ACE_Configuration_Heap;
          if(heap->open(get_opts.optarg))
          {
            ACE_ERROR_RETURN((LM_ERROR, 
                              "Error:: Opening persistent heap file '%s'\n", 
                              get_opts.optarg), -2);
          }
          config_ = heap;
        }
        break;
#if defined(ACE_WIN32)
      case 'r': // win32 registry implementation
        {
          HKEY root = ACE_Configuration_Win32Registry::resolve_key(HKEY_LOCAL_MACHINE, "Software\\TAO\\IR");
          config_ = new ACE_Configuration_Win32Registry(root);
        }
        break;
#endif
      case 't': // Set timeout value
        this->startup_timeout_ = ACE_Time_Value (ACE_OS::atoi (get_opts.optarg));
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s"
                           " [-d] <debug_level>"
                           " [-o] <ior_output_file>"
                           " [-r]"
                           " [-p] <persistence_file>"
                           " [-t] <timeout in seconds>"
                           "\n",
                           argv [0]),
                          1);
      }

  // if no persistent implementation specified, use a simple heap
  if(!config_)
  {
    ACE_Configuration_Heap* heap = new ACE_Configuration_Heap;
    heap->open();
    config_ = heap;
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

const ACE_Time_Value &
Options::startup_timeout (void) const
{
  return this->startup_timeout_;
}


ACE_Configuration * 
Options::config () const
{
  return this->config_;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
