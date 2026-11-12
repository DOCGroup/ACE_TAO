#include "TimeModuleS.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "orbsvcs/CosNamingC.h"

#include <cstdlib>
#include <ctime>

class Time_impl :
  public POA_TimeModule::Time
{
public:
  Time_impl () = default;
  Time_impl (Time_impl const &) = delete;
  Time_impl (Time_impl &&) = delete;
  Time_impl &operator= (Time_impl const &) = delete;
  Time_impl &operator= (Time_impl &&) = delete;

  TimeModule::TimeOfDay get_gmt () override;

  void Shutdown () override;
};

TimeModule::TimeOfDay
Time_impl::get_gmt ()
{
  std::time_t const time_now = std::time (nullptr);
  // std::gmtime returns C runtime-owned storage.
  std::tm const * const time_value = std::gmtime (&time_now);

  TimeModule::TimeOfDay tod {};
  if (time_value == nullptr)
    return tod;

  tod.hour = time_value->tm_hour;
  tod.minute = time_value->tm_min;
  tod.second = time_value->tm_sec;

  return tod;
}

void
Time_impl::Shutdown ()
{
  std::exit (0);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var manager = root_poa->the_POAManager ();
      manager->activate ();

      PortableServer::POA_var poa;

      TAO::Utils::PolicyList_Destroyer policy_list (3);
      policy_list.length (3);
      policy_list[0] = root_poa->create_lifespan_policy (PortableServer::PERSISTENT);
      policy_list[1] = root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      CORBA::Any callback_policy {};
      callback_policy <<= BiDirPolicy::BOTH;
      char const * const server_poa_name = "TelemetryServer";
      policy_list[2] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            callback_policy);

      poa = root_poa->create_POA (server_poa_name,
                                  manager.in (),
                                  policy_list);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("TimeServer");

      Time_impl * const time_servant = new Time_impl;
      // Keep an owning servant reference until the POA has been destroyed.
      PortableServer::ServantBase_var self_manage (time_servant);
      poa->activate_object_with_id (server_id.in (), time_servant);

      CORBA::Object_var object = poa->id_to_reference (server_id.in ());
      TimeModule::Time_var time_ref = TimeModule::Time::_narrow (object.in ());

      CORBA::Object_var name_obj = orb->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (name_obj.in ());

      if (CORBA::is_nil (naming_context.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Error fetching naming context\n"),
                          -1);

      CosNaming::Name service_name (1);
      service_name.length (1);
      service_name[0].id = CORBA::string_dup ("Time");
      naming_context->rebind (service_name, time_ref.in ());

      ACE_Time_Value tv (3, 0);
      orb->run (&tv);
      root_poa->destroy (false, false);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught an exception");
      return -1;
    }

  return 0;
}
