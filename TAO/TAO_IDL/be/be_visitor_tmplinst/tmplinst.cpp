//=============================================================================
/**
*  @file   tmplinst.cpp
*
*  $Id$
*
*  Base class for the concrete visitors that generate explicit template
*  instantiations for the stub side and skeleton side.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

be_visitor_tmplinst::be_visitor_tmplinst (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    mode_ (TMPL_CLASS),
    prefix_ (ACE::strnew ("  template class")),
    suffix_ (ACE::strnew (";")),
    linebreak_ (ACE::strnew (""))
{
}

be_visitor_tmplinst::~be_visitor_tmplinst (void)
{
  delete [] this->prefix_;
  this->prefix_ = 0;
  delete [] this->suffix_;
  this->suffix_ = 0;
  delete [] this->linebreak_;
  this->linebreak_ = 0;
}

int
be_visitor_tmplinst::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_tmplinst::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int 
be_visitor_tmplinst::visit_interface_fwd (be_interface_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_interface *fd = 
    be_interface::narrow_from_decl (node->full_definition ());

  // The logic in visit_interface() should handle what gets generated
  // and what doesn't.
  if (this->visit_interface (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_interface_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_valuetype *fd = 
    be_valuetype::narrow_from_decl (node->full_definition ());

  // The logic in visit_valuetype() should handle what gets generated
  // and what doesn't.
  int status = this->visit_valuetype (fd);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_valuetype_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int 
be_visitor_tmplinst::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int 
be_visitor_tmplinst::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int 
be_visitor_tmplinst::visit_structure_fwd (be_structure_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_structure *fd = be_structure::narrow_from_decl (node->full_definition ());

  if (this->visit_structure (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_structure_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_tmplinst::visit_union_fwd (be_union_fwd *node)
{
  if (this->this_mode_generated (node))
    {
      return 0;
    }

  be_union *fd = be_union::narrow_from_decl (node->full_definition ());

  if (this->visit_union (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_union_fwd - code generation failed\n"),
                        -1);
    }

  this->this_mode_generated (node, I_TRUE);
  return 0;
}

int
be_visitor_tmplinst::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_tmplinst::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_tmplinst::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

void
be_visitor_tmplinst::switch_mode (void)
{
  this->mode_ = PRAGMA_INST;
  delete [] this->prefix_;
  this->prefix_ = ACE::strnew ("# pragma instantiate");
  delete [] this->suffix_;
  this->suffix_ = ACE::strnew ("");
  delete [] this->linebreak_;
  this->linebreak_ = ACE::strnew (" \\");
}

