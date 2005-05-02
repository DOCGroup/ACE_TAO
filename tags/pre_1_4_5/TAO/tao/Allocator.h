/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Allocator.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen (jwillemsen@remedy.nl)
 */
//=============================================================================

#ifndef TAO_ALLOCATOR_H
#define TAO_ALLOCATOR_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class TAO_Allocator
   *
   * @brief Abstract class for TAO allocators. This will be used for derived
   * allocator templates that are capable of allocating a specific type of
   * objects
   */
  template <typename handler_type>
  class TAO_Allocator
  {
  public:

    virtual ~TAO_Allocator (void) {}

    virtual handler_type *allocate () = 0;
    virtual void release (handler_type *ptr) = 0;
  };
}

#include /**/ "ace/post.h"
#endif /* TAO_ALLOCATOR_H */
