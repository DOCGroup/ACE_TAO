// $Id$

#include "EC_Conjunction_Filter.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Conjunction_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Conjunction_Filter, "$Id$")

const int bits_per_word = sizeof(TAO_EC_Conjunction_Filter::Word) * CHAR_BIT;

TAO_EC_Conjunction_Filter::
    TAO_EC_Conjunction_Filter (TAO_EC_Filter* children[],
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

  this->nwords_ = this->n_ / bits_per_word + 1;
  ACE_NEW (this->bitvec_, Word[this->nwords_]);

  this->clear ();
}

TAO_EC_Conjunction_Filter::~TAO_EC_Conjunction_Filter (void)
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

  delete[] this->bitvec_;
  this->bitvec_ = 0;
}

int
TAO_EC_Conjunction_Filter::all_received (void) const
{
  Word* i = this->bitvec_;
  for (;
       i != this->bitvec_ + this->nwords_;
       ++i)
    {
      if (*i != ACE_static_cast(Word,~0))
        return 0;
    }
  return 0;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Conjunction_Filter::begin (void) const
{
  return this->children_;
}

TAO_EC_Filter::ChildrenIterator
TAO_EC_Conjunction_Filter::end (void) const
{
  return this->children_ + this->n_;
}

int
TAO_EC_Conjunction_Filter::size (void) const
{
  return this->n_;
}

int
TAO_EC_Conjunction_Filter::filter (const RtecEventComm::EventSet& event,
                                   TAO_EC_QOS_Info& qos_info,
                                   CORBA::Environment& ACE_TRY_ENV)
{
  ChildrenIterator end = this->end ();
  for (this->current_child_ = this->begin ();
       this->current_child_ != end;
       ++this->current_child_)
    {
      int n = (*this->current_child_)->filter (event, qos_info, ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);
      if (n != 0)
        return n;
    }
  return 0;
}

int
TAO_EC_Conjunction_Filter::filter_nocopy (RtecEventComm::EventSet& event,
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
TAO_EC_Conjunction_Filter::push (const RtecEventComm::EventSet& event,
                                 TAO_EC_QOS_Info& qos_info,
                                 CORBA::Environment& ACE_TRY_ENV)
{
  CORBA::Long pos = this->current_child_ - this->begin ();
  int w = pos / bits_per_word;
  int b = pos % bits_per_word;
  if (ACE_BIT_ENABLED (this->bitvec_[w], 1<<b))
    return;
  ACE_SET_BITS (this->bitvec_[w], 1<<b);
  CORBA::ULong n = event.length ();
  CORBA::ULong l = this->event_.length ();
  this->event_.length (l + n);
  for (CORBA::ULong i = 0; i != n; ++i)
    {
      this->event_[l + i] = event[i];
    }
  if (this->all_received () && this->parent () != 0)
    this->parent ()->push_nocopy (this->event_, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Conjunction_Filter::push_nocopy (RtecEventComm::EventSet& event,
                                        TAO_EC_QOS_Info& qos_info,
                                        CORBA::Environment& ACE_TRY_ENV)
{
  this->push (event, qos_info, ACE_TRY_ENV);
}

void
TAO_EC_Conjunction_Filter::clear (void)
{
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      (*i)->clear ();
    }
  Word* j = this->bitvec_;
  for (;
       j != this->bitvec_ + this->nwords_ - 1;
       ++j)
    {
      *j = 0;
    }
  int b = this->n_ % bits_per_word;
  Word last = ~(~0 << b);
  *j = last;

  this->event_.length (0);
}

CORBA::ULong
TAO_EC_Conjunction_Filter::max_event_size (void) const
{
  CORBA::ULong n = 0;
  ChildrenIterator end = this->end ();
  for (ChildrenIterator i = this->begin ();
       i != end;
       ++i)
    {
      n += (*i)->max_event_size ();
    }
  return n;
}

int
TAO_EC_Conjunction_Filter::can_match (
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
TAO_EC_Conjunction_Filter::add_dependencies (
      const RtecEventComm::EventHeader&,
      const TAO_EC_QOS_Info&,
      CORBA::Environment &)
{
  return 0;
}
