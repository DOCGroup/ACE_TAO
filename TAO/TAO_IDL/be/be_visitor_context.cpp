//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_context.cpp
//
// = DESCRIPTION
//     Maintains the context information for visitors
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_argument.h"
#include "be_array.h"
#include "be_attribute.h"
#include "be_constant.h"
#include "be_enum.h"
#include "be_enum_val.h"
#include "be_exception.h"
#include "be_field.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_module.h"
#include "be_operation.h"
#include "be_predefined_type.h"
#include "be_root.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_union_branch.h"

#include "be_visitor_context.h"

ACE_RCSID (be, 
           be_visitor_context, 
           "$Id$")


// constructor
be_visitor_context::be_visitor_context (void)
  : state_ (TAO_CodeGen::TAO_UNKNOWN),
    sub_state_ (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN),
    os_ (0),
    scope_ (0),
    node_ (0),
    alias_ (0),
    tdef_ (0),
    attr_ (0),
    exception_ (0),
    comma_ (0),
    interface_ (0)
{
}

be_visitor_context::be_visitor_context (const be_visitor_context &ctx)
  : state_ (ctx.state_),
    sub_state_ (ctx.sub_state_),
    os_ (ctx.os_),
    scope_ (ctx.scope_),
    node_ (ctx.node_),
    alias_ (ctx.alias_),
    tdef_ (ctx.tdef_),
    attr_ (ctx.attr_),
    exception_ (ctx.exception_),
    comma_ (ctx.comma_),
    interface_ (ctx.interface_)
{
}

be_visitor_context &
be_visitor_context::operator= (const be_visitor_context &ctx)
{
  this->state_ = ctx.state_;
  this->sub_state_ = ctx.sub_state_;
  this->os_ = ctx.os_;
  this->scope_ = ctx.scope_;
  this->node_ = ctx.node_;
  this->alias_ = ctx.alias_;
  this->tdef_ = ctx.tdef_;
  this->attr_ = ctx.attr_;
  this->exception_ = ctx.exception_;
  this->comma_ = ctx.comma_;
  this->interface_ = ctx.interface_;

  return *this;
}

be_visitor_context::~be_visitor_context (void)
{
  // we do not own anything
}

//= helpers

void
be_visitor_context::reset (void)
{
  this->state_ = TAO_CodeGen::TAO_INITIAL;
  this->sub_state_ = TAO_CodeGen::TAO_SUB_STATE_UNKNOWN;
  this->os_ = 0;
  this->scope_ = 0;
  this->node_ = 0;
  this->alias_ = 0;
  this->tdef_ = 0;
  this->attr_ = 0;
  this->exception_ = 0;
  this->comma_ = 0;

}

void
be_visitor_context::stream (TAO_OutStream *os)
{
  this->os_ = os;
}

TAO_OutStream *
be_visitor_context::stream (void)
{
  return this->os_;
}

void
be_visitor_context::scope (be_decl *s)
{
  this->scope_ = s;
}

be_decl *
be_visitor_context::scope (void)
{
  return this->scope_;
}

void
be_visitor_context::node (be_decl *n)
{
  this->node_ = n;
}

be_decl *
be_visitor_context::node (void)
{
  return this->node_;
}

void
be_visitor_context::state (TAO_CodeGen::CG_STATE st)
{
  this->state_ = st;
}

TAO_CodeGen::CG_STATE
be_visitor_context::state (void)
{
  return this->state_;
}

void
be_visitor_context::sub_state (TAO_CodeGen::CG_SUB_STATE st)
{
  this->sub_state_ = st;
}

TAO_CodeGen::CG_SUB_STATE
be_visitor_context::sub_state (void)
{
  return this->sub_state_;
}

void
be_visitor_context::tdef (be_typedef *node)
{
  this->tdef_ = node;
}

be_typedef *
be_visitor_context::tdef (void)
{
  return this->tdef_;
}

void
be_visitor_context::alias (be_typedef *node)
{
  this->alias_ = node;
}

be_typedef *
be_visitor_context::alias (void)
{
  return this->alias_;
}

void
be_visitor_context::attribute (be_attribute *node)
{
  this->attr_ = node;
}

be_attribute *
be_visitor_context::attribute (void)
{
  return this->attr_;
}

void
be_visitor_context::exception (idl_bool ib)
{
  this->exception_ = ib;
}

idl_bool
be_visitor_context::exception (void)
{
  return this->exception_;
}

