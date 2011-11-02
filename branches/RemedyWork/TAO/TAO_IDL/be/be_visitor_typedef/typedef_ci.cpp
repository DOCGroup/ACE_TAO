
//=============================================================================
/**
 *  @file    typedef_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Typedefs in the client inline file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ******************************************************
// Typedef visitor for client inline
// ******************************************************

be_visitor_typedef_ci::be_visitor_typedef_ci (be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_ci::~be_visitor_typedef_ci (void)
{
}

int
be_visitor_typedef_ci::visit_typedef (be_typedef *node)
{
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
  // will additionally have the _forany type as well as inlined *_alloc
  // methods.
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
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"),
                            -1);
        }

      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      this->ctx_->alias (0);
    }
  else
    {
      // the context has not stored any "tdef" node. So we must be in here for
      // the first time
      this->ctx_->tdef (node); // save the typedef node

      // grab the immediate base type node
      bt = be_type::narrow_from_decl (node->base_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad base type\n"),
                            -1);
        }

      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      this->ctx_->tdef (0);
    }

  return 0;
}

int
be_visitor_typedef_ci::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_type *bt = 0;

  // This doesn't catch 'typedef of a typedef' if the node is
  // imported, so we check for that below before generating
  // any code.
  if (this->ctx_->alias ())
    {
      // Typedef of a typedef.
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  // Is our base type an array node? If so, generate code for that array node.
  // In the first layer of typedef for an array, cli_hdr_gen() causes us to
  // skip all the code reached from the first branch. Then the ELSE branch is
  // skipped and we fail to generate any typedefs for that node. Adding the
  // check for cli_hdr_gen() to the IF statement keeps it in. Subsequent
  // layers of typedef, if any, assign the context alias to bt, so we go
  // straight to the ELSE branch.
  if (bt->node_type () == AST_Decl::NT_array && bt->cli_inline_gen () == 0)
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_array - "
                             "base class visitor failed\n"),
                            -1);
        }
    }
  else
    {
      if ((bt->imported () && tdef->imported ()) || bt->cli_inline_gen())
        {
          // Code below is generated in another file.
          return 0;
        }

      // generate the inline code for alloc, dup, copy, and free methods

      // alloc method
      *os << be_nl
          << "ACE_INLINE" << be_nl
          << tdef->name () << "_slice *" << be_nl;
      *os << tdef->name () << "_alloc (void)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "return " << bt->name () << "_alloc ();" << be_uidt_nl;
      *os << "}" << be_nl_2;

      // dup method
      *os << "ACE_INLINE" << be_nl
          << tdef->name () << "_slice *" << be_nl;
      *os << tdef->name () << "_dup (" << be_idt << be_idt_nl
          << "const " << tdef->name ()
          << "_slice *_tao_src" << be_uidt_nl
          << ")" << be_uidt_nl;
      *os << "{" << be_idt_nl;
      *os << "return " << bt->name () << "_dup (_tao_src);" << be_uidt_nl;
      *os << "}" << be_nl_2;

      // copy method
      *os << "ACE_INLINE" << be_nl
          << "void" << be_nl;
      *os << tdef->name () << "_copy (" << be_idt << be_idt_nl
          << tdef->name ()
          << "_slice *_tao_dest," << be_nl
          << "const " << tdef->name () << "_slice *_tao_src" << be_uidt_nl
          << ")" << be_uidt_nl;
      *os << "{" << be_idt_nl;
      *os << bt->name () << "_copy (_tao_dest, _tao_src);" << be_uidt_nl;
      *os << "}" << be_nl_2;

      // free method
      *os << "ACE_INLINE" << be_nl
          << "void" << be_nl;
      *os << tdef->name () << "_free (" << be_idt << be_idt_nl
          << tdef->name () << "_slice *_tao_src" << be_uidt_nl
          << ")" << be_uidt_nl;
      *os << "{" << be_idt_nl;
      *os << bt->name () << "_free (_tao_src);" << be_uidt_nl;
      *os << "}" << be_nl_2;
    }

  bt->cli_inline_gen (true);
  return 0;
}

int
be_visitor_typedef_ci::visit_sequence (be_sequence *node)
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
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_sequence - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_ci::visit_structure (be_structure *node)
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
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_structure - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_typedef_ci::visit_union (be_union *node)
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
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_union - "
                             "base class visitor failed\n"),
                            -1);
        }
    }

  return 0;
}
