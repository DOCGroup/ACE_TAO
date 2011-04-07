// $Id$

#include "orbsvcs/Event/EC_And_Filter.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_And_Filter::TAO_EC_And_Filter (TAO_EC_Filter* children[],
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

TAO_EC_And_Filter::~TAO_EC_And_Filter (void)
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
TAO_EC_And_Filter::begin (void) const
{
  return this->children_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_And_Filter::end (void) const
{
  return this->children_ + this->n_;
}

int
TAO_EC_And_Filter::size (void) const
{
  return static_cast<CORBA::ULong> (this->n_);
}

int
TAO_EC_And_Filter::filter (const RtecEventComm::EventSet& event,
                           TAO_EC_QOS_Info& qos_info)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin (); i != end; ++i)
    {
      int n = (*i)->filter (event, qos_info);
      if (n == 0)
        return 0;
    }

  // All children accepted the event, push up...
  if (this->parent () != 0)
    {
      this->parent ()->push (event, qos_info);
    }

  return 1;
}

int
TAO_EC_And_Filter::filter_nocopy (RtecEventComm::EventSet& event,
                                  TAO_EC_QOS_Info& qos_info)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin (); i != end; ++i)
    {
      int n = (*i)->filter_nocopy (event, qos_info);
      if (n == 0)
        return 0;
    }

  // All children accepted the event, push up...
  if (this->parent () != 0)
    {
      this->parent ()->push (event, qos_info);
    }

  return 1;
}

void
TAO_EC_And_Filter::push (const RtecEventComm::EventSet&,
                         TAO_EC_QOS_Info&)
{
}

void
TAO_EC_And_Filter::push_nocopy (RtecEventComm::EventSet&,
                                TAO_EC_QOS_Info&)
{
}

void
TAO_EC_And_Filter::clear (void)
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
TAO_EC_And_Filter::max_event_size (void) const
{
  CORBA::ULong n = 0;
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      CORBA::ULong c = (*i)->max_event_size ();
      if (n < c)
        n = c;
    }
  return n;
}

int
TAO_EC_And_Filter::can_match (
      const RtecEventComm::EventHeader& header) const
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      if ((*i)->can_match (header) == 0)
        return 0;
    }
  return 1;
}

int
TAO_EC_And_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info&)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
