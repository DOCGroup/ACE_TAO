
//=============================================================================
/**
 *  @file    typecode_defn.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TypeCode definitions for types.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "typecode.h"
#include "be_interface_fwd.h"

// This is an implementation of C++ "scoped lock" idiom in order to
// avoid repetitive code.
class Scoped_Compute_Queue_Guard
{
public:
  Scoped_Compute_Queue_Guard (be_visitor_typecode_defn* );
  ~Scoped_Compute_Queue_Guard (void);

private:
  be_visitor_typecode_defn* customer_;
};

Scoped_Compute_Queue_Guard::Scoped_Compute_Queue_Guard (
    be_visitor_typecode_defn* customer
  )
  :customer_ (customer)
{
  if (customer_ != 0)
    {
      // Reset the compute queue to set the stage for computing our
      // encapsulation length.
      customer_->queue_reset (customer_->compute_queue_);
    }
}

Scoped_Compute_Queue_Guard::~Scoped_Compute_Queue_Guard (void)
{
  if (customer_ != 0)
    {
      // Reset the compute queue since we must not affect computation of other
      // nodes.
      customer_->queue_reset (customer_->compute_queue_);
    }
}

// ******************************************************
// TypeCode Definitions
// ******************************************************

be_visitor_typecode_defn::be_visitor_typecode_defn (be_visitor_context * ctx)
    : be_visitor_scope (ctx),
      recursion_detect_ (false),
      computed_encap_len_ (0),
      computed_scope_encap_len_ (0),
      index_ (0)
{
  if (be_global->gen_anyop_files ())
    {
      // The context is always a copy, so this is ok.
      this->ctx_->stream (tao_cg->anyop_source ());
    }

  for (unsigned long i = 0UL; i < TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE; ++i)
    {
      scope_stack_[i] = 0;
    }
}

be_visitor_typecode_defn::~be_visitor_typecode_defn (void)
{
  this->queue_reset (this->tc_queue_);
  this->queue_reset (this->compute_queue_);
}

int
be_visitor_typecode_defn::gen_typecode_ptr (be_type * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  if (node->node_type () == AST_Decl::NT_string
      || node->node_type () == AST_Decl::NT_wstring
      || node->node_type () == AST_Decl::NT_sequence)
    {
      // Don't bother generating a TypeCode_ptr for these types.  They
      // are accessible only through an alias TypeCode.

      return 0;
    }

  // Is our enclosing scope a module? We need this check because for
  // platforms that support namespaces, the typecode must be declared
  // extern.
  if (node->is_nested () &&
      node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      be_module * const module =
        be_module::narrow_from_scope (node->defined_in ());

      if (module == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_typecode_defn::")
                             ACE_TEXT ("gen_typecode_ptr - ")
                             ACE_TEXT ("Error parsing nested name\n")),
                            -1);
        }

      be_util::gen_nested_namespace_begin (&os, module);

      os << "::CORBA::TypeCode_ptr const _tc_"
         << node->local_name ()
         << " =" << be_idt_nl
         << "&_tao_tc_"
         << node->flat_name () << ";"
         << be_uidt;

      be_util::gen_nested_namespace_end (&os, module);
    }
  else
    {
      // outermost scope.
      os << "::CORBA::TypeCode_ptr const ";

      // Tc name generation.
      os << node->tc_name ();

      os << " =" << be_idt_nl
         << "&_tao_tc_";

      // Flat name generation.
      os <<  node->flat_name ();

      os << ";" << be_uidt;
    }

  return 0;
}


int
be_visitor_typecode_defn::gen_base_typecode_name (be_type * base)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  if (base->is_nested ()
      && base->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      if (base->node_type () != AST_Decl::NT_string
          && base->node_type () != AST_Decl::NT_wstring
          && base->node_type () != AST_Decl::NT_sequence)
        {
          // Generate scope names only if types other than the ones
          // listed above since the corresponding TypeCodes are at the
          // file scope.
          os << base->tc_name ();
        }
      else
        {
          // Internally used TypeCodes.
          os << base->tc_name ();
        }
    }
  else
    {
      os << base->tc_name ();
    }

  return 0;
}

int
be_visitor_typecode_defn::gen_forward_declared_typecode (be_type * node)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  // Generate an extern TypeCode declaration to make sure TypeCodes
  // that refer to the corresponding TypeCode for the forward declared
  // interface have a valid reference to it.

  // Is our enclosing scope a module? We need this check because for
  // platforms that support namespaces, the typecode must be declared
  // extern.
  if (node->is_nested () &&
      node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      be_module * const module =
        be_module::narrow_from_scope (node->defined_in ());

      if (module == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_typecode_defn::")
                             ACE_TEXT ("gen_forward_declared_typecode - ")
                             ACE_TEXT ("Error parsing nested name\n")),
                            -1);
        }

      be_util::gen_nested_namespace_begin (&os, module);

      os << "extern ::CORBA::TypeCode_ptr const _tc_"
         << node->local_name () << ";";

      be_util::gen_nested_namespace_end (&os, module);
    }
  else
    {
      // outermost scope.
      os << "extern ::CORBA::TypeCode_ptr const "
         << node->tc_name () << ";" << be_uidt;
    }

  os << be_nl;

  return 0;
}

bool
be_visitor_typecode_defn::is_typecode_generation_required (be_type * node)
{
  AST_Decl::NodeType const nt = node->node_type ();

  if (nt == AST_Decl::NT_interface
      || nt == AST_Decl::NT_interface_fwd
      || nt == AST_Decl::NT_valuetype
      || nt == AST_Decl::NT_valuetype_fwd
      || nt == AST_Decl::NT_eventtype
      || nt == AST_Decl::NT_eventtype_fwd)
    {
      // Since all fwd decl 'full definition' members aren't updated when
      // the full definition is seen, we need to make sure we have the
      // full definition node (if it exists) on which to call is_defined().
      // The last 'true' arg below filters out full-def members of fwd decls.
      AST_Decl *d = node->defined_in ()->lookup_by_name (node->name (),
                                                         true);

      be_interface * const intf =
        d != 0 ? be_interface::narrow_from_decl (d) : 0;

      if (intf && intf->is_defined ())
        {
          return false;
        }
    }
  else if (nt == AST_Decl::NT_struct
           || nt == AST_Decl::NT_union)
    {
      // structures and unions

      AST_Structure * const st =
        AST_Structure::narrow_from_decl (node);

      if (st && st->is_defined ())
        {
          return false;
        }
    }

  return true;
}

void
be_visitor_typecode_defn::gen_begin_NS_for_anon (void)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << "namespace TAO" << be_nl
     << "{" << be_idt_nl
     << "namespace TypeCode" << be_nl
     << "{" << be_idt_nl
     << "namespace" << be_nl
     << "{" << be_idt_nl;
}

void
be_visitor_typecode_defn::gen_end_NS_for_anon (void)
{
  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_uidt_nl
     << "}" << be_uidt_nl
     << "}" << be_uidt_nl
     << "}" << be_nl;
}

int
be_visitor_typecode_defn::visit_array (be_array *node)
{
  be_type * const base = be_type::narrow_from_decl (node->base_type ());

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl_2
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // generate typecode for the base type
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);

  // Generate typecode for the base type, being careful to avoid doing
  // so for a typedef since that could recursively cause multiple base
  // type TypeCode definitions to be generated.
  if (!base || (base->node_type () != AST_Decl::NT_typedef
                && base->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::visit_array) - ")
                         ACE_TEXT ("failed to generate base typecode\n")),
                        -1);
    }

  if (this->recursion_detect_)
    {
      return 0;  // Nothing else to do.
    }

  // Multiple definition guards.
  // @todo Can we automate duplicate detection within the IDL compiler
  //       itself?
  os << "\n#ifndef _TAO_TYPECODE_" << node->flat_name () << "_GUARD"
     << "\n#define _TAO_TYPECODE_" << node->flat_name () << "_GUARD"
     << be_nl;

  os << be_global->core_versioning_begin () << be_nl;

  // Namespace begin.
  this->gen_begin_NS_for_anon ();

  ACE_CDR::ULong const ndims = node->n_dims ();

  for (ACE_CDR::ULong i = 0; i < ndims; ++i)
    {
      os << "TAO::TypeCode::Sequence< ::CORBA::TypeCode_ptr const *," << be_nl
         << "                        TAO::Null_RefCount_Policy>"
         << be_idt_nl
         << node->flat_name ();

      ACE_CDR::ULong const inner_ndims = ndims - i - 1;

      for (ACE_CDR::ULong j = inner_ndims; j < ndims; ++j)
        {
          os << "_" << node->dims ()[j]->ev ()->u.ulval;
        }

      os << " (" << be_idt_nl
         << "::CORBA::tk_array," << be_nl
         << "&";

      if (i == 0)
        {
          if (this->gen_base_typecode_name (base) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_typecode_defn::")
                                 ACE_TEXT ("visit_array - problem ")
                                 ACE_TEXT ("generating base ")
                                 ACE_TEXT ("TypeCode name.\n")),
                                -1);
            }
        }
      else
        {
          os << "tc_" << node->flat_name ();

          for (unsigned long j = inner_ndims + 1; j < ndims; ++j)
            {
              os << "_" <<  node->dims ()[j]->ev ()->u.ulval;
            }

        }

      os << "," << be_nl
         << node->dims ()[inner_ndims] << ");" << be_uidt_nl
         << be_uidt_nl;

      os << "::CORBA::TypeCode_ptr const tc_"
         << node->flat_name ();

      if (inner_ndims != 0)
        {
          // Only generate the array dimension sizes for the inner
          // dimensions, not the outermost (leftmost) one.
          for (unsigned long k = inner_ndims; k < ndims; ++k)
            {
              os << "_" << node->dims ()[k]->ev ()->u.ulval;
            }
        }

      os << " ="
         << be_idt_nl
         << "&" << node->flat_name ();

      for (unsigned long n = inner_ndims; n < ndims; ++n)
        {
          os << "_" << node->dims ()[n]->ev ()->u.ulval;
        }

      os << ";" << be_uidt;

      if (i < ndims - 1)
        {
          os << be_nl_2;
        }
    }

  // Namespace end,
  this->gen_end_NS_for_anon ();

  os << be_global->core_versioning_end ();

  os << "\n#endif /* _TAO_TYPECODE_" << node->flat_name () << "_GUARD */"
     << be_nl;

  return 0;
}

