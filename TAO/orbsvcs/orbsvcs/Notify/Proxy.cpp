// $Id$

#include "Proxy.h"
#include "Admin.h"

#if ! defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Proxy, "$Id$")

TAO_NS_Proxy::TAO_NS_Proxy (void)
{
  // Set initial proxy mode to broadcast.
  this->subscribed_types_.insert (TAO_NS_EventType::special ());
}


TAO_NS_Proxy::~TAO_NS_Proxy ()
{
}

void
TAO_NS_Proxy::preprocess (TAO_NS_EventTypeSeq& seq_added, TAO_NS_EventTypeSeq& seq_remove_seq)
{
  const TAO_NS_EventType& special = TAO_NS_EventType::special ();

  if (this->subscribed_types_.find (special) == 0)
    {
      if (seq_added.find (special) == 0)
	{
	  seq_added.reset ();
	  seq_remove_seq.reset ();
	}
      else
	{
	  this->subscribed_types_.reset ();
	  this->subscribed_types_.insert_seq (seq_added);

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
	      seq_remove_seq.insert_seq (this->subscribed_types_);

	      this->subscribed_types_.reset ();
	      this->subscribed_types_.insert (special);
	      
	      seq_added.reset ();
	      seq_added.insert (special);
	    }
	}
      else
	{
	  if (seq_remove_seq.find (special) == 0)
	    {
	    
	      seq_remove_seq.reset ();
	      seq_remove_seq.insert_seq (this->subscribed_types_);

	      this->subscribed_types_.reset ();
	      this->subscribed_types_.insert_seq (seq_added);
	    }
	  else
	    {
	      this->subscribed_types_.insert_seq (seq_added);
	      this->subscribed_types_.remove_seq (seq_remove_seq);
	    }
	}
    }
}
