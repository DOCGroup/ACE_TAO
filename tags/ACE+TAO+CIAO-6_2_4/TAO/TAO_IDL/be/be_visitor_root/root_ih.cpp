
//=============================================================================
/**
 *  @file    root_ih.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the server implementation header
 *
 *
 *  @author Yamuna  Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================

#include "root.h"

be_visitor_root_ih::be_visitor_root_ih (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ih::~be_visitor_root_ih (void)
{
}

int
be_visitor_root_ih::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ih::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ih::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  (void) tao_cg->end_implementation_header (
      be_global->be_get_implementation_hdr_fname (false));

  return 0;
}

int
be_visitor_root_ih::init (void)
{
  /// First open the implementation header file for writing.
  int status =
    tao_cg->start_implementation_header (
      be_global->be_get_implementation_hdr_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ih::init - ")
                         ACE_TEXT ("Error opening impl header file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->implementation_header ());
  return 0;
}
