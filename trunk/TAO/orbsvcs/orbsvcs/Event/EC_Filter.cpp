// $Id$

#include "EC_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Filter, "$Id$")

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
TAO_EC_Filter::get_qos_info (TAO_EC_QOS_Info&,
                             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_NO));
}

// ****************************************************************

int
TAO_EC_Null_Filter::filter (const RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& ACE_TRY_ENV)
{
  // This is a leaf, no need to query any children, and we accept all
  // events, so push it.
  this->push (event, qos_info, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return 1;
}

int
TAO_EC_Null_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  // This is a leaf, no need to query any children, and we accept all
  // events, so push it.
  this->push_nocopy (event, qos_info, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  return 1;
}

void
TAO_EC_Null_Filter::push (const RtecEventComm::EventSet& event,
                          TAO_EC_QOS_Info& qos_info,
                          CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Null_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info, ACE_TRY_ENV);
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
      const TAO_EC_QOS_Info &,
      CORBA::Environment &)

{
  return 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
