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
  // that capability, on VxWorks, for example, the program must be unloaded
  // for this to work.
{
public:
  static ACE_Object_Manager *instance ();
  // Accessor to singleton instance.

  void delete_at_exit (void *);
  // Register an object for deletion at program termination.

  void delete_array_at_exit (void *);
  // Register an array for deletion at program termination.

private:
  static ACE_Object_Manager *instance_;

  ACE_Unbounded_Queue<void *> registered_objects_;
  ACE_Unbounded_Queue<void *> registered_arrays_;

  ACE_Object_Manager ();
  ~ACE_Object_Manager ();

  friend class ACE_Object_Manager_Destroyer;
};

#endif /* ACE_OBJECT_MANAGER_H */
