//=============================================================================
/**
*  @file   be_visitor_arg_tmplinst.cpp
*
*  $Id$
*
*  This visitor generates explicit template instantiations, guarded so they
*  are seen only by compilers whose platforms require explicit template
*  instantiation, for the individual argument template helper classes.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#include "be_visitor_arg_tmplinst.h"
#include "be_visitor_context.h"
#include "be_interface.h"
#include "be_valuetype.h"
#include "be_interface_fwd.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_array.h"
#include "be_enum.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_field.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_typedef.h"
#include "be_helper.h"

be_visitor_arg_tmplinst::be_visitor_arg_tmplinst (be_visitor_tmplinst *rhs)
  : be_visitor_tmplinst (rhs)
{
}

be_visitor_arg_tmplinst::~be_visitor_arg_tmplinst (void)
{
}

int
be_visitor_arg_tmplinst::visit_interface (be_interface *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_interface_fwd (be_interface_fwd *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_valuetype (be_valuetype *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int 
be_visitor_arg_tmplinst::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_arg_tmplinst::visit_sequence (be_sequence *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_string (be_string *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_array (be_array *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_enum (be_enum *node)
{
  return 0;
}

int 
be_visitor_arg_tmplinst::visit_structure (be_structure *node)
{
  return 0;
}

int
be_visitor_arg_tmplinst::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_arg_tmplinst::visit_union (be_union *node)
{
  return 0;
}

int
be_visitor_arg_tmplinst::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"), 
                        -1);
    }

  return 0;
}

int 
be_visitor_arg_tmplinst::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_tmplinst::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

void
be_visitor_arg_tmplinst::cleanup (void)
{
}

