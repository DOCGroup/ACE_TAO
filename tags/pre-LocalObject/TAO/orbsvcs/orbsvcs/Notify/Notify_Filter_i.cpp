/* -*- C++ -*- $Id$ */

#include "ace/Auto_Ptr.h"
#include "Notify_Filter_i.h"
#include "Notify_Constraint_Visitors.h"

// Implementation skeleton constructor
TAO_Notify_Filter_i::TAO_Notify_Filter_i (void)
{
}

// Implementation skeleton destructor
TAO_Notify_Filter_i::~TAO_Notify_Filter_i (void)
{
}

CosNotifyFilter::Filter_ptr
TAO_Notify_Filter_i::get_ref (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyFilter::Filter_var filter;

  filter = _this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  return filter._retn ();
}

char*
TAO_Notify_Filter_i::constraint_grammar (
    CORBA::Environment & /* ACE_TRY_ENV */
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  return CORBA::string_dup ("TCL");
  // @@pradeep: change this when we support ExTCL.
}

void
TAO_Notify_Filter_i::add_constraints_i (
    const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint
  ))
{
  for (CORBA::ULong index = 0;
       index < constraint_info_seq.length ();
       ++index)
    {
      Notify_Constraint_Expr* notify_constr_expr = 0;

      ACE_TRY
        {
          const CosNotifyFilter::ConstraintExp& constr_exp =
            constraint_info_seq [index].constraint_expression;

          notify_constr_expr = 0;

          ACE_NEW_THROW_EX (notify_constr_expr,
                            Notify_Constraint_Expr (),
                            CORBA::NO_MEMORY ());

          notify_constr_expr->interpreter.
            build_tree (constraint_info_seq[index].constraint_expression.constraint_expr.in (),
                        ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (constraint_expr_list_.
              bind (constraint_info_seq[index].constraint_id,
                    notify_constr_expr) == -1)
            ACE_THROW (CORBA::NO_RESOURCES ());

          ACE_UNUSED_ARG (constr_exp);
        }
      ACE_CATCHANY
        {
          Notify_Constraint_Expr* undo_info;
          // Too bad: undo all the memory allocations done so far.
          delete notify_constr_expr; // first the one that failed us.

          for (u_int i = 0; i < index; i++) // those from previous iterations.
            {
              // Put the id back to the pool.
              constraint_expr_ids_.
                put (constraint_info_seq[index].constraint_id);

              // Unbind from the Hash Table
              if (constraint_expr_list_.unbind
                  (constraint_info_seq[index].constraint_id,
                   undo_info) != -1)
                delete undo_info;
            }
          ACE_RETHROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    } // for
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Filter_i::add_constraints (
    const CosNotifyFilter::ConstraintExpSeq& constraint_list,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint
  ))
{
  int constraint_length = constraint_list.length ();

  ACE_DEBUG ((LM_DEBUG, "constraint_length = %d\n", constraint_length));
  // create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq* infoseq;
  ACE_NEW_THROW_EX (infoseq,
                    CosNotifyFilter::ConstraintInfoSeq (constraint_length),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  infoseq->length (constraint_length);

  auto_ptr<CosNotifyFilter::ConstraintInfoSeq> auto_infoseq (infoseq);

  // populate infoseq
  for (int pop_index = 0;
       pop_index < constraint_length;
       ++pop_index)
    {
      (*infoseq)[pop_index].constraint_expression =
        constraint_list [pop_index] ;

      // Get an id.
      (*infoseq)[pop_index].constraint_id = constraint_expr_ids_.get ();
      // <add_constraints_i> will put the ids back to the id pool if things
      // go wrong.
    }

  this->add_constraints_i (*infoseq,
                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  auto_infoseq.release ();
  return infoseq;
}

void
TAO_Notify_Filter_i::modify_constraints (
    const CosNotifyFilter::ConstraintIDSeq & del_list,
    const CosNotifyFilter::ConstraintInfoSeq & modify_list,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::InvalidConstraint,
    CosNotifyFilter::ConstraintNotFound
  ))
{
  // first check if all the ids are valid.
  u_int index;
  for (index = 0; index < del_list.length (); ++index)
    {
      if (constraint_expr_list_.find (del_list [index]) == -1)
        ACE_THROW (CosNotifyFilter::ConstraintNotFound (del_list [index]));
    }

  for (index = 0; index < modify_list.length (); ++index)
    {
      if (constraint_expr_list_.find (modify_list [index].constraint_id)
          == -1)
        ACE_THROW (CosNotifyFilter::ConstraintNotFound (modify_list [index].constraint_id));
    }

  // remove previous entries and save them in case we need to reinstate them.
  ACE_Array<Notify_Constraint_Expr*> constr_saved (modify_list.length ());
  Notify_Constraint_Expr* constr_expr = 0;

  for (index = 0; index < modify_list.length (); ++index)
    {
      if (constraint_expr_list_.unbind (modify_list [index].constraint_id,
                                        constr_expr) != -1)
        constr_saved[index] = constr_expr;
    }

  // now add the new entries.
  // keep a list of ids generated in this session.

  ACE_TRY
    {
      this->add_constraints_i (modify_list,
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // restore
      for (index = 0; index < modify_list.length (); ++index)
        constraint_expr_list_.bind (modify_list [index].constraint_id,
                                    constr_saved[index]);
      ACE_RETHROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;

  // now go around deleting...
  // for the del_list
  for (index = 0; index < del_list.length (); ++index)
    {
      if (constraint_expr_list_.unbind (del_list [index],
                                        constr_expr) != -1)
        {
          delete constr_expr;
        }
    }
  // delete the old constraints.
  for (index = 0; index < constr_saved.max_size (); ++index)
    {
      delete constr_saved[index];
    }
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_Filter_i::get_constraints (
    const CosNotifyFilter::ConstraintIDSeq & id_list,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::ConstraintNotFound
  ))
{
  // create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq* infoseq;
  ACE_NEW_THROW_EX (infoseq,
                    CosNotifyFilter::ConstraintInfoSeq (id_list.length ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  auto_ptr<CosNotifyFilter::ConstraintInfoSeq> auto_infoseq (infoseq);

  Notify_Constraint_Expr *notify_constr_expr = 0;
  for (u_int index = 0; index < id_list.length (); ++index)
    {
      if (constraint_expr_list_.find (id_list[index],
                                      notify_constr_expr) == -1)
        ACE_THROW_RETURN (CosNotifyFilter::ConstraintNotFound (id_list[index]),
                          0);

      (*infoseq)[index].constraint_expression =
        notify_constr_expr->constr_expr;

      // Get an id.
      (*infoseq)[index].constraint_id = id_list[index];
    }

  auto_infoseq.release ();
  return infoseq;
}

CosNotifyFilter::ConstraintInfoSeq * TAO_Notify_Filter_i::get_all_constraints (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq* infoseq;
  ACE_NEW_THROW_EX (infoseq,
                    CosNotifyFilter::ConstraintInfoSeq
                    (constraint_expr_list_.current_size()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CONSTRAINT_EXPR_LIST_ITER iter (constraint_expr_list_);
  CONSTRAINT_EXPR_ENTRY *entry;

  for (u_int index = 0; iter.done () == 0; iter.advance (), ++index)
    {
      if (iter.next (entry) != 0)
        {
          (*infoseq)[index].constraint_expression =
            entry->int_id_->constr_expr;

          // Get an id.
          (*infoseq)[index].constraint_id = entry->ext_id_;
        }
    }
  return infoseq;
}

void TAO_Notify_Filter_i::remove_all_constraints (
                                                  CORBA::Environment & //ACE_TRY_ENV
                                                  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CONSTRAINT_EXPR_LIST_ITER iter (constraint_expr_list_);
  CONSTRAINT_EXPR_ENTRY *entry;

  ACE_Array<Notify_Constraint_Expr*>
    constr_saved (constraint_expr_list_.current_size ());

  u_int index;
  for (index = 0; iter.done () == 0; iter.advance (), ++index)
    {
      if (iter.next (entry) != 0)
        {
          constr_saved [index] =
            entry->int_id_;
        }
    }

  for (u_int dindex = 0; dindex < index; ++dindex)
    delete constr_saved [dindex];
}

void TAO_Notify_Filter_i::destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  this->remove_all_constraints (ACE_TRY_ENV);
  ACE_CHECK;

  //TODO: deactivate the object here!
}

CORBA::Boolean TAO_Notify_Filter_i::match (
                                           const CORBA::Any & /*filterable_data */,
                                           CORBA::Environment & //ACE_TRY_ENV
                                           )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::UnsupportedFilterableData
                   ))
{
  //Add your implementation here
  return 0;
}

CORBA::Boolean TAO_Notify_Filter_i::match_structured (
                                                      const CosNotification::StructuredEvent & filterable_data,
                                                      CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::UnsupportedFilterableData
  ))
{
  // We want to return true if atleast one constraint matches.
  CONSTRAINT_EXPR_LIST_ITER iter (constraint_expr_list_);
  CONSTRAINT_EXPR_ENTRY *entry;

  TAO_Notify_Constraint_Evaluator eval (filterable_data);

  for (; iter.done () == 0; iter.advance ())
    {
      if (iter.next (entry) != 0)
        {
          if (entry->int_id_->interpreter.evaluate (eval) == 1)
            return 1;
        }
    }

  return 0;
}

CORBA::Boolean TAO_Notify_Filter_i::match_typed (
                                                 const CosNotification::PropertySeq & /*filterable_data */,
                                                 CORBA::Environment & //ACE_TRY_ENV
                                                 )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::UnsupportedFilterableData
                   ))
{
  //Add your implementation here
  return 0;
}

CosNotifyFilter::CallbackID TAO_Notify_Filter_i::attach_callback (
                                                                  CosNotifyComm::NotifySubscribe_ptr /*callback */,
    CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
  return 0;
}

void TAO_Notify_Filter_i::detach_callback (
                                           CosNotifyFilter::CallbackID /*callback*/,
                                           CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::CallbackNotFound
  ))
{
  //Add your implementation here
}

CosNotifyFilter::CallbackIDSeq * TAO_Notify_Filter_i::get_callbacks (
                                                                     CORBA::Environment & //ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  //Add your implementation here
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr *>;
template class ACE_Hash_Map_Manager<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>;


template class ACE_Equal_To<CosNotifyFilter::ConstraintID>;

template class ACE_Array<TAO_Notify_Filter_i::Notify_Constraint_Expr*>;
template class ACE_Array_Base<TAO_Notify_Filter_i::Notify_Constraint_Expr*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*>
#pragma instantiate ACE_Hash_Map_Manager<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<CosNotifyFilter::ConstraintID,TAO_Notify_Filter_i::Notify_Constraint_Expr*,ACE_Hash<CosNotifyFilter::ConstraintID>, ACE_Equal_To<CosNotifyFilter::ConstraintID>,ACE_SYNCH_MUTEX>

#pragma instantiate ACE_Array<TAO_Notify_Filter_i::Notify_Constraint_Expr*>
#pragma instantiate ACE_Array_Base<TAO_Notify_Filter_i::Notify_Constraint_Expr*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
