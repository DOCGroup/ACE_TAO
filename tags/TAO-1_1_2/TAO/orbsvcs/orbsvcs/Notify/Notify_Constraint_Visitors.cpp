/* -*- C++ -*- $Id$ */
#include "Notify_Constraint_Visitors.h"

TAO_Notify_Constraint_Evaluator::
TAO_Notify_Constraint_Evaluator (const CosNotification::StructuredEvent& s_event)
{
  // Consider the filterable_data for now.
  for (u_int index = 0; index < s_event.filterable_data.length (); ++index)
    {
      ACE_CString name_str (s_event.filterable_data[index].name);
      property_lookup_.bind (name_str,
                             &s_event.filterable_data[index].value);
      //@@: dude, what if bind fails!?
    }
}

int
TAO_Notify_Constraint_Evaluator::
visit_property (TAO_Property_Constraint* literal)
{
  int return_value = -1;

  const CORBA::Any *cvalue = 0;
  ACE_CString name (literal->name ());

  if (this->property_lookup_.find (name, cvalue) == 0)
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      CORBA::Any *value = 0;

      ACE_TRY
        {
          ACE_NEW_THROW_EX (value,
                            CORBA::Any,
                            CORBA::NO_MEMORY ());
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          return -1;
        }
      ACE_ENDTRY;

      *value = *cvalue;
      // @@All this is so expensive, find a way to avoid this mem alloc.
      this->queue_.enqueue_head (TAO_Literal_Constraint (value));
      return_value = 0;
    }
  return return_value;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString, const CORBA::Any*, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry <ACE_CString, const CORBA::Any*>;
template class ACE_Hash_Map_Iterator<ACE_CString, const CORBA::Any*, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, const CORBA::Any*, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, const CORBA::Any*, ACE_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Entry <ACE_CString, const CORBA::Any*>;
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, const CORBA::Any*, ACE_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, const CORBA::Any*, ACE_SYNCH_MUTEX>;
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_MUTEX>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
