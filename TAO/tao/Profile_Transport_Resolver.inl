// -*- C++ -*-
// $Id$
namespace TAO
{
  ACE_INLINE TAO_Stub *
  Profile_Transport_Resolver::stub (void) const
  {
    return this->stub_;
  }

  ACE_INLINE CORBA::Object_ptr
  Profile_Transport_Resolver::object (void) const
  {
    return this->obj_;
  }

  ACE_INLINE TAO_Profile *
  Profile_Transport_Resolver::profile (void) const
  {
    return this->profile_;
  }

  ACE_INLINE TAO_Transport *
  Profile_Transport_Resolver::transport (void) const
  {
    return this->transport_;
  }
}
