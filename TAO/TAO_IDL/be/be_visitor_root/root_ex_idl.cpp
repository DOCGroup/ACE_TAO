
//=============================================================================
/**
 *  @file    root_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the CIAO executor IDL
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "root.h"

be_visitor_root_ex_idl::be_visitor_root_ex_idl (
    be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ex_idl::~be_visitor_root_ex_idl (void)
{
}

int
be_visitor_root_ex_idl::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ex_idl::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ex_idl::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_ciao_exec_idl ();

  return 0;
}

int
be_visitor_root_ex_idl::init (void)
{
  // First open the client-side header file for writing.
  int const status =
    tao_cg->start_ciao_exec_idl (
      be_global->be_get_ciao_exec_idl_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ex_idl::init - ")
                         ACE_TEXT ("Error opening CIAO executor ")
                         ACE_TEXT ("IDL file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_exec_idl ());

  return 0;
}