void
be_visitor_context::comma (idl_bool ib)
{
  this->comma_ = ib;
}

idl_bool
be_visitor_context::comma (void)
{
  return this->comma_;
}

void
be_visitor_context::interface (be_interface *interface)
{
  this->interface_ = interface;
}

be_interface *
be_visitor_context::interface (void) const
{
  return this->interface_;
}

// ****************************************************************

be_argument *
be_visitor_context::be_node_as_argument (void)
{
  if (this->node_ != 0)
    {
      return be_argument::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_array *
be_visitor_context::be_node_as_array (void)
{
  if (this->node_ != 0)
    {
      return be_array::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_attribute *
be_visitor_context::be_node_as_attribute (void)
{
  if (this->node_ != 0)
    {
      return be_attribute::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_constant *
be_visitor_context::be_node_as_constant (void)
{
  if (this->node_ != 0)
    {
      return be_constant::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_enum *
be_visitor_context::be_node_as_enum (void)
{
  if (this->node_ != 0)
    {
      return be_enum::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_enum_val *
be_visitor_context::be_node_as_enum_val (void)
{
  if (this->node_ != 0)
    {
      return be_enum_val::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
    return (be_enum_val *)0;
}

be_exception *
be_visitor_context::be_node_as_exception (void)
{
  if (this->node_ != 0)
    {
      return be_exception::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_field *
be_visitor_context::be_node_as_field (void)
{
  if (this->node_ != 0)
    {
      return be_field::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_interface *
be_visitor_context::be_node_as_interface (void)
{
  if (this->node_ != 0)
    {
      return be_interface::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_interface_fwd *
be_visitor_context::be_node_as_interface_fwd (void)
{
  if (this->node_ != 0)
    {
      return be_interface_fwd::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_module *
be_visitor_context::be_node_as_module (void)
{
  if (this->node_ != 0)
    {
      return be_module::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_operation *
be_visitor_context::be_node_as_operation (void)
{
  if (this->node_ != 0)
    {
      return be_operation::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_predefined_type *
be_visitor_context::be_node_as_predefined_type (void)
{
  if (this->node_ != 0)
    {
      return be_predefined_type::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_root *
be_visitor_context::be_node_as_root (void)
{
  if (this->node_ != 0)
    {
      return be_root::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_sequence *
be_visitor_context::be_node_as_sequence (void)
{
  if (this->node_ != 0)
    {
      return be_sequence::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_string *
be_visitor_context::be_node_as_string (void)
{
  if (this->node_ != 0)
    {
      return be_string::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_structure *
be_visitor_context::be_node_as_structure (void)
{
  if (this->node_ != 0)
    {
      return be_structure::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_typedef *
be_visitor_context::be_node_as_typedef (void)
{
  if (this->node_ != 0)
    {
      return be_typedef::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_union *
be_visitor_context::be_node_as_union (void)
{
  if (this->node_ != 0)
    {
      return be_union::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_union_branch *
be_visitor_context::be_node_as_union_branch (void)
{
  if (this->node_ != 0)
    {
      return be_union_branch::narrow_from_decl (this->node_);
    }
  else
    {
      return 0;
    }
}

be_enum *
be_visitor_context::be_scope_as_enum (void)
{
  if (this->scope_ != 0)
    {
      return be_enum::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}

be_exception *
be_visitor_context::be_scope_as_exception (void)
{
  if (this->scope_ != 0)
    {
      return be_exception::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}

be_interface *
be_visitor_context::be_scope_as_interface (void)
{
  if (this->scope_ != 0)
    {
      return be_interface::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
    return (be_interface *)0;
}

be_module *
be_visitor_context::be_scope_as_module (void)
{
  if (this->scope_ != 0)
    {
      return be_module::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}

be_operation *
be_visitor_context::be_scope_as_operation (void)
{
  if (this->scope_ != 0)
    {
      return be_operation::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}

be_root *
be_visitor_context::be_scope_as_root (void)
{
  if (this->scope_ != 0)
    {
      return be_root::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}

be_structure *
be_visitor_context::be_scope_as_structure (void)
{
  if (this->scope_ != 0)
    {
      return be_structure::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}

be_union *
be_visitor_context::be_scope_as_union (void)
{
  if (this->scope_ != 0)
    {
      return be_union::narrow_from_decl (this->scope_);
    }
  else
    {
      return 0;
    }
}
