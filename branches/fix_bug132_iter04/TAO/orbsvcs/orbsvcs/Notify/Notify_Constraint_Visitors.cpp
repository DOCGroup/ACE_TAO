/* -*- C++ -*- $Id$ */
#include "Notify_Constraint_Visitors.h"

TAO_Notify_Constraint_Evaluator::
TAO_Notify_Constraint_Evaluator (const CosNotification::StructuredEvent& s_event)
{
  // Consider the filterable_data for now.

  // @@ Pradeep: this whole class is *extremely* error prone:
  //    - You are holding a pointer to a component of an object that
  //    is possibly created on the stack (the event)
  //    - You store that pointer as a const object and then use it in
  //    a non-const object in the next method...
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
      // @@ Pradeep: now you are creating a copy of the Literal
      //    object, but putting in in a queue, what is to say that the
      //    literal will not survive the pointer to the value it
      //    holds?
      //    It seems like you are very sure that the
      //    Constraint_Evaluator is created on the stack, otherwise
      //    this whole thing makes no sense.  You should at least
      //    include some comments clarifying the situation.
      //    You must make sure that this code is const-correct, it is
      //    not right now...

      // @@ I added this cast to get the damn thing to compile,
      // Pradeep will have to fix it later.
      CORBA::Any *any = ACE_const_cast(CORBA::Any*,cvalue);
      this->queue_.enqueue_head (TAO_Literal_Constraint (any));
      return_value = 0;
    }
  return return_value;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString, const CORBA::Any*, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry <ACE_CString, const CORBA::Any*>;
template class ACE_Hash_Map_Iterator<ACE_CString, const CORBA::Any*, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, const CORBA::Any*, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, const CORBA::Any*, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry <ACE_CString, const CORBA::Any*>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, const CORBA::Any*, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, const CORBA::Any*, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, const CORBA::Any *, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
