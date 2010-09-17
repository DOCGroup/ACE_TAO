
//=============================================================================
/**
 *  @file    serializer_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor for code generation of Arrays for the
 *  TAO::DCPS::Serializer operators in the client header.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#include "be_visitor_sequence/serializer_op_ch.h"

// ***************************************************************************
// Array visitor for generating Serializer operator declarations in the client header
// ***************************************************************************

be_visitor_array_serializer_op_ch::be_visitor_array_serializer_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_array_serializer_op_ch::~be_visitor_array_serializer_op_ch (void)
{
}


void
be_visitor_array_serializer_op_ch::gen_arg_ref(be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->ctx_->tdef ())
    {
      be_scope* scope = be_scope::narrow_from_scope (node->defined_in ());
      be_decl* parent = scope->decl ();

      *os << parent->full_name ()
          << "::_" << node->local_name ()
          << "_forany &);" << be_nl;
    }
  else
    {
      *os << node->name () << "_forany &);" << be_nl;
    }
}

int
be_visitor_array_serializer_op_ch::visit_array (be_array *node)
{
  if (node->cli_hdr_serializer_op_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Decl::NodeType nt = bt->node_type ();

  // If the node is an array of anonymous sequence, we need to
  // generate the sequence's Serializer operator declaration here.
  if (nt == AST_Decl::NT_sequence && bt->anonymous ())
    {
      be_visitor_sequence_serializer_op_ch visitor (this->ctx_);

      if (bt->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_array_serializer_op_ch::"
                             "visit_array - "
                             "accept on anonymous base type failed\n"),
                            -1);
        }
    }

  // If the array is an anonymous member and if its element type
  // is a declaration (not a reference), we must generate code for
  // the declaration.
  if (this->ctx_->alias () == 0 // Not a typedef.
      && bt->is_child (this->ctx_->scope ()->decl ()))
    {
      int status = 0;
      be_visitor_context ctx (*this->ctx_);

      switch (nt)
      {
        case AST_Decl::NT_enum:
          {
            be_visitor_enum_serializer_op_ch ec_visitor (&ctx);
            status = bt->accept (&ec_visitor);
            break;
          }
        case AST_Decl::NT_struct:
          {
            be_visitor_structure_serializer_op_ch sc_visitor (&ctx);
            status = bt->accept (&sc_visitor);
            break;
          }
        case AST_Decl::NT_union:
          {
            be_visitor_union_serializer_op_ch uc_visitor (&ctx);
            status = bt->accept (&uc_visitor);
            break;
          }
        default:
          break;
      }

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_array_ch::"
                             "visit_array - "
                             "array base type codegen failed\n"),
                            -1);
        }
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  //  Set the sub state as generating code for the size method
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_max_marshaled_size ("
      << "const ";
  this->gen_arg_ref(node);

  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean _tao_is_bounded_size ("
      << "const ";
  this->gen_arg_ref(node);

  *os << be_global->stub_export_macro ()
      << " size_t _dcps_find_size ("
      << "const ";
  this->gen_arg_ref(node);

  // Generate the Serializer << and >> operator declarations.
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (TAO::DCPS::Serializer &, const ";
  this->gen_arg_ref(node);

  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (TAO::DCPS::Serializer &, ";
  this->gen_arg_ref(node);

  node->cli_hdr_serializer_op_gen (1);
  return 0;
}
