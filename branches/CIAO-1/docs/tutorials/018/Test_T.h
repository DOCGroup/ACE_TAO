// $Id$

#ifndef TEST_T_H
#define TEST_T_H

#include "ace/Task.h"
#include "ace/Atomic_Op.h"

/* We'll create a simple ACE_Task derivative for testing a couple of
   different locking mechanisms.  We've hidden the open() method to
   force our client into using the run() method instead.

   The naming convention *_T is fairly typical for ACE.  The _T suffix
   on the object name (and it's source files) indicates that this is a
   templated class.  Generally, there is a non-templated class defined
   also such as foobar.h that would be included instead of foobar_T.h.
 */

template <class MUTEX>
class Test_T : public ACE_Task<ACE_MT_SYNCH>
{
public:
  // Allow our derivative to name the class so that we can tell the
  // user what's going on as we test the lock.
  Test_T (const char *name);

  // This will run the entire test.  open() will be called to activate
  // the task's threads.  We then add a number of messages to the
  // queue for svc() to process.
  int run (void);

protected:

  // Activate a few threads
  int open (void *arg = 0);

  // Read some things from the message queue and exercise the lock.
  int svc (void);

  // Send a message block to svc().  If _message is 0 then send a
  // shutdown request (e.g., MB_HANGUP)
  int send (ACE_Message_Block * message = 0);

  // The object's name.  Typically provided by a derivative.
  const char *name_;

  // We want to barrier the svc() methods to give all of the threads a
  // fair chance
  ACE_Barrier barrier_;

  // As each thread enters svc() it will increment this.  While we
  // have a thread id available to us, I wanted a simple value to
  // display in debug messages.
  ACE_Atomic_Op<ACE_Mutex,int> thread_num_;

  // Set our mutex type based on the template parameter.  We then
  // build a guard type based on that type.
  typedef MUTEX mutex_t;
  typedef ACE_Guard<mutex_t> guard_t;

  // Our mutex.  We'll use this in svc() to protect imaginary shared
  // resources.
  mutex_t mutex_;
};

/* Although different compilers differ in their details, almost all of
   them require that you provide the definition of the templated
   object along with the declaration.  With any luck, this will change
   someday & we'll have smaller object files.  Until then, the lines
   below will take care of you.
 */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Test_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Test_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TEST_T_H */
