// -*- C++ -*-

//==========================================================================
/**
 *  @file    TSS_T.h
 *
 *  $Id$
 *
 *   Moved from Synch.h.
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//==========================================================================

#ifndef ACE_TSS_T_H
#define ACE_TSS_T_H
#include /**/ "ace/pre.h"

#include "ace/Lock.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread_Mutex.h"

// This should probably go somewhere else, but it's only used here and in Thread_Manager.
# if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION))
#   define ACE_TSS_TYPE(T) ACE_TSS< T >
#   if defined (ACE_HAS_BROKEN_CONVERSIONS)
#     define ACE_TSS_GET(I, T) (*(I))
#   else
#     define ACE_TSS_GET(I, T) ((I)->operator T * ())
#   endif /* ACE_HAS_BROKEN_CONVERSIONS */
# else
#   define ACE_TSS_TYPE(T) T
#   define ACE_TSS_GET(I, T) (I)
# endif /* ACE_HAS_THREADS && (ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATIOND) */

/**
 * @class ACE_TSS
 *
 * @brief Allows objects that are "physically" in thread specific
 * storage (i.e., private to a thread) to be accessed as though
 * they were "logically" global to a program.
 *
 * This class is a wrapper around the OS thread library
 * thread-specific functions.  It uses the <C++ operator->> to
 * shield applications from the details of accessing
 * thread-specific storage.
 *
 * NOTE: For maximal portability, <TYPE> cannot be a built-in type,
 * but instead should be a user-defined class (some compilers will
 * allow a built-in type, others won't).  See template class
 * ACE_TSS_Type_Adapter, below, for adapting built-in types to work
 * with ACE_TSS.
 */
template <class TYPE>
class ACE_TSS
{
public:
  // = Initialization and termination methods.

  /**
   * If caller has passed us a non-NULL ts_obj *, then we'll just use
   * this to initialize the thread-specific value (but only for the
   * calling thread).  Thus, subsequent calls to <operator->> in this
   * thread will return this value.  This is useful since it enables
   * us to assign objects to thread-specific data that have
   * arbitrarily complex constructors.
   */
  ACE_TSS (TYPE *ts_obj = 0);

  /// Deregister with thread-key administration.
  virtual ~ACE_TSS (void);

  // = Accessors.

  /**
   * Get the thread-specific object for the key associated with this
   * object.  Returns 0 if the data has never been initialized,
   * otherwise returns a pointer to the data.
   */
  TYPE *ts_object (void) const;

  /// Set the thread-specific object for the key associated with this
  /// object.
  TYPE *ts_object (TYPE *);

  /// Use a "smart pointer" to get the thread-specific object
  /// associated with the <key_>.
  TYPE *operator-> () const;

  /// Return or create and return the calling threads TYPE object.
  operator TYPE *(void) const;

  /// Hook for construction parameters.
  virtual TYPE *make_TSS_TYPE (void) const;

  // = Utility methods.

  /// Dump the state of an object.
  void dump (void) const;

  // ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  /// Actually implements the code that retrieves the object from
  /// thread-specific storage.
  TYPE *ts_get (void) const;

  /// Factors out common code for initializing TSS.  This must NOT be
  /// called with the lock held...
  int ts_init (void) const;

#if !(defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)))
  /// This implementation only works for non-threading systems...
  TYPE *type_;
#else
  /// Avoid race conditions during initialization.
  ACE_Thread_Mutex keylock_;

  /// "First time in" flag.
  int once_;

  /// Key for the thread-specific error data.
  ACE_thread_key_t key_;

  /// "Destructor" that deletes internal TYPE * when thread exits.
  static void cleanup (void *ptr);
#endif /* defined (ACE_HAS_THREADS) && (defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION)) */
  // = Disallow copying...
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_TSS<TYPE> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_TSS (const ACE_TSS<TYPE> &))
};

/**
 * @class ACE_TSS_Type_Adapter
 *
 * @brief Adapter that allows built-in types to be used with ACE_TSS.
 *
 * Wraps a value of a built-in type, providing conversions to
 * and from the type.  Example use with ACE_TSS:
 * ACE_TSS<ACE_TSS_Type_Adapter<int> > i;
 * *i = 37;
 * ACE_OS::fprintf (stderr, "%d\n", *i);
 * Unfortunately, though, some compilers have trouble with the
 * implicit type conversions.  This seems to work better:
 * ACE_TSS<ACE_TSS_Type_Adapter<int> > i;
 * i->operator int & () = 37;
 * ACE_OS::fprintf (stderr, "%d\n", i->operator int ());
 */
template <class TYPE>
class ACE_TSS_Type_Adapter
{
public:
  /// Constructor.  Inlined here so that it should _always_ be inlined.
  ACE_TSS_Type_Adapter (const TYPE value = 0): value_ (value) {}

  /// TYPE conversion.  Inlined here so that it should _always_ be
  /// inlined.
  operator TYPE () const { return value_; };

  /// TYPE & conversion.  Inlined here so that it should _always_ be
  /// inlined.
  operator TYPE &() { return value_; };

private:
  /// The wrapped value.
  TYPE value_;
};

#if defined (__ACE_INLINE__)
#include "ace/TSS_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/TSS_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("TSS_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_TSS_T_H */
