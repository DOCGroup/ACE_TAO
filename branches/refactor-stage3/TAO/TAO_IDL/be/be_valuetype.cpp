// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_valuetype.cpp
//
// = DESCRIPTION
//    Extension of class AST_Interface and be_interface that provides
//    additional means for C++ mapping of an valuetype.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_interface.cpp
//
// ============================================================================

#include "be_valuetype.h"
#include "be_visitor.h"
#include "be_extern.h"
#include "be_helper.h"
#include "ast_module.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

ACE_RCSID (be,
           be_valuetype,
           "$Id$")

// Default constructor.
be_valuetype::be_valuetype (void)
  : full_obv_skel_name_ (0)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);

  AST_Module *m = AST_Module::narrow_from_scope (this->defined_in ());

  if (m != 0)
    {
      m->set_has_nested_valuetype ();
    }

  // Always the case.
  this->has_constructor (I_TRUE);
}

// Constructor used to build the AST.
be_valuetype::be_valuetype (UTL_ScopedName *n,
                            AST_Interface **inherits,
                            long n_inherits,
                            AST_ValueType *inherits_concrete,
                            AST_Interface **inherits_flat,
                            long n_inherits_flat,
                            AST_Interface **supports,
                            long n_supports,
                            AST_Interface *supports_concrete,
                            idl_bool abstract,
                            idl_bool truncatable)
  : be_interface (n,
                  inherits,
                  n_inherits,
                  inherits_flat,
                  n_inherits_flat,
                  0,
                  abstract),
    be_type (AST_Decl::NT_valuetype,
             n),
    AST_ValueType (n,
                   inherits,
                   n_inherits,
                   inherits_concrete,
                   inherits_flat,
                   n_inherits_flat,
                   supports,
                   n_supports,
                   supports_concrete,
                   abstract,
                   truncatable),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   0,
                   abstract),
    AST_Decl (AST_Decl::NT_valuetype,
              n),
    UTL_Scope (AST_Decl::NT_valuetype),
    COMMON_Base (0,
                 abstract),
    full_obv_skel_name_ (0),
    supports_abstract_ (0),
    var_out_seq_decls_gen_ (0)
{
  // Check that redefine() copies all members.

  // Always the case.
  this->size_type (AST_Type::VARIABLE);

  AST_Module *m = AST_Module::narrow_from_scope (this->defined_in ());

  if (m != 0)
    {
      m->set_has_nested_valuetype ();
    }

  // Always the case.
  this->has_constructor (I_TRUE);

  for (long i = 0; i < this->pd_n_supports; ++i)
    {
      if (this->pd_supports[i]->is_abstract ())
        {
          this->supports_abstract_ = I_TRUE;
          break;
        }
    }

  // Set the flag that says we have a valuetype in this IDL file.
  if (this->is_defined ())
    {
      ACE_SET_BITS (idl_global->decls_seen_info_,
                    idl_global->decls_seen_masks.valuetype_seen_);
    }
}

be_valuetype::~be_valuetype (void)
{
}

void
be_valuetype::redefine (AST_Interface *from)
{
  be_valuetype *bv = be_valuetype::narrow_from_decl (from);
  bv->var_out_seq_decls_gen_ = bv->var_out_seq_decls_gen_;
  this->AST_ValueType::redefine (from);
}

// Is true if non-virtual accessor and modifier should be generated
// If #pragma TAO OBV opt_accessor (todo) is used or -Wb,obv_opt_accessor.
idl_bool
be_valuetype::opt_accessor (void)
{
  return be_global->obv_opt_accessor ();
}

// Compute stringified fully scoped skeleton name (OBV_name).
void
be_valuetype::compute_fullobvskelname (void)
{
  this->compute_full_skel_name ("OBV_",
                                this->full_obv_skel_name_);
}

// Retrieve the fully scoped skeleton name.
const char*
be_valuetype::full_obv_skel_name (void)
{
  if (!this->full_obv_skel_name_)
    {
      compute_fullobvskelname ();
    }

  return this->full_obv_skel_name_;
}

const char *
be_valuetype::fwd_helper_name (void) const
{
  return this->fwd_helper_name_.fast_rep ();
}

