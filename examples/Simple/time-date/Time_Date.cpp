// $Id$

#ifndef ACE_BUILD_SVC_DLL
# define ACE_BUILD_SVC_DLL
#endif

#ifndef Alt_Resource_Factory_BUILD_DLL
# define Alt_Resource_Factory_BUILD_DLL
#endif

#include "Time_Date_i.h"

#include "Time_Date.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/debug.h"
#include "ace/Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/Dynamic_Service.h"


ACE_Reactor *
My_Resource_Factory::get_reactor (void)
{
#if defined (ACE_HAS_THREADS)
  // Use whatever the default is if we've got threads.
  return TAO_Default_Resource_Factory::get_reactor ();
#else
  // Just use the Singleton Reactor.
  return ACE_Reactor::instance ();
#endif /* ACE_HAS_THREADS */
}

ACE_FACTORY_DEFINE (Alt_Resource_Factory, My_Resource_Factory)

int
DLL_ORB::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n\trunning ORB event loop (%t)\n\n"));

  try
    {
      // Run the ORB event loop in its own thread.
      this->orb_->run ();
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}

int
DLL_ORB::init (int argc, char *argv[])
{
  // Prevent TAO from registering with the ACE_Object_Manager so
  // that it can be dynamically unloaded successfully.

   int register_with_object_manager = 0;
   if (TAO_Singleton_Manager::instance ()->init (register_with_object_manager) == -1)
     {
//        ACE_ERROR_RETURN ((LM_ERROR,
//                          "%p\n",
//                          "Unable to pre-initialize ORB"),
//                         -1);
     }

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\tInitialize ORB (%t)\n\n"));

      // Initialize the ORB.
      // Initialize the ORB.
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "An ORB");

      if (CORBA::is_nil (this->orb_.in ()))
        return -1;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->poa_ =
        PortableServer::POA::_narrow (poa_object.in ());

      this->poa_manager_ =
        this->poa_->the_POAManager ();

      this->poa_manager_->activate ();

#if defined (ACE_HAS_THREADS)
      // Become an Active Object so that the ORB
      // will execute in a separate thread.
      return this->activate ();
#else
      return 0;
#endif /* ACE_HAS_THREADS */
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DLL_ORB::init");
    }

  return -1;
}

int
DLL_ORB::fini (void)
{
  return TAO_Singleton_Manager::instance ()->fini ();
  // return 0;
}

int
Time_Date_Servant::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:o:");
  int c = 0;

  this->orb_ = "ORB";

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
        // Find the ORB in the Service Repository.
      case 'n':
        this->orb_ = get_opts.opt_arg ();
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Time_Date_Servant::init (int argc, char *argv[])
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\tTime_Date servant\n\n"));

      this->parse_args (argc, argv);

      DLL_ORB *orb =
        ACE_Dynamic_Service<DLL_ORB>::instance (this->orb_);

      if (orb == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "can't find %s in the Service Repository\n",
                           this->orb_),
                          -1);

      Time_Date_i * servant = new Time_Date_i;
      PortableServer::ServantBase_var safe_servant = servant;

      CORBA::Object_var poa_object =
        orb->orb_->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      CORBA::Object_var obj =
        CORBA::Object::_narrow (object.in ());

      CORBA::String_var str =
        orb->orb_->object_to_string (obj.in ());

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s",
                           str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("DLL_ORB::init");
      return -1;
    }
  return 0;
}

// The following Factory is used by the <ACE_Service_Config> and
// dll_orb.conf file to dynamically initialize the state of the
// Time_Date service.
ACE_SVC_FACTORY_DEFINE (DLL_ORB)
ACE_SVC_FACTORY_DEFINE (Time_Date_Servant)

