// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Task.h"
#include "ace/Containers.h"
#include "ace/Synch.h"
#include "ace/Condition_T.h"

// Listing 4 code/ch16
class Follower
{
public:
  Follower (ACE_Thread_Mutex &leader_lock)
    : cond_(leader_lock)
  {
    owner_ = ACE_Thread::self ();
  }

  //FUZZ: disable check_for_lack_ACE_OS
  int wait (void)
  {
    return this->cond_.wait ();
  }

  int signal (void)
  {
    return this->cond_.signal ();
  }
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_thread_t owner (void)
  {
    return this->owner_;
  }

private:
  ACE_Condition<ACE_Thread_Mutex> cond_;
  ACE_thread_t owner_;
};
// Listing 4
// Listing 1 code/ch16
class LF_ThreadPool : public ACE_Task<ACE_MT_SYNCH>
{
public:
  LF_ThreadPool () : shutdown_(0), current_leader_(0)
  {
    ACE_TRACE ("LF_ThreadPool::TP");
  }

  virtual int svc (void);

  void shut_down (void)
  {
    shutdown_ = 1;
  }

private:
  int become_leader (void);

  Follower *make_follower (void);

  int elect_new_leader (void);

  int leader_active (void)
  {
    ACE_TRACE ("LF_ThreadPool::leader_active");
    return this->current_leader_ != 0;
  }

  void leader_active (ACE_thread_t leader)
  {
    ACE_TRACE ("LF_ThreadPool::leader_active");
    this->current_leader_ = leader;
  }

  void process_message (ACE_Message_Block *mb);

  int done (void)
  {
    return (shutdown_ == 1);
  }

private:
  int shutdown_;
  ACE_thread_t current_leader_;
  ACE_Thread_Mutex leader_lock_;
  ACE_Unbounded_Queue<Follower*> followers_;
  ACE_Thread_Mutex followers_lock_;
  static long LONG_TIME;
};
// Listing 1
// Listing 2 code/ch16
int
LF_ThreadPool::svc (void)
{
  ACE_TRACE ("LF_ThreadPool::svc");
  while (!done ())
    {
      become_leader ();  // Block until this thread is the leader.

      ACE_Message_Block *mb = 0;
      ACE_Time_Value tv (LONG_TIME);
      tv += ACE_OS::gettimeofday ();

      // Get a message, elect new leader, then process message.
      if (this->getq (mb, &tv) < 0)
        {
          if (elect_new_leader () == 0)
            break;
          continue;
        }

      elect_new_leader ();
      process_message (mb);
    }

  return 0;
}
// Listing 2
// Listing 3 code/ch16
int
LF_ThreadPool::become_leader (void)
{
  ACE_TRACE ("LF_ThreadPool::become_leader");

  ACE_GUARD_RETURN
    (ACE_Thread_Mutex, leader_mon, this->leader_lock_, -1);
  if (leader_active ())
    {
      Follower *fw = make_follower ();
      {
        // Wait until told to do so.
        while (leader_active ())
          fw->wait ();
      }

      delete fw;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Becoming the leader\n")));

  // Mark yourself as the active leader.
  leader_active (ACE_Thread::self ());
  return 0;
}

Follower*
LF_ThreadPool::make_follower (void)
{
  ACE_TRACE ("LF_ThreadPool::make_follower");

  ACE_GUARD_RETURN
    (ACE_Thread_Mutex, follower_mon, this->followers_lock_, 0);
  Follower *fw;
  ACE_NEW_RETURN (fw, Follower (this->leader_lock_), 0);
  this->followers_.enqueue_tail (fw);
  return fw;
}
// Listing 3
// Listing 5 code/ch16
int
LF_ThreadPool::elect_new_leader (void)
{
  ACE_TRACE ("LF_ThreadPool::elect_new_leader");

  ACE_GUARD_RETURN
    (ACE_Thread_Mutex, leader_mon, this->leader_lock_, -1);
  leader_active (0);

  // Wake up a follower
  if (!followers_.is_empty ())
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex,
                        follower_mon,
                        this->followers_lock_,
                        -1);
      // Get the old follower.
      Follower *fw;
      if (this->followers_.dequeue_head (fw) != 0)
        return -1;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Resigning and Electing %d\n"),
                  fw->owner ()));
      return (fw->signal () == 0) ? 0 : -1;
    }
  else
    {
      ACE_DEBUG
        ((LM_ERROR, ACE_TEXT ("(%t) Oops no followers left\n")));
      return -1;
    }
}
// Listing 5

void
LF_ThreadPool::process_message (ACE_Message_Block *mb)
{
  ACE_TRACE ("LF_ThreadPool::process_message");
  int msgId;
  ACE_OS::memcpy (&msgId, mb->rd_ptr (), sizeof(int));
  mb->release ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Started processing message:%d\n"),
              msgId));
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Finished processing message:%d\n"),
              msgId));
}

long LF_ThreadPool::LONG_TIME = 5L;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  LF_ThreadPool tp;
  tp.activate (THR_NEW_LWP| THR_JOINABLE, 5);

  // Wait for a few seconds...
  ACE_OS::sleep (2);
  ACE_Time_Value tv (1L);

  ACE_Message_Block *mb = 0;
  for (int i = 0; i < 30; i++)
    {
      ACE_NEW_RETURN (mb, ACE_Message_Block (sizeof(int)), -1);
      ACE_OS::memcpy (mb->wr_ptr (), &i, sizeof(int));
      ACE_OS::sleep (tv);

      // Add a new work item.
      tp.putq (mb);
    }

  ACE_Thread_Manager::instance ()->wait ();

  ACE_OS::sleep (10);

  return 0;
}

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
