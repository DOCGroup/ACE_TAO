/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Managed_Object.h
//
// = AUTHORS
//    David L. Levine
//
// ============================================================================

#if !defined (ACE_MANAGED_OBJECT_H)
#define ACE_MANAGED_OBJECT_H

#include "ace/OS.h"

template <class TYPE>
class ACE_Cleanup_Adapter : public ACE_Cleanup
  // = TITLE
  //     Adapter for ACE_Cleanup objects that allows them to be readily
  //     managed by the ACE_Object_Manager.
  //
  // = DESCRIPTION
  //     This template class wraps adapts an object of any type to be
  //     an ACE_Cleanup object.  The object can then be destroyed
  //     type-safely by the ACE_Object_Manager.
{
public:
  ACE_Cleanup_Adapter (void);
  // Default constructor.

  TYPE &object (void);
  // Accessor for contained object.
private:
  TYPE object_;
  // Contained object.
};

template <class TYPE>
class ACE_Managed_Object
  // = TITLE
  //     Wrapper for interface to allocate an object managed by the
  //     ACE_Object_Manager.
  //
  // = DESCRIPTION
  //     This template class wraps an interface that is used to
  //     allocate and access an object that is managed by the
  //     ACE_Object_Manager.  Because static template member functions
  //     are not supported by most compilers, it is a separate
  //     (template) class.
  //
  //     This interface is typically used to replace a static object
  //     with one that is dynamically allocated.  It helps to avoid
  //     problems with order of static object construction/destruction.
  //     Managed objects won't be allocated until needed, but should
  //     be allocated when first needed.  And they are destroyed in
  //     the reverse order of construction.
  //
  //     To use the get_object () interface, the caller must provide
  //     an identifier and an object pointer.  The identifer (usually)
  //     has a value of 0 the first time get_object () is called, in
  //     which case a new instance of the object is allocated on the
  //     heap.  (See the description of get_object () for return
  //     values.)
  //
  //     Subsequent calls to get_object (), with that identifier, will
  //     return the pointer to the identified object.  The caller is
  //     responsible for ensuring type safety by not casting the pointer
  //     that it holds in calls to get_object ().
  //
  //     get_preallocated_object () accesses a "preallocated" object,
  //     i.e., one that is identified by a value in the
  //     ACE_Object_Manager::  Preallocated_Object enum.  These
  //     objects are used internally by the ACE library.
  //
  //     Hooks are provided for the application to preallocate objects
  //     via the same mechanism.
  //     ACE_APPLICATION_PREALLOCATED_OBJECT_DECLARATIONS can be used
  //     to define enum values;
  //     ACE_APPLICATION_PREALLOCATED_OBJECT_DEFINITIONS can be used
  //     to define the corresponding objects.  The format of the ACE
  //     internal library definitions should be followed.  And similarly,
  //     ACE_APPLICATION_PREALLOCATED_ARRAY_DECLARATIONS and
  //     ACE_APPLICATION_PREALLOCATED_ARRAY_DEFINITIONS can be used to
  //     preallocate arrays.
  //
  //     By default, preallocation uses dynamic allocation.  The
  //     preallocated objects and arrays are allocated off the heap in
  //     the ACE_Object_Manager constructor.  The statically place the
  //     preallocated objects in program global data instead of on the
  //     heap, #define ACE_HAS_STATIC_PREALLOCATION prior to building
  //     the ACE library.
{
public:
  static int get_object (int &id, TYPE *&object);
  // Get the object identified by "id".  If "id" is 0, allocates a new
  // instance, and sets "id" to the new identifier for that instance.
  // Returns 0 on success.  On failure, returns -1 and sets errno to:
  //   ENOENT if the id is non-zero and unknown,
  //   ENOMEM if insufficient virtual memory to allocate a new instance, or
  //   ENOSPC if no more table slots are available:  see the
  //      ACE_MAX_MANAGED_OBJECTS config variable.

  static
  TYPE *get_preallocated_object (ACE_Object_Manager::Preallocated_Object id);
  // Get the preallocated object identified by "id".  Returns a
  // pointer to the object.  Beware: no error indication is provided,
  // because it can _only_ be used for accessing preallocated objects.

  static
  TYPE *get_preallocated_array (ACE_Object_Manager::Preallocated_Array id);
  // Get the preallocated array identified by "id".  Returns a
  // pointer to the array.  Beware: no error indication is provided,
  // because it can _only_ be used for accessing preallocated arrays.

private:
  // Disallow instantiation of this class.
  ACE_UNIMPLEMENTED_FUNC (ACE_Managed_Object ())
  ACE_UNIMPLEMENTED_FUNC (ACE_Managed_Object (const ACE_Managed_Object &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Managed_Object &operator=
                            (const ACE_Managed_Object &))

  friend class this_prevents_compiler_warning_about_only_private_constructors;
};

#if defined (__ACE_INLINE__)
#include "ace/Managed_Object.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Managed_Object.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Managed_Object.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MANAGED_OBJECT_H */
