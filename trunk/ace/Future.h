/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Future.h
//
// = AUTHOR
//    Andres Kruse <Andres.Kruse@cern.ch> and Douglas C. Schmidt
//    <schmidt@cs.wustl.edu> 
// 
// ============================================================================

#if !defined (ACE_FUTURE_H)
#define ACE_FUTURE_H

#include "ace/Synch.h"

#if defined (ACE_HAS_THREADS)

// Forward decl.
template <class T> class ACE_Future;

template <class T> class ACE_Future_Rep
// = TITLE
//     
//     ACE_Future_Rep<T>
//
// = DESCRIPTION
//     An ACE_Future_Rep<T> object encapsules a pointer to an 
//     object of class T which is the result of an asynchronous
//     method invocation. It is pointed to by ACE_Future<T> object[s]
//     and only accessible through them.
{
  friend class ACE_Future<T>;

private:

  int set (const T &r);
  // Set the result value.

  int get (T &value, ACE_Time_Value *tv);
  // Wait up to <tv> time to get the <value>.

  operator T ();
  // Type conversion. will block forever until the result is
  // available.  Note that this method is going away in a subsequent
  // release since it doesn't distinguish between failure results and
  // success results (exceptions should be used, but they aren't
  // portable...).  The <get> method should be used instead since it
  // separates the error value from the result, and also permits
  // timeouts.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  // = constructor and destructor private
  ACE_Future_Rep (void);
  ~ACE_Future_Rep (void);

  int ready (void);
  // Is result available?

  T *value_;
  // Pointer to the result.

  ACE_Atomic_Op<ACE_Thread_Mutex, int> ref_count_;
  // Reference count.

  // = Condition variable and mutex that protect the <value_>.
  ACE_Condition_Thread_Mutex value_ready_;
  ACE_Thread_Mutex value_ready_mutex_;
};

template <class T> class ACE_Future 
  // = TITLE
  //     This class implements a ``single write, multiple read'' pattern
  //     that can be used to return results from asynchronous method
  //     invocations.
  // = DESCRIPTION
{
public:
  // = Initialization and termination methods.
  ACE_Future (void);
  // Constructor.

  ACE_Future (const ACE_Future<T> &r);
  // Copy constructor binds <this> and <r> to the same
  // <ACE_Future_Rep>. An <ACE_Future_Rep> is created if necessary.

  ACE_Future (const T &r);
  // Constructor that initialises an <ACE_Future> to point to the
  // result <r> immediately.

  ~ACE_Future (void);
  // Destructor.

  void operator = (const ACE_Future<T> &r);
  // Assignment operator that binds <this> and <r> to the same
  // <ACE_Future_Rep>. An <ACE_Future_Rep> is created if necessary.

  int cancel (const T &r);
  // Cancel an <ACE_Future> and assign the value <r>.  It is used if a
  // client does not want to wait for <T> to be produced.

  int operator == (const ACE_Future<T> &r) const;
  // Equality operator that returns 1 if both ACE_Future<T> objects
  // point to the same ACE_Future_Rep<T> object.  Attention: It also
  // returns 1 if both objects have just been instantiated and not
  // used yet.

  int operator != (const ACE_Future<T> &r) const;
  // Inequality operator, which is the opposite of equality.

  int set (const T &r);
  // Make the result available. Is used by the server thread to give
  // the result to all waiting clients.

  int get (T &value, ACE_Time_Value *tv = 0);
  // Wait up to <tv> time to get the <value>.

  operator T ();
  // Type conversion, which obtains the result of the asynchronous
  // method invocation.  Will block forever.  Note that this method is
  // going away in a subsequent release since it doesn't distinguish
  // between failure results and success results (exceptions should be
  // used, but they aren't portable...).  The <get> method should be
  // used instead since it separates the error value from the result,
  // and also permits timeouts.

  int ready (void);
  // Check if the result is available.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Future_Rep<T> *create_rep_i (void) const;
  // Create the <ACE_Future_Rep> object.

  void* operator new (size_t nbytes);
  // Do not allow new operator.

  void operator delete(void *);
  // Do not allow delete operator

  void operator &();
  // Do not allow address-of operator.

  ACE_Future_Rep<T> *future_rep_;
  // the ACE_Future_Rep

  ACE_Thread_Mutex mutex_;
  // Protect operations on the <Future>.

};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Future.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Future.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */
#endif /* ACE_HAS_THREADS */
#endif /* ACE_FUTURE_H */
