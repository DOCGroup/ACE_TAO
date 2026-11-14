// *******************************************************************
//
// This test links in RTCORBA and RTPortableServer and creates two
// distinct RT ORB's.  It then activates an object using the RT POA
// from each RT ORB.  Prior to fixing the TAO_Policy_Validator,
// activating the second object would cause a segmentation fault.
// This was caused by the RT Policy Validator not being added correctly
// to the ORB Core's policy validator list.
//
// *******************************************************************

#include "tao/corba.h"
#include "testS.h"

class TestMonitorable_i : public virtual POA_Arch_Idl::Monitorable
{
public:
  TestMonitorable_i () = default;
  TestMonitorable_i (TestMonitorable_i const &) = delete;
  TestMonitorable_i (TestMonitorable_i &&) = delete;
  TestMonitorable_i &operator= (TestMonitorable_i const &) = delete;
  TestMonitorable_i &operator= (TestMonitorable_i &&) = delete;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb1 = CORBA::ORB_init (argc, argv, "orb1");
      CORBA::ORB_var const orb2 = CORBA::ORB_init (argc, argv, "orb2");

      CORBA::Object_var const poa_object1 = orb1->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa1 =
        PortableServer::POA::_narrow (poa_object1.in ());
      PortableServer::POAManager_var const poa_manager1 =
        root_poa1->the_POAManager ();

      CORBA::Object_var const poa_object2 = orb2->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa2 =
        PortableServer::POA::_narrow (poa_object2.in ());
      PortableServer::POAManager_var const poa_manager2 =
        root_poa2->the_POAManager ();

      poa_manager2->activate ();
      poa_manager1->activate ();

      TestMonitorable_i *monitorable_i {};
      ACE_NEW_RETURN (monitorable_i, TestMonitorable_i, 1);
      PortableServer::ServantBase_var const monitorable_var = monitorable_i;

      TestMonitorable_i *monitorable_i2 {};
      ACE_NEW_RETURN (monitorable_i2, TestMonitorable_i, 1);
      PortableServer::ServantBase_var const monitorable_var2 = monitorable_i2;

      PortableServer::ObjectId_var const monitorable_id1 =
        root_poa1->activate_object (monitorable_var.in ());
      CORBA::Object_var const object1 = root_poa1->id_to_reference (monitorable_id1.in ());
      Arch_Idl::Monitorable_var const monitorable1 = Arch_Idl::Monitorable::_narrow (object1.in ());

      PortableServer::ObjectId_var const monitorable_id2 =
        root_poa2->activate_object (monitorable_var2.in ());
      CORBA::Object_var const object2 = root_poa2->id_to_reference (monitorable_id2.in ());
      Arch_Idl::Monitorable_var const monitorable2 = Arch_Idl::Monitorable::_narrow (object2.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
