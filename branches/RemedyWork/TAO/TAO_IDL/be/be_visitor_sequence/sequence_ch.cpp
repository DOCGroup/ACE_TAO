
//=============================================================================
/**
 *  @file    sequence_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Sequence in the client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// Root visitor for client header.
be_visitor_sequence_ch::be_visitor_sequence_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_ch::~be_visitor_sequence_ch (void)
{
}

int be_visitor_sequence_ch::visit_sequence (be_sequence *node)
{
  if (node->defined_in () == 0)
    {
      // The node is a nested sequence, and has had no scope defined.
      node->set_defined_in (DeclAsScope (this->ctx_->scope ()->decl ()));
    }

  // First create a name for ourselves.
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_sequence_ch::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("failed creating name\n")),
                        -1);
    }

  // We don't check cli_hdr_gen() here. If we are generated more
  // than once as an anonymous sequence, the name guard will cause
  // the C++ preprocessor to catch it. If we are generated more than
  // once as a typedef (caused by a comma separated list of
  // typedefs), our name will be changed by the call above and the
  // name guard will not catch it, but that's ok - we want to
  // be generated for each typedef.
  if (node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Retrieve the base type since we may need to do some code
  // generation for the base type.
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_sequence_ch::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("Bad element type\n")),
                        -1);
    }

  bt->seen_in_sequence (true);
  AST_Decl::NodeType nt = bt->node_type ();

  // If our base type is an anonymous sequence, we must create a name
  // and generate a class declaration for it as well.
  if (nt == AST_Decl::NT_sequence)
    {
      // Temporarily make the context's tdef node 0 so the nested call
      // to create_name will not get confused and give our anonymous
      // sequence element type the same name as we have.
      be_typedef *tmp = this->ctx_->tdef ();
      this->ctx_->tdef (0);

      if (bt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_ch::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("codegen for anonymous ")
                             ACE_TEXT ("base type failed\n")),
                            -1);
        }

      // Restore the tdef value.
      this->ctx_->tdef (tmp);
    }

  *os << be_nl_2;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;


  os->gen_ifdef_macro (node->flat_name ());

  *os << be_nl_2;

  /// If we are using std::vector, we won't be using _vars
  /// and _outs. They may get redefined and reinstated later.
  if (!be_global->alt_mapping () || !node->unbounded ())
    {
      if (this->ctx_->tdef () != 0)
        {
          *os << "class " << node->local_name () << ";";
        }

      if (this->ctx_->tdef () != 0)
        {
          this->gen_varout_typedefs (node, bt);
        }
    }
  else
    {
      *os << "typedef std::vector< ";

      // Generate the base type for the buffer.
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
      be_visitor_sequence_buffer_type bt_visitor (&ctx);

      if (bt->accept (&bt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_ch::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("buffer type visit failed\n")),
                            -1);
        }

      *os << "> " << node->local_name () << ";";

      os->gen_endif ();
      node->cli_hdr_gen (true);
      return 0;
    }

  *os << be_nl_2
      << "class " << be_global->stub_export_macro () << " "
      << node->local_name () << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  int status =
    node->gen_base_class_name (os,
                               "",
                               this->ctx_->scope ()->decl ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_sequence_ch::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("Base class name ")
                         ACE_TEXT ("generation failed\n")),
                        -1);
    }

  *os << be_uidt << be_uidt << be_uidt;

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt;

  *os << be_nl
      << node->local_name () << " (void);";

  if (node->unbounded ())
    {
      *os << be_nl
          << node->local_name () << " ( ::CORBA::ULong max);";
    }

  /// If we are using std::vector, we can't implement this
  /// constructor.
  if (!be_global->alt_mapping () || !node->unbounded ())
    {
      *os << be_nl
          << node->local_name () << " (" << be_idt;

      if (node->unbounded ())
        {
          *os << be_nl
              << "::CORBA::ULong max,";
        }

      *os << be_nl
          << "::CORBA::ULong length," << be_nl;

      // Generate the base type for the buffer.
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_BUFFER_TYPE_CH);
      be_visitor_sequence_buffer_type bt_visitor (&ctx);

      if (bt->accept (&bt_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_sequence_ch::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("buffer type visit failed\n")),
                            -1);
        }

      *os << "* buffer, " << be_nl
          << "::CORBA::Boolean release = false);" << be_uidt;
  }

  *os << be_nl
      << node->local_name () << " (const " << node->local_name ()
      << " &);" << be_nl;
  *os << "virtual ~" << node->local_name () << " (void);";

  if (be_global->alt_mapping () && node->unbounded ())
    {
      *os << be_nl_2
          << "virtual ::CORBA::ULong length (void) const;"
          << be_nl
          << "virtual void length ( ::CORBA::ULong);"
          << be_nl_2
          << "virtual ::CORBA::ULong maximum (void) const;";
    }

  *os << be_nl;

  node->gen_stub_decls (os);

  // TAO provides extensions for octet sequences, first find out if
  // the base type is an octet (or an alias for octet).
  be_predefined_type *predef = 0;

  if (bt->base_node_type () == AST_Type::NT_pre_defined)
    {
      be_typedef* alias =
            be_typedef::narrow_from_decl (bt);

      if (alias == 0)
        {
          predef = be_predefined_type::narrow_from_decl (bt);
        }
      else
        {
          predef =
            be_predefined_type::narrow_from_decl (
                alias->primitive_base_type ()
              );
        }
    }

  // Now generate the extension...
  if (predef != 0
      && predef->pt () == AST_PredefinedType::PT_octet
      && node->unbounded ()
      && !be_global->alt_mapping ())
    {
      *os << be_nl_2
          << "\n\n#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)" << be_nl
          << node->local_name () << " (" << be_idt << be_idt_nl
          << "::CORBA::ULong length," << be_nl
          << "const ACE_Message_Block* mb" << be_uidt_nl
          << ")" << be_uidt_nl
          << "  : ::TAO::unbounded_value_sequence< ::CORBA::Octet>"
          << " (length, mb) {}" << "\n"
          << "#endif /* TAO_NO_COPY_OCTET_SEQUENCE == 1 */";
    }

  *os << be_uidt_nl
      << "};";

  os->gen_endif ();

  node->cli_hdr_gen (true);
  return 0;
}

void
be_visitor_sequence_ch::gen_varout_typedefs (be_sequence *node,
                                             be_type *elem)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

    AST_Type::SIZE_TYPE st = elem->size_type ();

    *os << "typedef" << be_idt_nl
        << (st == AST_Type::FIXED ? "::TAO_FixedSeq_Var_T<"
                                  : "::TAO_VarSeq_Var_T<")
        << be_idt << be_idt_nl
        << node->local_name ();

    *os << be_uidt_nl
        << ">" << be_uidt_nl
        << node->local_name () << "_var;" << be_uidt;

    *os << be_nl_2
        << "typedef" << be_idt_nl
        << "::TAO_Seq_Out_T<" << be_idt << be_idt_nl
        << node->local_name () << be_uidt_nl
        << ">" << be_uidt_nl
        << node->local_name () << "_out;" << be_uidt;
}
