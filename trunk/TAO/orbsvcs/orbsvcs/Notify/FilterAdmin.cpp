/* -*- C++ -*- $Id$ */
#include "FilterAdmin.h"
#include "ace/Refcounted_Auto_Ptr.h"

ACE_RCSID(Notify, FilterAdmin, "$Id$")

// Implementation skeleton constructor
TAO_NS_FilterAdmin::TAO_NS_FilterAdmin (void)
{
}

// Implementation skeleton destructor
TAO_NS_FilterAdmin::~TAO_NS_FilterAdmin (void)
{
}

CORBA::Boolean
TAO_NS_FilterAdmin::match (TAO_NS_Event_var &event ACE_ENV_ARG_DECL)
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
      ret_val = event->do_match (entry->int_id_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (ret_val == 1)
        return 1;
    }

  return 0;
}

CosNotifyFilter::FilterID
TAO_NS_FilterAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (new_filter))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CosNotifyFilter::FilterID new_id = this->filter_ids_.id ();

  CosNotifyFilter::Filter_var new_filter_var =
    CosNotifyFilter::Filter::_duplicate (new_filter);

  if (this->filter_list_.bind (new_id, new_filter_var) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      0);
  else
    {
      return new_id;
    }
}

void TAO_NS_FilterAdmin::remove_filter (CosNotifyFilter::FilterID filter_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  if (this->filter_list_.unbind (filter_id) == -1)
    ACE_THROW (CosNotifyFilter::FilterNotFound ());
}

CosNotifyFilter::Filter_ptr
TAO_NS_FilterAdmin::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
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
TAO_NS_FilterAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Figure out the length of the list.
  size_t len = this->filter_list_.current_size ();

  CosNotifyFilter::FilterIDSeq* list = 0;

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    CosNotifyFilter::FilterIDSeq (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  FILTER_LIST_ITER iter (this->filter_list_);
  FILTER_LIST_ENTRY *entry;

  u_int index;

  for (index = 0; iter.done () == 0; iter.advance (), ++index)
    {
      if (iter.next (entry) != 0)
        {
          list[index] = entry->ext_id_;
        }
    }

  return list;
}

void
TAO_NS_FilterAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->filter_list_.unbind_all ();
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

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,TAO_SYNCH_MUTEX>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
