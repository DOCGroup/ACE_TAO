// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Nested_Non_Servant_Upcalls
//
// = FILENAME
//     Nested_Non_Servant_Upcalls.cpp
//
// = DESCRIPTION
//     This program tests that nested non-servant upcalls are handled
//     correctly.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"
#include "ace/Task.h"
#include "ace/Synch.h"

class test_i : public virtual PortableServer::RefCountServantBase,
               public virtual POA_test
{
public:

  test_i (PortableServer::POA_ptr poa);

  ~test_i (void);

  void method (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_var poa_;

};

test_i::test_i (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i created: instance %x\n",
              this));
}

test_i::~test_i (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_i destroyed: instance %x\n",
              this));
}

void
test_i::method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

class Object_Activator : public ACE_Task_Base
{
public:

  Object_Activator (ACE_Thread_Manager &thread_manager,
                    PortableServer::POA_ptr poa);

  int svc (void);

  ACE_Auto_Event object_activated_;

  PortableServer::POA_var poa_;

};


Object_Activator::Object_Activator (ACE_Thread_Manager &thread_manager,
                                    PortableServer::POA_ptr poa)
  : ACE_Task_Base (&thread_manager),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

int
Object_Activator::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      test_i *servant =
        new test_i (this->poa_.in ());

      PortableServer::ServantBase_var safe_servant (servant);

      PortableServer::ObjectId_var id =
        this->poa_->activate_object (servant
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->object_activated_.signal ();

      this->poa_->deactivate_object (id.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in activator thread");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

Object_Activator *global_object_activator = 0;

class Servant_Activator :
  public PortableServer::ServantActivator
{
public:

  Servant_Activator (PortableServer::POA_ptr poa);

  PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                     PortableServer::POA_ptr poa
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  void etherealize (const PortableServer::ObjectId &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_var poa_;

  PortableServer::ObjectId_var id_;

};

Servant_Activator::Servant_Activator (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::Servant
Servant_Activator::incarnate (const PortableServer::ObjectId &,
                              PortableServer::POA_ptr
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  test_i *servant =
    new test_i (this->poa_.in ());

  PortableServer::ServantBase_var safe_servant (servant);

  this->id_ =
    this->poa_->activate_object (servant
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  this->poa_->deactivate_object (this->id_.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  int result =
    global_object_activator->activate ();
  ACE_ASSERT (result != -1);

  ACE_Time_Value timeout (5);

  result =
    global_object_activator->object_activated_.wait (&timeout, 0);

  if (result == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Object Activator was able to make progress during "
                  "recursive non-servant upcall: test failed\n"));

      ACE_ASSERT (0);
    }
  else if (result == -1 && errno == ETIME)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Object Activator was not able to make progress during "
                  "recursive non-servant upcall: test succeeded\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Unexpected error during event.wait (): %d\n",
                  result));

      ACE_ASSERT (0);
    }

  return new test_i (this->poa_.in ());
}

void
Servant_Activator::etherealize (const PortableServer::ObjectId &,
                                PortableServer::POA_ptr,
                                PortableServer::Servant servant,
                                CORBA::Boolean,
                                CORBA::Boolean
                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  servant->_remove_ref ();
}

int
main (int argc, char **argv)
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies;
      CORBA::ULong current_length = 0;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Servant_Activator servant_activator (child_poa.in ());
      child_poa->set_servant_manager (&servant_activator
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var first_object =
        child_poa->create_reference ("IDL:test:1.0"
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var first_test =
        test::_narrow (first_object.in ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        child_poa->reference_to_id (first_object.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Thread_Manager thread_manager;

      Object_Activator object_activator (thread_manager,
                                         child_poa.in ());

      global_object_activator =
        &object_activator;

      first_test->method (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      child_poa->deactivate_object (id.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Wait for the Object_Activator thread to exit.
      thread_manager.wait ();

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
