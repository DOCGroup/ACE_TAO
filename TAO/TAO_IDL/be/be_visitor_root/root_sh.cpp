
//=============================================================================
/**
 *  @file    root_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Root in the server header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ***********************************
// Root visitor for server header
// ***********************************

be_visitor_root_sh::be_visitor_root_sh (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_sh::~be_visitor_root_sh (void)
{
}

int
be_visitor_root_sh::visit_root (be_root *node)
{
  if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_sh::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  /// The SI and SS cases are caught in BE_produce(). We
  /// want to generate an empty skeleton header file, which
  /// has been done, so -SS can flag a skip of the scope
  /// traversal, but we want to generate the skeleton
  /// end-of-header file stuff, so we don't bail completely.
  if (be_global->gen_skel_files ())
    {
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_root_sh::visit_root - ")
                             ACE_TEXT ("codegen for scope failed\n")),
                            -1);
        }
    }

  (void) tao_cg->end_server_header ();

  return 0;
}

int
be_visitor_root_sh::init (void)
{
  /// First open the server-side file for writing
  int status =
    tao_cg->start_server_header (
      be_global->be_get_server_hdr_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_sh::init - ")
                         ACE_TEXT ("Error opening server header file\n")),
                        -1);
    }

  /// Initialize the stream.
  this->ctx_->stream (tao_cg->server_header ());
  return 0;
}
