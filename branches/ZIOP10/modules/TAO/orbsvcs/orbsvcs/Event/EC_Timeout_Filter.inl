// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE const TAO_EC_QOS_Info&
TAO_EC_Timeout_Filter::qos_info (void) const
{
  return this->qos_info_;
}

ACE_INLINE RtecEventComm::EventType
TAO_EC_Timeout_Filter::type (void) const
{
  return this->type_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
