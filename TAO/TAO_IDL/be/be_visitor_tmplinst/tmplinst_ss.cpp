//=============================================================================
/**
*  @file   be_visitor_tmplinst_ss.cpp
*
*  $Id$
*
*  This visitor generates explicit template instantiations, guarded so they
*  are seen only by compilers whose platforms require explicit template
*  instantiation.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

be_visitor_tmplinst_ss::be_visitor_tmplinst_ss (be_visitor_context *ctx)
  : be_visitor_tmplinst (ctx)
{
}

be_visitor_tmplinst_ss::~be_visitor_tmplinst_ss (void)
{
}

int
be_visitor_tmplinst_ss::visit_interface (be_interface *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_interface - visit scope failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst_ss::visit_valuetype (be_valuetype *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_valuetype - visit scope failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst_ss::visit_operation (be_operation *node)
{
  if (node->imported ())
    {
      return 0;
    }

  const char * S = "S";

  be_visitor_arg_tmplinst visitor (this->ctx_,
                                   this->mode_,
                                   this->prefix_,
                                   this->suffix_,
                                   this->linebreak_,
                                   S);
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  visitor.direction ();

  if (bt->accept (&visitor) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst_ss::"
                         "visit_operation - "
                         "codegen for return type failed\n"), 
                        -1);
    }

  be_argument *arg = 0;

  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      arg = be_argument::narrow_from_decl (si.item ());
      visitor.direction (arg->direction ());
      bt = be_type::narrow_from_decl (arg->field_type ());

      if (bt->accept (&visitor) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_tmplinst_ss::"
                             "visit_operation - "
                             "codegen for argument failed\n"), 
                            -1);
        }
    }

  return 0;
}

idl_bool
be_visitor_tmplinst_ss::this_mode_generated (be_decl *node) const
{
  switch (this->mode_)
    {
      case TMPL_CLASS:
        return node->srv_tmpl_class_gen ();
      case PRAGMA_INST:
        return node->srv_pragma_inst_gen ();
      default:
        return I_FALSE;
    }
}

void
be_visitor_tmplinst_ss::this_mode_generated (be_decl *node,
                                             idl_bool val)
{
  switch (this->mode_)
    {
      case TMPL_CLASS:
        node->srv_tmpl_class_gen (val);
        break;
      case PRAGMA_INST:
        node->srv_pragma_inst_gen (val);
        break;
      default:
        break;
    }
}

