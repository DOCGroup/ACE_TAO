/* -*- C++ -*- $Id$ */
#include "orbsvcs/orbsvcs/Notify/Notify_FilterAdmin_i.h"

// Implementation skeleton constructor
TAO_Notify_FilterAdmin_i::TAO_Notify_FilterAdmin_i (void)
{
}

// Implementation skeleton destructor
TAO_Notify_FilterAdmin_i::~TAO_Notify_FilterAdmin_i (void)
{
}

CORBA::Boolean
TAO_Notify_FilterAdmin_i::match (
    const CORBA::Any& filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ))
{
  // @@ TODO: later!
  return 1; // lies, all lies
}

CORBA::Boolean
TAO_Notify_FilterAdmin_i::match_structured (
    const CosNotification::StructuredEvent& filterable_data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ))
{
  // If no filter is active, treat it as a '*' i.e, let all events pass.
  if (filter_list_.current_size () == 0)
    return 1;

  // We want to return true if atleast one constraint matches.
  FILTER_LIST_ITER iter (filter_list_);
  FILTER_ENTRY *entry;
  CORBA::Boolean ret_val = 0;

  for (; iter.done () == 0; iter.advance ())
    {
      if (iter.next (entry) != 0)
        {
          ret_val = entry->int_id_->match_structured (filterable_data,
                                                      ACE_TRY_ENV);
          ACE_CHECK_RETURN (0);

          if (ret_val == 1)
            return 1;
        }
    }

  return 0;
}

CosNotifyFilter::FilterID
TAO_Notify_FilterAdmin_i::add_filter (
    CosNotifyFilter::Filter_ptr new_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  if (CORBA::is_nil (new_filter))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CosNotifyFilter::FilterID new_id = filter_ids_.get ();

  CosNotifyFilter::Filter_var new_filter_var =
    CosNotifyFilter::Filter::_duplicate (new_filter);

  if (filter_list_.bind (new_id, new_filter_var) == -1)
    {
      filter_ids_.put (new_id); // return the id to the pool

      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        0);
    }
  else
    return new_id;
}

void TAO_Notify_FilterAdmin_i::remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ))
{
  if (filter_list_.unbind (filter) == -1)
    ACE_THROW (CosNotifyFilter::FilterNotFound ());
}

CosNotifyFilter::Filter_ptr
TAO_Notify_FilterAdmin_i::get_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
    ))
{
  CosNotifyFilter::Filter_var filter_var;

  if (filter_list_.find (filter,
                         filter_var) == -1)
    ACE_THROW_RETURN (CosNotifyFilter::FilterNotFound (),
                      0);

  return CosNotifyFilter::Filter::_duplicate (filter_var.in ());
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_FilterAdmin_i::get_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
  return 0;
}

void
TAO_Notify_FilterAdmin_i::remove_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>;
template class ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>;

template class ID_Pool<CosNotifyFilter::FilterID>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var>;
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::FilterID,CosNotifyFilter::Filter_var,ACE_Hash<CosNotifyFilter::FilterID>, ACE_Equal_To<CosNotifyFilter::FilterID>,ACE_SYNCH_MUTEX>

#pragma instantiate ID_Pool<CosNotifyFilter::FilterID>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
