
//=============================================================================
/**
 *  @file    root_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Root in the server skeletons file
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ******************************************************
// Root visitor for server skeletons
// ******************************************************

be_visitor_root_ss::be_visitor_root_ss (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ss::~be_visitor_root_ss (void)
{
}

int
be_visitor_root_ss::visit_root (be_root *node)
{
   if (this->init () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ss::init - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  if (this->gen_arg_traits (node) == -1)
    {
      /// Error message already output.
      return -1;
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ss::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  if (be_global->gen_tie_classes ())
    {
      (void) tao_cg->end_server_template_skeletons ();
    }

  (void) tao_cg->end_server_skeletons ();

  return 0;
}

int
be_visitor_root_ss::init (void)
{
  /// First open the server-side file for writing
  int status =
    tao_cg->start_server_skeletons (
      be_global->be_get_server_skeleton_fname ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ss::init - ")
                         ACE_TEXT ("Error opening server header file\n")),
                        -1);
    }

  if (be_global->gen_tie_classes ())
    {
      status =
        tao_cg->start_server_template_skeletons (
              be_global->be_get_server_template_skeleton_fname ());

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_root_ss::init - ")
                             ACE_TEXT ("Error opening server ")
                             ACE_TEXT ("template skeleton file\n")),
                            -1);
        }
    }

  // set stream
  this->ctx_->stream (tao_cg->server_skeletons ());
  return 0;
}

int
be_visitor_root_ss::gen_arg_traits (be_root *node)
{
  be_visitor_context ctx = *this->ctx_;
  be_visitor_arg_traits arg_visitor ("S", &ctx);
  int status = node->accept (&arg_visitor);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ss::")
                         ACE_TEXT ("gen_arg_traits - failed to ")
                         ACE_TEXT ("generate skeleton arg traits\n")),
                        -1);
    }

  if (be_global->gen_thru_poa_collocation ()
      || be_global->gen_direct_collocation ())
    {
      be_visitor_arg_traits arg_visitor ("", &ctx);
      status = node->accept (&arg_visitor);

      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_root_ss::")
                             ACE_TEXT ("gen_arg_traits - failed to ")
                             ACE_TEXT ("generate collocated ")
                             ACE_TEXT ("skeleton arg traits\n")),
                            -1);
        }
    }

  return 0;
}

