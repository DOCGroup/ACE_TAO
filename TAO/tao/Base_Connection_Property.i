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
  : endpoint_ (endpoint),
    endpoint_flag_ (flag)
{
}


ACE_INLINE TAO_Endpoint *
TAO_Base_Connection_Property::endpoint (void)
{
  return this->endpoint_;
}


ACE_INLINE
TAO_Base_Connection_Property::TAO_Base_Connection_Property (
    const TAO_Base_Connection_Property &rhs)
{
  this->endpoint_ = rhs.endpoint_->duplicate ();
  this->endpoint_flag_ = 1;
}

ACE_INLINE void
TAO_Base_Connection_Property::operator= (
    const TAO_Base_Connection_Property &rhs)
{
  if (this == &rhs)
    return;

  this->endpoint_ = rhs.endpoint_->duplicate ();
  this->endpoint_flag_ = 1;
}

ACE_INLINE int
TAO_Base_Connection_Property::operator== (
    const TAO_Base_Connection_Property &rhs) const
{
  return this->endpoint_->is_equivalent (rhs.endpoint_);
}

ACE_INLINE int
TAO_Base_Connection_Property::operator!= (
    const TAO_Base_Connection_Property &rhs) const
{
  return !(this->endpoint_->is_equivalent (rhs.endpoint_));
}

ACE_INLINE u_long
TAO_Base_Connection_Property::hash (void) const
{
  return this->endpoint_->hash ();
}
