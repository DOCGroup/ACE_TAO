//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_is.cpp
//
// = DESCRIPTION
//    Visitor generating code for the Root in the implementation skeletons file
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

ACE_RCSID (be_visitor_root, 
           root_is, 
           "$Id$")

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
be_visitor_root_is::init (void)
{
  // First open the file for writing.
  if (tao_cg->start_implementation_skeleton (
          be_global->be_get_implementation_skel_fname ()
        )
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_is::init - "
                         "Error opening implementation skeletons file\n"), 
                        -1);
    }

  this->ctx_->stream (tao_cg->implementation_skeleton ());
  return 0;
}
