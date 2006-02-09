//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Sequences in the client stubs file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence,
           sequence_cs,
           "$Id$")

// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_sequence_cs::be_visitor_sequence_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_cs::~be_visitor_sequence_cs (void)
{
}

int be_visitor_sequence_cs::visit_sequence (be_sequence *node)
{
  if (node->imported () || node->cli_stub_gen ())
    {
      return 0;
    }

  be_type *bt = be_type::narrow_from_decl (node->base_type ());
  AST_Decl::NodeType nt = bt->node_type ();

  // If our base type is an anonymous sequence, generate code for it here.
  if (nt == AST_Decl::NT_sequence)
    {
      if (bt->accept (this) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                            "codegen for anonymous base type failed\n"),
                           -1);
        }

    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// "__FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name ());

  // default constructor
  *os << be_nl << be_nl
      << node->name () << "::" << node->local_name () << " (void)" << be_nl
      << "{}";

  // for unbounded sequences, we have a different set of constructors
  if (node->unbounded ())
    {
      *os << be_nl << be_nl
          << node->name () << "::" << node->local_name () << " ("
          << be_idt << be_idt_nl
          << "::CORBA::ULong max" << be_uidt_nl
          << ")" << be_nl
          << ": " << be_idt;

      // Pass it to the base constructor.
      if (node->gen_base_class_name (os, "", this->ctx_->scope ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_sequence_cs::"
                             "visit_sequence - "
                             "codegen for base sequence class failed\n"),
                           -1);
        }


      *os << be_nl << "(max)" << be_uidt << be_uidt_nl
          << "{}";
    }

  // constructor with the buffer
  *os << be_nl << be_nl
      << node->name () << "::" << node->local_name () << " ("
      << be_idt << be_idt_nl;

  if (node->unbounded ())
    {
      // Unbounded seq takes this extra parameter.
      *os << "::CORBA::ULong max," << be_nl;
    }

  *os << "::CORBA::ULong length," << be_nl;

  // generate the base type for the buffer
  be_visitor_context ctx (*this->ctx_);
  be_visitor_sequence_buffer_type bt_visitor (&ctx);

  if (bt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "base type visit failed\n"),
                        -1);
    }

  *os << " * buffer," << be_nl
      << "::CORBA::Boolean release" << be_uidt_nl
      << ")" << be_uidt_nl
      << "  : " << be_idt << be_idt;

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os, "", this->ctx_->scope ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"),
                        -1);
    }

  *os << be_nl << "(";

  if (node->unbounded ())
    {
      *os << "max, ";
    }

  *os << "length, buffer, release)" << be_uidt << be_uidt_nl
      << "{}";

  // Copy constructor.
  *os << be_nl << be_nl
      << node->name () << "::" << node->local_name ()
      << " (" << be_idt << be_idt_nl
      << "const " << node->local_name ()
      << " &seq" << be_uidt_nl
      << ")" << be_uidt_nl
      << "  : " << be_idt << be_idt;

  // Pass it to the base constructor.
  if (node->gen_base_class_name (os, "", this->ctx_->scope ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_sequence_cs::"
                         "visit_sequence - "
                         "codegen for base sequence class\n"),
                        -1);
    }

  *os << be_nl << "(seq)" << be_uidt << be_uidt_nl
      << "{}";

  // Destructor.
  *os << be_nl << be_nl
      << node->name () << "::~" << node->local_name ()
      << " (void)" << be_nl
      << "{}";


  if (be_global->any_support () && !node->anonymous ())
    {
      *os << be_nl << be_nl
          << "void "
          << node->name () << "::_tao_any_destructor (" << be_idt << be_idt_nl
          << "void * _tao_void_pointer" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << node->local_name () << " * _tao_tmp_pointer =" << be_idt_nl
          << "static_cast<" << node->local_name ()
          << " *> (_tao_void_pointer);" << be_uidt_nl
          << "delete _tao_tmp_pointer;" << be_uidt_nl
          << "}";
    }

  os->gen_endif ();

  node->cli_stub_gen (true);
  return 0;
}
