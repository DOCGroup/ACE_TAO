//=============================================================================
/**
 *  @file    root_is.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Root in the implementation skeletons file
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================

// ******************************************************
// Root visitor for implementation  skeletons
// ******************************************************

be_visitor_root_is::be_visitor_root_is (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_is::~be_visitor_root_is (void)
{
}

int
be_visitor_root_is::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_is::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_is::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_root_is::init (void)
{
  /// First open the file for writing.
  int status =
    tao_cg->start_implementation_skeleton (
      be_global->be_get_implementation_skel_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_is::init - ")
                         ACE_TEXT ("Error opening impl source file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->implementation_skeleton ());
  return 0;
}
