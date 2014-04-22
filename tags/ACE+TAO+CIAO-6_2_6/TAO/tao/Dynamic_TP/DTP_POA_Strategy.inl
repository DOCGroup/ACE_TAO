// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
/// Constructor.
TAO_DTP_POA_Strategy::TAO_DTP_POA_Strategy (ACE_CString tp_config_name,
                                            bool ss)
  : serialize_servants_ (ss),
    config_initialized_ (false)
{
  this->dynamic_tp_config_name_ = tp_config_name;
}

ACE_INLINE
TAO_DTP_POA_Strategy::TAO_DTP_POA_Strategy (TAO_DTP_Definition * tp_config,
                                            bool ss)
  : serialize_servants_ (ss),
    config_initialized_ (true)
{
  this->set_dtp_config (*tp_config);
}

ACE_INLINE
void
TAO_DTP_POA_Strategy::set_servant_serialization (bool serialize_servants)
{
  // Simple Mutator.
  this->serialize_servants_ = serialize_servants;
}


TAO_END_VERSIONED_NAMESPACE_DECL
