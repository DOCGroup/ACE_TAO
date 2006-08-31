// -*- C++ -*-

//=============================================================================
/**
 * @file        Node_Daemon_Client.cpp
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#include "Node_DaemonC.h"
#include "Client_Options.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

//
// parse_args
//
int parse_args (int argc, char * argv[])
{
  // Setup the <ACE_Get_Opt> variable.
  const char * opts = "a:f:hlp:v";
  ACE_Get_Opt get_opt (argc, argv, opts);

  // Setup the long options for the command-line
  get_opt.long_option ("args", 'a', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("help", 'h', ACE_Get_Opt::NO_ARG);
  get_opt.long_option ("localhost", 'l', ACE_Get_Opt::NO_ARG);
  get_opt.long_option ("port", 'p', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("verbose", 'v', ACE_Get_Opt::NO_ARG);

  get_opt.long_option ("kill", ACE_Get_Opt::ARG_OPTIONAL);
  get_opt.long_option ("spawn", ACE_Get_Opt::ARG_OPTIONAL);
  get_opt.long_option ("shutdown", ACE_Get_Opt::NO_ARG);
  get_opt.long_option ("details", ACE_Get_Opt::NO_ARG);

  int option;
  while ((option = get_opt ()) != EOF)
  {
    switch (option)
    {
    case 0:
      if (ACE_OS::strcmp (get_opt.long_option (), "shutdown") == 0)
      {
        CLIENT_OPTIONS ()->shutdown_ = true;
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "kill") == 0)
      {
        CLIENT_OPTIONS ()->action_ = Client_Options::CA_KILL;

        if (get_opt.opt_arg () != 0)
        {
          size_t n = ACE_OS::atoi (get_opt.opt_arg ());

          if (n != 0)
          {
            CLIENT_OPTIONS ()->count_ = n;
          }
          else
          {
            ACE_ERROR ((LM_ERROR,
                        "invalid argument for option -%c\n",
                        get_opt.opt_arg ()));
          }
        }
      }
      else if (ACE_OS::strcmp (get_opt.long_option (), "spawn") == 0)
      {
        CLIENT_OPTIONS ()->action_ = Client_Options::CA_SPAWN;

        do
        {
          size_t n = ACE_OS::atoi (get_opt.opt_arg ());

          if (n != 0)
          {
            CLIENT_OPTIONS ()->count_ = n;
          }
          else
          {
            ACE_ERROR ((LM_ERROR,
                        "invalid argument for option -%c\n",
                        get_opt.opt_arg ()));
          }
        } while (0);
      }
      break;

    case 'a':
      // Get the arguments for the "spawn" operation.
      CLIENT_OPTIONS ()->args_ = get_opt.opt_arg ();
      break;

    case 'h':
      // we need to display the help then exit
      ACE_OS::exit (0);
      break;

    case 'l':
      CLIENT_OPTIONS ()->localhost_ = true;
      break;

    case 'p':
      // The user has opted to use a different port number.
      do
      {
        u_short port = ACE_OS::atoi (get_opt.opt_arg ());

        if (port != 0)
          CLIENT_OPTIONS ()->port_ = port;
        else
          ACE_ERROR ((LM_ERROR, "invalid port number\n"));
      } while (0);
      break;

    case 'v':
      CLIENT_OPTIONS ()->verbose_ = true;
      break;

    case '?':
      // unknown option; do nothing
      break;

    case ':':
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%c is missing an argument\n",
                         get_opt.opt_opt ()),
                         1);
      break;

    default:
      /* do nothing */;
    }
  }

  return 0;
}

