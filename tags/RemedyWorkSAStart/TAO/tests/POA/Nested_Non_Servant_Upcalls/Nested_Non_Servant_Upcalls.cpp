
//=============================================================================
/**
 *  @file     Nested_Non_Servant_Upcalls.cpp
 *
 *  $Id$
 *
 *   This program tests that nested non-servant upcalls are handled
 *   correctly.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "ace/Task.h"
#include "ace/Auto_Event.h"
#include "tao/PortableServer/ServantActivatorC.h"

class test_i :
  public virtual POA_test
{
public:

  test_i (PortableServer::POA_ptr poa);

  ~test_i (void);

  void method (void);

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
test_i::method (void)
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
  try
    {
      test_i *servant =
        new test_i (this->poa_.in ());

      PortableServer::ServantBase_var safe_servant (servant);

      PortableServer::ObjectId_var id =
        this->poa_->activate_object (servant);

      this->object_activated_.signal ();

      this->poa_->deactivate_object (id.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception caught in activator thread");
      return -1;
    }

  return 0;
}

Object_Activator *global_object_activator = 0;

class Servant_Activator :
  public PortableServer::ServantActivator
{
public:

  Servant_Activator (PortableServer::POA_ptr poa);

  PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                     PortableServer::POA_ptr poa);

  void etherealize (const PortableServer::ObjectId &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations);

  PortableServer::POA_var poa_;

  PortableServer::ObjectId_var id_;

};

Servant_Activator::Servant_Activator (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::Servant
Servant_Activator::incarnate (const PortableServer::ObjectId &,
                              PortableServer::POA_ptr)
{
  test_i *servant =
    new test_i (this->poa_.in ());

  PortableServer::ServantBase_var safe_servant (servant);

  this->id_ =
    this->poa_->activate_object (servant);

  this->poa_->deactivate_object (this->id_.in ());

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
                                CORBA::Boolean)
{
  servant->_remove_ref ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      CORBA::PolicyList policies;
      CORBA::ULong current_length = 0;

      policies.length (current_length + 1);
      policies[current_length++] =
        root_poa->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

      PortableServer::POA_var child_poa =
        root_poa->create_POA ("child",
                              poa_manager.in (),
                              policies);

      Servant_Activator servant_activator (child_poa.in ());
      child_poa->set_servant_manager (&servant_activator);

      CORBA::Object_var first_object =
        child_poa->create_reference ("IDL:test:1.0");

      test_var first_test =
        test::_narrow (first_object.in ());

      PortableServer::ObjectId_var id =
        child_poa->reference_to_id (first_object.in ());

      ACE_Thread_Manager thread_manager;

      Object_Activator object_activator (thread_manager,
                                         child_poa.in ());

      global_object_activator =
        &object_activator;

      first_test->method ();

      child_poa->deactivate_object (id.in ());

      // Wait for the Object_Activator thread to exit.
      thread_manager.wait ();

      root_poa->destroy (1,
                         1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught");
      return -1;
    }

  return 0;
}