int
be_visitor_typecode_defn::visit_interface (be_interface * node)
{
  // Only handle forward declared interfaces here.  Defined interfaces
  // have their own TypeCode visitor.
  if (node->is_defined ())
    {
      return 0;
    }

  return this->gen_forward_declared_typecode (node);
}

int
be_visitor_typecode_defn::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_typecode_defn::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

int
be_visitor_typecode_defn::visit_interface_fwd (be_interface_fwd * node)
{
  return this->gen_forward_declared_typecode (node);
}

int
be_visitor_typecode_defn::visit_sequence (be_sequence * node)
{
  be_type * const base = be_type::narrow_from_decl (node->base_type ());

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl_2
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // generate typecode for the base type
  this->ctx_->sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE_NESTED);

  // Generate typecode for the base type, being careful to avoid doing
  // so for a typedef since that could recursively cause multiple base
  // type TypeCode definitions to be generated.
  if (!base || (base->node_type () != AST_Decl::NT_typedef
                && base->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::visit_sequence) - ")
                         ACE_TEXT ("failed to generate base typecode\n")),
                        -1);
    }

  if (this->recursion_detect_)
    {
      return 0;  // Nothing else to do.
    }

  // Multiple definition guards.
  // @todo Can we automate duplicate detection within the IDL compiler
  //       itself?
  os << "\n#ifndef _TAO_TYPECODE_" << node->flat_name () << "_GUARD"
     << "\n#define _TAO_TYPECODE_" << node->flat_name () << "_GUARD"
     << be_nl;

  // Namespace begin.

  os << be_global->core_versioning_begin () << be_nl;

  this->gen_begin_NS_for_anon ();

  os << "TAO::TypeCode::Sequence< ::CORBA::TypeCode_ptr const *," << be_nl
     << "                        TAO::Null_RefCount_Policy>"
     << be_idt_nl
     << node->flat_name () << "_" << node->max_size()->ev ()->u.ulval

     << " (" << be_idt_nl
     << "::CORBA::tk_sequence," << be_nl
     << "&";

  if (this->gen_base_typecode_name (base) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("problem generating base ")
                         ACE_TEXT ("TypeCode name.\n")),
                        -1);
    }

  os << "," << be_nl
     << node->max_size () << ");" << be_uidt_nl
     << be_uidt_nl;

  os << "::CORBA::TypeCode_ptr const tc_"
     << node->flat_name () << "_" << node->max_size()->ev ()->u.ulval << " ="
     << be_idt_nl
     << "&" << node->flat_name () << "_" << node->max_size()->ev ()->u.ulval
     << ";" << be_uidt;

  // Namespace end.
  this->gen_end_NS_for_anon ();

  os << be_global->core_versioning_end ();

  os << "\n#endif /* _TAO_TYPECODE_" << node->flat_name () << "_GUARD */"
     << be_nl_2;

  return 0; // this->gen_typecode_ptr (node);
}

