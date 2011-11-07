// $Id$

#include "orbsvcs/Notify/ETCL_Filter.h"
#include "ace/Auto_Ptr.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Notify_Constraint_Visitors.h"
#include "orbsvcs/Notify/Topology_Saver.h"

#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Constraint_Expr::TAO_Notify_Constraint_Expr (void)
{
}


TAO_Notify_Constraint_Expr::~TAO_Notify_Constraint_Expr ()
{
}


void
TAO_Notify_Constraint_Expr::save_persistent (
  TAO_Notify::Topology_Saver& saver)
{
  CosNotification::EventTypeSeq& event_types = this->constr_expr.event_types;
  CORBA::ULong len = event_types.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
  {
    TAO_Notify::NVPList attrs;
    bool changed = true;

    attrs.push_back(TAO_Notify::NVP("Domain", event_types[i].domain_name.in()));
    attrs.push_back(TAO_Notify::NVP("Type", event_types[i].type_name.in()));
    saver.begin_object(0, "EventType", attrs, changed);
    saver.end_object(0, "EventType");
  }
}


void
TAO_Notify_Constraint_Expr::load_attrs(
  const TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::load_attrs (attrs);
  const char* expr = 0;
  if (attrs.find ("Expression", expr))
  {
    this->constr_expr.constraint_expr = CORBA::string_dup (expr);
  }
}


TAO_Notify::Topology_Object*
TAO_Notify_Constraint_Expr::load_child (
  const ACE_CString &type,
  CORBA::Long id,
  const TAO_Notify::NVPList& attrs)
{
  ACE_UNUSED_ARG (id);
  TAO_Notify::Topology_Object* result = this;
  if (type == "EventType")
  {
    const char* domain = 0;
    const char* type = 0;
    attrs.find ("Domain", domain);
    attrs.find ("Type", type);

    CORBA::ULong len = this->constr_expr.event_types.length ();
    if (DEBUG_LEVEL)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) reload EventType %d \n"),
                  len + 1));

    this->constr_expr.event_types.length (len + 1);
    this->constr_expr.event_types[len].domain_name = CORBA::string_dup (domain);
    this->constr_expr.event_types[len].type_name = CORBA::string_dup (type);

    this->interpreter.build_tree (this->constr_expr);
  }

  return result;
}


void
TAO_Notify_Constraint_Expr::release (void)
{
  delete this;
  //@@ inform factory
}



TAO_Notify_ETCL_Filter::TAO_Notify_ETCL_Filter (PortableServer::POA_ptr poa,
                                                const char *constraint_grammar,
                                                const TAO_Notify_Object::ID& id)
  :constraint_expr_ids_ (0),
   poa_ (PortableServer::POA::_duplicate (poa)),
   id_ (id),
   grammar_ (constraint_grammar)
{
}


TAO_Notify_ETCL_Filter::~TAO_Notify_ETCL_Filter ()
{
  try
    {
      this->destroy();
    }
  catch (const CORBA::Exception&)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Error in Filter dtor\n")));

      // @@ eat exception.
    }

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Filter Destroyed\n")));
}

char*
TAO_Notify_ETCL_Filter::constraint_grammar (void)
{
  return CORBA::string_dup (this->grammar_.c_str ());
}

void
TAO_Notify_ETCL_Filter::add_constraints_i (
  const CosNotifyFilter::ConstraintInfoSeq& constraint_info_seq)
{
  for (CORBA::ULong index = 0; index < constraint_info_seq.length (); ++index)
    {
      this->add_constraint_i (constraint_info_seq[index]);
    }
}

TAO_Notify_Constraint_Expr*
TAO_Notify_ETCL_Filter::add_constraint_i (CosNotifyFilter::ConstraintID cnstr_id)
{
  TAO_Notify_Constraint_Expr* notify_constr_expr = 0;

  ACE_NEW_THROW_EX (notify_constr_expr,
    TAO_Notify_Constraint_Expr (),
    CORBA::NO_MEMORY ());
  auto_ptr <TAO_Notify_Constraint_Expr> auto_expr (notify_constr_expr);

  if (TAO_debug_level > 1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Added an empty constraint to filter\n")));

  if (this->constraint_expr_list_.bind (cnstr_id, notify_constr_expr) == -1)
    throw CORBA::INTERNAL ();

  auto_expr.release ();

  return notify_constr_expr;
}


void
TAO_Notify_ETCL_Filter::add_constraint_i
        (const CosNotifyFilter::ConstraintInfo& constraint,
         CosNotifyFilter::ConstraintID cnstr_id)
{
  TAO_Notify_Constraint_Expr* notify_constr_expr = 0;

  ACE_NEW_THROW_EX (notify_constr_expr,
    TAO_Notify_Constraint_Expr (),
    CORBA::NO_MEMORY ());
  auto_ptr <TAO_Notify_Constraint_Expr> auto_expr (notify_constr_expr);

  CosNotifyFilter::ConstraintExp const & expr =
    constraint.constraint_expression;

  notify_constr_expr->interpreter.build_tree (expr);

  notify_constr_expr->constr_expr = expr;

  if (cnstr_id == 0)
  {
    if (TAO_debug_level > 1)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Added constraint %C to filter %d\n"),
                  expr.constraint_expr.in (), this->id_));

    cnstr_id = ++constraint_expr_ids_;
  }
  else
  {
    if (TAO_debug_level > 1)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Loaded constraint %C to filter %d\n"),
                  expr.constraint_expr.in (), this->id_));
  }

  if (this->constraint_expr_list_.bind (cnstr_id, notify_constr_expr) == -1)
    throw CORBA::INTERNAL ();

  auto_expr.release ();
}


