// $Id$

#include "Server_Thread_Pool.h"
#include "TestC.h"
#include "ace/OS_NS_unistd.h"

time_t last_success;

int
Thread_Pool::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) worker thread closing down\n"));
  return 0;
}

Thread_Pool::Thread_Pool (CORBA::ORB_ptr orb,
                          ACE_Thread_Manager *thr_mgr,
                          int n_threads)
  : ACE_Task<ACE_SYNCH> (thr_mgr),
  orb_(CORBA::ORB::_duplicate(orb)),
  nt_(n_threads)
{
  if (this->activate (THR_NEW_LWP,
                      n_threads) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "activate failed\n"));
}

Thread_Pool::~Thread_Pool (void)
{
}

int
Thread_Pool::shutdown (void)
{
  thr_mgr_->cancel_grp (grp_id_);

  int n_threads = nt_.value ();
  for (int i = 0; i < n_threads; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) eof, sending block for thread=%d\n",
                  i + 1));
      ACE_Message_Block *mb1;
      ACE_NEW_RETURN (mb1,
                      ACE_Message_Block ((char*)0),
                      -1);
      mb1->length (0);

      if (this->put (mb1) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%t) %p\n",
                    "put"));

      // this sleep helps to shutdown correctly -> was an error!
      ACE_OS::sleep (1);
    }

  return 0;
}

// Simply enqueue the Message_Block into the end of the queue.
int
Thread_Pool::put (Test::Echo_ptr echoptr)
{
  char * charData = (char *) Test::Echo::_duplicate (echoptr);

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN(mb, ACE_Message_Block(charData), -1);
  return this->put (mb);
}

int
Thread_Pool::put (ACE_Message_Block *mb,
                  ACE_Time_Value *tv)
{
  return this->putq (mb, tv);
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

int
Thread_Pool::svc (void)
{
  // Note that the <ACE_Task::svc_run> method automatically adds us to
  // the Thread_Manager when the thread begins.

  int count = 1;

  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (;; count++)
    {
      ACE_Message_Block *mb = 0;

#if 0
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) in iteration %d before getq ()\n",
                  count));
#endif

      if (this->getq (mb) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t) in iteration %d, got result -1, exiting\n",
                      count));
          break;
        }

#if 0
      if (mb->length() == 0)
        {
          //FUZZ: disable check_for_NULL
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) in iteration %d, got NULL message, exiting\n",
                      count));
          //FUZZ: enable check_for_NULL
          break;
        }
#endif

      Test::Echo_var echo = (Test::Echo_ptr)mb->base();

      // Echo_var is responsible for deallocating this.
      // mb->release ();

      if (CORBA::is_nil(echo.in()))
        {
          //FUZZ: disable check_for_NULL
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) in iteration %d, got NULL message, exiting\n",
                      count));
          //FUZZ: enable check_for_NULL
          break;
        }

      // Keep calling a few times after receiving exceptions
      for(int exception_count = 50; exception_count; --exception_count)
        {
          try
            {
              // keep calling until get an exception
              while (true)
                {
#if 0
                  if (0)
                    {
                      Test::Payload pload (10);
                      pload.length (10);
                      ACE_OS::memset (pload.get_buffer(), pload.length(), 0);
                      echo->echo_payload (pload);

                    }
                  else
#endif /*if 0*/
                    {
                      Test::Payload_var pout;
                      echo->echo_payload_out (pout.out());

                      // time_t last_success = ACE_OS::time();
                    }
              }
            }
          catch (const CORBA::Exception&)
            {
              // Just forget the exception and continue
            }
        }

    }

  --nt_;
  if (nt_ == 0)
    {
      orb_->shutdown (0);
    }

  // Note that the <ACE_Task::svc_run> method automatically removes us
  // from the <ACE_Thread_Manager> when the thread exits.
  return 0;
}
