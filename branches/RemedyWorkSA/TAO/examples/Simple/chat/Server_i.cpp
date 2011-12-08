
//=============================================================================
/**
 *  @file    Server_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the Chat Server_i class.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#include "Server_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

Server_i::Server_i ()
  : ior_file_name_ (ACE_TEXT ("chat.ior"))
{
  Broadcaster_i *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Broadcaster_i (),
                    CORBA::NO_MEMORY ());
  this->broadcaster_i_ = tmp;
}

Server_i::~Server_i (void)
{
  // NO Op.
}

int
Server_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':  // get the file name to write to
        this->ior_file_name_ = get_opts.opt_arg ();
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s")
                           ACE_TEXT (" [-o] <ior_output_file>")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

int
Server_i::init (int argc,
                ACE_TCHAR *argv[])
{
  // Parse the command line options.
  if (this->parse_args(argc, argv) == -1)
    return -1;

  if (this->orb_manager_.init (argc,
                               argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("orb manager init failed")),
                      -1);

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Activate the servant.
  CORBA::String_var str =
    this->orb_manager_.activate (this->broadcaster_i_.in ());

  // Write the IOR to a file.
  this->write_IOR (str.in ());
  return 0;
}

int
Server_i::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Running chat server...\n")));

  // Run the main event loop for the ORB.
  int ret = this->orb_manager_.run ();
  if (ret == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Server_i::run")),
                      -1);
  return 0;
}

int
Server_i::write_IOR (const char* ior)
{
  FILE* ior_output_file_ =
    ACE_OS::fopen (this->ior_file_name_, "w");

  if (ior_output_file_)
    {
      ACE_OS::fprintf (ior_output_file_,
                       ACE_TEXT ("%s"),
                       ior);
      ACE_OS::fclose (ior_output_file_);
    }

  return 0;
}