int
be_valuetype::gen_helper_header (char*,
                                 char*)
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_header ();

  *os << be_nl << be_nl
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "TAO_NAMESPACE CORBA" << be_nl
      << "{"
      << be_idt_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void add_ref ("
      << this->full_name () << " *);" << be_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void remove_ref ("
      << this->full_name () << " *);";

  *os <<  be_uidt_nl
      << "}" << be_nl
      << "TAO_NAMESPACE_CLOSE";

  return 0;
}

int
be_valuetype::gen_helper_inline (char*,
                                 char*)
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_inline ();

  // There is a problem, here. Looks like the if defined __ACE_INLINE
  // is not getting generated... Actually this is a much bigger
  // problem. Just hacking  it up for the timebeing..

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "#if defined (__ACE_INLINE__)" << be_nl << be_nl
      << "TAO_NAMESPACE CORBA" << be_nl
      << "{"
      << be_idt_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void add_ref ("
      << this->full_name () << " *);" << be_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void remove_ref ("
      << this->full_name () << " *);"
      <<  be_uidt_nl
      << "}" << be_nl
      << "TAO_NAMESPACE_CLOSE" << be_nl << be_nl 
      << "#endif /*__ACE_INLINE__*/";

  return 0;
}


int
be_valuetype::gen_helper_stubs (char* ,
                                char* )
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_stubs ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "void" << be_nl
      << "CORBA::add_ref (" << this->full_name () << " * vt)" << be_nl
      << "{" << be_idt_nl
      << "if (vt != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "vt->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "void" << be_nl
      << "CORBA::remove_ref (" << this->full_name () << " * vt)" << be_nl
      << "{" << be_idt_nl
      << "if (vt != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "vt->_remove_ref ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  return 0;
}

// Generate the forward declarations and static methods used by the
// interface _var and _out template classes, as well as by the
// template sequence classes for object references.
void
be_valuetype:: gen_var_out_seq_decls (void)
{
  if (this->var_out_seq_decls_gen_ == 1)
    {
      return;
    }

  TAO_OutStream *os = tao_cg->client_header ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the ifdefined macro for this interface.
  os->gen_ifdef_macro (this->flat_name (),
                       "var_out");

  const char *lname = this->local_name ();

  *os << be_nl << be_nl
      << "class " << lname << ";" << be_nl
      << "struct tao_" << lname << "_life;" << be_nl << be_nl
      << "typedef" << be_idt_nl
      << "TAO_Value_Var_T<" << be_idt << be_idt_nl
      << lname << "," << be_nl
      << "tao_" << lname << "_life" << be_uidt_nl
      << ">" << be_uidt_nl
      << lname << "_var;" << be_uidt_nl << be_nl
      << "typedef" << be_idt_nl
      << "TAO_Value_Out_T<" << be_idt << be_idt_nl
      << lname << "," << be_nl
      << "tao_" << lname << "_life" << be_uidt_nl
      << ">" << be_uidt_nl 
      << lname << "_out;" << be_uidt;

  *os << be_nl << be_nl
      << "struct " << be_global->stub_export_macro () 
      << " tao_" << lname << "_life" << be_nl
      << "{" << be_idt_nl
      << "static void tao_add_ref (" 
      << lname << " *);" << be_nl
      << "static void tao_remove_ref (" << lname << " *);" << be_uidt_nl
      << "};";

  os->gen_endif ();

  this->var_out_seq_decls_gen_ = 1;
}

// For building the pre and postfix of private data fields.
const char *
be_valuetype::field_pd_prefix (void)
{
  return "_pd_";
}

const char *
be_valuetype::field_pd_postfix (void)
{
  return "";
}

be_valuetype *
be_valuetype::statefull_inherit (void)
{
  if (this->pd_inherits_concrete != 0)
    {
      return be_valuetype::narrow_from_decl (this->pd_inherits_concrete);
    }
  else
    {
      return 0;
    }
}

// Accept a visitor.
int
be_valuetype::accept (be_visitor *visitor)
{
  return visitor->visit_valuetype (this);
}

void
be_valuetype::destroy (void)
{
  this->be_interface::destroy ();
}

ACE_CDR::ULong
be_valuetype::data_members_count (AST_Field::Visibility vis)
{
  ACE_CDR::ULong count = 0;

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::data_members_count - "
                             "bad node in this scope\n"), 0);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      if (vis != AST_Field::vis_NA)
        {
          if (vis == field->visibility ()) ++count;
        }
      else
        {
          ++count;
        }
    } // end of for loop
  return count;
}

idl_bool
be_valuetype::in_recursion (AST_Type *node)
{
  if (node == 0)
    {
      node = this;
    }

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::in_recursion - "
                             "bad node in this scope\n"),
                            0);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      AST_Type *type = field->field_type ();

      // A valuetype may contain itself as a member. This will not
      // cause a problem when checking if the valuetype itself is
      // recursive, but if another valuetype contains a recursive
      // one, the string compare below is not sufficient, and we
      // will go into an infinite recursion of calls to in_recursion ;-).
      // The check below will catch that use case.
      if (this == type)
        {
          return 1;
        }

      if (!type)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::in_recursion - "
                             "bad base type\n"),
                            0);
        }

      // IDL doesn't have such a feature as name reuse so
      // just compare fully qualified names.
      if (!ACE_OS::strcmp (node->full_name (),
                           type->full_name ()))
        {
          return 1;
        }

      // Now hand over to our field type.
      if (type->in_recursion (node))
        {
          return 1;
        }

    } // end of for loop

  return 0;
}

idl_bool
be_valuetype::supports_abstract (void) const
{
  return this->supports_abstract_;
}

