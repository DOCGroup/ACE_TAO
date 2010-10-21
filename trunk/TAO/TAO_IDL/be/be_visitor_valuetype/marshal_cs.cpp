
//=============================================================================
/**
 *  @file    marshal_cs.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for marshalling.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


be_visitor_valuetype_marshal_cs::be_visitor_valuetype_marshal_cs (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_marshal_cs::~be_visitor_valuetype_marshal_cs (void)
{
}

int
be_visitor_valuetype_marshal_cs::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "::CORBA::Boolean" << be_nl;

  this->class_name (node, os);

  *os << "::_tao_marshal_state (TAO_OutputCDR &";

  be_valuetype *inh = node->statefull_inherit ();

  // If the valuetype has no fields, and no stateful inherit,
  // the stream arg is unused.
  if (inh != 0 || node->data_members_count () > 0)
    {
      *os << "strm";
    }

  *os << ", TAO_ChunkInfo&";
  // If the valuetype has no fields, and no stateful inherit,
  // the chunking helper arg is unused.
  if (inh != 0 || node->data_members_count () > 0)
    {
      *os << "ci";
    }

  *os << ") const" << be_nl
      << "{" << be_idt_nl;

  if (inh)
    {
      *os << "if (! ci.start_chunk (strm))" << be_idt_nl;
      *os << "return false;" << be_uidt_nl << be_nl;

      if (inh->opt_accessor ())
        {
          *os << "if (!this->";

          this->class_name (inh, os);

          *os << "::_tao_marshal_state (strm, ci))" << be_idt_nl
              << "{" << be_idt_nl
              << "return false;" << be_uidt_nl
              << "}" << be_uidt_nl << be_nl;
        }
      // Can access base class only via virtual function.
      else
        {
          *os << "if (! this->_tao_marshal__"
              <<       inh->flat_name ()
              << " (strm, ci))" << be_idt_nl
              << "{" << be_idt_nl
              << "return false;" << be_uidt_nl
              << "}" << be_uidt_nl << be_nl;
        }
    }

  be_visitor_context new_ctx = *this->ctx_;
  be_visitor_valuetype_field_cdr_decl field_out_cdr (&new_ctx);

  if (field_out_cdr.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_marshal_cs::"
                         "visit_valuetype - "
                         "codegen for field out cdr scope failed\n"),
                        -1);
    }

  if (node->data_members_count () > 0)
    {
      *os << "if (! ci.start_chunk (strm))" << be_idt_nl;
      *os << "return false;" << be_uidt_nl << be_nl;
      *os << "CORBA::Boolean const ret = " << be_idt << be_idt_nl;

  // All we have to do is to visit the scope and generate code.
  this->gen_fields (node,
                    *this->ctx_);

      *os << ";" << be_uidt << be_uidt_nl;

      *os << "if ( ! ret) " << be_idt_nl;
      *os << "return false; " << be_uidt_nl << be_nl;
      *os << "if (! ci.end_chunk (strm))" << be_idt_nl;
      *os << "return false;" << be_uidt_nl << be_nl;
    }

  if (inh)
    {
      *os << "if (! ci.end_chunk (strm))" << be_idt_nl;
      *os << "return false;" << be_uidt_nl << be_nl;
    }

  *os << "return true;" << be_uidt_nl;
  *os << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);

  *os << "::CORBA::Boolean" << be_nl;

  this->class_name (node, os);

  *os << "::_tao_unmarshal_state (TAO_InputCDR &";

  // If the valuetype has no fields, and no stateful inherit,
  // the stream arg is unused.
  if (inh != 0 || node->data_members_count () > 0)
    {
      *os << "strm";
    }

  *os << ", TAO_ChunkInfo&";
  // If the valuetype has no fields, and no stateful inherit,
  // the chunking helper arg is unused.
  if (inh != 0 || node->data_members_count () > 0)
    {
      *os << "ci";
    }

  *os << ")" << be_nl
      << "{" << be_idt_nl;

  if (inh)
    {
      *os << "if (!ci.handle_chunking (strm))" << be_idt_nl;
      *os << "return false;" << be_uidt_nl << be_nl;

      if (inh->opt_accessor ())
        {
          *os << "if (!this->";

          this->class_name (inh, os);

          *os << "::_tao_unmarshal_state (strm, ci))" << be_idt_nl
              << "{" << be_idt_nl
              << "return false;" << be_uidt_nl
              << "}" << be_uidt_nl << be_nl;
        }
      else // only can access base class via virtual function
        {
          *os << "if (!this->_tao_unmarshal__"
              <<       inh->flat_name ()
              << " (strm, ci))" << be_idt_nl
              << "{" << be_idt_nl
              << "return false;" << be_uidt_nl
              << "}" << be_uidt_nl << be_nl;
        }
    }

  be_visitor_valuetype_field_cdr_decl field_in_cdr (&new_ctx);
  field_in_cdr.visit_scope (node);

  if (node->data_members_count () > 0)
    {
      *os << "if (!ci.handle_chunking (strm))" << be_idt_nl;
      *os << "return false;" << be_uidt_nl << be_nl;
      *os << "CORBA::Boolean const ret = " << be_idt << be_idt_nl;

  // All we have to do is to visit the scope and generate code.
  this->gen_fields (node,
                    *this->ctx_);

      *os << ";" << be_uidt << be_uidt_nl;

      *os << "if (!ret) " << be_idt_nl;
      *os << "return false; " << be_uidt_nl << be_nl;
      *os << "if (this->require_truncation_)" << be_idt_nl;
      *os << "return ci.skip_chunks (strm);" << be_uidt_nl << be_nl;
      *os << "else" << be_idt_nl;
      *os << "return ci.handle_chunking (strm);" << be_uidt_nl << be_nl;
    }
    else
      *os << "return true;";
  *os << be_uidt_nl << "}" << be_nl_2;

  *os << "void" << be_nl;

  this->class_name (node, os);

  *os << "::truncation_hook (void)" << be_nl
      << "{" << be_idt_nl
      << "this->require_truncation_ = true;" << be_uidt_nl
      << "}" << be_nl_2;

  return 0;
}

int
be_visitor_valuetype_marshal_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

void
be_visitor_valuetype_marshal_cs::class_name (be_valuetype *node,
                                             TAO_OutStream *os)
{
  if (node->opt_accessor ())
    {
      be_decl *scope =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << scope->name () << "::"
          << node->local_name ();
    }
  else
    {
      *os << node->full_obv_skel_name ();
    }
}

// Operations for field marshaling.
int
be_visitor_valuetype_marshal_cs::gen_fields (be_valuetype *node,
                                             be_visitor_context &ctx)
{
  int n_processed = 0;
  TAO_OutStream *os = ctx.stream ();
  this->elem_number_ = 0;

  // Initialize an iterator to iterate thru our scope.
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_scope::visit_scope - "
                             "bad node in this scope\n"),
                            -1);
        }

      be_field *field = be_field::narrow_from_decl (d);

      if (field)
        {
          if (n_processed > 0)
            {
              *os << " &&" << be_nl;
            }

          ++n_processed;
          be_visitor_valuetype_field_cdr_cs visitor (&ctx);
          visitor.pre_ = node->field_pd_prefix ();
          visitor.post_ = node->field_pd_postfix ();

          if (visitor.visit_field (field) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_valuetype_marshal_cs::"
                                 "visit_valuetype - "
                                 "codegen for scope failed\n"),
                                -1);
            }
        }
    }

  if (n_processed == 0)
    {
      *os << "true";
    }

  return 0;
}
