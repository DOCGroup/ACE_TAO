//$Id$
#include "TimeModuleS.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/OS_NS_time.h"

class Time_impl :
  public POA_TimeModule::Time
{
public:
  virtual TimeModule::TimeOfDay
    get_gmt (void);

  void Shutdown (void);
};


TimeModule::TimeOfDay Time_impl::get_gmt (void)
{

  time_t time_now   = ACE_OS::time(0);
  struct tm *time_p = ACE_OS::gmtime(&time_now);

  TimeModule::TimeOfDay tod;

  tod.hour   = time_p->tm_hour;
  tod.minute = time_p->tm_min;
  tod.second = time_p->tm_sec;

  return tod;
}

void
Time_impl::Shutdown (void)
{
  ACE_OS::exit(0);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize orb
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Get reference to Root POA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var rootpoa =
        PortableServer::POA::_narrow (obj.in ());

      // Activate POA manager
      PortableServer::POAManager_var mgr =
        rootpoa->the_POAManager ();

      mgr->activate ();

      PortableServer::POA_var poa;

      TAO::Utils::PolicyList_Destroyer PolicyList (3);
      PolicyList.length (3);

      PolicyList [0] =
        rootpoa->create_lifespan_policy (PortableServer::PERSISTENT);

      PolicyList [1] =
        rootpoa->create_id_assignment_policy (PortableServer::USER_ID);

      CORBA::Any CallbackPolicy;
      CallbackPolicy <<= BiDirPolicy::BOTH;
      const char* sServerPoaName = "TelemetryServer";

      PolicyList [2] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            CallbackPolicy);

      poa = rootpoa->create_POA (sServerPoaName,
                                 mgr.in(),
                                 PolicyList);


      PortableServer::ObjectId_var ServerId =
        PortableServer::string_to_ObjectId ("TimeServer");

      // Create an object
      Time_impl *time_servant = new Time_impl;
      PortableServer::ServantBase_var self_manage (time_servant);

      poa->activate_object_with_id (ServerId.in (),
                                    time_servant);

      // Get a reference after activating the object
      CORBA::Object_var object = poa->id_to_reference (ServerId.in ());
      TimeModule::Time_var tm = TimeModule::Time::_narrow (object.in ());

      // Get reference to initial naming context
      CORBA::Object_var name_obj =
        orb->resolve_initial_references ("NameService");

      CosNaming::NamingContext_var inc =
        CosNaming::NamingContext::_narrow (name_obj.in ());

      if (CORBA::is_nil (inc.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Error fetching naming context \n"));
        }

      CosNaming::Name service_name;
      service_name.length(1);
      service_name[0].id   =
        CORBA::string_dup ("Time");

      inc->rebind (service_name,
                   tm.in ());

      // Run the event loop for fun
      ACE_Time_Value tv (3, 0);

      // Accept requests
      orb->run (&tv);

      rootpoa->destroy (0 , 0);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught an exception\n");

      return -1;
    }

  return 0;
}
