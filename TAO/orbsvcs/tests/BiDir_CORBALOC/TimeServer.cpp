//$Id$
#include "TimeModuleS.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"
#include "tao/AnyTypeCode/Any.h"

class Time_impl :
  public POA_TimeModule::Time
{
public:
  virtual TimeModule::TimeOfDay
    get_gmt (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void Shutdown (void)
    ACE_THROW_SPEC ((CORBA::SystemException));
};


TimeModule::TimeOfDay Time_impl::get_gmt (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  time_t time_now   = time(0);
  struct tm *time_p = gmtime(&time_now);

  TimeModule::TimeOfDay tod;

  tod.hour   = time_p->tm_hour;
  tod.minute = time_p->tm_min;
  tod.second = time_p->tm_sec;

  return tod;
}

void
Time_impl::Shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  exit(0);
}

int
main(int argc, char * argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize orb
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         ACE_ENV_ARG_PARAMETER);

      // Get reference to Root POA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);

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
        rootpoa->create_lifespan_policy (PortableServer::PERSISTENT
                                         ACE_ENV_ARG_PARAMETER);

      PolicyList [1] =
        rootpoa->create_id_assignment_policy (PortableServer::USER_ID
                                              ACE_ENV_ARG_PARAMETER);

      CORBA::Any CallbackPolicy;
      CallbackPolicy <<= BiDirPolicy::BOTH;
      const char* sServerPoaName = "TelemetryServer";

      PolicyList [2] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            CallbackPolicy
                            ACE_ENV_ARG_PARAMETER);

      poa = rootpoa->create_POA (sServerPoaName,
                                 mgr.in(),
                                 PolicyList
                                 ACE_ENV_ARG_PARAMETER);


      PortableServer::ObjectId_var ServerId =
        PortableServer::string_to_ObjectId ("TimeServer");

      // Create an object
      Time_impl *time_servant = new Time_impl;
      PortableServer::ServantBase_var self_manage (time_servant);

      poa->activate_object_with_id (ServerId.in (),
                                    time_servant
                                    ACE_ENV_ARG_PARAMETER);

      // Get a reference after activating the object
      TimeModule::Time_var tm = time_servant->_this();

      // Get reference to initial naming context
      CORBA::Object_var name_obj =
        orb->resolve_initial_references ("NameService"
                                         ACE_ENV_ARG_PARAMETER);

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
                   tm.in ()
                   ACE_ENV_ARG_PARAMETER);

      // Run the event loop for fun
      ACE_Time_Value tv (3, 0);

      // Accept requests
      orb->run (&tv
                ACE_ENV_ARG_PARAMETER);

      rootpoa->destroy (0 , 0 ACE_ENV_ARG_PARAMETER);

      orb->destroy ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught an exception\n");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
