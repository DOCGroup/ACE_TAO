/* -*- C++ -*- $Id$ */
#include "Notify_FilterAdmin_i.h"

ACE_RCSID(Notify, Notify_FilterAdmin_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_FilterAdmin_i::TAO_Notify_FilterAdmin_i (void)
{
}

// Implementation skeleton destructor
TAO_Notify_FilterAdmin_i::~TAO_Notify_FilterAdmin_i (void)
{
}

CORBA::Boolean
TAO_Notify_FilterAdmin_i::match (TAO_Notify_Event &event ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::UnsupportedFilterableData
                   ))
{
  // If no filter is active, treat it as a '*' i.e, let all events pass.
  // or if its the special type, let it pass.
  if (this->filter_list_.current_size () == 0) // || event.is_special_event_type ())
    return 1;

  // We want to return true if atleast one constraint matches.
  FILTER_LIST::ITERATOR iter (this->filter_list_);
  FILTER_LIST::ENTRY *entry;
  CORBA::Boolean ret_val = 0;

  for (; iter.next (entry); iter.advance ())
    {
      ret_val = event.do_match (entry->int_id_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ret_val == 1)
        return 1;
    }

  return 0;
}

CosNotifyFilter::FilterID
TAO_Notify_FilterAdmin_i::add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  if (CORBA::is_nil (new_filter))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CosNotifyFilter::FilterID new_id = this->filter_ids_.get ();

  CosNotifyFilter::Filter_var new_filter_var =
    CosNotifyFilter::Filter::_duplicate (new_filter);

  if (this->filter_list_.bind (new_id, new_filter_var) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      0);
  else
    {
      this->filter_ids_.next ();
      return new_id;
    }
}

void TAO_Notify_FilterAdmin_i::remove_filter (CosNotifyFilter::FilterID filter_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  if (this->filter_list_.unbind (filter_id) == -1)
    ACE_THROW (CosNotifyFilter::FilterNotFound ());

  this->filter_ids_.put (filter_id);
}

CosNotifyFilter::Filter_ptr
TAO_Notify_FilterAdmin_i::get_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
    ))
{
  CosNotifyFilter::Filter_var filter_var;

  if (this->filter_list_.find (filter,
                               filter_var) == -1)
    ACE_THROW_RETURN (CosNotifyFilter::FilterNotFound (),
                      0);

  return CosNotifyFilter::Filter::_duplicate (filter_var.in ());
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_FilterAdmin_i::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_ids_.get_sequence(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_FilterAdmin_i::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotifyFilter::FilterIDSeq* id_list =
    this->get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  for (CORBA::ULong i = 0; i < id_list->length (); ++i)
    {
      CosNotifyFilter::FilterID id = (*id_list)[i];
      this->remove_filter (id ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  delete id_list;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>;
template class ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>;
template class TAO_Notify_ID_Pool_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::FilterIDSeq>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate TAO_Notify_ID_Pool_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::FilterIDSeq>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
