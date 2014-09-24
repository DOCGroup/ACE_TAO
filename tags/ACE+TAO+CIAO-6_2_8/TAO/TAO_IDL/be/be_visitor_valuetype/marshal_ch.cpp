
//=============================================================================
/**
 *  @file    marshal_ch.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for marshalling.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on code from Aniruddha Gokhale
 */
//=============================================================================

#include "valuetype.h"

// ***************************************************************************
// Structure visitor for generating  declarations
// ***************************************************************************

be_visitor_valuetype_marshal_ch::be_visitor_valuetype_marshal_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_marshal_ch::~be_visitor_valuetype_marshal_ch (void)
{
}

int
be_visitor_valuetype_marshal_ch::visit_valuetype (be_valuetype *node)
{
  this->elem_number_ = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"),
                            -1);

        }

      be_field *field = be_field::narrow_from_decl (d);

      if (field)
        {
          be_visitor_context ctx (*this->ctx_);
          be_visitor_valuetype_field_cdr_ch visitor (&ctx);
          visitor.pre_ = node->field_pd_prefix ();
          visitor.post_ = node->field_pd_postfix ();

          if (visitor.visit_field (field) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_marshal_ch::"
                                 "visit_valuetype - "
                                 "codegen for scope failed\n"),
                                -1);
            }
        }
    }

  return 0;
}

int
be_visitor_valuetype_marshal_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

