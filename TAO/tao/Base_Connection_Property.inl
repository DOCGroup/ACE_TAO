/* -*- C++ -*- */
// $Id$

ACE_INLINE
TAO_Base_Connection_Property::TAO_Base_Connection_Property (void)
{

}

ACE_INLINE
TAO_Base_Connection_Property::
    TAO_Base_Connection_Property (TAO_Endpoint *endpoint,
                                  CORBA::Boolean flag)
  : TAO_Connection_Descriptor_Interface (endpoint,
                                         flag)
{
}

ACE_INLINE
TAO_Base_Connection_Property::TAO_Base_Connection_Property (
    const TAO_Base_Connection_Property &rhs)
{
  this->endpoint_ = rhs.endpoint_->duplicate ();
  this->endpoint_from_heap_ = 1;
}
