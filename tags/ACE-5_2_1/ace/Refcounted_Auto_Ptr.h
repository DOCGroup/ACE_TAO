/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Refcounted_Auto_Ptr.h
 *
 *  $Id$
 *
 *  @author John Tucker <JTucker@infoglide.com>
 */
//=============================================================================

#ifndef ACE_REFCOUNTED_AUTO_PTR_H
#define ACE_REFCOUNTED_AUTO_PTR_H
#include "ace/pre.h"

#include "ace/OS.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decl.
template <class X, class ACE_LOCK> class ACE_Refcounted_Auto_Ptr_Rep;
template <class X, class ACE_LOCK> class ACE_Refcounted_Auto_Ptr;

/**
 * @class ACE_Refcounted_Auto_Ptr
 *
 * @brief This class implements support for a reference counted auto_ptr.
 * Assigning or copying instances of an ACE_Refcounted_Auto_Ptr
 * will automatically increment the reference count. When the last
 * instance that references a ACE_Refcounted_Auto_Ptr instance is
 * destroyed or overwritten, it will invoke delete on its underlying
 * pointer.
 */
template <class X, class ACE_LOCK>
class ACE_Refcounted_Auto_Ptr
{
public:

  // = Initialization and termination methods.

  /// Constructor that initializes an <ACE_Refcounted_Auto_Ptr> to
  /// point to the result <r> immediately.
  ACE_Refcounted_Auto_Ptr (X *p = 0);

  /// Copy constructor binds <this> and <r> to the same 
  /// <ACE_Refcounted_Auto_Ptr_Rep>. An <ACE_Refcounted_Auto_Ptr_Rep> 
  /// is created if necessary.
  ACE_Refcounted_Auto_Ptr (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r);

  /// Destructor.
  virtual ~ACE_Refcounted_Auto_Ptr (void);

  /// Assignment operator that binds <this> and <r> to the same 
  /// <ACE_Refcounted_Auto_Ptr_Rep>. An <ACE_Refcounted_Auto_Ptr_Rep> 
  /// is created if necessary.
  void operator = (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r);

  /// Equality operator that returns 1 if both 
  /// ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> objects point to the same 
  /// ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> object.  Attention: It 
  /// also returns 1 if both objects have just been instantiated and 
  /// not used yet.
  int operator == (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r) const;

  /// Inequality operator, which is the opposite of equality.
  int operator != (const ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> &r) const;

  /// Redirection operator
  X *operator-> (void) const;

  // = Accessor methods.

  X &operator *() const;

  /// Sets the pointer value to 0 and returns its old value.
  X *release (void);
  
  /// Invokes delete on the previous pointer value and then sets the
  /// pointer value to the specified value.
  void reset (X *p = 0);

  /// Get the pointer value.
  X *get (void);

  /// Get the reference count value.
  int count (void) const;

  // = Utility method.
  
  /// Allows us to check for NULL on all ACE_Refcounted_Auto_Ptr objects.
  int null (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// the ACE_Refcounted_Auto_Ptr_Rep
  typedef ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> AUTO_REFCOUNTED_PTR_REP;

  /// Protect operations on the <ACE_Refcounted_Auto_Ptr>.
  AUTO_REFCOUNTED_PTR_REP *rep_;
};

/**
 * @class ACE_Refcounted_Auto_Ptr_Rep
 *
 * @brief An ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> object
 * incapsulates a pointer to an object of type X.  It is pointed to by
 * ACE_Refcounted_Auto_Ptr<X, ACE_LOCK> object[s] and only accessible
 * through them. 
 */
template <class X, class ACE_LOCK>
class ACE_Refcounted_Auto_Ptr_Rep
{
private:
  friend class ACE_Refcounted_Auto_Ptr<X, ACE_LOCK>;

  /// Sets the pointer value to 0 and returns its old value.
  X *release (void);

  /// Invokes delete on the previous pointer value and then
  /// sets the pointer value to the specified value.
  void reset (X *p = 0);

  /// Get the pointer value.
  X *get (void);

  /// Get the reference count value.
  int count (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  // = Encapsulate reference count and object lifetime of instances.
  // These methods must go after the others to work around a bug with
  // Borland's C++ Builder...

  /// Allocate a new ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> instance,
  /// returning NULL if it cannot be created.
  static ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *internal_create (X *p);

  /// Create a ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> and initialize
  /// the reference count.
  static ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *create (X *p);

  /// Increase the reference count and return argument. Uses the
  /// attribute "ace_lock_" to synchronize reference count updating.
  ///
  /// Precondition (rep != 0).
  static ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *attach (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *&rep);

  /// Decreases the reference count and and deletes rep if there are no
  /// more references to rep.
  ///
  /// Precondition (rep != 0)
  static void detach (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *&rep);

  /// Decreases the rep's reference count and and deletes rep if there
  /// are no more references to rep. Then assigns new_rep to rep.
  ///
  /// Precondition (rep != 0 && new_rep != 0)
  static void assign (ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *&rep,
                      ACE_Refcounted_Auto_Ptr_Rep<X, ACE_LOCK> *new_rep);

  /// Pointer to the result.
  ACE_Auto_Basic_Ptr<X> ptr_;

  /// Reference count.
  int ref_count_;

  // = Mutex variable to protect the <ptr_>.

  /// Synchronization variable for the MT_SAFE <ACE_Hash_Map_Manager_Ex>.
  ACE_LOCK lock_;

private:
  /// Allows us to check for NULL on all ACE_Refcounted_Auto_Ptr objects.
  int null (void) const;

  // = Constructor and destructor private.
  ACE_Refcounted_Auto_Ptr_Rep (X *p = 0);
  ~ACE_Refcounted_Auto_Ptr_Rep (void);
};

#include "ace/Refcounted_Auto_Ptr.i"

#include "ace/post.h"
#endif /* ACE_AUTO_PTR_H */
