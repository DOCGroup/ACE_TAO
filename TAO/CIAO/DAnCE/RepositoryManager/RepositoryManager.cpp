
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
 *       RepositoryManagerDeamon [int:nthreads]
 *
 * @author Stoyan Paunov
 */
//======================================================================

#include "RepositoryManager_Impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/streams.h"
#include "ace/Auto_Ptr.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

namespace CIAO
{
  namespace RepositoryManager
  {
    /// Name of the file holding the IOR of the RM
    const char * RMior = "RepositoryManagerDeamon.ior";

    /// Default number of worker threads to run in the multi-threaded RM
    static unsigned int nthreads = 3;
    static ACE_CString HTTPserver = "127.0.0.1:5432";
  }
}

//forward declaration
bool parse_args (int argc, char *argv[]);

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


///Main function

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    //init the ORB
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (!parse_args (argc, argv))
       return -1;

    //Get the root POA object
    CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

    //downcast to POA type
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());

    //activate the POA manager
    PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
    mgr->activate ();

    //create a servant
    CIAO_RepositoryManagerDaemon_i* repo;
    ACE_NEW_RETURN (repo,
                    CIAO_RepositoryManagerDaemon_i (
                          orb.in (),
                          CIAO::RepositoryManager::HTTPserver.c_str ()),
                    1);

    //trasfer ownership to the POA
    PortableServer::ServantBase_var owner_transfer(repo);

    //register and implicitly activate servant
    CIAO::RepositoryManagerDaemon_var RepositoryManagerDeamon = repo->_this ();

    //convert the IOR to string
    CORBA::String_var ior = orb->object_to_string (RepositoryManagerDeamon.in ());

    //output the IOR to a file
    FILE* ior_out = ACE_OS::fopen (CIAO::RepositoryManager::RMior, "w");

    if (ior_out == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                            CIAO::RepositoryManager::RMior),
                            1);

    ACE_OS::fprintf (ior_out, "%s", ior.in ());
    ACE_OS::fclose (ior_out);

    Worker worker (orb.in ());
    if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                         CIAO::RepositoryManager::nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate worker threads\n"),
                           1);

    worker.thr_mgr ()->wait ();

    ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

    //done
    return 0;

    //Start accepting requests
    orb->run ();

    //allow objects registered with the POA ot get cleaned-up
    root_poa->destroy (1, 1);

    //shutdown the orb
    orb->shutdown (1);

    return 0;
  }
  catch (CORBA::Exception &ex) {
    cerr << "CORBA Exception: " << ex << endl;

    return 1;
  }


  return 0;
}


// ****************************************************************

///Code to parse the arguments

    bool
    parse_args (int argc, char *argv[])
    {
      ACE_Get_Opt get_opts (argc, argv, "s:n:");
      int c;
      while ((c = get_opts ()) != -1)
        switch (c)
          {
          case 's':
            CIAO::RepositoryManager::HTTPserver = get_opts.opt_arg ();
            break;
          case 'n':
            CIAO::RepositoryManager::nthreads = ACE_OS::atoi (get_opts.opt_arg ());
            break;
          case '?':  // display help for use of the server.
            ACE_DEBUG ((LM_INFO,
                        "usage:  %s\n"
                        "-s <IP:PORT for HTTP server>\n"
                        "-n <number of threads>\n",
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
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return 0;
}


