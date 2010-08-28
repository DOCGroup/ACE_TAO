// $Id$

#include "HelloS.h"

#include "ace/OS_NS_time.h"

#include "tao/Messaging/Messaging.h"

#include "tao/Transport_Cache_Manager.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Transport.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include "ace/Task.h"
#include "ace/Synch.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

class Callback_i : public POA_Test::CallBack
{
public:
  void method2(void)
  {
    ACE_DEBUG ((LM_DEBUG,"(%t) Callback_i::method2 called\n"));
  }
};

class Worker : public ACE_Task_Base
{
public:
  CORBA::ORB_var orb_;
  Test::Hello_var hello_;
  Test::Hello_var asynch_hello_;
  Test::CallBack_var callback_;

private:
  ACE_Mutex lock_;
  int orb_threads_;
  int busy_threads_;
  CORBA::Short message_counter_;

  int run_test (void);
public:
  Worker( int ot);
  ~Worker();

  int svc (void);
};

Worker::Worker(int ot)
  :orb_threads_ (ot),
   busy_threads_ (0),
   message_counter_ (0)
{
}

Worker::~Worker()
{
  this->orb_->destroy();
}

int
Worker::svc()
{
  {
    ACE_GUARD_RETURN (ACE_Mutex, ace_mon, this->lock_, 0);
    if (this->orb_threads_ > 0)
      {
        --this->orb_threads_;
        hello_->set_callback(this->callback_.in());
        ace_mon.release();
        this->orb_->run();
        return 0;
      }
    else
      ++this->busy_threads_;
  }
  bool excep = false;
  ACE_DEBUG ((LM_DEBUG, "(%t) starting loop\n"));
  for (int i = 0; i < 40; i++)
    {
      try
        {
          CORBA::Short n = 0;
          {
            ACE_GUARD_RETURN (ACE_Mutex, ace_mon, this->lock_, 0);
            n = ++this->message_counter_;
          }
          this->asynch_hello_->method (n);

          ACE_Time_Value udelay(0,400);
          struct timespec ts = udelay;
          ACE_OS::nanosleep (&ts);

        }
      catch (CORBA::Exception &ex)
        {
          excep = true;
          ACE_ERROR ((LM_ERROR,
                      "(%t) Exception caught: %s after %d invocations\n",
                      ex._name(), i));
          i = 39;
        }
    }
  if (!excep)
    ACE_DEBUG ((LM_DEBUG, "(%t) Did all iterations\n"));

  {
    ACE_GUARD_RETURN (ACE_Mutex, ace_mon, this->lock_, 0);
    --this->busy_threads_;
    if (this->busy_threads_)
      return 0;
  }

  try
    {
      this->orb_->shutdown();
    }
  catch (CORBA::Exception &)
    {
    }

  return 0;
}

int
init_callback (Worker &w)
{
  CORBA::Object_var obj =
    w.orb_->resolve_initial_references ("RootPOA");

  if (CORBA::is_nil (obj.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Unable to initialize the POA.\n"),
                      1);

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in ());

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager ();

  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (1);
  policies.length (1);

  CORBA::Any pol;
  pol <<= BiDirPolicy::BOTH;
  policies[0] =
    w.orb_->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                        pol);

  // Create POA as child of RootPOA with the above policies.  This POA
  // will receive request in the same connection in which it sent
  // the request
  PortableServer::POA_var child_poa =
    root_poa->create_POA ("childPOA",
                          poa_manager.in (),
                          policies);


  Callback_i *servant = new Callback_i;
  PortableServer::ServantBase_var owner = servant;

  PortableServer::ObjectId_var id = child_poa->activate_object (servant);
  obj = child_poa->id_to_reference (id.in());
  w.callback_ = Test::CallBack::_narrow(obj.in());

  // Creation of childPOA is over. Destroy the Policy objects.
  for (CORBA::ULong i = 0;
       i < policies.length ();
       ++i)
    {
      policies[i]->destroy ();
    }

  poa_manager->activate ();

  return 0;
}


int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  static const int orb_threads = 5;
  static const int total_threads = 15;

  // It must be ensured that there are more total threads that there are
  // that are dedicated to running the ORB.
  ACE_ASSERT (total_threads > orb_threads);

  Worker worker (orb_threads);
  try
    {
      worker.orb_ =
        CORBA::ORB_init (argc, argv, "test");

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG,"using ior = %s\n",ior));

      CORBA::Object_var tmp = worker.orb_->string_to_object(ior);

      if (CORBA::is_nil (tmp.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR, "Invalid IOR.\n")
                            ,1);
        }
      worker.hello_ = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (worker.hello_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

//       bool x = worker.hello_->_non_existent();
//       ACE_DEBUG ((LM_DEBUG, "_non_existent returned %d\n",x));

      {
        // Set the Synch Scopes

        CORBA::Any scope_as_any;

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Applying SYNC_NONE.\n"));
        scope_as_any <<= Messaging::SYNC_NONE;
        CORBA::PolicyList policies (1);
        policies.length (1);
        policies[0] =
          worker.orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                              scope_as_any);

        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) - Applying Synch Scope at Object level.\n"));
        tmp = tmp->_set_policy_overrides (policies, CORBA::SET_OVERRIDE);
        policies[0]->destroy ();
      }

      worker.asynch_hello_ = Test::Hello::_narrow(tmp.in ());

      if (CORBA::is_nil (worker.asynch_hello_.in ())) {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Nil Test::Hello reference <%s>\n",
                           ior),
                          1);
      }

      init_callback(worker);

    }
  catch (CORBA::Exception &ex)
    {
      ACE_ERROR ((LM_ERROR, "Exception caught: %s\"%s\"\n"
                  , ex._name(), ex._rep_id ()));
      return 1;
    }

  worker.activate (THR_NEW_LWP | THR_JOINABLE, total_threads);
  worker.wait();

  return 0;
}
