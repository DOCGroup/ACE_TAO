// $Id$

/**
 * @file Object_Set_T.h
 *
 * Helper template classes for maintaining and managing object
 * reference sets.
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_OBJECT_SET_T_H
#define CIAO_OBJECT_SET_T_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * @class Object_Set
   *
   * @brief A helper template class for maintaining and managing a set
   * of object reference.
   *
   * This class provides a naive implementation of an object reference
   * set.  It is naive because it simply keeps object references in an
   * objref_var array without checking for duplication.  More
   * importantly, it takes O(n) to to look for an object reference and
   * return it.  What we really need is a thread-safe hash_set<> like
   * class here.  Since this is mostly used only during setup/assemble
   * time for a CIAO application, i.e., not on critical path, this
   * class serves as a place holder for later, more efficient
   * implementation.
   */
  template <class T, class T_var>
  class Object_Set
  {
  public:
    /**
     * Default contructor that also allows initializing the initial
     * capacity of the set and the increment step.
     */
    Object_Set (CORBA::ULong init_capacity = 10,
                CORBA::ULong step = 10);

    /// Default destructor.
    ~Object_Set (void);

    /// Adding a new object reference to the set.  Return -1 if error
    /// occurred.
    CORBA::Long add (T *objref);

    /// Removing an object from the set. Return -1 if error occurred.
    CORBA::Long remove (T *objref);

    /// Access the underlying T_var array directly.  This is added to
    /// get around a bug in TAO's sequence of object C++ mapping.
    T_var &at (CORBA::ULong index);

    /**
     * Get a copy of the object reference set into the incoming array
     * @c buffer with capacity of @c size.  @c buffer is usually
     * allocated using a sequence's @c allocbuf method.  Notice that
     * caller is responsible to release the object references in
     * buffer.
     *
     * @retval actual number of objrefs copied into @c buffer.
     */
    CORBA::ULong copy (CORBA::ULong size,
                       T **buffer);

    /**
     * Check if an object is in the set.  Return 0 if false.
     */
    int object_in_set (T *objref);

    /// Query the current size the set contains.
    CORBA::ULong size (void) const;

    /// Query the current capacity the set.
    CORBA::ULong capacity (void) const;

    /**
     * Release all object references.
     */
    void release (void);

  protected:
    /// Grow the internal array holding the object references.
    void grow (void);

    /// Pointer to the dynamically allocated buffer that holds the
    /// object references.
    T_var *buffer_;

    /// Maximun capacity of this->buffer_;
    CORBA::ULong capacity_;

    /// Current size in the set.
    CORBA::ULong size_;

    /// How many more slots to add each time we expand the capacity of
    /// this set.
    CORBA::ULong step_;
  };

}

#if defined (__ACE_INLINE__)
#include "Object_Set_T.inl"
#endif /* !defined INLINE */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Object_Set_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_Set_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* CIAO_OBJECT_SET_T_H */
