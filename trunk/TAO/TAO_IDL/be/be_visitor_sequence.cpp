// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_sequence.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Sequence
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_sequence.h"

// Root visitor for client header
be_visitor_sequence_ch::be_visitor_sequence_ch (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->client_header ();
}

be_visitor_sequence_ch::~be_visitor_sequence_ch (void)
{
}

int be_visitor_sequence_ch::visit_sequence (be_sequence *node)
{
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream &os = this->stream ();

  // generate the ifdefined macro for the sequence type
  os.gen_ifdef_macro (node->flatname ());

  os.indent (); // start with the current indentation level

      // retrieve the base type since we may need to do some code
      // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence.cpp - "
                         "Bad base type\n"),
                        -1);
    }

  os << "// *************************************************************"
      << nl;
  os << "// " << node->local_name () << nl;
  os << "// *************************************************************"
      << nl << nl;

  // generate a typedef to a parametrized sequence
  os << "typedef ";
  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF: // sequence of objrefs
      if (node->unbounded ())
        os << "TAO_Unbounded_ObjectSequence <";
      else
        os << "TAO_Bounded_ObjectSequence <";
      break;
    case be_sequence::MNG_STRING: // sequence of objrefs
      if (node->unbounded ())
        os << "TAO_Unbounded_StringSequence ";
      else
        os << "TAO_Bounded_StringSequence <";
      break;
    default: // not a managed type
      if (node->unbounded ())
        os << "TAO_Unbounded_Sequence <";
      else
        os << "TAO_Bounded_Sequence <";
    }

  be_visitor *visitor_sequence_base_ch = cg->make_visitor
    (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  visitor_sequence_base_ch->be_node (node); // save the sequence node
  if (bt->accept (visitor_sequence_base_ch) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence.cpp - "
                         "base type visit failed\n"),
                        -1);
    }

  if (node->unbounded ())
    {
      if (node->managed_type () != be_sequence::MNG_STRING)
        os << " >";
    }
  else
    {
      if (node->managed_type () != be_sequence::MNG_STRING)
        os << ", ";
      os << node->max_size () << " >";
    }

  os << " " << node->local_name () << ";" << nl;

      // Generate the typecode decl
  if (node->is_nested ())
    {
      // we have a scoped name
      os.indent ();
      os << "static CORBA::TypeCode_ptr " << node->tc_name
        ()->last_component () << ";\n\n";
    }
  else
    {
      // we are in the ROOT scope
      os.indent ();
      os << "extern CORBA::TypeCode_ptr " << node->tc_name
        ()->last_component () << ";\n\n";
    }


  os.gen_endif (); // endif macro

  // generate the ifdefined macro for the var type
  os.gen_ifdef_macro (node->flatname (), "_var");

  // generate the var and out types
  if (node->gen_var_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_client_header - "
                         "codegen for _var failed\n"),
                        -1);
    }

  os.gen_endif ();

      // generate the ifdefined macro for the var type
  os.gen_ifdef_macro (node->flatname (), "_out");

  if (node->gen_out_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_client_header - "
                         "codegen for _out failed\n"),
                        -1);
    }
  os.gen_endif ();

  return 0;
}

// sequence base visitor for client header
be_visitor_sequence_base_ch::be_visitor_sequence_base_ch (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->client_header ();
}

be_visitor_sequence_base_ch::~be_visitor_sequence_base_ch (void)
{
}

be_decl *
be_visitor_sequence_base_ch::seq_scope (void)
{
  be_decl *scope = 0;
  if (this->node_->is_nested ())
    scope = be_scope::narrow_from_scope (this->node_->defined_in ())->decl ();
  return scope;
}

int
be_visitor_sequence_base_ch::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream &os = this->stream ();
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      os << "CORBA::Object, CORBA::Object_ptr";
    default:
      os << node->name ();
    }
  return 0;
}

int
be_visitor_sequence_base_ch::visit_sequence (be_sequence *node)
{
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  TAO_OutStream &os = this->stream ();

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_base_ch - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF: // sequence of objrefs
      if (node->unbounded ())
        os << "TAO_Unbounded_ObjectSequence <";
      else
        os << "TAO_Bounded_ObjectSequence <";
      break;
    case be_sequence::MNG_STRING: // sequence of objrefs
      if (node->unbounded ())
        os << "TAO_Unbounded_StringSequence ";
      else
        os << "TAO_Bounded_StringSequence <";
      break;
    default: // not a managed type
      if (node->unbounded ())
        os << "TAO_Unbounded_Sequence <";
      else
        os << "TAO_Bounded_Sequence <";
    }

  be_visitor *visitor_sequence_base_ch = cg->make_visitor
    (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  visitor_sequence_base_ch->be_node (node); // save the sequence node
  if (bt->accept (visitor_sequence_base_ch) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_base_ch - "
                         "base type visit failed\n"),
                        -1);
    }

  if (node->unbounded ())
    {
      if (node->managed_type () != be_sequence::MNG_STRING)
        os << " >";
    }
  else
    {
      if (node->managed_type () != be_sequence::MNG_STRING)
        os << ", ";
      os << node->max_size () << " >";
    }

}

// helper
int
be_visitor_sequence_base_ch::visit_node (be_type *node)
{
  TAO_OutStream &os = this->stream ();
  os << node->nested_type_name (this->seq_scope ());
  return 0;
}

int
be_visitor_sequence_base_ch::visit_interface (be_interface *node)
{
  TAO_OutStream &os = this->stream ();
  os << node->nested_type_name (this->seq_scope ());
  os << ", ";
  os << node->nested_type_name (this->seq_scope (), "_ptr");
  return 0;
}

