// $Id$

#include "EventTypeSeq.h"

#if ! defined (__ACE_INLINE__)
#include "EventTypeSeq.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_EventTypeSeq, "$Id$")

TAO_NS_EventTypeSeq::TAO_NS_EventTypeSeq (void)
{
}

TAO_NS_EventTypeSeq::TAO_NS_EventTypeSeq (const CosNotification::EventTypeSeq& event_type_seq)
{
  this->insert_seq (event_type_seq);
}

void
TAO_NS_EventTypeSeq::populate (CosNotification::EventTypeSeq& event_type_seq) const
{
  event_type_seq.length (this->size ());

  inherited::CONST_ITERATOR iter (*this);

  TAO_NS_EventType* event_type;

  CORBA::ULong i = 0;
  for (iter.first (); iter.next (event_type); iter.advance (), ++i)
    event_type_seq[i] = event_type->native ();
}

void
TAO_NS_EventTypeSeq::insert_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_NS_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::insert (event_type);
    }
}

void
TAO_NS_EventTypeSeq::remove_seq (const CosNotification::EventTypeSeq& event_type_seq)
{
  TAO_NS_EventType event_type;

  for (CORBA::ULong i = 0; i < event_type_seq.length (); ++i)
    {
      event_type = event_type_seq[i];
      inherited::remove (event_type);
    }
}

void
TAO_NS_EventTypeSeq::insert_seq (const TAO_NS_EventTypeSeq& event_type_seq)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (event_type_seq);

  TAO_NS_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    inherited::insert (*event_type);
}

void
TAO_NS_EventTypeSeq::remove_seq (const TAO_NS_EventTypeSeq& event_type_seq)
{
  TAO_NS_EventTypeSeq::CONST_ITERATOR iter (event_type_seq);

  TAO_NS_EventType* event_type;

  for (iter.first (); iter.next (event_type); iter.advance ())
    inherited::remove (*event_type);
}

void
TAO_NS_EventTypeSeq::init (TAO_NS_EventTypeSeq& seq_added, TAO_NS_EventTypeSeq& seq_remove_seq)
{
  const TAO_NS_EventType& special = TAO_NS_EventType::special ();

  if (this->find (special) == 0)
    {
      if (seq_added.find (special) == 0)
        {
          seq_added.reset ();
          seq_remove_seq.reset ();
        }
      else
        {
          this->reset ();
          this->insert_seq (seq_added);

          seq_remove_seq.reset ();
          seq_remove_seq.insert (special);
        }
    }
  else
    {
      if (seq_added.find (special) == 0)
        {
          if (seq_remove_seq.find (special) == 0)
            {
              seq_added.reset ();
              seq_remove_seq.reset ();
            }
          else
            {
              seq_remove_seq.reset ();
              seq_remove_seq.insert_seq (*this);

              this->reset ();
              this->insert (special);

              seq_added.reset ();
              seq_added.insert (special);
            }
        }
      else
        {
          if (seq_remove_seq.find (special) == 0)
            {

              seq_remove_seq.reset ();
              seq_remove_seq.insert_seq (*this);

              this->reset ();
              this->insert_seq (seq_added);
            }
          else
            {
              this->insert_seq (seq_added);
              this->remove_seq (seq_remove_seq);
            }
        }
    }
}
