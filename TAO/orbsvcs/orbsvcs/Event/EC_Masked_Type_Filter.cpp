// $Id$

#include "orbsvcs/Event/EC_Masked_Type_Filter.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Masked_Type_Filter::
    TAO_EC_Masked_Type_Filter (CORBA::ULong source_mask,
                               CORBA::ULong type_mask,
                               CORBA::ULong source_value,
                               CORBA::ULong type_value)
  :  source_mask_ (source_mask),
     type_mask_ (type_mask),
     source_value_ (source_value),
     type_value_ (type_value)
{
}

TAO_EC_Masked_Type_Filter::~TAO_EC_Masked_Type_Filter (void)
{
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Masked_Type_Filter::begin (void) const
{
  return 0;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Masked_Type_Filter::end (void) const
{
  return 0;
}

int
TAO_EC_Masked_Type_Filter::size (void) const
{
  return 0;
}

int
TAO_EC_Masked_Type_Filter::filter (const RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info)
{
  if (event.length () != 1)
    return 0;

  if ((event[0].header.type & this->type_mask_) != this->type_value_
      || (event[0].header.source & this->source_mask_) != this->source_value_)
    return 0;

  if (this->parent () != 0)
    {
      this->parent ()->push (event, qos_info);
    }
  return 1;
}

int
TAO_EC_Masked_Type_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                          TAO_EC_QOS_Info& qos_info)
{
  if (event.length () != 1)
    return 0;

  if ((event[0].header.type & this->type_mask_) != this->type_value_
      || (event[0].header.source & this->source_mask_) != this->source_value_)
    return 0;

  if (this->parent () != 0)
    {
      this->parent ()->push_nocopy (event, qos_info);
    }
  return 1;
}

void
TAO_EC_Masked_Type_Filter::push (const RtecEventComm::EventSet &,
                                 TAO_EC_QOS_Info &)
{
}

void
TAO_EC_Masked_Type_Filter::push_nocopy (RtecEventComm::EventSet &,
                                        TAO_EC_QOS_Info &)
{
}

void
TAO_EC_Masked_Type_Filter::clear (void)
{
}

CORBA::ULong
TAO_EC_Masked_Type_Filter::max_event_size (void) const
{
  return 1;
}

int
TAO_EC_Masked_Type_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  if ((header.type & this->type_mask_) == this->type_value_
      && (header.source & this->source_mask_) == this->source_value_)
    return 1;

  return 0;
}

int
TAO_EC_Masked_Type_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info &)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
