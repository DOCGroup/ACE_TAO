//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ex_idl.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the CIAO executor IDL
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_ex_idl,
           "$Id$")

// ********************************
// Root visitor for CIAO executor IDL
// ********************************

be_visitor_root_ex_idl::be_visitor_root_ex_idl (
    be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ex_idl::~be_visitor_root_ex_idl (void)
{
}

int
be_visitor_root_ex_idl::init (void)
{
  // First open the client-side header file for writing.
  int status =
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
