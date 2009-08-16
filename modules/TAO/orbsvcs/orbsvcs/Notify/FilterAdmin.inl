// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_Notify_FilterAdmin::match (const TAO_Notify_Event* event)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // If no filter is active, match is successfull.
  if (this->filter_list_.current_size () == 0)
    return 1;

  // We want to return true if atleast one constraint matches.
  FILTER_LIST::ITERATOR iter (this->filter_list_);
  FILTER_LIST::ENTRY *entry = 0;
  CORBA::Boolean ret_val = 0;

  for (; iter.next (entry); iter.advance ())
    {
      ret_val = event->do_match (entry->int_id_.in ());

      if (ret_val == 1)
        return 1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
