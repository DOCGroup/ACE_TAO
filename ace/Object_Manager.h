/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Object_Manager.h
//
// = AUTHOR
//    David L. Levine
//
// ============================================================================

#if !defined (ACE_OBJECT_MANAGER_H)
#define ACE_OBJECT_MANAGER_H

#include "ace/Containers.h"

class ACE_Export ACE_Object_Manager
  // = TITLE
  //     Manager for ACE library services.
  //
  // = DESCRIPTION
  // This class shuts down ACE library services, reclaiming their storage,
  // at program termination.  It does that by creating a static instance,
  // whose destructor gets called along with those of all other static
  // objects.  Hooks are provided for application code to register objects
  // and arrays for destruction.
  // Please note that the order of such destructor calls is not specified.
  // Therefore, these destructors should not depend on any of the static
  // instances.  Also note that ACE_Log_Msg currently takes care of its
  // own cleanup.
  // It would be worth adding a capability to do the shutdown prior to
  // static object destruction, e.g., via an at_exit () call.  Without
  // that capability, on VxWorks, for example, the program can be unloaded
  // for this to work.  (On VxWorks, alternatively, the explicity OS calls
  // to call all static destructors and constructors could be used.)
{
public:
  static ACE_Object_Manager *instance ();
  // Accessor to singleton instance.

  static int delete_at_exit (void *object);
  // Register an object for deletion at program termination.
  // Returns 0 on success, non-zero on failure: -1 if virtual
  // memory is exhausted or 1 if the object had already been
  // registered.

  static int delete_array_at_exit (void *array);
  // Register an array for deletion at program termination.
  // Returns 0 on success, non-zero on failure: -1 if virtual
  // memory is exhausted or 1 if the object had already been
  // registered.

private:
  static ACE_Object_Manager *instance_;

  ACE_Unbounded_Queue<void *> registered_objects_;
  ACE_Unbounded_Queue<void *> registered_arrays_;

  ACE_Object_Manager ();
  ~ACE_Object_Manager ();

  int delete_at_exit_ (void *);
  // Register an object for deletion at program termination.
  // See description of static version above for return values.

  int delete_array_at_exit_ (void *);
  // Register an array for deletion at program termination.
  // See description of static version above for return values.

  friend class ACE_Object_Manager_Destroyer;
};

#if defined (__ACE_INLINE__)
#include "ace/Object_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_OBJECT_MANAGER_H */
