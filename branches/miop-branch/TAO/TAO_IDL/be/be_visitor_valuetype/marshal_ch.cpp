//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    marshal_ch.cpp
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for marshalling.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================


#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, marshal_ch, "$Id$")

// ***************************************************************************
// Structure visitor for generating  declarations
// ***************************************************************************

be_visitor_valuetype_marshal_ch::be_visitor_valuetype_marshal_ch
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_marshal_ch::~be_visitor_valuetype_marshal_ch (void)
{
}

int
be_visitor_valuetype_marshal_ch::visit_valuetype (be_valuetype *node)
{
  // Proceed if the number of members in our scope is greater than 0.
  if (node->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      this->elem_number_ = 0;

      // Continue until each field is visited.
      for (;!si->is_done ();si->next())
        {
          AST_Decl *d = si->item ();

          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"),
                                -1);

            }

          be_field *field = be_field::narrow_from_decl (d);

          if (field)
            {
              be_visitor_context* new_ctx =
                new be_visitor_context (*this->ctx_);
              be_visitor_valuetype_field_cdr_ch visitor (new_ctx);
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

      delete si;
    }

  return 0;
}
