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

template <class TYPE>
class ACE_Managed_Cleanup : public ACE_Cleanup
{
public:
  ACE_Managed_Cleanup (void);
  TYPE &object (void);
private:
  TYPE object_;
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
{
public:
  static int get_object (u_int &id, TYPE *&object);
  // Get the object identified by "id".  If "id" is 0, allocates a new
  // instance, and sets "id" to the new identifier for that instance.
  // Returns 0 on success.  On failure, returns -1 and sets errno to:
  //   ENOENT if the id is non-zero and unknown,
  //   ENOMEM if insufficient virtual memory to allocate a new instance, or
  //   ENOSPC if no more table slots are available:  see the
  //      ACE_MAX_MANAGED_OBJECTS config variable.

  static TYPE *get_object (u_int &id);
  // Get the object identified by "id".  Returns a pointer to the
  // object, or 0 if any error was encountered.  Because no other
  // error indication is provided, it should _only_ be used for
  // accessing preallocated objects.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Managed_Object.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Managed_Object.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MANAGED_OBJECT_H */