CosNotifyFilter::ConstraintInfoSeq*
TAO_Notify_ETCL_Filter::add_constraints (
  const CosNotifyFilter::ConstraintExpSeq& constraint_list)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  CORBA::ULong const constraint_length = constraint_list.length ();

  if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("constraint_length = %d\n"),
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
                      ACE_TEXT ("Adding constraint %d, %C\n"),
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
  CORBA::ULong index;

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

  this->self_change ();
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

  for (CORBA::ULong index = 0; index < id_list.length (); ++index)
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

  CORBA::ULong const current_size =
    static_cast<CORBA::ULong> (this->constraint_expr_list_.current_size ());

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

  for (CORBA::ULong index = 0; iter.done () == 0; iter.advance (), ++index)
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

  if (!CORBA::is_nil (this->poa_.in()))
    {
      this->remove_all_constraints_i ();

      PortableServer::ObjectId_var refTemp = this->poa_->servant_to_id (this);
      this->poa_->deactivate_object (refTemp.in ());
      this->poa_ = PortableServer::POA::_nil();
    }
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
                            const CosNotification::PropertySeq & /* filterable_data */)
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


void
TAO_Notify_ETCL_Filter::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  TAO_Notify::NVPList attrs;
  bool changed = true;
  attrs.push_back(TAO_Notify::NVP("FilterId", this->id_));
  attrs.push_back(TAO_Notify::NVP("Grammar", this->constraint_grammar()));
  saver.begin_object(0, "filter", attrs, changed);

  {
    int index = 0;
    CONSTRAINT_EXPR_LIST::ITERATOR iterator (this->constraint_expr_list_);

    for (CONSTRAINT_EXPR_LIST::ENTRY *entry = 0;
      iterator.next (entry) != 0;
      iterator.advance (), ++index)
    {
      TAO_Notify::NVPList attrs;
      bool changed = true;
      attrs.push_back(TAO_Notify::NVP("ConstraintId", entry->ext_id_));
      attrs.push_back(TAO_Notify::NVP("Expression",
        entry->int_id_->constr_expr.constraint_expr.in ()));
      saver.begin_object(0, "constraint", attrs, changed);

      entry->int_id_->save_persistent (saver);

      saver.end_object(0, "constraint");
    }

    saver.end_object(0, "filter");
  }
}


void
TAO_Notify_ETCL_Filter::release (void)
{
  delete this;
  //@@ inform factory
}


void
TAO_Notify_ETCL_Filter::load_attrs(const TAO_Notify::NVPList& attrs)
{
  const char* value = 0;
  TAO_Notify_Object::load_attrs (attrs);
  if (attrs.find ("FilterId", value))
  {
    ACE_ASSERT (this->id_ == ACE_OS::atoi (value));
  }

  if (attrs.find ("Grammar", value))
  {
    this->grammar_ = value;
  }
}

TAO_Notify::Topology_Object*
TAO_Notify_ETCL_Filter::load_child (const ACE_CString &type,
  CORBA::Long, const TAO_Notify::NVPList& attrs)
{
  TAO_Notify::Topology_Object* result = this;
  if (type == "constraint")
  {
    const char* value = 0;
    if (attrs.find ("ConstraintId", value))
    {
      TAO_Notify_Object::ID id = ACE_OS::atoi (value);
      constraint_expr_ids_ = id;

      if (DEBUG_LEVEL)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) reload filter %d constraint %d\n"),
                    static_cast<int> (this->id_), static_cast<int> (id)));

      TAO_Notify_Constraint_Expr* expr
        = this->add_constraint_i (id);
      expr->load_attrs (attrs);

      return expr;
    }
  }
  return result;
}


TAO_END_VERSIONED_NAMESPACE_DECL
