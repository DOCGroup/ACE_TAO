#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_main.h"
#include "ace/OS_Memory.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Thread_Mutex.h"
#include <memory>

// This file exists primarily to get code into the book to show different
// ways to do the same thing. For complete context and explanation, please
// see APG chapter 12.

class HA_Device_Repository {
public:
  HA_Device_Repository () = default;
  int update_device (int const device_id);

private:
  HA_Device_Repository (HA_Device_Repository const &) = delete;
  HA_Device_Repository (HA_Device_Repository &&) = delete;
  HA_Device_Repository &operator= (HA_Device_Repository const &) = delete;
  HA_Device_Repository &operator= (HA_Device_Repository &&) = delete;

  ACE_Thread_Mutex mutex_;
};

class Object {
};
static std::unique_ptr<Object> object {};

#if 0
// This is less-desired way to do this...

// Listing 1 code/ch12
int
HA_Device_Repository::update_device (int const device_id)
{
  this->mutex_.acquire ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Updating device %d\n"),
              device_id));

  // Allocate a new object.
  Object *new_object {};
  ACE_NEW_RETURN (new_object, Object, -1);
  object.reset (new_object);
  // ...
  // Use the object

  this->mutex_.release ();
}
// Listing 1
// Listing 2 code/ch12
int
HA_Device_Repository::update_device (int const device_id)
{
  // Construct a guard specifying the type of the mutex as
  // a template parameter and passing in the mutex to hold
  // as a parameter.
  // FUZZ: disable check_for_ACE_Guard
  ACE_Guard<ACE_Thread_Mutex> guard (this->mutex_);
  // FUZZ: enable check_for_ACE_Guard

  // This can throw an exception that is not caught here.
  Object *new_object {};
  ACE_NEW_RETURN (new_object, Object, -1);
  object.reset (new_object);
  // ..
  // Use the object.
  // ..
  // Guard is destroyed, automatically releasing the lock.
}
// Listing 2
#endif /* 0 */

// Listing 3 code/ch12
int
HA_Device_Repository::update_device (int const /* device_id */)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, mutex_, -1);

  Object *new_object {};
  ACE_NEW_RETURN (new_object, Object, -1);
  object.reset (new_object);
  // Use the object.
  // ...
  return 0;
}
// Listing 3

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_Device_Repository rep;
  rep.update_device (42);
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
