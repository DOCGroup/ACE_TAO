
//=============================================================================
/**
 *  @file     Single_Threaded_POA.cpp
 *
 *  $Id$
 *
 *   This program tests to make sure that two threads cannot call
 *   servants in a single threaded POA simultaneously. At the same
 *   time, it makes sure that a servant can call itself or other
 *   servants in the same POA while in an upcall.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

class test_i : public virtual POA_test
{
public:
  test_i (PortableServer::POA_ptr poa);

  void method (void);

  PortableServer::POA_ptr _default_POA (void);

private:
  PortableServer::POA_var poa_;
  int called_self_;
};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    called_self_ (0)
{
}

void
test_i::method (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Entering Worker::svc from %t and sleeping....\n"));

  ACE_OS::sleep (2);

  ACE_DEBUG ((LM_DEBUG,
              "Done resting from %t\n"));

  if (this->called_self_ == 0)
    {
      this->called_self_ = 1;

      ACE_DEBUG ((LM_DEBUG,
                  "Calling self from %t\n"));

      PortableServer::ObjectId_var id =
        this->poa_->activate_object (this);

      CORBA::Object_var object = this->poa_->id_to_reference (id.in ());

      test_var self = test::_narrow (object.in ());

      self->method ();
    }
}

PortableServer::POA_ptr
test_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

class Worker : public ACE_Task_Base
{
public:
  Worker (test_ptr t);
  int svc (void);

private:
  test_var test_;
};

Worker::Worker (test_ptr t)
  : test_ (test::_duplicate (t))
{
}

int
Worker::svc (void)
{
  try
    {
      this->test_->method ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in thread");
      return -1;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Policies for the new POA.
      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION);

      policies[1] =
        root_poa->create_thread_policy (PortableServer::SINGLE_THREAD_MODEL);

      // Creation of the child POA.
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      // Destroy the policies
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy ();
        }

      poa_manager->activate ();

      test_i servant1 (child_poa.in ());
      test_i servant2 (child_poa.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&servant1);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      test_var object1 = test::_narrow (object_act.in ());

      id = root_poa->activate_object (&servant2);

      object_act = root_poa->id_to_reference (id.in ());

      test_var object2 = test::_narrow (object_act.in ());

      Worker worker1 (object1.in ());
      Worker worker2 (object2.in ());

      int result =
        worker1.activate () != 0 ||
        worker2.activate () != 0;
      ACE_ASSERT (result == 0);

      result = ACE_Thread_Manager::instance ()->wait ();
      ACE_ASSERT (result == 0);

      // In non-debug compiles, asserts will disappear.
      ACE_UNUSED_ARG (result);

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
