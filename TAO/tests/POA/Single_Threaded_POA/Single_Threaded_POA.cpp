// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Single_Threaded_POA
//
// = FILENAME
//     Single_Threaded_POA.cpp
//
// = DESCRIPTION
//     This program tests to make sure that two threads cannot call
//     servants in a single threaded POA simultaneously. At the same
//     time, it makes sure that a servant can call itself or other
//     servants in the same POA while in an upcall.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"
#include "ace/Task.h"

class test_i : public virtual POA_test
{
public:
  test_i (PortableServer::POA_ptr poa);

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

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
test_i::method (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Entering Worker::svc from %t and sleeping....\n"));

  ACE_OS::sleep (3);

  ACE_DEBUG ((LM_DEBUG,
              "Done resting from %t\n"));

  if (this->called_self_ == 0)
    {
      this->called_self_ = 1;

      ACE_DEBUG ((LM_DEBUG,
                  "Calling self from %t\n"));

      test_var self = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      self->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

PortableServer::POA_ptr
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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
  ACE_TRY_NEW_ENV
    {
      this->test_->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in thread");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA"
                                                               ACE_ENV_ARG_PARAMETER);

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Policies for the new POA.
      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_implicit_activation_policy (PortableServer::IMPLICIT_ACTIVATION
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[1] =
        root_poa->create_thread_policy (PortableServer::SINGLE_THREAD_MODEL
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creation of the child POA.
      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Destroy the policies
      for (CORBA::ULong i = 0;
           i < policies.length ();
           ++i)
        {
          policies[i]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_i servant1 (child_poa.in ());
      test_i servant2 (child_poa.in ());

      test_var object1 = servant1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var object2 = servant2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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

      root_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
