// -*- C++ -*-

//=============================================================================
/**
 * @file        Node_Daemon_Server.cpp
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#include "Node_Daemon_i.h"
#include "Server_Options.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Process_Mutex.h"
#include "ace/streams.h"

//
// parse_args
//
int parse_args (int argc, char * argv [])
{
  // Setup the <ACE_Get_Opt> variable.
  const char * opts = "vo:d:p:a:";
  ACE_Get_Opt get_opt (argc, argv, opts);

  // Setup the long options for the command-line
  get_opt.long_option ("working-directory", 'd', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("ior-file", 'o', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("path-to-node-manager", 'p', ACE_Get_Opt::ARG_REQUIRED);
  get_opt.long_option ("verbose", 'v', ACE_Get_Opt::NO_ARG);

  int option;
  while ((option = get_opt ()) != EOF)
  {
    switch (option)
    {
    case 'd':
      SERVER_OPTIONS ()->init_dir_ = get_opt.opt_arg ();
      break;

    case 'o':
      SERVER_OPTIONS ()->ior_file_ = get_opt.opt_arg ();
      break;

    case 'p':
      SERVER_OPTIONS ()->node_manager_path (get_opt.opt_arg ());
      break;

    case 'v':
      SERVER_OPTIONS ()->verbose_ = true ;
      break;

    case '?':
      /* unknown option; do nothing */
      break;

    case ':':
      ACE_ERROR ((LM_ERROR,
                  "%c is missing an argument\n",
                  get_opt.opt_opt ()));
      return -1;
      break;

    default:
      /* do nothing */;
    }
  }

  return 0;
}

//
// write_ior_to_file
//
void write_ior_to_file (const char * ior)
{
  if (SERVER_OPTIONS ()->ior_file_.empty () || ior == 0)
    return;

  try
  {
    // Open the IOR file for writing.
    ofstream iorfile;
    iorfile.open (SERVER_OPTIONS ()->ior_file_.c_str ());

    if (iorfile.is_open ())
    {
      if (SERVER_OPTIONS ()->verbose_)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "writing node daemon IOR to file %s\n",
                    SERVER_OPTIONS ()->ior_file_.c_str ()));
      }

      // Write the IOR to the file.
      iorfile << ior << std::endl;
      iorfile.close ();
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "failed to open %s for writing\n",
                  SERVER_OPTIONS ()->ior_file_.c_str ()));
    }
  }
  catch (::CORBA::Exception & ex)
  {
    ACE_PRINT_TAO_EXCEPTION (ex, "(%N:%l) caught CORBA exception");
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR, "(%N:%l) caught unknown exception\n"));
  }
}

//
// main
//
int main (int argc, char * argv [])
{
  // We only allow one instance of the daemon to run at a
  // time. This way we don't have any confusion as to which
  // one we are talking to.
  ACE_Process_Mutex process_lock ("cutsnode_d");
  ACE_Guard <ACE_Process_Mutex> guard (process_lock, 0);

  if (guard.locked () == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
                       "cutsnode_d is already active\n"),
                       1);
  }

  try
  {
    // Initalize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    if (parse_args (argc, argv) < 0)
      return 1;

    // Get a reference to the <IORTable>.
    VERBOSE_MESSAGE ((LM_DEBUG,
                      "resolving initial reference to IOR table\n"));

    ::CORBA::Object_var obj = orb->resolve_initial_references ("IORTable");
    ::IORTable::Table_var ior_table = ::IORTable::Table::_narrow (obj.in ());

    if (::CORBA::is_nil (ior_table.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) failed to resolve IOR table\n"),
                         1);
    }

    // Get a reference to the <RootPOA>
    VERBOSE_MESSAGE ((LM_DEBUG,
                      "resolving initial reference to RootPOA\n"));
    obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());

    // Activate the POAManager
    VERBOSE_MESSAGE ((LM_DEBUG,
                      "getting reference to POAManager\n"));
    PortableServer::POAManager_var mgr = poa->the_POAManager ();
    mgr->activate ();

    // Create a <CUTS::Node_Daemon>
    VERBOSE_MESSAGE ((LM_DEBUG,
                      "creating the node daemon server\n"));
    CUTS::Node_Daemon_i * daemon_i = 0;
    ACE_NEW_RETURN (daemon_i, CUTS::Node_Daemon_i (orb.in ()), 1);

    // Attempt the recover any lost nodes.
    size_t count = daemon_i->recover ();

    VERBOSE_MESSAGE ((LM_DEBUG,
                      "recovered %u node manager(s)\n",
                      count));

    // Activate the <CUTS::Node_Daemon> and write it's IOR to file.
    CUTS::Node_Daemon_var daemon = daemon_i->_this ();

    // Convert the object to its string format and write the
    // IOR to file and to the IOR table.
    ::CORBA::String_var objstr = orb->object_to_string (daemon);
    VERBOSE_MESSAGE ((LM_DEBUG, "NodeDaemon %s\n", objstr.in ()));

    write_ior_to_file (objstr.in ());
    ior_table->bind ("CUTS/NodeDaemon", objstr.in ());

    ::PortableServer::ServantBase_var servant = daemon_i;

    // Run the ORB...
    VERBOSE_MESSAGE ((LM_DEBUG, "activating node daemon ORB\n"));
    orb->run ();

    // Destroy the RootPOA.
    VERBOSE_MESSAGE ((LM_DEBUG, "destroying the RootPOA\n"));
    poa->destroy (1, 1);

    // Destroy the ORB.
    VERBOSE_MESSAGE ((LM_DEBUG, "destroying the ORB\n"))
    orb->destroy ();

    return 0;
  }
  catch (::CORBA::Exception & ex)
  {
    ACE_PRINT_TAO_EXCEPTION (ex, "(%N:%l) caught CORBA exception");
  }
  catch (...)
  {
    ACE_ERROR ((LM_ERROR, "(%N:%l) caught unknown exception\n"));
  }

  return 1;
}