int
be_visitor_typecode_defn::visit_string (be_string * node)
{
  if (node->max_size ()->ev()->u.ulval == 0)
    {
      // No need to generate a TypeCode for unbounded strings.  Just
      // use the {w}string TypeCode constant.
      return 0;
    }

  if (this->recursion_detect_)
    {
      return 0;  // Nothing else to do.
    }

  TAO_OutStream & os = *this->ctx_->stream ();

  os << be_nl_2
     << "// TAO_IDL - Generated from" << be_nl
     << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Multiple definition guards.
  // @todo Can we automate duplicate detection within the IDL compiler
  //       itself?
  os << "\n#ifndef _TAO_TYPECODE_" << node->flat_name () << "_GUARD"
     << "\n#define _TAO_TYPECODE_" << node->flat_name () << "_GUARD" << be_nl;

  // Namespace begin.

  os << be_global->core_versioning_begin () << be_nl;

  this->gen_begin_NS_for_anon ();

  // Generate the TypeCode instantiation.
  os
    << "TAO::TypeCode::String<TAO::Null_RefCount_Policy>"
    << be_idt_nl
    << node->flat_name () << " (" << be_idt_nl
    << "::CORBA::tk_" << (node->width () == 1 ? "string" : "wstring") << ","
    << be_nl
    << node->max_size () << ");" << be_uidt_nl
    << be_uidt_nl;

  os << "::CORBA::TypeCode_ptr const tc_"
     << node->flat_name () << " ="
     << be_idt_nl
     << "&" << node->flat_name () << ";" << be_uidt;

  // Namespace end.
  this->gen_end_NS_for_anon ();

  os << be_global->core_versioning_end ();

  os << "\n#endif /* _TAO_TYPECODE_" << node->flat_name () << "_GUARD */"
     << be_nl_2;

  return 0;
}

