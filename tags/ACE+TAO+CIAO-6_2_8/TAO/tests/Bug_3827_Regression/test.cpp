// $Id$

#include "MessageC.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

int status = 0;

class ORB_Task : public ACE_Task_Base
{
public:
  ORB_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  virtual int svc (void)
  {
    try
      {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) Event loop started\n"));
        this->orb_->run ();
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) Event loop finished\n"));
      }
    catch (const CORBA::Exception& )
      {
      }
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

class Worker_Task : public ACE_Task_Base
{
public:
  Worker_Task (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  virtual int svc (void)
  {
    try
      {
        Message_init *mi = new Message_init ();
        CORBA::ValueFactoryBase_var safe (mi);

        while (1)
          {
            CORBA::ValueFactoryBase_var tmp1 =
              this->orb_->register_value_factory (mi->tao_repository_id (), mi);

            CORBA::ValueFactoryBase_var tmp2 =
              this->orb_->lookup_value_factory (mi->tao_repository_id ());

            this->orb_->unregister_value_factory (mi->tao_repository_id ());
          }
      }
    catch (const CORBA::BAD_INV_ORDER&)
      {
        // It's ok to ignore BAD_INV_ORDER which can be thrown after orb
        // is shutdown from other thread.
      }
    catch (const CORBA::Exception&)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("ERROR: Unexpected exception.\n")));
        status = 1;
      }
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      ORB_Task orb_runner (orb.in ());
      if (orb_runner.activate (THR_NEW_LWP | THR_JOINABLE, 1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Can not start ORB thread\n"),
                            -1);
        }

      ACE_OS::sleep (1);

      Worker_Task work_runner (orb.in ());
      if (work_runner.activate (THR_NEW_LWP | THR_JOINABLE, 10) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Can not start work threads\n"),
                            -1);
        }

      ACE_OS::sleep (1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Shutting down / destroying ORB\n")));

      orb->shutdown (false);

      ACE_Thread_Manager::instance ()->wait ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return status;
}
