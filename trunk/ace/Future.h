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
// = AUTHOR (S)
//    Andres Kruse <Andres.Kruse@cern.ch>,
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>,
//    Per Andersson <Per.Andersson@hfera.ericsson.se>, and
//    John Tucker <jtucker@infoglide.com>
//
// ============================================================================

#ifndef ACE_FUTURE_H
#define ACE_FUTURE_H

#include /**/ "ace/Containers.h"
#include /**/ "ace/Synch.h"
#include /**/ "ace/Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_THREADS)

// Forward decl.
template <class T> class ACE_Future_Holder;
template <class T> class ACE_Future_Observer;
template <class T> class ACE_Future_Rep;
template <class T> class ACE_Future;

template <class T>
class ACE_Export ACE_Future_Holder
{
  // = TITLE
  //     Implementation of object which has holds ACE_Future.

public:
  ACE_Future_Holder (const ACE_Future<T> &future);
  ~ACE_Future_Holder (void);

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  ACE_Future<T> item_;

protected:
  ACE_Future_Holder (void);
};

template <class T>
class ACE_Future_Observer
{
  // = TITLE
  //     ACE_Future_Observer<T>
  //
  // = DESCRIPTION
  //     An ACE_Future_Observer object implements an object that is
  //     subscribed with an ACE_Future object so that it may be
  //     notified when the value of the ACE_Future object is
  //     written to by a writer thread.
  //
  //     It uses the Observer pattern
public:
  // = Destructor
  virtual ~ACE_Future_Observer (void);

  virtual void update (const ACE_Future<T> &future) = 0;
  // Called by the ACE_Future in which we are subscribed to when
  // its value is written to.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
protected:

  // = Constructor
  ACE_Future_Observer (void);
};

template <class T>
class ACE_Future_Rep
{
  // = TITLE
  //     ACE_Future_Rep<T>
  //
  // = DESCRIPTION
  //     An ACE_Future_Rep<T> object encapsules a pointer to an object
  //     of class T which is the result of an asynchronous method
  //     invocation. It is pointed to by ACE_Future<T> object[s] and
  //     only accessible through them.
private:
  friend class ACE_Future<T>;

  int set (const T &r,
           ACE_Future<T> &caller);
  // Set the result value.  The specified <caller> represents the
  // future that invoked this <set> method, which is used to notify
  // the list of future observers.

  int get (T &value,
           ACE_Time_Value *tv);
  // Wait up to <tv> time to get the <value>.  Note that <tv> must be
  // specified in absolute time rather than relative time.

  int attach (ACE_Future_Observer<T> *observer,
               ACE_Future<T> &caller);
  // Attaches the specified observer to a subject (i.e. the
  // <ACE_Future_Rep>).  The update method of the specified subject will
  // be invoked with a copy of the written-to <ACE_Future> as input when
  // the result gets set.
  //
  // Returns 0 if the observer is successfully attached, 1 if the
  // observer is already attached, and -1 if failures occur.

  int detach (ACE_Future_Observer<T> *observer);
  // Detaches the specified observer from a subject (i.e. the
  // <ACE_Future_Rep>).  The update method of the specified subject will
  // not be invoked when the <ACE_Future_Rep>s result gets set.  Returns
  // 1 if the specified observer was actually attached to the subject
  // prior to this call and 0 if was not.
  //
  // Returns 0 if the observer was successfully detached, and -1 if the
  // observer was not attached in the first place.

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

  // = Encapsulate reference count and object lifetime of instances.

  static ACE_Future_Rep<T> *create (void);
  // Create a ACE_Future_Rep<T> and initialize the reference count.

  static ACE_Future_Rep<T> *attach (ACE_Future_Rep<T> *&rep);
  // Increase the reference count and return argument. Uses the
  // attribute "value_ready_mutex_" to synchronize reference count
  // updating.
  //
  // Precondition (rep != 0).

  static void detach (ACE_Future_Rep<T> *&rep);
  // Decreases the reference count and and deletes rep if there are no
  // more references to rep.
  //
  // Precondition (rep != 0)

  static void assign (ACE_Future_Rep<T> *&rep,
                      ACE_Future_Rep<T> *new_rep);
  // Decreases the rep's reference count and and deletes rep if there
  // are no more references to rep. Then assigns new_rep to rep.
  //
  // Precondition (rep != 0 && new_rep != 0)

  // = Constructor and destructor private
  ACE_Future_Rep (void);
  ~ACE_Future_Rep (void);

  int ready (void);
  // Is result available?

  T *value_;
  // Pointer to the result.

  int ref_count_;
  // Reference count.

  typedef ACE_Future_Observer<T>
	    OBSERVER;

  typedef ACE_Unbounded_Set<OBSERVER *>
        OBSERVER_COLLECTION;

  OBSERVER_COLLECTION observer_collection_;
  // Keep a list of ACE_Future_Observers unread by client's reader thread.

  // = Condition variable and mutex that protect the <value_>.
  ACE_Condition_Thread_Mutex value_ready_;
  ACE_Thread_Mutex value_ready_mutex_;
};

template <class T>
class ACE_Future
{
  // = TITLE
  //     This class implements a ``single write, multiple read''
  //     pattern that can be used to return results from asynchronous
  //     method invocations.
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

  int cancel (void);
  // Cancel an <ACE_Future>.  Put the future into its initial
  // state. Returns 0 on succes and -1 on failure. It is now possible
  // to reuse the ACE_Future<T>. But remember, the ACE_Future<T>
  // is now bound to a new ACE_Future_Rep<T>.

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

  int get (T &value,
           ACE_Time_Value *tv = 0);
  // Wait up to <tv> time to get the <value>.  Note that <tv> must be
  // specified in absolute time rather than relative time.

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

  int attach (ACE_Future_Observer<T> *observer);
  // Attaches the specified observer to a subject (i.e. the
  // <ACE_Future>).  The update method of the specified subject will be
  // invoked with a copy of the associated <ACE_Future> as input when
  // the result gets set.  If the result is already set when this
  // method gets invoked, then the update method of the specified
  // subject will be invoked immediately.
  //
  // Returns 0 if the observer is successfully attached, 1 if the
  // observer is already attached, and -1 if failures occur.

  int detach (ACE_Future_Observer<T> *observer);
  // Detaches the specified observer from a subject (i.e. the
  // <ACE_Future_Rep>).  The update method of the specified subject will
  // not be invoked when the <ACE_Future_Reps> result gets set.  Returns
  // 1 if the specified observer was actually attached to the subject
  // prior to this call and 0 if was not.
  //
  // Returns 0 if the observer was successfully detached, and -1 if the observer was
  // not attached in the first place.

  void dump (void) const;
  // Dump the state of an object.

  ACE_Future_Rep<T> *get_rep ();
  // Get the underlying <ACE_Future_Rep>*. Note that this method should
  // rarely, if ever, be used and that modifying the undlerlying <ACE_Future_Rep>*
  // should be done with extreme caution.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  void *operator new (size_t nbytes);
  // Do not allow new operator.

  void operator delete (void *);
  // Do not allow delete operator

  void operator & ();
  // Do not allow address-of operator.

  // the ACE_Future_Rep
  typedef ACE_Future_Rep<T> FUTURE_REP;
  FUTURE_REP *future_rep_;
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
