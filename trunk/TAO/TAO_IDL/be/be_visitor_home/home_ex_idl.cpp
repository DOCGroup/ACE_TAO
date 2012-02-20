
//=============================================================================
/**
 *  @file    home_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Homes in the CIAO executor IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "home.h"

be_visitor_home_ex_idl::be_visitor_home_ex_idl (
    be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    node_ (0),
    os_ (*ctx->stream ())
{
}

be_visitor_home_ex_idl::~be_visitor_home_ex_idl (void)
{
}

int
be_visitor_home_ex_idl::visit_home (be_home *node)
{
  if (node->imported ())
    {
      return 0;
    }

  node_ = node;

  /// The CCM preproc visitor moved everything in our scope to
  /// the implied *Explicit node. Before we generate the executor
  /// IDL, we need to move it all back.
  this->restore_scope ();

  be_util::gen_nesting_open (os_, node_);

  this->gen_implicit ();
  this->gen_explicit ();
  this->gen_derived ();

  be_util::gen_nesting_close (os_, node_);

  this->gen_home_executor ();

  return 0;
}

int
be_visitor_home_ex_idl::visit_attribute (be_attribute *node)
{
   bool const rd_only = node->readonly ();

   // Keep output statements separate because of side effects.
   // No need to check for anonymous array - anonymous types not
   // accepted by parser for attributes.
   os_ << be_nl
       << (rd_only ? "readonly " : "") << "attribute ";

   be_type *ft = be_type::narrow_from_decl (node->field_type ());

   os_ << IdentifierHelper::type_name (ft, this);
   os_ << " "
       << IdentifierHelper::try_escape (node->original_local_name ()).c_str ();

   this->gen_exception_list (node->get_get_exceptions (),
                             rd_only ? "" : "get");

   this->gen_exception_list (node->get_set_exceptions (),
                             "set");

   os_ << ";";

  return 0;
}

int
be_visitor_home_ex_idl::visit_operation (be_operation *node)
{
  os_ << be_nl;

  if (node->flags () == AST_Operation::OP_oneway)
    {
      os_ << "oneway ";
    }

  be_type *rt = be_type::narrow_from_decl (node->return_type ());

  os_ << IdentifierHelper::type_name (rt, this);

  os_ << " "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " (" << be_idt << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_home_ex_idl::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }

  os_ << ")"
      << be_uidt << be_uidt;

  this->gen_exception_list (node->exceptions ());

  os_ << ";";

  return 0;
}

int
be_visitor_home_ex_idl::visit_argument (be_argument *node)
{
  os_ << be_nl;

  switch (node->direction ())
    {
      case AST_Argument::dir_IN:
        os_ << "in ";
        break;
      case AST_Argument::dir_INOUT:
        os_ << "inout ";
        break;
      case AST_Argument::dir_OUT:
        os_ << "out ";
        break;
      default:
        return -1;
    }

  be_type *ft = be_type::narrow_from_decl (node->field_type ());

  os_ << IdentifierHelper::type_name (ft, this)
      << " "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << (this->last_node (node) ? "" : ",");

  return 0;
}

int
be_visitor_home_ex_idl::visit_factory (be_factory *node)
{
  os_ << be_nl
      << "::Components::EnterpriseComponent "
      << IdentifierHelper::try_escape (node->original_local_name ()).c_str ()
      << " (" << be_idt << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_home_ex_idl::")
                  ACE_TEXT ("visit_factory - ")
                  ACE_TEXT ("codegen for scope failed\n")));
    }

  os_ << ")"
      << be_uidt << be_uidt;

  this->gen_exception_list (node->exceptions (), "", true);

  os_ << ";";

  return 0;
}

int
be_visitor_home_ex_idl::visit_sequence (be_sequence *node)
{
  // Keep output statements separate because of side effects.
  os_ << "sequence<";

  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  os_ << IdentifierHelper::type_name (bt, this);

  if (!node->unbounded ())
    {
      os_ << ", " << node->max_size ()->ev ()->u.ulval;
    }

  os_ << "> ";

  return 0;
}

int
be_visitor_home_ex_idl::visit_string (be_string *node)
{
  bool wide = static_cast<size_t> (node->width ()) > sizeof (char);

  os_ << (wide ? "w" : "") << "string";

  ACE_CDR::ULong bound = node->max_size ()->ev ()->u.ulval;

  if (bound > 0UL)
    {
      os_ << "<" << bound << ">";
    }

  return 0;
}

void
be_visitor_home_ex_idl::gen_implicit (void)
{
  os_ << be_nl
      << "local interface CCM_"
      << IdentifierHelper::try_escape (node_->original_local_name ()).c_str ()
      << "Implicit" << be_nl
      << "{" << be_idt_nl
      << "::Components::EnterpriseComponent create ()" << be_idt_nl
      << "raises ( ::Components::CCMException);"
      << be_uidt << be_uidt_nl
      << "};";
}

void
be_visitor_home_ex_idl::gen_explicit (void)
{
  os_ << be_nl_2
      << "local interface CCM_"
      << IdentifierHelper::try_escape (node_->original_local_name ()).c_str ()
      << "Explicit" << be_idt_nl
      << ": ";

  AST_Home *base = node_->base_home ();

  if (base == 0)
    {
      os_ << "::Components::HomeExecutorBase";

      this->gen_supported ();
    }
  else
    {
      ACE_CString sname_str =
        IdentifierHelper::orig_sn (
          ScopeAsDecl (base->defined_in ())->name ());

      const char *sname = sname_str.c_str ();
      const char *global = (sname_str == "" ? "" : "::");

      os_ << global << sname << "::CCM_"
          << IdentifierHelper::try_escape (base->local_name ()).c_str ()
          << "Explicit";
    }

  os_ << be_uidt_nl
      << "{" << be_idt;

  if (this->visit_scope (node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_home_ex_idl::")
                  ACE_TEXT ("gen_explicit - ")
                  ACE_TEXT ("visit_scope() failed\n")));
    }

  os_ << be_uidt_nl
      << "};";
}

void
be_visitor_home_ex_idl::gen_derived (void)
{
  ACE_CString lname_str =
    IdentifierHelper::try_escape (node_->original_local_name ());
  const char *lname = lname_str.c_str ();

  os_ << be_nl_2
      << "local interface CCM_" << lname << be_nl
      << "  : CCM_" << lname << "Explicit," << be_nl
      << "    CCM_" << lname << "Implicit" << be_nl
      << "{" << be_nl
      << "};";
}

void
be_visitor_home_ex_idl::gen_supported (void)
{
  os_ << be_idt;

  AST_Type **supported = node_->supports ();

  for (long i = 0; i < node_->n_supports (); ++i)
    {
      os_ << "," << be_nl
          << "::"
          << IdentifierHelper::orig_sn (supported[i]->name ()).c_str ();
    }

  os_ << be_uidt;
}

void
be_visitor_home_ex_idl::gen_exception_list (
  UTL_ExceptList *exceptions,
  const char *prefix,
  bool init_op)
{
  ACE_CDR::Long cutoff = (init_op ? 1 : 0);

  if (exceptions != 0 && exceptions->length () > cutoff)
    {
      os_ << be_idt_nl
          << prefix << "raises ( ";

      for (UTL_ExceptlistActiveIterator ei (exceptions);
           !ei.is_done ()
           ;)
        {
          AST_Type *ex = ei.item ();
          ACE_CString ex_name =
            IdentifierHelper::orig_sn (ex->name ());

          if (init_op)
            {
              /// These don't get passed on to the executor IDL.
              bool init_match =
                ex_name == "Components::CreateFailure"
                || ex_name == "Components::FinderFailure";

              if (init_match)
                {
                  ei.next ();
                  continue;
                }
            }

          os_ << "::" << ex_name.c_str ();

          ei.next ();

          if (!ei.is_done ())
            {
              os_ << ", ";
            }
        }

      os_ << ")" << be_uidt;
    }
}

void
be_visitor_home_ex_idl::gen_home_executor (void)
{
  AST_Component *comp = node_->managed_component ();
  AST_Decl *scope = ScopeAsDecl (node_->defined_in ());

  ACE_CString sname_str =
    IdentifierHelper::orig_sn (scope->name ());
  const char *sname = sname_str.c_str ();

  ACE_CString lname_str =
    IdentifierHelper::try_escape (node_->original_local_name ());
  const char *lname = lname_str.c_str ();

  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl_2
      << "module CIAO_" << comp->flat_name () << "_Impl" << be_nl
      << "{" << be_idt_nl
      << "local interface " << lname << "_Exec" << be_idt_nl
      << ": " << global << sname << "::CCM_" << lname << be_uidt_nl
      << "{" << be_nl
      << "};" << be_uidt_nl
      << "};";
}

void
be_visitor_home_ex_idl::restore_scope (void)
{
  for (UTL_ScopeActiveIterator iter (node_, UTL_Scope::IK_decls);
       ! iter.is_done ();
       iter.next ())
    {
      AST_Decl *d = iter.item ();

      d->set_defined_in (node_);

      UTL_ScopedName *nconc_name =
        new UTL_ScopedName (d->local_name ()->copy (),
                            0);

      UTL_ScopedName *new_name =
        dynamic_cast<UTL_ScopedName *> (node_->name ()->copy ());

      new_name->nconc (nconc_name);
      d->set_name (new_name);
    }
}
