// $Id$

#include "EC_Negation_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Negation_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Negation_Filter, "$Id$")

TAO_EC_Negation_Filter::
    TAO_EC_Negation_Filter (TAO_EC_Filter* child)
  :  child_ (child)
{
  this->adopt_child (this->child_);
}

TAO_EC_Negation_Filter::~TAO_EC_Negation_Filter (void)
{
  delete this->child_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Negation_Filter::begin (void) const
{
  return &this->child_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Negation_Filter::end (void) const
{
  return &this->child_ + 1;
}

ACE_INLINE int
TAO_EC_Negation_Filter::size (void) const
{
  return 1;
}

int
TAO_EC_Negation_Filter::filter (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info,
                                CORBA::Environment& ACE_TRY_ENV)
{
  int n =
    this->child_->filter (event, qos_info, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (this->parent () != 0 && n == 0)
    {
      this->parent ()->push (event, qos_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  return 1;
}

int
TAO_EC_Negation_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  int n =
    this->child_->filter_nocopy (event, qos_info, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  if (this->parent () != 0 && n == 0)
    {
      this->parent ()->push_nocopy (event, qos_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
    }
  return 1;
}

void
TAO_EC_Negation_Filter::push (const RtecEventComm::EventSet&,
                                 TAO_EC_QOS_Info&,
                                 CORBA::Environment&)
{
}

void
TAO_EC_Negation_Filter::push_nocopy (RtecEventComm::EventSet&,
                                        TAO_EC_QOS_Info&,
                                        CORBA::Environment&)
{
}

void
TAO_EC_Negation_Filter::clear (void)
{
  this->child_->clear ();
}

CORBA::ULong
TAO_EC_Negation_Filter::max_event_size (void) const
{
  return this->child_->max_event_size ();
}

int
TAO_EC_Negation_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  return !this->child_->can_match (header);
}

int
TAO_EC_Negation_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info &,
      CORBA::Environment &)
{
  return 0;
}
