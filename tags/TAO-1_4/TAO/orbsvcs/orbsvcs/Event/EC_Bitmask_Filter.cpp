// $Id$

#include "EC_Bitmask_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Bitmask_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Bitmask_Filter, "$Id$")

TAO_EC_Bitmask_Filter::
    TAO_EC_Bitmask_Filter (CORBA::ULong source_mask,
                           CORBA::ULong type_mask,
                           TAO_EC_Filter* child)
  :  source_mask_ (source_mask),
     type_mask_ (type_mask),
     child_ (child)
{
  this->adopt_child (this->child_);
}

TAO_EC_Bitmask_Filter::~TAO_EC_Bitmask_Filter (void)
{
  delete this->child_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Bitmask_Filter::begin (void) const
{
  return ACE_const_cast(TAO_EC_Filter**,&this->child_);
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Bitmask_Filter::end (void) const
{
  return ACE_const_cast(TAO_EC_Filter**,&this->child_) + 1;
}

int
TAO_EC_Bitmask_Filter::size (void) const
{
  return 1;
}

int
TAO_EC_Bitmask_Filter::filter (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info
                                ACE_ENV_ARG_DECL)
{
  if (event.length () != 1)
    return 0;

  if ((event[0].header.type & this->type_mask_) == 0
      || (event[0].header.source & this->source_mask_) == 0)
    return 0;

  return this->child_->filter (event, qos_info ACE_ENV_ARG_PARAMETER);
}

int
TAO_EC_Bitmask_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info
                                   ACE_ENV_ARG_DECL)
{
  if (event.length () != 1)
    return 0;

  if ((event[0].header.type & this->type_mask_) == 0
      || (event[0].header.source & this->source_mask_) == 0)
    return 0;

  return this->child_->filter_nocopy (event, qos_info ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Bitmask_Filter::push (const RtecEventComm::EventSet &event,
                             TAO_EC_QOS_Info &qos_info
                             ACE_ENV_ARG_DECL)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Bitmask_Filter::push_nocopy (RtecEventComm::EventSet &event,
                                    TAO_EC_QOS_Info &qos_info
                                    ACE_ENV_ARG_DECL)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Bitmask_Filter::clear (void)
{
  this->child_->clear ();
}

CORBA::ULong
TAO_EC_Bitmask_Filter::max_event_size (void) const
{
  return this->child_->max_event_size ();
}

int
TAO_EC_Bitmask_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  if ((header.type & this->type_mask_) == 0
      || (header.source & this->source_mask_) == 0)
    return 0;

  return this->child_->can_match (header);
}

int
TAO_EC_Bitmask_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info &
      ACE_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}
