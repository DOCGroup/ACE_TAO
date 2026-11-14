#if !defined (ACE_ID_GENERATOR_C)
#define ACE_ID_GENERATOR_C

#include "ace/Guard_T.h"
#include "ace/Object_Manager.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Thread_Mutex.h"
#include "ace/OS_NS_time.h"
#include "ID_Generator.h"

time_t ACE_ID_Generator::last_time_ = 0;

size_t ACE_ID_Generator::last_number_ = 0;

ACE_SYNCH_MUTEX *ACE_ID_Generator::lock_ = nullptr;

char *
ACE_ID_Generator::get_new_id (char *id)
{
  time_t t;
  size_t sn;

  ACE_ID_Generator::get_serial_id (t, sn);
  ACE_NEW_RETURN (id, char [ACE_OFFER_ID_LENGTH], nullptr);

  ACE_OS::sprintf (id, "%014d%06d", t, sn);
  return id;
}

void
ACE_ID_Generator::get_serial_id (time_t &t, size_t &s)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, *ACE_ID_Generator::get_lock ()));
  ACE_OS::time (&t);

  if (t != ACE_ID_Generator::last_time_)
    {
      ACE_ID_Generator::last_time_ = t;
      s = ACE_ID_Generator::last_number_ = 0;
    }
  else
      s = ACE_ID_Generator::last_number_++;
}

ACE_SYNCH_MUTEX *
ACE_ID_Generator::get_lock ()
{
#if defined (ACE_HAS_THREADS)
  if (ACE_ID_Generator::lock_ == nullptr)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon, *ACE_Static_Object_Lock::instance (), nullptr));

      // Double-checked Locking Optimization.
      if (ACE_ID_Generator::lock_ == nullptr)
        ACE_NEW_RETURN (ACE_ID_Generator::lock_, ACE_SYNCH_MUTEX, nullptr);
    }
#endif /* ACE_HAS_THREADS */
  return ACE_ID_Generator::lock_;
}

#endif /* ACE_ID_GENERATOR_C */
