#if !defined (ACE_ID_GENERATOR_C)
#define ACE_ID_GENERATOR_C

#include "ace/Guard_T.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"
#include "ace/OS_NS_time.h"
#include "ID_Generator.h"

#include <cinttypes>
#include <cstdio>
#include <memory>

time_t ACE_ID_Generator::last_time_ = 0;

size_t ACE_ID_Generator::last_number_ = 0;

char *
ACE_ID_Generator::get_new_id (char *id)
{
  time_t t {};
  size_t sn {};

  ACE_ID_Generator::get_serial_id (t, sn);
  ACE_NEW_RETURN (id, char [ACE_OFFER_ID_LENGTH], nullptr);
  std::unique_ptr<char[]> id_owner (id);

  if (std::snprintf (id_owner.get (),
                     ACE_OFFER_ID_LENGTH,
                     "%014" PRId64 "%06" PRIu64,
                     static_cast<int64_t> (t),
                     static_cast<uint64_t> (sn)) != ACE_OFFER_ID_LENGTH - 1)
    {
      return nullptr;
    }
  return id_owner.release ();
}

void
ACE_ID_Generator::get_serial_id (time_t &t, size_t &s)
{
  ACE_MT (ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, ACE_ID_Generator::get_lock ()));
  ACE_OS::time (std::addressof (t));

  if (t != ACE_ID_Generator::last_time_)
    {
      ACE_ID_Generator::last_time_ = t;
      s = ACE_ID_Generator::last_number_ = 0;
    }
  else
    {
      s = ACE_ID_Generator::last_number_++;
    }
}

ACE_SYNCH_MUTEX &
ACE_ID_Generator::get_lock ()
{
  static ACE_SYNCH_MUTEX lock;
  return lock;
}

#endif /* ACE_ID_GENERATOR_C */
