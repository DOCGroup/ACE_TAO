//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typedef_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Typedefs in the client inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_typedef.h"

ACE_RCSID(be_visitor_typedef, typedef_ci, "$Id$")


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
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();

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

  be_type *bt; // base type

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
                             "bad primitive base type\n"
                             ),  -1);
        }
      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
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
                             "bad base type\n"
                             ),  -1);
        }
      // accept on this base type
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"
                             ),  -1);
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
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_array) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_array (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_array - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  else
    {
      // generate the inline code for alloc, dup, copy, and free methods

      // alloc method
      os->indent ();
      *os << "ACE_INLINE " << tdef->name () << "_slice *" << be_nl;
      *os << tdef->name () << "_alloc (void)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "return " << bt->name () << "_alloc ();" << be_uidt_nl;
      *os << "}\n\n";

      // dup method
      os->indent ();
      *os << "ACE_INLINE " << tdef->name () << "_slice *" << be_nl;
      *os << tdef->name () << "_dup (const " << tdef->name ()
          << "_slice *_tao_src)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << "return " << bt->name () << "_dup (_tao_src);" << be_uidt_nl;
      *os << "}\n\n";

      // copy method
      os->indent ();
      *os << "ACE_INLINE void" << be_nl;
      *os << tdef->name () << "_copy (" << tdef->name ()
          << "_slice *_tao_dest, const " << tdef->name ()
          << "_slice *_tao_src)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << bt->name () << "_copy (_tao_dest, _tao_src);" << be_uidt_nl;
      *os << "}\n\n";

      // free method
      os->indent ();
      *os << "ACE_INLINE void" << be_nl;
      *os << tdef->name () << "_free (" << tdef->name ()
          << "_slice *_tao_src)" << be_nl;
      *os << "{" << be_idt_nl;
      *os << bt->name () << "_free (_tao_src);" << be_uidt_nl;
      *os << "}\n\n";
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_sequence) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_sequence (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_sequence - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_struct) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_structure (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_structure - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}

int
be_visitor_typedef_ci::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // output stream
  be_typedef *tdef = this->ctx_->tdef (); // typedef node
  be_decl *scope = this->ctx_->scope (); // scope in which it is used
  be_type *bt;

  if (this->ctx_->alias ()) // typedef of a typedef
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (bt->node_type () == AST_Decl::NT_union) // direct typedef of a base node
                                              // type
    {
      // let the base class visitor handle this case
      if (this->be_visitor_typedef::visit_union (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ci::"
                             "visit_union - "
                             "base class visitor failed \n"
                             ),  -1);
        }
    }
  return 0;
}
