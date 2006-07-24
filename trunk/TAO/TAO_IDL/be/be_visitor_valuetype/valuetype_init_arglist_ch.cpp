//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_arglist_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for the parameter list of the OBV factory
//    signature.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype_init,
           arglist_ch,
           "$Id$")

be_visitor_valuetype_init_arglist_ch::be_visitor_valuetype_init_arglist_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_init_arglist_ch::~be_visitor_valuetype_init_arglist_ch (
    void
  )
{
}

int
be_visitor_valuetype_init_arglist_ch::visit_factory (be_factory *node)
{
  TAO_OutStream& os = *(this->ctx_->stream ());

  os << " (";

  if (node->nmembers () > 0)
    {
      os << be_idt << be_idt_nl;

      // All we do is hand over code generation to our scope.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                            "visit_factory - "
                            "codegen for scope failed\n"),
                            -1);
        }

      os << be_uidt_nl
         << ")";
    }
  else
    {
      os << "void)" << be_idt;
    }

  // Now generate the throw specs.
  if (this->gen_throw_spec (node) == -1)
    {
      ACE_ERROR_RETURN ((
          LM_ERROR,
          ACE_TEXT ("(%N:%l) be_visitor_valuetype_init_arglist_ch")
          ACE_TEXT ("::visit_factory - ")
          ACE_TEXT ("Failed to generate throw spec\n")
        ),
        -1
      );
    }

  return 0;
}

int
be_visitor_valuetype_init_arglist_ch::visit_argument (be_argument *node)
{
  // Get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_args_arglist visitor (&ctx);
  int status = node->accept (&visitor);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                         "visit_argument - "
                         "codegen for arglist failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_init_arglist_ch::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // If we are not the last node in the list of arguments, generate a comma.
  if (!this->last_node (bd))
    {
      *os << "," << be_nl;
    }

  return 0;
}

int
be_visitor_valuetype_init_arglist_ch::gen_throw_spec (be_factory *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->exceptions ())
    {
      const char *throw_spec_open = "throw (";
      const char *throw_spec_close = ")";

      if (!be_global->use_raw_throw ())
        {
          throw_spec_open = "ACE_THROW_SPEC ((";
          throw_spec_close = "))";
        }

      *os << be_nl << throw_spec_open << be_idt;
      
      bool first = true;

      // Initialize an iterator to iterate thru the exception list.
      for (UTL_ExceptlistActiveIterator ei (node->exceptions ());
           !ei.is_done ();
           ei.next ())
        {
          be_exception *excp =
            be_exception::narrow_from_decl (ei.item ());

          if (excp == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation"
                                 "gen_throw_spec - "
                                 "bad exception node\n"),
                                -1);

            }

          *os << (first ? "" : ",") << be_nl
              << excp->name ();
              
           first = false;
        }
        
      *os << be_uidt_nl << throw_spec_close << be_uidt;
    }

  return 0;
}

