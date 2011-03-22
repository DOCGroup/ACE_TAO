
/* -*- C++ -*- */

//======================================================================
/**
 * @file RepositoryManager.cpp
 *
 * $Id$
 *
 * Description:
 *  Main driver program for the CIAO RepositoryManager
 *  Please run as follows:
 *       RepositoryManagerDaemon [int:nthreads]
 *
 * @author Stoyan Paunov
 */
//======================================================================

#include "RepositoryManager_Impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/iosfwd.h"
#include "ace/Auto_Ptr.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

#include "orbsvcs/CosNamingC.h"

namespace CIAO
{
  namespace RepositoryManager
  {
    /// Name of the file holding the IOR of the RM
    const char * RMior = "RepositoryManagerDaemon.ior";

    // Name of RepoMan
    const char * repoman_name_ = "RepositoryManager";

    //Name service of the RM
    const char * RMname_service = "RepositoryManager";

    /// Default number of worker threads to run in the multi-threaded RM
    static unsigned int nthreads = 3;
    static ACE_CString HTTPserver = "127.0.0.1:5432";

    static bool register_with_ns_ = false;
    static bool write_to_ior_ = true;
  }
}

// Forward declaration
bool parse_args (int argc, ACE_TCHAR *argv[]);

/**
 * @class Worker
 *
 * Class that implements the service routine of the worker threads
 * of the repository manager
 */
class Worker : public ACE_Task_Base
{
public:
  /// ctor
  Worker (CORBA::ORB_ptr orb);

  /// The thread entry point.
  virtual int svc (void);

private:
  /// The orb
  CORBA::ORB_var orb_;
};

bool
write_ior_file (CORBA::ORB_ptr orb,
                CIAO::RepositoryManagerDaemon_ptr obj)
{
  CORBA::String_var ior = orb->object_to_string (obj);

  FILE* RMior_file = ACE_OS::fopen (DAnCE::RepositoryManager::RMior, "w");

  if (RMior_file)
    {
      ACE_OS::fprintf (RMior_file,
                       "%s",
                       ior.in ());
      ACE_OS::fclose (RMior_file);
    }
  else
    {
      return false;
    }

  return true;
}

bool
register_with_ns (CORBA::ORB_ptr orb,
                  CIAO::RepositoryManagerDaemon_ptr obj)
{
  if (ACE_OS::strlen (DAnCE::RepositoryManager::repoman_name_) > 0)
    {
      DAnCE::RepositoryManager::RMname_service =
        DAnCE::RepositoryManager::repoman_name_;
    }

  // Naming Service related operations
  CORBA::Object_var naming_context_object =
    orb->resolve_initial_references ("NameService");

  CosNaming::NamingContext_var naming_context =
    CosNaming::NamingContext::_narrow (naming_context_object.in ());

  // Initialize the Naming Sequence
  CosNaming::Name name (1);
  name.length (1);

  // String dup required for MSVC6
  name[0].id = CORBA::string_dup (DAnCE::RepositoryManager::RMname_service);

  // Register the servant with the Naming Service
  naming_context->rebind (name, obj);

  return true;
}

///Main function

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Init the ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (!parse_args (argc, argv))
       return -1;

    // Get the root POA object
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

    // Downcast to POA type
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

    // Activate the POA manager
    PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
    mgr->activate ();

    // Create a servant
    CIAO_RepositoryManagerDaemon_i* repo = 0;
    ACE_NEW_RETURN (repo,
                    CIAO_RepositoryManagerDaemon_i (
                          orb.in (),
                          DAnCE::RepositoryManager::HTTPserver.c_str (),
                          DAnCE::RepositoryManager::repoman_name_),
                    1);

    // Transfer ownership to the POA
    PortableServer::ServantBase_var owner_transfer(repo);
    PortableServer::ObjectId_var id =
      root_poa->activate_object (ci_srv);
    CORBA::Object_var repo_object = root_poa->id_to_reference (id.in ());
    CIAO::RepositoryManagerDaemon_var RepositoryManagerDaemon =
      CIAO::RepositoryManagerDaemon::_narrow (repo_object.in ());

    bool retval = false;

    if (DAnCE::RepositoryManager::write_to_ior_)
      {
        retval = write_ior_file (orb.in (), RepositoryManagerDaemon.in ());
      }
    if (retval && DAnCE::RepositoryManager::register_with_ns_)
      {
        retval = register_with_ns (orb.in (), RepositoryManagerDaemon.in ());
      }

    if (!retval)
      {
        return -1;
      }

    Worker worker (orb.in ());
    if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                         DAnCE::RepositoryManager::nthreads) != 0)
        DANCE_ERROR_RETURN (1, (LM_ERROR,
                           "Cannot activate worker threads\n"),
                           1);

    worker.thr_mgr ()->wait ();

    DANCE_DEBUG (6, (LM_DEBUG, "event loop finished\n"));

    orb->destroy ();
  }
  catch (const CORBA::Exception &ex) {
    cerr << "CORBA Exception: " << ex << endl;

    return 1;
  }

  return 0;
}


// ****************************************************************

///Code to parse the arguments

    bool
    parse_args (int argc, ACE_TCHAR *argv[])
    {
      ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ov:s:n:"));
      int c;
      while ((c = get_opts ()) != -1)
        switch (c)
          {
          case 'o':
            DAnCE::RepositoryManager::write_to_ior_ = true;
            DAnCE::RepositoryManager::register_with_ns_ = false;
            break;
          case 'v':
            DAnCE::RepositoryManager::write_to_ior_ = false;
            DAnCE::RepositoryManager::register_with_ns_ = true;
            DAnCE::RepositoryManager::repoman_name_ = get_opts.opt_arg ();
            break;
          case 's':
            DAnCE::RepositoryManager::HTTPserver = get_opts.opt_arg ();
            break;
          case 'n':
            DAnCE::RepositoryManager::nthreads = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case '?':  // display help for use of the server.
            DANCE_DEBUG (8, (LM_INFO,
                        ACE_TEXT("usage: %s\n")
                        ACE_TEXT("-o <using ior file>\n")
                        ACE_TEXT("-v <name of naming service>\n")
                        ACE_TEXT("-s <IP:PORT for HTTP server>\n")
                        ACE_TEXT("-n <number of threads>\n"),
                        argv [0]));
            return false;
            break;
          default:
            ;
          }

      return true;
    }



// ****************************************************************

///Constuctor for the worker class
Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

///implementation of the service routine inherited from ACE::Task_Base

int Worker::svc (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::Exception&)
    {
    }
  return 0;
}


