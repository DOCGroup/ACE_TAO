
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Cdr operators in the client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "typedef.h"

// ***************************************************************************
// Typedef visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_typedef_cdr_op_cs::be_visitor_typedef_cdr_op_cs (
    be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_cdr_op_cs::~be_visitor_typedef_cdr_op_cs (void)
{
}

int
be_visitor_typedef_cdr_op_cs::visit_typedef (be_typedef *node)
{
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  // In general, we may have a chain of typedefs. i.e.,
  // typedef sequence<long> X;
  // typedef X Y;
  // typedef Y Z; and so on
  // The first time we will be in will be for node Z for which the code
  // generation has to take place. However, it is not enough to just generate
  // code that looks like -
  // typedef Y Z;
  // For different types (in this case we have a sequence), we will need
  // typedefs for the _var and _out types for Z. If it had been an array, we
  // will additionally have the _forcdr type as well as inlined *_alloc, _dup,
  // and _free methods.
  //
  // Finally, we need to differentiate between the case where we were
  // generating code for
  // typedef sequence<long> X; and
  // typedef Y Z; where Y was somehow aliased to the sequence. In the former
  // case, we will need to generate all the code for sequence<long> or whatever
  // the type maybe. In the latter, we just need typedefs for the type and all
  // associated _var, _out, and other types.

  be_type *bt = 0; // base type

  if (this->ctx_->tdef ())
    {
      // the fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node

      this->ctx_->alias (node); // save this alias

      // grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method
      bt = node->primitive_base_type ();

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_typedef - "
                             "bad primitive base type\n"),
                            -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      this->ctx_->alias (0); // reset
    }
  else
    {
      // the context has not stored cdr "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_typedef - "
                             "bad base type\n"),
                            -1);
        }

      // accept on this base type, but generate code for the typedef node
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      this->ctx_->tdef (0); // reset
    }

  node->cli_stub_cdr_op_gen (1);
  return 0;
}

int
be_visitor_typedef_cdr_op_cs::visit_array (be_array *node)
{
  be_type *bt = 0;

  if (this->ctx_->alias ()) // typedef of a typedef
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (!bt->imported () &&
      bt->node_type () == AST_Decl::NT_array) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_array - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_cdr_op_cs::visit_sequence (be_sequence *node)
{
  be_type *bt = 0;

  if (this->ctx_->alias ()) // typedef of a typedef
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_sequence) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_sequence - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_cdr_op_cs::visit_structure (be_structure *node)
{
  be_type *bt = 0;

  if (this->ctx_->alias ()) // typedef of a typedef
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_struct) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_structure - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_cdr_op_cs::visit_union (be_union *node)
{
  be_type *bt = 0;

  if (this->ctx_->alias ()) // typedef of a typedef
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  if (bt->node_type () == AST_Decl::NT_union) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_cdr_op_cs::"
                             "visit_union - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}
