/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Buffer_Allocator_T.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen (jwillemsen@remedy.nl)
 */
//=============================================================================

#ifndef TAO_BUFFER_ALLOCATOR_T_H
#define TAO_BUFFER_ALLOCATOR_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Allocator.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class TAO_Buffer_Allocator
   *
   * @brief Generic buffer allocator for TAO
   *
   * handler_type is the type of object to allocator, alloc_type is the type
   * of allocator to be used to allocate the object.
   */
  template <typename handler_type, typename alloc_type>
  class TAO_Buffer_Allocator : public TAO_Allocator<handler_type>
  {
    public:
      typedef handler_type HANDLER_TYPE;
      typedef alloc_type ALLOC_TYPE;
      TAO_Buffer_Allocator (alloc_type *allocator);

      virtual handler_type *allocate (void);
      virtual void release (handler_type *ptr);
    private:
      alloc_type* allocator_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Buffer_Allocator_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Buffer_Allocator_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_BUFFER_ALLOCATOR_T_H */
