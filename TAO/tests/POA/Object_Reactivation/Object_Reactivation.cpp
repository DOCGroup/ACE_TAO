
//=============================================================================
/**
 *  @file     Object_Reactivation.cpp
 *
 *  $Id$
 *
 *   This program tests the reactivation of a servant that has been
 *   deactivated but not removed from the Active Object Map yet.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "testS.h"
#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/Auto_Event.h"
#include "ace/OS_NS_unistd.h"

static int debug = 1;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        debug = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d debug "
                           "\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

class test_i : public POA_test
{
public:
  test_i (ACE_Auto_Event &event);

  void deactivate_self (void);

private:
  ACE_Auto_Event &event_;
};

test_i::test_i (ACE_Auto_Event &event)
  : event_ (event)
{
}

void
test_i::deactivate_self (void)
{
  PortableServer::POA_var poa = this->_default_POA ();

  PortableServer::ObjectId_var id = poa->servant_to_id (this);

  if (debug)
    ACE_DEBUG ((LM_DEBUG, "(%t) Deactivating servant\n"));

  poa->deactivate_object (id.in ());

  if (debug)
    ACE_DEBUG ((LM_DEBUG, "(%t) Deactivation complete: signaling main thread and going to sleep\n"));

  int result = this->event_.signal ();
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  ACE_OS::sleep (3);

  if (debug)
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
  if (debug)
    ACE_DEBUG ((LM_DEBUG, "(%t) Waiting for deactivation to complete\n"));

  int result = this->event_.wait ();
  ACE_ASSERT (result == 0);
  ACE_UNUSED_ARG (result);

  if (debug)
    ACE_DEBUG ((LM_DEBUG, "(%t) Deactivation complete, trying to activate\n"));

  try
    {
      if (this->task_id_ == "first thread")
        {
          PortableServer::ObjectId_var id =
            this->poa_->activate_object (this->servant_);
        }
      else
        {
          this->poa_->activate_object_with_id (this->id_,
                                               this->servant_);
        }

      if (debug)
        ACE_DEBUG ((LM_DEBUG, "(%t) Activation complete\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Activator::svc");
      return -1;
    }
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
  try
    {
      this->test_->deactivate_self ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Deactivator::svc");
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

      int parse_args_result =
        parse_args (argc, argv);

      if (parse_args_result != 0)
        return parse_args_result;

      // Obtain the RootPOA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

      // Get the POAManager of the RootPOA.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      ACE_Auto_Event event1;
      test_i servant1 (event1);

      ACE_Auto_Event event2;
      test_i servant2 (event2);

      PortableServer::ObjectId_var id_act =
        root_poa->activate_object (&servant1);

      CORBA::Object_var object_act = root_poa->id_to_reference (id_act.in ());

      test_var test_object1 = test::_narrow (object_act.in ());

      id_act = root_poa->activate_object (&servant2);

      object_act = root_poa->id_to_reference (id_act.in ());

      test_var test_object2 = test::_narrow (object_act.in ());

      PortableServer::ObjectId_var id1 =
        root_poa->reference_to_id (test_object1.in ());

      PortableServer::ObjectId_var id2 =
        root_poa->reference_to_id (test_object2.in ());

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

      if (activator1.activate () != 0 ||
          activator2.activate () != 0 ||
          deactivator1.activate () != 0 ||
          deactivator2.activate () != 0)
        return -1;

      int result = ACE_Thread_Manager::instance ()->wait ();

      if (result != 0)
        return result;

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
