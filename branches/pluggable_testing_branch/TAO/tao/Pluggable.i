// -*- C++ -*-
// $Id$

ACE_INLINE
TAO_Profile::TAO_Profile (CORBA::ULong tag)
  :  tag_ (tag),
     forward_to_ (0),
     refcount_ (1)
{
}

ACE_INLINE void
TAO_Profile::forward_to (TAO_MProfile *mprofiles)
{
  this->forward_to_ = mprofiles;
}

ACE_INLINE TAO_MProfile *
TAO_Profile::forward_to (void)
{
  return this->forward_to_;
}

ACE_INLINE TAO_MProfile *
TAO_Profile::forward_to_i (void)
{
  return this->forward_to_;
}
