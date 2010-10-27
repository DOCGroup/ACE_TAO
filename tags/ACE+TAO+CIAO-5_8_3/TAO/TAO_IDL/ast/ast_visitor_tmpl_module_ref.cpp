/**
 *  @file   ast_visitor_tmpl_module_ref.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "ast_visitor_tmpl_module_ref.h"
#include "ast_visitor_context.h"
#include "ast_generator.h"

#include "ast_module.h"
#include "ast_template_module.h"
#include "ast_template_module_ref.h"

#include "global_extern.h"
#include "nr_extern.h"

ast_visitor_tmpl_module_ref::ast_visitor_tmpl_module_ref (
      ast_visitor_context *ctx)
  : ast_visitor_tmpl_module_inst (ctx, true)
{
}

ast_visitor_tmpl_module_ref::~ast_visitor_tmpl_module_ref (void)
{
}

int
ast_visitor_tmpl_module_ref::visit_template_module_ref (
  AST_Template_Module_Ref *node)
{
  // This mehod is an override of the base class, and creates
  // the necessary implied IDL in the template module.
  // Later, the base class visitor then skips the alias and
  // processes the implied IDL. It's a bit inefficient to have
  // the implied IDL in both the template module and its
  // instantiations, but otherwise the lookup issues are
  // extremely complicated. This approach allows
  // lookup_by_name_local() to just skip over the alias and
  // match the module of the same name occurring later in the
  // template module scope. From that vantage point, the
  // reifying visitor uses its established mechanism to jump
  // to the module of the same name in the instantiated template
  // module scope (see ast_visitor_reifying::check_and_store()
  // and ast_visitor_reifying::template_module_rel_name()).
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Module *added_module =
    idl_global->gen ()->create_module (idl_global->scopes (). top (),
                                       &sn);

  idl_global->scopes ().top ()->add_to_scope (added_module);

  idl_global->scopes ().push (added_module);

  // Visit the scope of referenced template module. No need to
  // update the template parameter list since its param list has
  // to be a subset of the one we're in.
  if (this->visit_scope (node->ref ()) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_ref::")
                         ACE_TEXT ("visit_template_module_ref - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}


