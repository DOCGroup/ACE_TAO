/* -*- C++ -*- $Id$ */

#include "FilterAdmin.h"

#if ! defined (__ACE_INLINE__)
#include "FilterAdmin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, FilterAdmin, "$Id$")

#include "ace/Refcounted_Auto_Ptr.h"

// Implementation skeleton constructor
TAO_Notify_FilterAdmin::TAO_Notify_FilterAdmin (void)
{
}

// Implementation skeleton destructor
TAO_Notify_FilterAdmin::~TAO_Notify_FilterAdmin (void)
{
}

CosNotifyFilter::FilterID
TAO_Notify_FilterAdmin::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (new_filter))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CosNotifyFilter::FilterID new_id = this->filter_ids_.id ();

  CosNotifyFilter::Filter_var new_filter_var =
    CosNotifyFilter::Filter::_duplicate (new_filter);

  if (this->filter_list_.bind (new_id, new_filter_var) == -1)
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
  else
    return new_id;
}

void
TAO_Notify_FilterAdmin::remove_filter (CosNotifyFilter::FilterID filter_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->filter_list_.unbind (filter_id) == -1)
    ACE_THROW (CosNotifyFilter::FilterNotFound ());
}

CosNotifyFilter::Filter_ptr
TAO_Notify_FilterAdmin::get_filter (CosNotifyFilter::FilterID filter_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  CosNotifyFilter::Filter_var filter_var;

  if (this->filter_list_.find (filter_id,
                               filter_var) == -1)
    ACE_THROW_RETURN (CosNotifyFilter::FilterNotFound (),
                      0);

  return filter_var._retn ();
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_FilterAdmin::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  // Figure out the length of the list.
  size_t len = this->filter_list_.current_size ();

  CosNotifyFilter::FilterIDSeq* list_ptr;

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list_ptr,
                    CosNotifyFilter::FilterIDSeq,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CosNotifyFilter::FilterIDSeq_var list (list_ptr);

  list->length (ACE_static_cast (CORBA::ULong, len));

  FILTER_LIST::ITERATOR iter (this->filter_list_);
  FILTER_LIST::ENTRY *entry;

  u_int index;

  for (index = 0; iter.next (entry) != 0; iter.advance (), ++index)
    {
      list[index] = entry->ext_id_;
    }

  return list._retn ();
}

void
TAO_Notify_FilterAdmin::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  this->filter_list_.unbind_all ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>;
template class ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_Null_Mutex>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
