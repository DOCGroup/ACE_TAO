// $Id$

#include "orbsvcs/Event/EC_Type_Filter.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Type_Filter::TAO_EC_Type_Filter (const RtecEventComm::EventHeader& header)
  : header_ (header)
{
}

int
TAO_EC_Type_Filter::filter (const RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info)
{
  if (event.length () != 1)
    return this->filter_set (event, qos_info);

  if (this->can_match (event[0].header))
    {
      this->push (event, qos_info);
      return 1;
    }
  return 0;
}

int
TAO_EC_Type_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info)
{
  if (event.length () != 1)
    return this->filter_set (event, qos_info);

  if (this->can_match (event[0].header))
    {
      this->push_nocopy (event, qos_info);
      return 1;
    }
  return 0;
}

void
TAO_EC_Type_Filter::push (const RtecEventComm::EventSet& event,
                          TAO_EC_QOS_Info& qos_info)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info);
}

void
TAO_EC_Type_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info);
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
    {
      if (this->header_.type == 0 || header.type == 0)
        return 1;
      else
        return this->header_.type == header.type;
    }

  if (this->header_.type == 0)
    {
      if (header.source == 0)
        return 1;
      else
        return this->header_.source == header.source;
    }

  if (header.source == 0)
    {
      if (header.type != 0)
        return this->header_.type == header.type;
      return 1;
    }

  if (header.type == 0)
    {
      return this->header_.source == header.source;
    }

  return (this->header_.type == header.type
          && this->header_.source == header.source);
}

int
TAO_EC_Type_Filter::add_dependencies (
      const RtecEventComm::EventHeader& header,
      const TAO_EC_QOS_Info &)
{
  return this->can_match (header);
}

int
TAO_EC_Type_Filter::filter_set (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info)
{
  CORBA::ULong maximum = event.length ();
  if (event.maximum () == 0)
    return 0;

  RtecEventComm::EventSet matched (maximum);
  CORBA::ULong next_slot = 0;
  for (CORBA::ULong i = 0; i != maximum; ++i)
    {
      if (!this->can_match (event[i].header))
        continue;
      matched.length (next_slot + 1);
      matched[next_slot] = event[i];
      next_slot++;
    }
  if (matched.length () == 0)
    return 0;

  this->push (matched, qos_info);

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
