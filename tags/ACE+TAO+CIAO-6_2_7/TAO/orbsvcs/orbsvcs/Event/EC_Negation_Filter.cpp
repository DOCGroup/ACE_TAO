// $Id$

#include "orbsvcs/Event/EC_Negation_Filter.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Negation_Filter::TAO_EC_Negation_Filter (TAO_EC_Filter* child)
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
  return const_cast<TAO_EC_Filter**> (&this->child_);
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Negation_Filter::end (void) const
{
  return const_cast<TAO_EC_Filter**> (&this->child_) + 1;
}

int
TAO_EC_Negation_Filter::size (void) const
{
  return 1;
}

int
TAO_EC_Negation_Filter::filter (const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info& qos_info)
{
  int n =
    this->child_->filter (event, qos_info);
  if (this->parent () != 0 && n == 0)
    {
      this->parent ()->push (event, qos_info);
      return 1;
    }
  return 0;
}

int
TAO_EC_Negation_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info)
{
  int n =
    this->child_->filter_nocopy (event, qos_info);
  if (this->parent () != 0 && n == 0)
    {
      this->parent ()->push_nocopy (event, qos_info);
      return 1;
    }
  return 0;
}

void
TAO_EC_Negation_Filter::push (const RtecEventComm::EventSet&,
                                 TAO_EC_QOS_Info&)
{
}

void
TAO_EC_Negation_Filter::push_nocopy (RtecEventComm::EventSet&,
                                        TAO_EC_QOS_Info&)
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
      const TAO_EC_QOS_Info &)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