int
be_visitor_typecode_defn::visit_structure (be_structure * node)
{
  // Handle only forward declared structures here.  Defined structures
  // have their own TypeCode visitor.
  if (node->is_defined ())
    {
      return 0;
    }

  return this->gen_forward_declared_typecode (node);
}

int
be_visitor_typecode_defn::visit_typedef (be_typedef * node)
{
  // Only used for recursion detection.

  // The only base types with no-op visitors that will be potentially
  // visited are strings and sequences.  All others have their own
  // full-fledged visitors (e.g. objref_typecode, etc.)

  be_type * const base = be_type::narrow_from_decl (node->base_type ());

  this->recursion_detect_ = true;

  // Generate typecode for the base type, being careful to avoid doing
  // so a for a typedef since that could recursively cause multiple
  // base type TypeCode definitions to be generated.
  if (!base || (base->accept (this) == -1))
    {
      this->recursion_detect_ = false;

      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::visit_typedef) - ")
                         ACE_TEXT ("failed to visit base typecode\n")),
                        -1);
    }

  this->recursion_detect_ = false;

  return 0;
}

int
be_visitor_typecode_defn::visit_union (be_union *node)
{
  // Only handle forward declared unions here.  Defined unions
  // have their own TypeCode visitor.
  if (node->is_defined ())
    {
      return 0;
    }

  return this->gen_forward_declared_typecode (node);
}