// Template method that traverses the inheritance graph in a breadth-first
// style. The actual work on each element in the inheritance graph is carried
// out by the function passed as argument.
int
be_valuetype::traverse_supports_list_graphs (
    be_interface::tao_code_emitter gen,
    TAO_OutStream *os,
    idl_bool abstract_paths_only
  )
{
  long n_supports = this->n_supports ();

  if (n_supports == 0)
    {
      return 0;
    }

  // Make sure the queues are empty.
  this->insert_queue.reset ();
  this->del_queue.reset ();

  be_interface *supported_interface = 0;

  for (long i = 0; i < n_supports; ++i)
    {
      if (abstract_paths_only
          && ! this->pd_supports[i]->is_abstract ())
        {
          continue;
        }

      supported_interface =
        be_interface::narrow_from_decl (this->pd_supports[i]);

      // Insert a supported interface in the queue.
      if (this->insert_queue.enqueue_tail (supported_interface) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::"
                             "traverse_supports_graph_list - "
                             "error generating entries\n"),
                            -1);
        }
    }

  be_code_emitter_wrapper wrapper (gen);

  return this->traverse_inheritance_graph (wrapper,
                                           os,
                                           abstract_paths_only);
}

int
be_valuetype::traverse_concrete_inheritance_graph (tao_code_emitter gen,
                                                   TAO_OutStream *os)
{
  AST_Interface *supported = this->supports_concrete ();

  if (supported == 0)
    {
      return 0;
    }

  be_interface *concrete = be_interface::narrow_from_decl (supported);

  // Make sure the queues are empty.
  this->insert_queue.reset ();
  this->del_queue.reset ();

  // Insert the supported concrete interface in the queue.
  if (this->insert_queue.enqueue_tail (concrete) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_valuetype::"
                         "traverse_concrete_inheritance_graph - "
                         "error generating entries\n"),
                        -1);
    }

  be_code_emitter_wrapper wrapper (gen);

  return this->traverse_inheritance_graph (wrapper,
                                           os);
}

int
be_valuetype::abstract_supports_helper (be_interface *,
                                        be_interface *base,
                                        TAO_OutStream *os)
{
  *os << "," << be_nl
      << "public virtual "
      << base->name ();
/*
  if (base->is_nested ())
    {
      UTL_Scope *parent_scope = base->defined_in ();
      AST_Decl *parent_decl = ScopeAsDecl (parent_scope);

      *os << "ACE_NESTED_CLASS ("
          << parent_decl->name () << ", "
          << base->local_name () << ")";
    }
  else
    {
      *os << base->local_name ();
    }
*/
  return 0;
}

int
be_valuetype::gen_abstract_init_helper (be_interface *,
                                        be_interface *,
                                        TAO_OutStream *)
{
  return 0;
}

int
be_valuetype::gen_skel_helper (be_interface *concrete,
                               be_interface *ancestor,
                               TAO_OutStream *os)
{
  // If derived and ancestor are same, skip it.
  if (concrete == ancestor)
    {
      return 0;
    }

  // If an operation or an attribute is abstract (declared in an
  // abstract interface), we will either generate the full
  // definition (if there are no concrete interfaces between the
  // abstract ancestor and us) or, if there is a concrete ancestor
  // in between, we will catch its definition elsewhere in this
  // traversal.
  if (ancestor->is_abstract ())
    {
      return 0;
    }

  // Else generate code that does the cast to the appropriate type.

  if (ancestor->nmembers () > 0)
    {
      // If there are elements in ancestor scope i.e., any operations and
      // attributes defined by "ancestor", become methods on the derived class
      // which call the corresponding method of the base class by doing the
      // proper casting.

      for (UTL_ScopeActiveIterator si (ancestor, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // Get the next AST decl node
          AST_Decl *d = si.item ();

          if (d->node_type () == AST_Decl::NT_op)
            {
              // Start from current indentation level.
              os->indent ();

              *os << "// TAO_IDL - Generated from" << be_nl
                  << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // Generate the static method corresponding to this method.
                  *os << "static void " << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req, " << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
                      << ");" << be_uidt << "\n\n";
                }
              else
                { // Generate code in the inline file.
                  // Generate the static method corresponding to this method.
                  *os << "ACE_INLINE void "
                      << concrete->full_skel_name () << "::"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL" << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl;
                  *os << ancestor->full_skel_name ()
                      << "_ptr impl = ("
                      << concrete->full_skel_name ()
                      << "_ptr) obj;" << be_nl;
                  *os << ancestor->full_skel_name ()
                      << "::" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "(" << ancestor->full_skel_name ()
                      << "_ptr) impl," << be_nl
                      << "context" << be_nl
                      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
                      << ");" << be_uidt << be_uidt_nl
                      << "}\n";
                }
            }
        } // End of scope iterator.
    }

  return 0;
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_valuetype, be_interface, AST_ValueType)
IMPL_NARROW_FROM_DECL (be_valuetype)
IMPL_NARROW_FROM_SCOPE (be_valuetype)
