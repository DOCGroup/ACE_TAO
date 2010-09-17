// $Id$

#include "subscriber_impl.h"
#include "publisherC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://ior.out");

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
  // Indicates successful parsing of the command line
  return 0;
}


class ThreadPool : public ACE_Task_Base
{
public:
  ThreadPool (CORBA::ORB_ptr orb);
  virtual int svc (void);
private:
  CORBA::ORB_var orb_;
};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object = orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (
      poa_object.in ());
      PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager();

      Subscriber_impl subscriber(orb.in ());
      PortableServer::ObjectId_var id =
      root_poa->activate_object (&subscriber);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Subscriber_var subscriber_var = Subscriber::_narrow (object.in ());

      object = orb->string_to_object(ior);
      Publisher_var publisher = Publisher::_narrow(object.in());

      publisher->subscribe(subscriber_var.in());

      poa_manager->activate();

      ThreadPool pool (orb.in ());
      if (pool.activate(THR_NEW_LWP | THR_JOINABLE, 5) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      pool.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

    return 0;
}

// ****************************************************************

ThreadPool::ThreadPool(CORBA::ORB_ptr orb)
: orb_(CORBA::ORB::_duplicate (orb))
{
}

int ThreadPool::svc (void)
{
  try
    {
      this->orb_->run ();
    }
  catch (...)
    {
    }
  return 0;
}