//
// process_spawn_action
//
void process_spawn_action (::CUTS::Node_Daemon_ptr daemon)
{
  if (CLIENT_OPTIONS ()->count_ < 1)
    return;

  // We only permit one spawn type at a time from the client.
  ::CUTS::Spawn_Detail detail;
  detail.bindings.length (CLIENT_OPTIONS ()->count_);
  detail.params = ::CORBA::string_dup (CLIENT_OPTIONS ()->args_.c_str ());

  VERBOSE_MESSAGE ((LM_DEBUG,
                    "scheduled to spawn %u node manager(s)\n",
                    CLIENT_OPTIONS ()->count_));

  u_short port = CLIENT_OPTIONS ()->port_;

  for (size_t i = 0; i < CLIENT_OPTIONS ()->count_; i ++)
  {
    ::CUTS::Node_Binding & binding = detail.bindings[i];

    binding.localhost = CLIENT_OPTIONS ()->localhost_;
    binding.port = port ++;
  }

  // Signal the server to spawn the nodes.
  size_t spawn_count = daemon->spawn (detail);
  VERBOSE_MESSAGE ((LM_DEBUG,
                    "successfully spawned %u node manager(s)\n",
                    spawn_count));
}

//
// process_kill_action
//
void process_kill_action (::CUTS::Node_Daemon_ptr daemon)
{
  if (CLIENT_OPTIONS ()->count_ < 1)
    return;

  // Create a node bindings data type for killing.
  ::CUTS::Node_Bindings bindings (CLIENT_OPTIONS ()->count_);
  bindings.length (CLIENT_OPTIONS ()->count_);
  u_short port = CLIENT_OPTIONS ()->port_;

  // Initialize the node bindings.
  for (size_t i = 0; i < CLIENT_OPTIONS ()->count_; i ++)
  {
    ::CUTS::Node_Binding & binding = bindings[i];

    binding.localhost = CLIENT_OPTIONS ()->localhost_;
    binding.port = port ++;
  }

  // Signal the daemon to kill.
  daemon->kill (bindings);
}

//
// process_client_options
//
void process_client_options (::CUTS::Node_Daemon_ptr daemon)
{
  VERBOSE_MESSAGE ((LM_DEBUG, "processing client options\n"));

  switch (CLIENT_OPTIONS ()->action_)
  {
  case Client_Options::CA_SPAWN:
    process_spawn_action (daemon);
    break;

  case Client_Options::CA_KILL:
    process_kill_action (daemon);
    break;

  default:
    VERBOSE_MESSAGE ((LM_DEBUG, "no client options to process\n"));
  }
}

//
// main
//
int main (int argc, char * argv [])
{
  try
  {
    // initalize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    // Resolve the initiale reference to the Node_Daemon
    VERBOSE_MESSAGE ((LM_DEBUG,
                      "resolving initial reference to NodeDaemon\n"));
    ::CORBA::Object_var obj = orb->resolve_initial_references ("NodeDaemon");
    ::CORBA::String_var strobj = orb->object_to_string (obj.in ());

    VERBOSE_MESSAGE ((LM_DEBUG,
                      "NodeDaemon %s\n",
                      strobj.in ()));

    if (::CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR_RETURN ((
        LM_ERROR,
        "failed to resolved initial reference to NodeDaemon\n"),
        1);
    }

    // Narrow the generic object to a CUTS/Node_Daemon object.
    VERBOSE_MESSAGE ((LM_DEBUG,
                      "extracting node daemon from object reference\n"));

    ::CUTS::Node_Daemon_var daemon =
      ::CUTS::Node_Daemon::_narrow (obj.in ());

    if (::CORBA::is_nil (daemon.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "object was not a %s\n",
                         daemon->_interface_repository_id ()),
                         1);
    }

    process_client_options (daemon.in ());

    // Shutdown the target node daemon, if necessary.
    if (CLIENT_OPTIONS ()->shutdown_)
    {
      VERBOSE_MESSAGE ((LM_DEBUG,
                        "shutting down target node daemon\n"));
      daemon->shutdown ();
    }

    // Destroy the ORB.
    VERBOSE_MESSAGE ((LM_DEBUG, "destroying the ORB\n"));
    orb->destroy ();
    return 0;
  }
  catch (::CORBA::Exception & ex)
  {
    ACE_PRINT_TAO_EXCEPTION (ex, "caught exception");
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR, "caught unknown exception\n"));
  }

  return 1;
}
