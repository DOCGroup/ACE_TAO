// -*- C++ -*-
//$Id$

ACE_INLINE void 
TAO_SHMIOP_Client_Transport::use_lite (CORBA::Boolean flag)
{
  this->lite_flag_ = flag;
}
