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
        os << "TAO_Unbounded_Object_Sequence<";
      else
        os << "TAO_Bounded_Object_Sequence<";
      break;
    case be_sequence::MNG_STRING: // sequence of strings
      if (node->unbounded ())
	os << "TAO_Unbounded_String_Sequence";
      else
	os << "TAO_Bounded_String_Sequence<";
      break;
    default: // not a managed type
      if (node->unbounded ())
        os << "TAO_Unbounded_Sequence<";
      else
        os << "TAO_Bounded_Sequence<";
      break;
    }

  
  be_visitor_sequence_base_ch base (&os, node, bt);
  if (bt->accept (&base) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence.cpp - "
                         "base type visit failed\n"),
                        -1);
    }

  if (node->managed_type () == be_sequence::MNG_STRING)
    {
      if (!node->unbounded ())
	{
	  os << "<" << node->max_size () << ">";
	}
    }
  else
    {
      if (node->unbounded ())
	{
	  os << " >";
	}
      else
	{
	  os << ", " << node->max_size () << " >";
	}
    }

  os << " " << node->local_name () << ";" << nl;

  os << "typedef " << node->local_name () << "* "
     << node->local_name () << "_ptr;\n";

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
      os << "extern CORBA::TypeCode_ptr "
	 << node->tc_name ()->last_component () << ";\n\n";
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

// ****************************************************************
// We have to generate the parameters for the template that implements
// each sequence type.
// ****************************************************************

be_visitor_sequence_base_ch::
be_visitor_sequence_base_ch (TAO_OutStream *stream,
			     be_decl *sequence_scope,
			     be_type *base_type)
{
  this->os_ = stream;
  this->be_node (sequence_scope);
  this->current_type_ = base_type;
}

be_visitor_sequence_base_ch::~be_visitor_sequence_base_ch (void)
{
}

be_decl *
be_visitor_sequence_base_ch::seq_scope (void)
{
  if (this->node_ == 0)
    return 0;

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
      os << "CORBA::Object";
    default:
      os << node->name ();
    }
  return 0;
}

int
be_visitor_sequence_base_ch::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

// helper
int
be_visitor_sequence_base_ch::visit_node (be_type *)
{
  TAO_OutStream &os = this->stream ();
  os << this->current_type_->nested_type_name (this->seq_scope ());
  return 0;
}

int
be_visitor_sequence_base_ch::visit_interface (be_interface *)
{
  TAO_OutStream &os = this->stream ();
  os << this->current_type_->nested_type_name (this->seq_scope ());
  return 0;
}

int
be_visitor_sequence_base_ch::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream &os = this->stream ();
  os << this->current_type_->nested_type_name (this->seq_scope ());
  return 0;
}

int
be_visitor_sequence_base_ch::visit_string (be_string * )
{
  // NO-OP, we have ad-hoc classes from strings.
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
  return node->primitive_base_type ()->accept (this);
}

// ***********************************************************
// sequence element type visitor for return typs of [] methods
// ***********************************************************


be_visitor_sequence_elemtype::
be_visitor_sequence_elemtype (TAO_OutStream *stream,
			      be_decl *sequence_scope,
			      be_type *base_type)
{
  this->os_ = stream;
  this->be_node (sequence_scope);
  this->current_type_ = base_type;
}

be_visitor_sequence_elemtype::~be_visitor_sequence_elemtype (void)
{
}

be_decl *
be_visitor_sequence_elemtype::seq_scope (void)
{
  if (this->node_ == 0)
    return 0;

  be_decl *scope = 0;
  if (this->node_->is_nested ())
    scope = be_scope::narrow_from_scope (this->node_->defined_in ())->decl ();
  return scope;
}

int
be_visitor_sequence_elemtype::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream &os = this->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      os << "TAO_Object_Manager<CORBA::Object> ";
      break;
    default:
      os << this->current_type_->name () << " &";
    }
  return 0;
}

int
be_visitor_sequence_elemtype::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

// helper
int
be_visitor_sequence_elemtype::visit_node (be_type *)
{
  TAO_OutStream &os = this->stream ();

  if (os.stream_type () == TAO_OutStream::TAO_CLI_HDR)
    os << this->current_type_->nested_type_name (this->seq_scope ()) << " &";
  else
    os << this->current_type_->name () << " &";
  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface (be_interface *)
{
  TAO_OutStream &os = this->stream ();

  os << "TAO_Object_Manager <"
     << this->current_type_->nested_type_name (this->seq_scope ())
     << " > ";

  return 0;
}

int
be_visitor_sequence_elemtype::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream &os = this->stream ();

  os << "TAO_Object_Manager <"
     << this->current_type_->nested_type_name (this->seq_scope ())
     << " > ";

  return 0;
}

int
be_visitor_sequence_elemtype::visit_string (be_string *)
{
  TAO_OutStream &os = this->stream ();

  os << "TAO_String_Manager ";
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
  return node->primitive_base_type ()->accept (this);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<be_visitor_sequence_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<be_visitor_sequence_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#endif
