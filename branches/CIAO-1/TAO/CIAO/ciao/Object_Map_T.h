// $Id$

/**
 * @file Object_Map_T.h
 *
 * A CIAO implementation of
 *
 * @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

#ifndef CIAO_OBJECT_MAP_T_H
#define CIAO_OBJECT_MAP_T_H
#include "ace/pre.h"

#include "ace/Active_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  /**
   * @class CIAO::Object_Map
   *
   * @brief An templatized Active Object Map for managing a collection
   *        of object references indexed by keys.
   */
  template <class TYPE>
  class Object_Map
  {
  public:
    /// Initialize an Object_Map with @c ACE_DEFAULT_MAP_SIZE
    Object_Map (ACE_Allocator *alloc = 0);

    /// Initialize an Object_Map with @c size entry
    Object_Map (size_t size,
                ACE_Allocator *alloc = 0);

    /// Close down the Object_Map and release dynamically allocated
    /// resources.
    ~Object_Map (void);

    /// Initialize the Object_Map with size @c length.
    int open (size_t length = ACE_DEFAULT_MAP_SIZE,
              ACE_Allocator *alloc = 0);

    /// Close down the Object_Map and release dynamically allocated
    /// resources.
    int close (void);



  protected:

  };
}

#if defined (__ACE_INLINE__)
#include "Object_Map_T.inl"
#endif /* !defined INLINE */


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Object_Map_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Object_Map_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* CIAO_OBJECT_MAP_T_H */
