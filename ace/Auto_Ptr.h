/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Auto_Ptr.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@uci.edu>
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author John Tucker <JTucker@infoglide.com>
 *  @author Jack Reeves <jack@fx.com>
 *  @author Dr. Harald M. Mueller <mueller@garwein.hai.siemens.co.at>
 */
//=============================================================================


#ifndef ACE_AUTO_PTR_H
#define ACE_AUTO_PTR_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Auto_Basic_Ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 * This class allows one to work on non-object (basic) types
 */
template <class X>
class ACE_Auto_Basic_Ptr
{
public:
  // = Initialization and termination methods
  ACE_EXPLICIT ACE_Auto_Basic_Ptr (X *p = 0) : p_ (p) {}

  ACE_Auto_Basic_Ptr (ACE_Auto_Basic_Ptr<X> &ap);
  ACE_Auto_Basic_Ptr<X> &operator= (ACE_Auto_Basic_Ptr<X> &rhs);
  ~ACE_Auto_Basic_Ptr (void);

  // = Accessor methods.
  X &operator *() const;
  X *get (void) const;
  X *release (void);
  void reset (X *p = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  X *p_;
};

#if !defined (ACE_LACKS_AUTO_PTR) && \
     defined (ACE_HAS_STANDARD_CPP_LIBRARY) && \
            (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#include <memory>
#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)
using std::auto_ptr;
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
#else /* ACE_HAS_STANDARD_CPP_LIBRARY */

/**
 * @class auto_ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 */
template <class X>
class auto_ptr : public ACE_Auto_Basic_Ptr <X>
{
public:
  // = Initialization and termination methods
  ACE_EXPLICIT auto_ptr (X *p = 0) : ACE_Auto_Basic_Ptr<X> (p) {}

  X *operator-> () const;
};

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

/**
 * @class ACE_Auto_Basic_Array_Ptr
 *
 * @brief Implements an extension to the draft C++ standard auto_ptr
 * abstraction.  This class allows one to work on non-object
 * (basic) types that must be treated as an array, e.g.,
 * deallocated via "delete [] foo".
 */
template<class X>
class ACE_Auto_Basic_Array_Ptr
{
public:
  // = Initialization and termination methods.
  ACE_EXPLICIT ACE_Auto_Basic_Array_Ptr (X *p = 0) : p_ (p) {}

  ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> &ap);
  ACE_Auto_Basic_Array_Ptr<X> &operator= (ACE_Auto_Basic_Array_Ptr<X> &rhs);
  ~ACE_Auto_Basic_Array_Ptr (void);

  // = Accessor methods.
  X &operator* () const;
  X &operator[] (int i) const;
  X *get (void) const;
  X *release (void);
  void reset (X *p = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  X *p_;
};

/**
 * @class ACE_Auto_Array_Ptr
 *
 * @brief Implements an extension to the draft C++ standard auto_ptr
 * abstraction.
 */
template<class X>
class ACE_Auto_Array_Ptr : public ACE_Auto_Basic_Array_Ptr<X>
{
public:
  // = Initialization and termination methods.
  ACE_EXPLICIT ACE_Auto_Array_Ptr (X *p = 0)
    : ACE_Auto_Basic_Array_Ptr<X> (p) {}

  X *operator-> () const;
};

// Some platforms have an older version of auto_ptr
// support, which lacks reset, and cannot be disabled
// easily.  Portability to these platforms requires
// use of the following ACE_AUTO_PTR_RESET macro.
# if defined (ACE_AUTO_PTR_LACKS_RESET)
#   define ACE_AUTO_PTR_RESET(X,Y,Z) \
      do { \
        if (Y != X.get ()) \
          { \
            X.release (); \
            X = auto_ptr<Z> (Y); \
          } \
      } while (0)
# else /* ! ACE_AUTO_PTR_LACKS_RESET */
#   define ACE_AUTO_PTR_RESET(X,Y,Z) \
      do { \
         X.reset (Y); \
      } while (0)
# endif /* ACE_AUTO_PTR_LACKS_RESET */

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
  ~ACE_Refcounted_Auto_Ptr (void);

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

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  // = Encapsulate reference count and object lifetime of instances.
  // These methods must go after the others to work around a bug with
  // Borland's C++ Builder...

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
  // = Constructor and destructor private.
  ACE_Refcounted_Auto_Ptr_Rep (X *p = 0);
  ~ACE_Refcounted_Auto_Ptr_Rep (void);
};

#if defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Auto_Ptr.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Auto_Ptr.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_AUTO_PTR_H */
