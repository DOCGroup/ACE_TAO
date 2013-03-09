
//=============================================================================
/**
 *  @file    root_cnh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the CIAO
 *  connector impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "root.h"

be_visitor_root_cnh::be_visitor_root_cnh (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_cnh::~be_visitor_root_cnh (void)
{
}

int
be_visitor_root_cnh::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cnh::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cnh::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_ciao_conn_header ();

  return 0;
}

int
be_visitor_root_cnh::init (void)
{
  // First open the client-side header file for writing.
  int status =
    tao_cg->start_ciao_conn_header (
      be_global->be_get_ciao_conn_hdr_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_cnh::init - ")
                         ACE_TEXT ("Error opening CIAO conn impl ")
                         ACE_TEXT ("header file\n")),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->ciao_conn_header ());

  return 0;
}
