// $Id$

#include "EC_Type_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Type_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Type_Filter, "$Id$")

TAO_EC_Type_Filter::TAO_EC_Type_Filter (const RtecEventComm::EventHeader& header)
  : header_ (header)
{
}

int
TAO_EC_Type_Filter::filter (const RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& ACE_TRY_ENV)
{
  // @@ It this the right way to do this?
  if (event.length () != 1)
    return 0;

  if (this->can_match (event[0].header)) // TAO_EC_Filter::matches (this->header
    {
      this->push (event, qos_info, ACE_TRY_ENV);
      return 1;
    }
  return 0;
}

int
TAO_EC_Type_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  // @@ It this the right way to do this?
  if (event.length () != 1)
    return 0;

  if (this->can_match (event[0].header)) // TAO_EC_Filter::matches (this->header
    {
      this->push_nocopy (event, qos_info, ACE_TRY_ENV);
      return 1;
    }
  return 0;
}

void
TAO_EC_Type_Filter::push (const RtecEventComm::EventSet& event,
                          TAO_EC_QOS_Info& qos_info,
                          CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Type_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Type_Filter::clear (void)
{
}

CORBA::ULong
TAO_EC_Type_Filter::max_event_size (void) const
{
  return 1;
}

int
TAO_EC_Type_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  if (this->header_.source == 0)
    if (this->header_.type == 0)
      return 1;
    else
      return this->header_.type == header.type;
  else if (this->header_.type == 0)
    return this->header_.source == header.source;

  return (this->header_.type == header.type
          && this->header_.source == header.source);
}

int
TAO_EC_Type_Filter::add_dependencies (
      const RtecEventComm::EventHeader& header,
      const TAO_EC_QOS_Info &,
      CORBA::Environment &)
{
  return this->can_match (header);
}

