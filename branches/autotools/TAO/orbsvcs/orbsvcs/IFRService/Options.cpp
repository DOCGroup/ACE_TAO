// $Id$

#include "Options.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

// Default Constructor
Options::Options ()
  : ior_output_file_ (ACE_OS::strdup ("if_repo.ior")),
    persistent_ (0),
    persistent_file_ (ACE_OS::strdup ("ifr_default_backing_store")),
    using_registry_ (0),
    enable_locking_ (0)
{
}

Options::~Options ()
{
  ACE_OS::free (this->ior_output_file_);
  ACE_OS::free (this->persistent_file_);
}

int
Options::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:pb:mr");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // Set the IOR output filename.
        this->ior_output_file_ = get_opts.opt_arg ();
        break;
      case 'p': // Make the IFR persistent
        this->persistent_ = 1;
        this->using_registry_ = 0;
        break;
      case 'b':
        this->persistent_file_ = get_opts.opt_arg ();
        break;
      case 'm':
#if defined (ACE_HAS_THREADS)
        this->enable_locking_ = 1;
#endif /* ACE_HAS_THREADS */
        break;
      case 'r': // win32 registry implementation
#if defined (ACE_WIN32)
        if (this->persistent_ == 0)
          this->using_registry_ = 1;
        break;
#else /* ACE_WIN32 */
        ACE_ERROR_RETURN ((
            LM_ERROR,
            ACE_TEXT ("parse_args: not a win32 platform\n")
          ),
          1
        );
#endif /* ACE_WIN32 */
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s"
                           " [-o] <ior_output_file>"
                           " [-r]"
                           " [-m]"
                           " [-p]"
                           " [-b] <persistence_file>"
                            "\n",
                           argv [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

const char *
Options::ior_output_file (void) const
{
  return this->ior_output_file_;
}

int
Options::persistent (void) const
{
  return this->persistent_;
}

const char *
Options::persistent_file (void) const
{
  return this->persistent_file_;
}

int
Options::using_registry (void) const
{
  return this->using_registry_;
}

int
Options::enable_locking (void) const
{
  return this->enable_locking_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */













