// $Id$

#include "orbsvcs/Event/EC_Filter.h"
#include "orbsvcs/Event/EC_QOS_Info.h"

#include "tao/ORB_Constants.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Filter.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Filter::~TAO_EC_Filter (void)
{
}

void
TAO_EC_Filter::adopt_child (TAO_EC_Filter* child)
{
  child->parent_ = this;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Filter::begin (void) const
{
  return 0;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Filter::end (void) const
{
  return 0;
}

int
TAO_EC_Filter::size (void) const
{
  return 0;
}

void
TAO_EC_Filter::get_qos_info (TAO_EC_QOS_Info& qos_info)
{
  ACE_UNUSED_ARG (qos_info);
  throw CORBA::NO_IMPLEMENT (TAO::VMCID, CORBA::COMPLETED_NO);
}

// ****************************************************************

int
TAO_EC_Null_Filter::filter (const RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info)
{
  // This is a leaf, no need to query any children, and we accept all
  // events, so push it.
  this->push (event, qos_info);
  return 1;
}

int
TAO_EC_Null_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info)
{
  // This is a leaf, no need to query any children, and we accept all
  // events, so push it.
  this->push_nocopy (event, qos_info);
  return 1;
}

void
TAO_EC_Null_Filter::push (const RtecEventComm::EventSet& event,
                          TAO_EC_QOS_Info& qos_info)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info);
}

void
TAO_EC_Null_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info);
}

void
TAO_EC_Null_Filter::clear (void)
{
  // do nothing
}

CORBA::ULong
TAO_EC_Null_Filter::max_event_size (void) const
{
  // @@ Is there a better way to express this?
  return 0;
}

int
TAO_EC_Null_Filter::can_match (const RtecEventComm::EventHeader&) const
{
  // @@ This method should be correctly so we can implement null
  // filtering at the consumers but real filtering on the suppliers.
  return 1;
}

int
TAO_EC_Null_Filter::add_dependencies (
      const RtecEventComm::EventHeader &,
      const TAO_EC_QOS_Info &)

{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
