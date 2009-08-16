//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    operation_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation in the CIAO servants file.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_operation,
           operation_svs,
           "$Id$")

// ************************************************************
// Operation visitor for CIAO servant implementations
// ************************************************************

be_visitor_operation_svs::be_visitor_operation_svs (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    scope_ (0),
    for_facets_ (false),
    swapping_ (be_global->gen_component_swapping ())
{
}

be_visitor_operation_svs::~be_visitor_operation_svs (void)
{
}

int
be_visitor_operation_svs::visit_operation (be_operation *node)
{
  TAO_OutStream &os = *this->ctx_->stream ();
  this->ctx_->node (node);

  os << be_nl << be_nl;

  // Retrieve the operation return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_svs::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Generate the return type mapping (same as in the header file)
  be_visitor_context ctx = *this->ctx_;
  be_visitor_operation_rettype rt_visitor (&ctx);

  if (bt->accept (&rt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_svs::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  // Generate the operation name, avoiding possible _cxx_ prefix.
  os << be_nl
     << scope_->original_local_name ()->get_string ()
     << "_Servant";

  os << "::" << node->local_name ();

  // Generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  be_visitor_operation_arglist al_visitor (&ctx);

  if (node->accept (&al_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_svs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  return this->gen_op_body (node);
}

int
be_visitor_operation_svs::gen_op_body (be_operation *node)
{
  TAO_OutStream &os = *this->ctx_->stream ();

  os << be_nl
     << "{" << be_idt_nl;

  if (swapping_)
    {
      os << "this->activate_component ();" << be_nl;
    }

  os << "if ( ::CORBA::is_nil (this->executor_.in ()))" << be_idt_nl
     << "{"<< be_idt_nl
     << "throw ::CORBA::INV_OBJREF ();" << be_uidt_nl
     << "}" << be_uidt_nl << be_nl;

  bool const vrt = node->void_return_type ();

  if (!vrt)
    {
      os << "return ";
    }

  os << "this->executor_->" << node->local_name () << " (";

  if (node->argument_count () == 0)
    {
      os << ");";
    }
  else
    {
      os << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_operation_svs")
                             ACE_TEXT ("::gen_op_body - ")
                             ACE_TEXT ("visit_scope() failed\n")),
                            -1);
        }
    }

  os << be_uidt_nl
     << "}";

  return 0;
}

int
be_visitor_operation_svs::visit_argument (be_argument *node)
{
  TAO_OutStream &os = *this->ctx_->stream ();

  os << node->local_name ();

  return 0;
}

int
be_visitor_operation_svs::post_process (be_decl *bd)
{
  TAO_OutStream &os = *this->ctx_->stream ();

  if (this->last_node (bd))
    {
      os << ");" << be_uidt;
    }
  else
    {
      os << "," << be_nl;
    }

  return 0;
}

void
be_visitor_operation_svs::for_facets (bool val)
{
  this->for_facets_ = val;
}

void
be_visitor_operation_svs::scope (be_interface *node)
{
  this->scope_ = node;
}
