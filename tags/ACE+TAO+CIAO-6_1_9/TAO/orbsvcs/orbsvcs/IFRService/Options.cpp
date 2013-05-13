// $Id$

#include "orbsvcs/IFRService/Options.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

Options::Options ()
  : ior_output_file_ (ACE_OS::strdup ("if_repo.ior")),
    persistent_ (0),
    persistent_file_ (ACE_OS::strdup ("ifr_default_backing_store")),
    using_registry_ (0),
    enable_locking_ (0),
    support_multicast_(0)
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
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:pb:lm:r"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // Set the IOR output filename.
        ACE_OS::free (this->ior_output_file_);
        this->ior_output_file_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'p': // Make the IFR persistent
        this->persistent_ = 1;
        this->using_registry_ = 0;
        break;
      case 'b':
        ACE_OS::free (this->persistent_file_);
        this->persistent_file_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'l':
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
        ORBSVCS_ERROR_RETURN ((
            LM_ERROR,
            ACE_TEXT ("parse_args: not a win32 platform\n")
          ),
          1
        );
#endif /* ACE_WIN32 */
      case 'm':
        this->support_multicast_ = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case '?':  // display help for use of the server.
      default:
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s"
                           " [-o] <ior_output_file>"
                           " [-r]"
                           " [-l]"
                           " [-m] <0|1>"
                           " [-p]"
                           " [-b] <persistence_file>\n\n"
                           "  -o  <filename> Output service IOR to <filename>\n"
                           "  -r  Persist contents in the win32 registry\n"
                           "  -l  Enable locking (off by default)\n"
                           "  -m  <0|1> Enable multicast discovery of this service\n"
                           "  -p  Persist contents using a memory mapped file\n"
                           "  -b  <filename> Used with the file persistence option. Specifies the storage file name.\n"
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

int
Options::support_multicast_discovery (void) const
{
  return this->support_multicast_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
