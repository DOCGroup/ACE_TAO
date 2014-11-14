
//=============================================================================
/**
 *  @file    typedef_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Typedef in the client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "typedef.h"

// ******************************************************
// Typedef visitor for client stubs
// ******************************************************

be_visitor_typedef_cs::be_visitor_typedef_cs (be_visitor_context *ctx)
  : be_visitor_typedef (ctx)
{
}

be_visitor_typedef_cs::~be_visitor_typedef_cs (void)
{
}

int
be_visitor_typedef_cs::visit_typedef (be_typedef *node)
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
      // The fact that we are here indicates that we were generating code for a
      // typedef node whose base type also happens to be another typedef-ed
      // (i.e. an alias) node for another (possibly alias) node.

      this->ctx_->alias (node); // save this alias

      // Grab the most primitive base type in the chain to avoid recusrsively
      // going thru this visit method.
      bt = node->primitive_base_type ();

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad primitive base type\n"),
                            -1);
        }

      // Accept on this base type.
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
      // The context has not stored any "tdef" node. So we must be in here for
      // the first time.
      this->ctx_->tdef (node);

      // Grab the immediate base type node.
      bt = be_type::narrow_from_decl (node->base_type ());

      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "bad base type\n"),
                            -1);
        }

      // Accept on this base type.
      if (bt->accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_typedef_ch::"
                             "visit_typedef - "
                             "failed to accept visitor\n"),
                            -1);
        }

      if (!node->imported () && be_global->tc_support ())
        {
          be_visitor_context ctx (*this->ctx_);
          // ctx.sub_state (TAO_CodeGen::TAO_TC_DEFN_TYPECODE);
          TAO::be_visitor_alias_typecode visitor (&ctx);

          if (node->accept (&visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_typedef_cs::"
                                 "visit_typedef - "
                                 "TypeCode definition failed\n"),
                                -1);
            }

        }

      this->ctx_->tdef (0);
    }

  return 0;
}
