// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Fault_Tolerance_Service::TAO_Fault_Tolerance_Service (void)
  : ft_service_callback_ (0)
{
}

ACE_INLINE TAO_Service_Callbacks *
TAO_Fault_Tolerance_Service::service_callback (void)
{
  return this->ft_service_callback_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
