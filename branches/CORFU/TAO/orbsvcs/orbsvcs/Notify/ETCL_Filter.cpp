// $Id$

#include "orbsvcs/Notify/ETCL_Filter.h"

ACE_RCSID(Notify,
          TAO_Notify_ETCL_Filter,
          "$Id$")

#include "ace/Auto_Ptr.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Notify_Constraint_Visitors.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ETCL_Filter::TAO_Notify_ETCL_Filter (PortableServer::POA_ptr poa)
  : constraint_expr_ids_ (0),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Notify_ETCL_Filter::~TAO_Notify_ETCL_Filter ()
{
  try
    {
      this->remove_all_constraints ();
    }
  catch (const CORBA::Exception&)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, "Error in Filter dtor\n"));

      // @@ eat exception.
    }

  if (TAO_debug_level > 1)
      ACE_DEBUG ((LM_DEBUG, "Filter Destroyed\n"));
}

char*
TAO_Notify_ETCL_Filter::constraint_grammar (void)
{
  return CORBA::string_dup ("ETCL");
}

void
TAO_Notify_ETCL_Filter::add_constraints_i (
  const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq)
{
  for (CORBA::ULong index = 0; index < constraint_info_seq.length (); ++index)
    {
      TAO_Notify_Constraint_Expr* notify_constr_expr = 0;

      ACE_NEW_THROW_EX (notify_constr_expr,
                        TAO_Notify_Constraint_Expr (),
                        CORBA::NO_MEMORY ());
      auto_ptr <TAO_Notify_Constraint_Expr> auto_expr (notify_constr_expr);

      const CosNotifyFilter::ConstraintExp& expr =
        constraint_info_seq[index].constraint_expression;

      notify_constr_expr->interpreter.
        build_tree (expr.constraint_expr.in ());

      notify_constr_expr->constr_expr = expr;

      CosNotifyFilter::ConstraintID cnstr_id = ++constraint_expr_ids_;

      if (this->constraint_expr_list_.bind (cnstr_id, notify_constr_expr) == -1)
        throw CORBA::INTERNAL ();

      if (TAO_debug_level > 1)
        ACE_DEBUG ((LM_DEBUG, "Added constraint to filter %x\n", this, expr.constraint_expr.in ()));

      auto_expr.release ();
    }
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_ETCL_Filter::add_constraints (
  const CosNotifyFilter::ConstraintExpSeq& constraint_list)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  CORBA::ULong constraint_length = constraint_list.length ();

  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, "constraint_length = %d\n",
                  constraint_length));

  // Create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq* infoseq_ptr;
  ACE_NEW_THROW_EX (infoseq_ptr,
                    CosNotifyFilter::ConstraintInfoSeq (constraint_length),
                    CORBA::NO_MEMORY ());

  CosNotifyFilter::ConstraintInfoSeq_var infoseq (infoseq_ptr);
  infoseq->length (constraint_length);

  // Populate infoseq.
  for (CORBA::ULong pop_index = 0; pop_index < constraint_length; ++pop_index)
    {
      infoseq[pop_index].constraint_expression =
        constraint_list [pop_index];

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Adding constraint %d, %s\n",
                      pop_index,
                      constraint_list [pop_index].constraint_expr.in ()));
        }
    }

  this->add_constraints_i (infoseq.in ());

  return infoseq._retn ();
}

void
TAO_Notify_ETCL_Filter::modify_constraints (
  const CosNotifyFilter::ConstraintIDSeq & del_list,
  const CosNotifyFilter::ConstraintInfoSeq & modify_list)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // First check if all the ids are valid.
  u_int index;

  for (index = 0; index < del_list.length (); ++index)
    {
      if (this->constraint_expr_list_.find (del_list [index]) == -1)
        {
          throw CosNotifyFilter::ConstraintNotFound (del_list [index]);
        }
    }

  for (index = 0; index < modify_list.length (); ++index)
    {
      if (this->constraint_expr_list_.find (modify_list [index].constraint_id) == -1)
        {
          throw CosNotifyFilter::ConstraintNotFound (
            modify_list [index].constraint_id);
        }
    }

  // Remove previous entries and save them in case we need to reinstate them.
  ACE_Array<TAO_Notify_Constraint_Expr*> constr_saved (modify_list.length ());
  TAO_Notify_Constraint_Expr* constr_expr = 0;

  for (index = 0; index < modify_list.length (); ++index)
    {
      CosNotifyFilter::ConstraintID cnstr_id =
        modify_list [index].constraint_id;

      if (this->constraint_expr_list_.unbind (cnstr_id, constr_expr) != -1)
        {
          constr_saved[index] = constr_expr;
        }
    }

  // Now add the new entries.
  // Keep a list of ids generated in this session.
  try
    {
      this->add_constraints_i (modify_list);
    }
  catch (const CORBA::Exception&)
    {
      // Restore,
      for (index = 0; index < modify_list.length (); ++index)
        {
          CosNotifyFilter::ConstraintID cnstr_id = ++this->constraint_expr_ids_;

          if (constraint_expr_list_.bind (cnstr_id, constr_saved[index]) == -1)
            throw CORBA::NO_RESOURCES ();
        }

      throw;
    }

  // Now go around deleting...
  // for the del_list.
  for (index = 0; index < del_list.length (); ++index)
    {
      if (this->constraint_expr_list_.unbind (del_list [index], constr_expr) != -1)
        {
          delete constr_expr;
        }
    }

  // Delete the old constraints.
  for (index = 0; index < constr_saved.max_size (); ++index)
    {
      delete constr_saved[index];
    }
}

CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_ETCL_Filter::get_constraints (
  const CosNotifyFilter::ConstraintIDSeq & id_list)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // Create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq *infoseq_ptr;
  ACE_NEW_THROW_EX (infoseq_ptr,
                    CosNotifyFilter::ConstraintInfoSeq (id_list.length ()),
                    CORBA::NO_MEMORY ());

  CosNotifyFilter::ConstraintInfoSeq_var infoseq (infoseq_ptr);

  TAO_Notify_Constraint_Expr *notify_constr_expr = 0;

  for (u_int index = 0; index < id_list.length (); ++index)
    {
      if (this->constraint_expr_list_.find (id_list[index],
                                            notify_constr_expr) == -1)
        throw CosNotifyFilter::ConstraintNotFound (id_list[index]);

      infoseq[index].constraint_expression =
        notify_constr_expr->constr_expr;

      // Get an id.
      infoseq[index].constraint_id = id_list[index];
    }

  return infoseq._retn ();
}

CosNotifyFilter::ConstraintInfoSeq *
TAO_Notify_ETCL_Filter::get_all_constraints (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  CORBA::ULong current_size = static_cast<CORBA::ULong> (this->constraint_expr_list_.current_size ());

  // Create the list that goes out.
  CosNotifyFilter::ConstraintInfoSeq *infoseq_ptr;
  ACE_NEW_THROW_EX (infoseq_ptr,
                    CosNotifyFilter::ConstraintInfoSeq (current_size),
                    CORBA::NO_MEMORY ());

  CosNotifyFilter::ConstraintInfoSeq_var infoseq (infoseq_ptr);

  infoseq->length (current_size);

  CONSTRAINT_EXPR_LIST::ITERATOR iter (this->constraint_expr_list_);
  CONSTRAINT_EXPR_LIST::ENTRY *entry;

  for (u_int index = 0; iter.done () == 0; iter.advance (), ++index)
    {
      if (iter.next (entry) != 0)
        {
          // Why do we cast to a const object?
          // We want to force the TAO::String_Manager to make a
          // copy of the string. It wouldn't unless we coax it to use
          // the correct assignment operator.
          infoseq[index].constraint_expression =
            static_cast<const CosNotifyFilter::ConstraintExp> (entry->int_id_->constr_expr);

          infoseq[index].constraint_id = entry->ext_id_;
        }
    }

  return infoseq._retn ();
}

void
TAO_Notify_ETCL_Filter::remove_all_constraints (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  this->remove_all_constraints_i ();
}

void
TAO_Notify_ETCL_Filter::remove_all_constraints_i (void)
{
  CONSTRAINT_EXPR_LIST::ITERATOR iter (this->constraint_expr_list_);
  CONSTRAINT_EXPR_LIST::ENTRY *entry;

  u_int index;

  for (index = 0; iter.done () == 0; iter.advance (), ++index)
    {
      if (iter.next (entry) != 0)
        {
          delete entry->int_id_;
          entry->int_id_ = 0;
        }
    }

  this->constraint_expr_list_.unbind_all ();
}

void
TAO_Notify_ETCL_Filter::destroy (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  this->remove_all_constraints_i ();

  PortableServer::ObjectId_var refTemp = poa_->servant_to_id (this);

  poa_->deactivate_object (refTemp.in ());
}

CORBA::Boolean
TAO_Notify_ETCL_Filter::match (const CORBA::Any & /*filterable_data */)
{
  throw CORBA::NO_IMPLEMENT ();
}

CORBA::Boolean
TAO_Notify_ETCL_Filter::match_structured (
  const CosNotification::StructuredEvent & filterable_data)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // We want to return true if at least one constraint matches.
  CONSTRAINT_EXPR_LIST::ITERATOR iter (this->constraint_expr_list_);
  CONSTRAINT_EXPR_LIST::ENTRY *entry;

  TAO_Notify_Constraint_Visitor visitor;

  if (visitor.bind_structured_event (filterable_data) != 0)
    {
      // Maybe throw some kind of exception here, or lower down,
      return 0;
    }

  for (; iter.done () == 0; iter.advance ())
    {
      if (iter.next (entry) != 0)
        {
          if (entry->int_id_->interpreter.evaluate (visitor) == 1)
            {
              return 1;
            }
        }
    }

  return 0;
}

CORBA::Boolean
TAO_Notify_ETCL_Filter::match_typed (
                            const CosNotification::PropertySeq & /* filterable_data */
                            )
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::CallbackID
TAO_Notify_ETCL_Filter::attach_callback (
  CosNotifyComm::NotifySubscribe_ptr /* callback */)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_Notify_ETCL_Filter::detach_callback (
  CosNotifyFilter::CallbackID /* callback */)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::CallbackIDSeq *
TAO_Notify_ETCL_Filter::get_callbacks (void)
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
