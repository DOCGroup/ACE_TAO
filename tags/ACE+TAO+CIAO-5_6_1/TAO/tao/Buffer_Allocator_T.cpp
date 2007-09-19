// $Id$

#ifndef TAO_BUFFER_ALLOCATOR_T_CPP
#define TAO_BUFFER_ALLOCATOR_T_CPP

#include "tao/Buffer_Allocator_T.h"
#include "ace/Global_Macros.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template <typename handler_type, typename alloc_type>
  TAO_Buffer_Allocator<handler_type, alloc_type>::TAO_Buffer_Allocator (alloc_type *allocator) :
    allocator_(allocator)
  {
  }

  template <typename handler_type, typename alloc_type>
  handler_type*
  TAO_Buffer_Allocator<handler_type, alloc_type>::allocate (void)
  {
    handler_type* myvalue = 0;
    ACE_NEW_MALLOC_RETURN (myvalue,
                           static_cast <handler_type*>(
                               allocator_->malloc (sizeof (handler_type))),
                           handler_type (), 0);

    return myvalue;
  }

  template <typename handler_type, typename alloc_type>
  void
  TAO_Buffer_Allocator<handler_type, alloc_type>::release (handler_type *ptr)
  {
      ACE_DES_FREE (ptr,
                    allocator_->free,
                    handler_type);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_BUFFER_ALLOCATOR_T_CPP */
