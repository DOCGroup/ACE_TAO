// *******************************************************************
//        File: TestCombinedThreads.cpp
//      Author: Chad Elliott (elliott_c@ociweb.com)
// Create Date: 4/11/2000
//         $Id$
// *******************************************************************

#include "TestCombinedThreads.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Object_Manager.h"
#include "ace/Thread_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#ifdef VXWORKS
# include <routeLib.h>
#endif

typedef ACE_Hash_Map_Manager<ACE_hthread_t,
                             const char*,
                             ACE_Null_Mutex> TestORBIDMap;
typedef ACE_Hash_Map_Manager<ACE_hthread_t,
                             ACE_Time_Value,
                             ACE_Null_Mutex> TestTimeValueMap;

static TestORBIDMap*       idMap          = 0;
static TestTimeValueMap*   timeMap        = 0;
static ACE_Thread_Manager* thread_manager = 0;
static const ACE_hthread_t largestId      = 0x7fffffff;

// *******************************************************************
// Method      : TAO_TestCombinedThreads::TAO_TestCombinedThreads
// Argument    : TEST_MAIN_TYPE_FUNC func - function to spawn as a
//               thread
// Argument    : char** args - arguments for the spawned function
// Description : Constructor - copies the function arguments.
// Notes       :
// *******************************************************************