int
be_visitor_sequence_base_ch::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream &os = this->stream ();
  os << node->nested_type_name (this->seq_scope ());
  os << ", ";
  os << node->nested_type_name (this->seq_scope (), "_ptr");
  return 0;
}

int
be_visitor_sequence_base_ch::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base_ch::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base_ch::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base_ch::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_base_ch::visit_typedef (be_typedef *node)
{
  return this->visit_node (node);
}

// ***********************************************************
// sequence element type visitor for return typs of [] methods
// ***********************************************************

be_visitor_sequence_elemtype::be_visitor_sequence_elemtype (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->client_header (); // default
}

be_visitor_sequence_elemtype::~be_visitor_sequence_elemtype (void)
{
}

be_decl *
be_visitor_sequence_elemtype::seq_scope (void)
{
  be_decl *scope = 0;
  if (this->node_->is_nested ())
    scope = be_scope::narrow_from_scope (this->node_->defined_in ())->decl ();
  return scope;
}

int
be_visitor_sequence_elemtype::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream &os = this->stream ();
  be_sequence *seq = be_sequence::narrow_from_decl (this->be_node ());
  if (!seq)
    return -1;

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      if (seq->unbounded ())
        os << "TAO_Unbounded_ObjectSequence<CORBA::Object, " <<
          "CORBA::Object_ptr>::TAO_ObjRefMngType ";
    default:
      os << node->name () << " &";
    }
  return 0;
}

int
be_visitor_sequence_elemtype::visit_sequence (be_sequence *node)
{
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  TAO_OutStream &os = this->stream ();

  // retrieve the base type since we may need to do some code
  // generation for the base type.
  bt = be_type::narrow_from_decl (node->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_elemtype - "
                         "Bad base type\n"),
                        -1);
    }

  // generate the appropriate sequence type
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF: // sequence of objrefs
      if (node->unbounded ())
        os << "TAO_Unbounded_ObjectSequence <";
      else
        os << "TAO_Bounded_ObjectSequence <";
      break;
    case be_sequence::MNG_STRING: // sequence of objrefs
      if (node->unbounded ())
        os << "TAO_Unbounded_StringSequence ";
      else
        os << "TAO_Bounded_StringSequence <";
      break;
    default: // not a managed type
      if (node->unbounded ())
        os << "TAO_Unbounded_Sequence <";
      else
        os << "TAO_Bounded_Sequence <";
    }

  be_visitor *visitor_sequence_base_ch = cg->make_visitor
    (TAO_CodeGen::TAO_SEQUENCE_BASE_CH);
  visitor_sequence_base_ch->be_node (node); // save the sequence node
  if (bt->accept (visitor_sequence_base_ch) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_elemtype - "
                         "base type visit failed\n"),
                        -1);
    }

  if (node->unbounded ())
    {
      if (node->managed_type () != be_sequence::MNG_STRING)
        os << " > &";
    }
  else
    {
      if (node->managed_type () != be_sequence::MNG_STRING)
        os << ", ";
      os << node->max_size () << " > &";
    }

}

// helper
int
be_visitor_sequence_elemtype::visit_node (be_type *node)
{
  TAO_OutStream &os = this->stream ();

  if (os.stream_type () == TAO_OutStream::TAO_CLI_HDR)
    os << node->nested_type_name (this->seq_scope ()) << " &";
  else
    os << node->name () << " &";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface (be_interface *node)
{
  TAO_OutStream &os = this->stream ();
  be_sequence *seq = be_sequence::narrow_from_decl (this->be_node ());
  if (!seq)
    return -1;

  if (seq->unbounded ())
    os << "TAO_Unbounded_ObjectSequence <";
  else
    os << "TAO_Bounded_ObjectSequence <";
  if (os.stream_type () == TAO_OutStream::TAO_CLI_HDR)
    {
      os << node->nested_type_name (this->seq_scope ());
      os << ", ";
      os << node->nested_type_name (this->seq_scope (), "_ptr");
    }
  else
    {
      os << node->name () << ", " << node->name () << "_ptr";
    }

  if (!seq->unbounded ())
    {
      os << ", " << seq->max_size ();
    }
  os << ">::TAO_ObjRefMngType ";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream &os = this->stream ();
  be_sequence *seq = be_sequence::narrow_from_decl (this->be_node ());
  if (!seq)
    return -1;

  if (seq->unbounded ())
    os << "TAO_Unbounded_ObjectSequence <";
  else
    os << "TAO_Bounded_ObjectSequence <";
  if (os.stream_type () == TAO_OutStream::TAO_CLI_HDR)
    {
      os << node->nested_type_name (this->seq_scope ());
      os << ", ";
      os << node->nested_type_name (this->seq_scope (), "_ptr");
    }
  else
    {
      os << node->name () << ", " << node->name () << "_ptr";
    }

  if (!seq->unbounded ())
    {
      os << ", " << seq->max_size ();
    }
  os << ">::TAO_ObjRefMngType ";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_string (be_string * /*node*/)
{
  TAO_OutStream &os = this->stream ();
  be_sequence *seq = be_sequence::narrow_from_decl (this->be_node ());
  if (!seq)
    return -1;

  if (seq->unbounded ())
    os << "TAO_Unbounded_StringSequence";
  else
    os << "TAO_Bounded_ObjectSequence <";
  if (!seq->unbounded ())
    {
      os << seq->max_size () << ">";
    }
  os << "::TAO_StringMngType ";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_sequence_elemtype::visit_typedef (be_typedef *node)
{
  return this->visit_node (node);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<be_visitor_sequence_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_sequence_base_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_sequence_elemtype, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<be_visitor_sequence_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_sequence_base_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_sequence_elemtype, ACE_SYNCH_RECURSIVE_MUTEX>
#endif
