/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Select_Reactor_Handler_Repository.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SELECT_REACTOR_HANDLER_REPOSITORY_H
#define ACE_SELECT_REACTOR_HANDLER_REPOSITORY_H

#include "ace/pre.h"

#include "ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "Event_Handler.h"

class ACE_Select_Reactor_Impl;
class ACE_Event_Handler;

/**
 * @class ACE_Event_Tuple
 *
 * @brief An ACE_Event_Handler and its associated ACE_HANDLE.
 *
 * One <ACE_Event_Handler> is registered for one or more
 * <ACE_HANDLE>.  At various points, this information must be
 * stored explicitly.  This class provides a lightweight
 * mechanism to do so.
 */
class ACE_Export ACE_Event_Tuple
{
public:
  /// Default constructor.
  ACE_Event_Tuple (void);

  /// Constructor.
  ACE_Event_Tuple (ACE_Event_Handler *eh,
                   ACE_HANDLE h);

  /// Destructor.
  ~ACE_Event_Tuple (void);

  /// Equality operator.
  int operator== (const ACE_Event_Tuple &rhs) const;

  /// Inequality operator.
  int operator!= (const ACE_Event_Tuple &rhs) const;

  /// Handle.
  ACE_HANDLE handle_;

  /// <ACE_Event_Handler> associated with the <ACE_HANDLE>.
  ACE_Event_Handler *event_handler_;
};


//===================================================================
/**
 * @class ACE_Select_Reactor_Handler_Repository
 *
 * @brief Used to map <ACE_HANDLE>s onto the appropriate
 * <ACE_Event_Handler> *.
 *
 * This class is necessary to shield differences between UNIX
 * and Win32.  In UNIX, <ACE_HANDLE> is an int, whereas in Win32
 * it's a void *.  This class hides all these details from the
 * bulk of the <ACE_Select_Reactor> code.  All of these methods
 * are called with the main <Select_Reactor> token lock held.
 */
class ACE_Export ACE_Select_Reactor_Handler_Repository
{
public:
  friend class ACE_Select_Reactor_Handler_Repository_Iterator;

  // = Initialization and termination methods.
  /// Default "do-nothing" constructor.
  ACE_Select_Reactor_Handler_Repository (ACE_Select_Reactor_Impl &);

  /// Destructor.
  ~ACE_Select_Reactor_Handler_Repository (void);

  /// Initialize a repository of the appropriate <size>.
  /**
   * On Unix platforms, the size parameter should be as large as the
   * maximum number of file descriptors allowed for a given process.
   * This is necessary since a file descriptor is used to directly
   * index the array of event handlers maintained by the Reactor's
   * handler repository.  Direct indexing is used for efficiency
   * reasons.
   */
  int open (size_t size);

  /// Close down the repository.
  int close (void);

  // = Search structure operations.

  /**
   * Return the <ACE_Event_Handler *> associated with <ACE_HANDLE>.
   * If <index_p> is non-0, then return the index location of the
   * <handle>, if found.
   */
  ACE_Event_Handler *find (ACE_HANDLE handle, size_t *index_p = 0);

  /// Bind the <ACE_Event_Handler *> to the <ACE_HANDLE> with the
  /// appropriate <ACE_Reactor_Mask> settings.
  int bind (ACE_HANDLE,
            ACE_Event_Handler *,
            ACE_Reactor_Mask);

  /// Remove the binding of <ACE_HANDLE> in accordance with the <mask>.
  int unbind (ACE_HANDLE,
              ACE_Reactor_Mask mask);

  /// Remove all the <ACE_HANDLE, ACE_Event_Handler> tuples.
  int unbind_all (void);

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of legal handles (i.e., >= 0 && < max_size_).
  int invalid_handle (ACE_HANDLE handle);

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of currently registered handles (i.e., >= 0 && <
  // max_handlep1_).
  int handle_in_range (ACE_HANDLE handle);

  // = Accessors.
  /// Returns the current table size.
  size_t size (void) const;

  /// Maximum ACE_HANDLE value, plus 1.
  size_t max_handlep1 (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Reference to our <Select_Reactor>.
  ACE_Select_Reactor_Impl &select_reactor_;

  /// Maximum number of handles.
  ssize_t max_size_;

  /// The highest currently active handle, plus 1 (ranges between 0 and
  /// <max_size_>.
  int max_handlep1_;

#if defined (ACE_WIN32)
  // = The mapping from <HANDLES> to <Event_Handlers>.

  /**
   * The NT version implements this via a dynamically allocated
   * array of <ACE_Event_Tuple *>.  Since NT implements ACE_HANDLE
   * as a void * we can't directly index into this array.  Therefore,
   * we just do a linear search (for now).  Next, we'll modify
   * things to use hashing or something faster...
   */
  ACE_Event_Tuple *event_handlers_;
#else
  /**
   * The UNIX version implements this via a dynamically allocated
   * array of <ACE_Event_Handler *> that is indexed directly using
   * the ACE_HANDLE value.
   */
  ACE_Event_Handler **event_handlers_;
#endif /* ACE_WIN32 */
};

//=================================================================

/**
 * @class ACE_Select_Reactor_Handler_Repository_Iterator
 *
 * @brief Iterate through the <ACE_Select_Reactor_Handler_Repository>.
 */
class ACE_Export ACE_Select_Reactor_Handler_Repository_Iterator
{
public:
  // = Initialization method.
  ACE_Select_Reactor_Handler_Repository_Iterator (const ACE_Select_Reactor_Handler_Repository *s);

  /// dtor.
  ~ACE_Select_Reactor_Handler_Repository_Iterator (void);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (ACE_Event_Handler *&next_item);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Reference to the Handler_Repository we are iterating over.
  const ACE_Select_Reactor_Handler_Repository *rep_;

  /// Pointer to the current iteration level.
  ssize_t current_;
};


#if defined (__ACE_INLINE__)
#include "Select_Reactor_Handler_Repository.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*ACE_SELECT_REACTOR_HANDLER_REPOSITORY_H*/
