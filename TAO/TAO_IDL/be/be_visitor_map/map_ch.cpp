
//=============================================================================
/**
 *  @file    map_ch.cpp
 *
 *  Visitor generating code for Sequence in the client header
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "map.h"

// Root visitor for client header.
be_visitor_map_ch::be_visitor_map_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_map_ch::~be_visitor_map_ch ()
{
}

int be_visitor_map_ch::visit_map (be_map *node)
{
  if (node->defined_in () == nullptr)
    {
      // The node is a nested map, and has had no scope defined.
      node->set_defined_in (DeclAsScope (this->ctx_->scope ()->decl ()));
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the key type since we may need to do some code
  // generation for the base type.
  be_type *kt = dynamic_cast<be_type*> (node->key_type ());

  if (kt == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_map_ch::")
                         ACE_TEXT ("visit_map - ")
                         ACE_TEXT ("Bad element type\n")),
                        -1);
    }

  kt->seen_in_map (true);
  AST_Decl::NodeType knt = kt->node_type ();

  if (knt == AST_Decl::NT_map)
    {
      // Temporarily make the context's tdef node 0 so the nested call
      // to create_name will not get confused and give our anonymous
      // map element type the same name as we have.
      be_typedef *tmp = this->ctx_->tdef ();
      this->ctx_->tdef (nullptr);

      if (kt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_map_ch::")
                             ACE_TEXT ("visit_map - ")
                             ACE_TEXT ("codegen for anonymous ")
                             ACE_TEXT ("base type failed\n")),
                            -1);
        }

      // Restore the tdef value.
      this->ctx_->tdef (tmp);
    }

  be_type *vt = dynamic_cast<be_type*> (node->value_type ());

  if (vt == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_map_ch::")
                         ACE_TEXT ("visit_map - ")
                         ACE_TEXT ("Bad element type\n")),
                        -1);
    }

  vt->seen_in_map (true);
  AST_Decl::NodeType vnt = vt->node_type ();


  if (vnt == AST_Decl::NT_map)
    {
      // Temporarily make the context's tdef node 0 so the nested call
      // to create_name will not get confused and give our anonymous
      // map element type the same name as we have.
      be_typedef *tmp = this->ctx_->tdef ();
      this->ctx_->tdef (nullptr);

      if (kt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_map_ch::")
                             ACE_TEXT ("visit_map - ")
                             ACE_TEXT ("codegen for anonymous ")
                             ACE_TEXT ("base type failed\n")),
                            -1);
        }

      // Restore the tdef value.
      this->ctx_->tdef (tmp);
    }

  *os << be_nl_2;

  TAO_INSERT_COMMENT (os);

  *os << "std::map<"
      << node->key_type ()->full_name ()
      << ", "
      << node->value_type ()->full_name ()
      << ">";

  node->cli_hdr_gen (true);
  return 0;
}

void
be_visitor_map_ch::gen_varout_typedefs (be_map *node,
                                             be_type *elem)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl;

  AST_Type::SIZE_TYPE st = elem->size_type ();

  *os << "typedef "
      << (st == AST_Type::FIXED ? "::TAO_FixedSeq_Var_T<"
                                : "::TAO_VarSeq_Var_T<")
      << node->local_name ();

  *os << "> "
      << node->local_name () << "_var;" << be_nl;

  *os << "typedef ::TAO_Seq_Out_T<"
      << node->local_name ()
      << "> " << node->local_name () << "_out;" << be_nl;
}
