
//=============================================================================
/**
 *  @file    be_visitor_context.cpp
 *
 *  $Id$
 *
 *   Maintains the context information for visitors
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_visitor_context.h"
#include "be_extern.h"

be_visitor_context::be_visitor_context (void)
  : ast_visitor_context (),
    state_ (TAO_CodeGen::TAO_INITIAL),
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
  : ast_visitor_context (ctx),
    state_ (ctx.state_),
    sub_state_ (ctx.sub_state_),
    os_ (ctx.os_),
    scope_ (ctx.scope_),
    node_ (ctx.node_),
    alias_ (ctx.alias_),
    tdef_ (ctx.tdef_),
    attr_ (ctx.attr_),
    exception_ (ctx.exception_),
    comma_ (ctx.comma_),
    interface_ (ctx.interface_),
    port_prefix_ (ctx.port_prefix_)
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
  this->template_args_ = ctx.template_args_;
  this->template_params_ = ctx.template_params_;
  this->port_prefix_ = ctx.port_prefix_;

  return *this;
}

be_visitor_context::~be_visitor_context (void)
{
  // We do not own anything.
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
  this->interface_ = 0;
  this->template_args_ = 0;
  this->template_params_ = 0;
  this->port_prefix_ = "";
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
be_visitor_context::scope (be_scope *s)
{
  this->scope_ = s;
}

be_scope *
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
be_visitor_context::exception (bool ib)
{
  this->exception_ = ib;
}

bool
be_visitor_context::exception (void)
{
  return this->exception_;
}

void
be_visitor_context::comma (bool ib)
{
  this->comma_ = ib;
}

bool
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

ACE_CString &
be_visitor_context::port_prefix (void)
{
  return this->port_prefix_;
}

const char *
be_visitor_context::export_macro (void) const
{
  switch (this->state_)
    {
      // If -GA is used, but the anyop macro hasn't been set,
      // default to the stub macro.
      case TAO_CodeGen::TAO_ROOT_ANY_OP_CH:
        return (be_global->gen_anyop_files ()
                ? this->non_null_export_macro ()
                : be_global->stub_export_macro ());
      case TAO_CodeGen::TAO_ARRAY_CH:
      case TAO_CodeGen::TAO_INTERFACE_CH:
      case TAO_CodeGen::TAO_INTERFACE_SMART_PROXY_CH:
      case TAO_CodeGen::TAO_ROOT_CH:
        return be_global->stub_export_macro ();
      case TAO_CodeGen::TAO_INTERFACE_DIRECT_PROXY_IMPL_SH:
      case TAO_CodeGen::TAO_INTERFACE_AMH_RH_SH:
      case TAO_CodeGen::TAO_ROOT_SH:
        return be_global->skel_export_macro ();
      default:
        return "";
    }
}

const char *
be_visitor_context::non_null_export_macro (void) const
{
  const char *anyop_export = be_global->anyop_export_macro ();

  return (ACE_OS::strcmp (anyop_export, "") == 0
          ? be_global->stub_export_macro ()
          : anyop_export);
}
