// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/Object_Reactivation
//
// = FILENAME
//     Object_Reactivation.cpp
//
// = DESCRIPTION
//     This program tests the reactivation of a servant that has been
//     deactivated but not removed from the Active Object Map yet.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "testS.h"
#include "ace/Task.h"

class test_i : public POA_test
{
public:
  test_i (ACE_Auto_Event &event);

  void deactivate_self (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_Auto_Event &event_;
};

test_i::test_i (ACE_Auto_Event &event)
  : event_ (event)
{
}

void
test_i::deactivate_self (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_var poa = this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id = poa->servant_to_id (this
                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "(%t) Deactivating servant\n"));
  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "(%t) Deactivation complete: signaling main thread and going to sleep\n"));
  int result = this->event_.signal ();
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  ACE_OS::sleep (3);
  ACE_DEBUG ((LM_DEBUG, "(%t) test_i::deactivate_self complete\n"));
}

class Activator : public ACE_Task_Base
{
public:
  Activator (test_ptr t,
             ACE_Auto_Event &event,
             PortableServer::POA_ptr poa,
             PortableServer::Servant servant,
             const ACE_CString &task_id,
             const PortableServer::ObjectId &id);
  int svc (void);

private:
  test_var test_;
  ACE_Auto_Event &event_;
  PortableServer::POA_var poa_;
  PortableServer::Servant servant_;
  ACE_CString task_id_;
  PortableServer::ObjectId id_;
};

Activator::Activator (test_ptr t,
                      ACE_Auto_Event &event,
                      PortableServer::POA_ptr poa,
                      PortableServer::Servant servant,
                      const ACE_CString &task_id,
                      const PortableServer::ObjectId &id)
  : test_ (test::_duplicate (t)),
    event_ (event),
    poa_ (PortableServer::POA::_duplicate (poa)),
    servant_ (servant),
    task_id_ (task_id),
    id_ (id)
{
}

int
Activator::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Waiting for deactivation to complete\n"));

  int result = this->event_.wait ();
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  ACE_DEBUG ((LM_DEBUG, "(%t) Deactivation complete, trying to activate\n"));

  ACE_TRY_NEW_ENV
    {
      if (this->task_id_ == "first thread")
        {
          PortableServer::ObjectId_var id =
            this->poa_->activate_object (this->servant_
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          this->poa_->activate_object_with_id (this->id_,
                                               this->servant_
                                               ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) Activation complete\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Activator::svc");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

class Deactivator : public ACE_Task_Base
{
public:
  Deactivator (test_ptr t);
  int svc (void);

private:
  test_var test_;
};

Deactivator::Deactivator (test_ptr t)
  : test_ (test::_duplicate (t))
{
}

int
Deactivator::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      this->test_->deactivate_self (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Deactivator::svc");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB first.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_Auto_Event event1;
      test_i servant1 (event1);

      ACE_Auto_Event event2;
      test_i servant2 (event2);

      test_var test_object1 = servant1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test_var test_object2 = servant2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id1 =
        root_poa->reference_to_id (test_object1.in ());
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id2 =
        root_poa->reference_to_id (test_object2.in ());
      ACE_TRY_CHECK;

      Activator activator1 (test_object1.in (),
                            event1,
                            root_poa.in (),
                            &servant1,
                            "first thread",
                            id1.in ());

      Activator activator2 (test_object2.in (),
                            event2,
                            root_poa.in (),
                            &servant2,
                            "second thread",
                            id2.in ());

      Deactivator deactivator1 (test_object1.in ());

      Deactivator deactivator2 (test_object2.in ());

      if (activator1.activate (THR_BOUND) != 0 ||
          activator2.activate (THR_BOUND) != 0 ||
          deactivator1.activate (THR_BOUND) != 0 ||
          deactivator2.activate (THR_BOUND) != 0)
        return -1;

      int result = ACE_Thread_Manager::instance ()->wait ();

      if (result != 0)
        return result;

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
  ACE_CHECK_RETURN (-1);

  return 0;
}
