// $Id$

#include "ace/OS_main.h"

#if 0

// Listing 1 code/ch12
int
HA_Device_Repository::update_device (int device_id)
{
  this->mutex_.acquire ();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Updating device %d\n"),
              device_id));

  // Allocate a new object.
  ACE_NEW_RETURN (object, Object, -1);
  // ...
  // Use the object

  this->mutex_.release ();
}
// Listing 1
// Listing 2 code/ch12
int
HA_Device_Repository::update_device (int device_id)
{
  // Construct a guard specifying the type of the mutex as
  // a template parameter and passing in the mutex to hold
  // as a parameter.
  ACE_Guard<ACE_Thread_Mutex> guard (this->mutex_);

  // This can throw an exception that is not caught here.
  ACE_NEW_RETURN (object, Object, -1);
  // ..
  // Use the object.
  // ..
  // Guard is destroyed, automatically releasing the lock.
}
// Listing 2
// Listing 3 code/ch12
int
HA_Device_Repository::update_device (int device_id)
{
  ACE_GUARD_RETURN (ACE_Thread_Mutex, mon, mutex_, -1);

  ACE_NEW_RETURN (object, Object, -1);
  // Use the object.
  // ...
}
// Listing 3

#endif /* 0 */
int ACE_TMAIN (int, ACE_TCHAR *[])
{ return 0; }