TAO_TestCombinedThreads::TAO_TestCombinedThreads (TEST_MAIN_TYPE_FUNC func,
                                                  char** args)
  : timeout_ (0),
    namingServiceHack_ (0),
    args_ (0)
{
  this->initialize (func, args);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::TAO_TestCombinedThreads
// Argument    : const TAO_TestCombinedThreads& rhs - right hand side
// Description : Copy Constructor
// Notes       :
// *******************************************************************

TAO_TestCombinedThreads::TAO_TestCombinedThreads (
                                    const TAO_TestCombinedThreads& rhs)
  : timeout_ (0),
    namingServiceHack_ (0),
    args_ (0)
{
  this->initialize ();
  *this = rhs;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::~TAO_TestCombinedThreads
// Description : Destructor - deletes allocated arguments.
// Notes       :
// *******************************************************************

TAO_TestCombinedThreads::~TAO_TestCombinedThreads (void)
{
  this->deleteArguments (this->testArgs_.argv_);
  this->deleteArguments (this->args_);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::operator=
// Argument    : const TAO_TestCombinedThreads& rhs - right hand side
// Description : Assignment Operator
// Notes       :
// *******************************************************************

TAO_TestCombinedThreads&
TAO_TestCombinedThreads::operator= (const TAO_TestCombinedThreads& rhs)
{
  if (this != &rhs)
    {
      // Remove the old allocated memory (if any)
      this->deleteArguments (this->args_);
      this->deleteArguments (this->testArgs_.argv_);

      // Initialize the testArgs_ and args_ data members.
      // timeout_ will be set by calling the wait() method.
      this->initialize (rhs.testArgs_.func_, rhs.args_);
      this->namingServiceHack_ = rhs.namingServiceHack_;
    }
  return *this;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::addNamingServiceTimeout
// Argument    : int timeout - the number of seconds for the orb to
//               timeout.
// Description : Sets the timeout in the argument in the naming
//               service arguments.
// Notes       :
// *******************************************************************

void
TAO_TestCombinedThreads::addNamingServiceTimeout (unsigned int timeout)
{
  // If this is enabled and we are passed a timeout
  // then we will check for the -t option.  If the -t option
  // exists we will do nothing.  Otherwise, we will add the -t
  // with the given timeout.
  static const char* timeoutOption = "-t";
  int timeoutFound = 0;
  for (int i = 0; !timeoutFound && this->testArgs_.argv_[i] != 0; i++)
    {
      if (ACE_OS_String::strcmp (this->testArgs_.argv_[i],
                                 timeoutOption) == 0)
        {
          timeoutFound = 1;
        }
    }
  if (!timeoutFound)
    {
      // Add the timeout option
      ACE_NEW (this->testArgs_.argv_[this->testArgs_.argc_],
               char[ACE_OS_String::strlen (timeoutOption) + 1]);
      ACE_OS_String::strcpy (this->testArgs_.argv_[testArgs_.argc_],
                             timeoutOption);
      this->testArgs_.argc_++;

      // Add the number of seconds
      char number[64];
      ACE_OS::sprintf (number, "%d", timeout);
      ACE_NEW (this->testArgs_.argv_[this->testArgs_.argc_],
               char[ACE_OS::strlen (number) + 1]);
      ACE_OS_String::strcpy (this->testArgs_.argv_[this->testArgs_.argc_],
                             number);
      this->testArgs_.argc_++;

      // Add the NULL terminator
      this->testArgs_.argv_[this->testArgs_.argc_] = 0;
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::run
// Argument    : int timeout - the number of seconds for the orb to
//               timeout.
// Description : Sets the timeout in the argument structure and then
//               spawns a new thread and returns the thread group id.
// Notes       :
// *******************************************************************

int
TAO_TestCombinedThreads::run (unsigned int timeout)
{
  if (this->args_ != 0)
    {
      // Count up the arguments
      int blank = 0;
      for (this->testArgs_.argc_ = 0;
           this->args_[this->testArgs_.argc_] != 0;
           this->testArgs_.argc_++)
        {
          if (this->args_[this->testArgs_.argc_][0] == '\0')
            {
              blank++;
            }
        }
      // Copy the arguments
      int count = this->testArgs_.argc_ + 1 +
                  (this->namingServiceHack_ ? 2: 0) - blank;
      ACE_NEW_RETURN (this->testArgs_.argv_, char*[count], -1);
      int p = 0;
      for (int i = 0; i < this->testArgs_.argc_; i++)
        {
          if (this->args_[i][0] != '\0')
            {
              ACE_NEW_RETURN (this->testArgs_.argv_[p],
                              char[ACE_OS_String::strlen (this->args_[i]) + 1],
                              -1);
              ACE_OS_String::strcpy (this->testArgs_.argv_[p],
                                     this->args_[i]);
              p++;
            }
        }
      this->testArgs_.argv_[p] = 0;
      this->testArgs_.argc_ -= blank;
    }

  if (timeout > 0)
    {
      this->testArgs_.timeout_ = ACE_Time_Value (timeout);

      if (this->namingServiceHack_)
        {
          this->addNamingServiceTimeout (timeout);
        }
    }
  return thread_manager->spawn (TAO_TestCombinedThreads::spawned,
                                &(this->testArgs_));
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::wait
// Argument    : unsigned int seconds
// Description : Wait for all threads; if seconds is > 0, timeout
//               after seconds expire.
// Notes       :
// *******************************************************************

int
TAO_TestCombinedThreads::wait (unsigned int seconds)
{
  this->timeout_.reset ((seconds > 0 ?
                   new ACE_Time_Value (seconds += ACE_OS::time ()) : 0));
  thread_manager->wait (this->timeout_.get ());
  return 0;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::waitForFileTimed
// Argument    : const char* file - the file for which to wait
// Argument    : unsigned int seconds - how many seconds to wait
// Description : Wait seconds for file.
// Notes       : static
// *******************************************************************

int
TAO_TestCombinedThreads::waitForFileTimed (const char* file,
                                           unsigned int seconds)
{
  struct stat buf;
  while (seconds-- != 0)
    {
      if (ACE_OS::stat (file, &buf) == 0 && buf.st_size > 0)
        {
          return 0;
        }
      ACE_OS::sleep (1);
    }
  return -1;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::TAO_TestCombinedThreads
// Argument    : TEST_MAIN_TYPE_FUNC func - function to spawn as a
//               thread
// Argument    : char** args - arguments for the spawned function
// Description : Constructor - copies the function arguments.
// Notes       : protected
// *******************************************************************

TAO_TestCombinedThreads::TAO_TestCombinedThreads (TEST_MAIN_TYPE_FUNC func,
                                                  char** args,
                                                  int namingServiceHack)
  : timeout_ (0),
    namingServiceHack_ (namingServiceHack),
    args_ (0)
{
  this->initialize (func, args);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::initialize
// Argument    : TEST_MAIN_TYPE_FUNC func - function to spawn as a
//               thread
// Description : Copies the function arguments.
// Notes       :
// *******************************************************************

void TAO_TestCombinedThreads::initialize (TEST_MAIN_TYPE_FUNC func,
                                          char** args)
{
  this->testArgs_.func_ = func;
  this->testArgs_.argc_ = 0;
  this->testArgs_.argv_ = 0;
  this->copyArgv(args, this->args_);

  if (thread_manager == 0)
    {
#if defined (VXWORKS) && defined (ACE_HAS_IP_MULTICAST)
      char host[MAXHOSTNAMELEN];
      if (ACE_OS::hostname (host, MAXHOSTNAMELEN) != -1)
        {
          routeAdd ("224.0.0.0", host);
        }
#endif
      ACE_NEW (thread_manager,
               ACE_Thread_Manager);
      ACE_Object_Manager::at_exit (
                            thread_manager,
                            &TAO_TestCombinedThreads::destroyThreadManager,
                            0);
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::copyArgv
// Argument    : char** argv - original char* array
// Argument    : char**& dest - destination char* array
// Description : Copy argv type array
// Notes       :
// *******************************************************************

int
TAO_TestCombinedThreads::copyArgv (char** argv,
                                   char**& dest)
{
  int argc = 0;
  if (argv != 0)
    {
      for (; argv[argc] != 0; argc++);

      ACE_NEW_RETURN (dest, char*[argc + 1], -1);
      for (int i = 0; i < argc; i++)
        {
          ACE_NEW_RETURN (dest[i], char[strlen (argv[i]) + 1], 0);
          ACE_OS_String::strcpy (dest[i],
                                 argv[i]);
        }
      dest[argc] = 0;
    }
  return argc;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::deleteArguments
// Argument    : char** args - char* array to be deleted
// Description : Deletes the copied arguments.
// Notes       : private
// *******************************************************************

void
TAO_TestCombinedThreads::deleteArguments (char** args)
{
  if (args != 0)
    {
      for (int i = 0; args[i] != 0; i++)
        {
          delete [] args[i];
        }
      delete [] args;
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::spawned
// Argument    : void* args
// Description : Find the orbid (if any) and associate the thread id
//               with the orbid.  Also, set the timeout for this
//               thread.  Call the function and then disassociate the
//               orbid.
// Notes       : static
// *******************************************************************

void*
TAO_TestCombinedThreads::spawned (void* args)
{
  TestArgs* testArgs = (TestArgs*)args;

  // This ORB id per thread hack is here since so many tests and service
  // executables use _this to activate objects with the default POA and
  // very few overide the _default_POA method.  I keep a map of ORB ids
  // per thread id and use a hacked version of ORB_Core.cpp to find the
  // ORB id depending on the current thread id.  It seems to work pretty
  // well.
  TAO_TestCombinedThreads::associateORB (testArgs->argv_[0]);


  // Set the timeout for hacked versions of calls to orb->run() or
  // orbmanager.run().  These hacked versions allow threads to exit
  // under their own accord.
  TAO_TestCombinedThreads::setTimeout (testArgs->timeout_);

  // Call the main function and print out the return value
  int status = -1;
  if (testArgs->func_ != 0)
    {
      status = testArgs->func_ (testArgs->argc_, testArgs->argv_);
    }
  else
    {
      ACE_DEBUG ((LM_ERROR, "ERROR: testArgs->func_ is zero\n"));
    }
  ACE_DEBUG ((LM_DEBUG, "Return status from %s: %d\n",
                        testArgs->argv_[0], status));

  // Disassociate the orb since the thread is done.
  TAO_TestCombinedThreads::disassociateORB ();

  return ACE_reinterpret_cast (void*, status);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::setTimeout
// Argument    : ACE_Time_Value* timeout
// Description : Set the timeout for the current thread id
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::setTimeout (ACE_Time_Value timeout)
{
  static ACE_Time_Value largest;
  // Only bind if timeout is not zero
  if (timeout != ACE_Time_Value::zero)
    {
      ACE_hthread_t threadId;
      ACE_OS::thr_self (threadId);

      TAO_TestCombinedThreads::allocateTimeMap ();

      timeMap->bind (threadId, timeout);
      if (timeout > largest)
        {
          ACE_Time_Value dummy;
          largest = timeout;
          timeMap->unbind (largestId, dummy);
          timeMap->bind (largestId, largest);
        }
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::associateORB
// Argument    : const char* orbId
// Description : Associate the current thread id with the orb id.
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::associateORB (const char* orbId)
{
  ACE_hthread_t threadId;
  ACE_OS::thr_self (threadId);

  TAO_TestCombinedThreads::allocateORBIdMap ();

  idMap->bind (threadId, orbId);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::disassociateORB
// Description : Disassociate the current thread id from the orb id
//               map.
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::disassociateORB (void)
{
  ACE_hthread_t threadId;
  ACE_OS::thr_self (threadId);

  if (idMap != 0)
    {
      idMap->unbind (threadId);
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::getORBId
// Description : Return the orb id associated with the current thread
//               id.
// Notes       : static
// *******************************************************************

const char*
TAO_TestCombinedThreads::getORBId (void)
{
  const char* id = "";
  ACE_hthread_t threadId;
  ACE_OS::thr_self (threadId);

  if (idMap != 0)
    {
      idMap->find (threadId, id);
    }

  return id;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::getTimeout
// Description : Return the timeout associated with the current thread
//               id.
// Notes       : static
// *******************************************************************

ACE_Time_Value*
TAO_TestCombinedThreads::getTimeout (void)
{
  ACE_Time_Value* timeout = 0;
  ACE_hthread_t threadId;
  ACE_OS::thr_self (threadId);

  if (timeMap != 0)
    {
      ACE_Hash_Map_Entry<ACE_hthread_t, ACE_Time_Value>* entry;
      if (timeMap->find (threadId, entry) == 0)
        {
          timeout = &(entry->int_id_);
        }
      if (timeout == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "WARNING: Could not find timeout for task %d\n",
                      threadId));
          // Couldn't find the timeout for the current thread, so
          // we try to get the largest timeout registered.
          if (timeMap->find (largestId, entry) == 0)
            {
              timeout = &(entry->int_id_);
            }
          else
            {
              // If that doesn't work, fall back to the 60 second default
              timeMap->bind (threadId, ACE_Time_Value(60));
              if (timeMap->find (threadId, entry) == 0)
                {
                  timeout = &(entry->int_id_);
                }
            }
        }
    }

  return timeout;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::getRandomPortBase
// Description : Returns a random port number between 0 and 32767
// Notes       : static - The port returned may already be in use.
//               I don't care to seed the rand function; it really
//               doesn't matter how random it is.
// *******************************************************************

int
TAO_TestCombinedThreads::getRandomPortBase (void)
{
  static const int min = 0;
  static const int max = 0x7fff;
  int port = (ACE_OS::rand () & max);
  return (port < min ? port + min : port);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::thr_join
// Description : Wait on the specified thread name
// Notes       : static
// *******************************************************************

int
TAO_TestCombinedThreads::thr_join (ACE_thread_t thr_handle,
                               ACE_thread_t *thr_id,
                               void** status)
{
  ACE_UNUSED_ARG (thr_id);
  return TAO_TestCombinedThreads::thr_join (taskNameToId (thr_handle),
                                            status);
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::thr_join
// Description : Wait on the specified thread handle
// Notes       : static
// *******************************************************************

int
TAO_TestCombinedThreads::thr_join (ACE_hthread_t thr_handle,
                                   void** status)
{
  ACE_UNUSED_ARG (status);

  // This method can not support joining all threads
  if (ACE_OS::thr_cmp (thr_handle, ACE_OS::NULL_hthread))
    {
      ACE_NOTSUP_RETURN (-1);
    }

  int retval = ESRCH;
  ACE_hthread_t current;
  ACE_OS::thr_self (current);

  // Make sure we are not joining ourself
  if (ACE_OS::thr_cmp (thr_handle, current))
    {
      retval = EDEADLK;
    }
  else
    {
      // Whether the task exists or not
      // we will return a successful value
      retval = 0;

      // Verify that the task id still exists
      while (taskIdVerify (thr_handle) == OK)
        {
          // Wait a bit to see if the task is still active.
          // I'm not sure what a good value would be.
          ACE_OS::sleep (5);
        }
    }

  // Adapt the return value into errno and return value.
  // The ACE_ADAPT_RETVAL macro doesn't exactly do what
  // we need to do here, so we do it manually.
  if (retval != 0)
    {
      errno = retval;
      retval = -1;
    }

  return retval;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::getRandomString
// Description : Returns a random string based on the starting point
// Notes       : static
// *******************************************************************

ACE_CString
TAO_TestCombinedThreads::getRandomString (const char* base)
{
  ACE_CString string ("");
  char number[32];

  if (base != 0)
    {
      string = base;
    }
  string += ACE_OS_String::itoa (ACE_OS::rand (), number, 10);

  return string;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::cleanORBIdMap
// Description : Delete the orb id hash map.
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::cleanORBIdMap (void*, void*)
{
  delete idMap;
  idMap = 0;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::allocateORBIdMap
// Description : Allocate the orb id hash map and register it with the
//               object manager.
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::allocateORBIdMap (void)
{
  if (idMap == 0)
    {
      ACE_NEW (idMap, TestORBIDMap ());
      idMap->open (5);
      ACE_Object_Manager::at_exit (idMap,
                                   &TAO_TestCombinedThreads::cleanORBIdMap,
                                   0);
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::cleanTimeMap
// Description : Delete the time value hash map.
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::cleanTimeMap (void*, void*)
{
  delete timeMap;
  timeMap = 0;
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::allocateTimeMap
// Description : Allocate the time value hash map and register it with
//               the object manager.
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::allocateTimeMap (void)
{
  if (timeMap == 0)
    {
      ACE_NEW (timeMap, TestTimeValueMap ());
      timeMap->open (5);
      ACE_Object_Manager::at_exit (timeMap,
                                   &TAO_TestCombinedThreads::cleanTimeMap,
                                   0);
    }
}


// *******************************************************************
// Method      : TAO_TestCombinedThreads::destroyThreadManager
// Description : Delete the thread manager
// Notes       : static
// *******************************************************************

void
TAO_TestCombinedThreads::destroyThreadManager (void*, void*)
{
  delete thread_manager;
  thread_manager = 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<ACE_Time_Value>;
template class ACE_Hash_Map_Manager<ACE_hthread_t, const char*, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_hthread_t,const char*>;
template class ACE_Hash_Map_Manager<ACE_hthread_t, ACE_Time_Value, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_hthread_t,ACE_Time_Value>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<ACE_Time_Value>
#pragma instantiate ACE_Hash_Map_Manager<ACE_hthread_t, const char*, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_hthread_t, const char*, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_hthread_t,const char*>
#pragma instantiate ACE_Hash_Map_Manager<ACE_hthread_t, ACE_Time_Value, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_hthread_t, ACE_Time_Value, ACE_Hash<ACE_hthread_t>, ACE_Equal_To<ACE_hthread_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_hthread_t,ACE_Time_Value>
#endif

