//$Id$

#include "tao/Connection_Descriptor_Interface.h"



#if !defined (__ACE_INLINE__)
# include "tao/Connection_Descriptor_Interface.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Connection_Descriptor_Interface, "$Id$")


TAO_Connection_Descriptor_Interface::
    ~TAO_Connection_Descriptor_Interface (void)
{
  if (this->endpoint_from_heap_)
    delete this->endpoint_;
}
