// $Id$

#include "EC_Disjunction_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Disjunction_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Disjunction_Filter, "$Id$")

TAO_EC_Disjunction_Filter::
    TAO_EC_Disjunction_Filter (TAO_EC_Filter* children[],
                               size_t n)
  :  children_ (children),
     n_ (n)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      this->adopt_child (*i);
    }
}

TAO_EC_Disjunction_Filter::~TAO_EC_Disjunction_Filter (void)
{
  TAO_EC_Filter** end = this->children_ + this->n_;
  for (TAO_EC_Filter** i = this->children_;
       i != end;
       ++i)
    {
      delete *i;
      *i = 0;
    }
  delete[] this->children_;
  this->children_ = 0;
  this->n_ = 0;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Disjunction_Filter::begin (void) const
{
  return this->children_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Disjunction_Filter::end (void) const
{
  return this->children_ + this->n_;
}

ACE_INLINE int
TAO_EC_Disjunction_Filter::size (void) const
{
  return this->n_;
}

int
TAO_EC_Disjunction_Filter::filter (const RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      int n = (*i)->filter (event, qos_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      if (n != 0)
        return n;
    }
  return 0;
}

int
TAO_EC_Disjunction_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      int n = (*i)->filter (event, qos_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      if (n != 0)
        return n;
    }
  return 0;
}

void
TAO_EC_Disjunction_Filter::push (const RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Disjunction_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  if (this->parent () != 0)
    this->parent ()->push_nocopy (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Disjunction_Filter::clear (void)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->clear ();
    }
}

CORBA::ULong
TAO_EC_Disjunction_Filter::max_event_size (void) const
{
  CORBA::ULong n = 0;
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      CORBA::ULong m = (*i)->max_event_size ();
      if (n < m)
        n = m;
    }
  return n;
}

int
TAO_EC_Disjunction_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      if ((*i)->can_match (header) != 0)
        return 1;
    }
  return 0;
}

int
TAO_EC_Disjunction_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info &,
      CORBA::Environment &)
{
  return 0;
}
