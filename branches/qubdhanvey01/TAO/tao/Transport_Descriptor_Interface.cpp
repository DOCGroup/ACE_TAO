#include "tao/Transport_Descriptor_Interface.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Descriptor_Interface.inl"
#endif /* __ACE_INLINE__ */

#include "tao/Endpoint.h"

ACE_RCSID (tao,
           Transport_Descriptor_Interface,
           "$Id$")


TAO_Transport_Descriptor_Interface::
    ~TAO_Transport_Descriptor_Interface (void)
{
  if (this->endpoint_from_heap_)
    delete this->endpoint_;
}