int
be_visitor_typecode_defn::visit_valuetype (be_valuetype * node)
{
  // Only handle forward declared valuetypes here.  Defined valuetypes
  // have their own TypeCode visitor.
  if (node->is_defined ())
    {
      return 0;
    }

  return this->gen_forward_declared_typecode (node);
}

int
be_visitor_typecode_defn::visit_eventtype (be_eventtype * node)
{
  return this->visit_valuetype (node);
}

// post processing
int
be_visitor_typecode_defn::post_process (be_decl *)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_TC_DEFN_SCOPE_LEN)
    {
      this->computed_scope_encap_len_ += this->computed_encap_len_;
    }

  return 0;
}

// scope stack routines
int
be_visitor_typecode_defn::push (ACE_CDR::Long val)
{
  if (this->index_ >= TAO_BE_VISITOR_TYPECODE_DEFN_MAX_STACK_SIZE)
    {
      return -1;
    }

  this->scope_stack_ [this->index_++] = val;  // Must post-increment
                                              // to avoid buffer
                                              // overrun.
  return 0;
}

int
be_visitor_typecode_defn::pop (ACE_CDR::Long &val)
{
  val = this->scope_stack_[this->index_--];  // Must post-increment!
  return 0;
}

const be_visitor_typecode_defn::QNode *
be_visitor_typecode_defn::queue_insert (
    ACE_Unbounded_Queue <be_visitor_typecode_defn::QNode *> &queue,
    be_type *node,
    ACE_CDR::Long offset
  )
{
  be_visitor_typecode_defn::QNode *qnode = 0;

  ACE_NEW_RETURN (qnode, be_visitor_typecode_defn::QNode, 0);

  qnode->node = node;
  qnode->offset = offset;

  if (queue.enqueue_tail (qnode) == -1)
    {
      delete qnode;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_typecode_defn")
                         ACE_TEXT ("::queue_insert - failed\n")),
                        0);
    }

  return qnode;
}

const be_visitor_typecode_defn::QNode *
be_visitor_typecode_defn::queue_lookup (
    ACE_Unbounded_Queue <be_visitor_typecode_defn::QNode *> &queue,
    be_type *node
  )
{
  for (ACE_Unbounded_Queue_Iterator<be_visitor_typecode_defn::QNode *>
         iter (queue);
       !iter.done ();
       iter.advance ())
    {
      be_visitor_typecode_defn::QNode **addr = 0;
      be_visitor_typecode_defn::QNode *item = 0;
      iter.next (addr);
      item = *addr;

      if (!ACE_OS::strcmp (item->node->full_name (),
                           node->full_name ()))
        {
          // Found.
          return item;
        }
    }

  return 0;
}

void
be_visitor_typecode_defn::
queue_reset (ACE_Unbounded_Queue <be_visitor_typecode_defn::QNode *> & queue)
{
  while (!queue.is_empty ())
    {
      be_visitor_typecode_defn::QNode * qnode = 0;
      (void) queue.dequeue_head (qnode);
      delete qnode;
    }
}
