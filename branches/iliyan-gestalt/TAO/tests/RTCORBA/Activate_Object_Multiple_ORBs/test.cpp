// $Id$

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

class TestMonitorable_i: public virtual POA_Arch_Idl::Monitorable
{
public:
  TestMonitorable_i () {}
};


int main(int argc, char* argv[])
{
  try
    {
      CORBA::ORB_var orb1 =
                    CORBA::ORB_init(argc, argv, "orb1");
      CORBA::ORB_var orb2 =
                    CORBA::ORB_init(argc, argv, "orb2");

      CORBA::Object_var poaObj_var =
                    orb1->resolve_initial_references ("RootPOA");
      PortableServer::POA_var rootPoa =
                    PortableServer::POA::_narrow (poaObj_var.in ());
      PortableServer::POAManager_var poaManager =
                    rootPoa->the_POAManager ();

      CORBA::Object_var poaObj_var2 =
                    orb2->resolve_initial_references ("RootPOA");
      PortableServer::POA_var rootPoa2 =
                    PortableServer::POA::_narrow (poaObj_var2.in ());
      PortableServer::POAManager_var poaManager2 =
                    rootPoa2->the_POAManager ();

      poaManager2->activate ();
      poaManager->activate ();

      TestMonitorable_i* monitorable_i = 0;
      ACE_NEW_RETURN (monitorable_i, TestMonitorable_i, 1);
      PortableServer::ServantBase_var monitorable_var = monitorable_i;

      TestMonitorable_i* m2 = 0;
      ACE_NEW_RETURN (m2, TestMonitorable_i, 1);
      PortableServer::ServantBase_var m2_var = m2;

      PortableServer::ObjectId_var monitorableId1_var =
                    rootPoa->activate_object (monitorable_var.in ());
      CORBA::Object_var obj1 =
                    rootPoa->id_to_reference (monitorableId1_var.in ());
      Arch_Idl::Monitorable_var monitorable1 =
                    Arch_Idl::Monitorable::_narrow (obj1.in ());

      PortableServer::ObjectId_var monitorableId2_var =
                    rootPoa2->activate_object (m2_var.in ());
      CORBA::Object_var obj2 =
                    rootPoa2->id_to_reference (monitorableId2_var.in ());
      Arch_Idl::Monitorable_var monitorable2 =
                    Arch_Idl::Monitorable::_narrow (obj2.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
