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
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// "__FILE__ << ":" << __LINE__;

  *os << be_nl
      << "\n#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)" << be_idt_nl;

  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      {
        be_interface *elem = be_interface::narrow_from_decl (bt);

        if (node->unbounded ())
          {
            *os << "template class" << be_idt_nl 
                << "TAO_Unbounded_Object_Sequence<" << be_idt << be_idt_nl
                << elem->name () << "," << be_nl
                << elem->name () << "_var," << be_nl
                << elem->fwd_helper_name () << "_life," << be_nl
                << elem->fwd_helper_name () << "_cast" << be_uidt_nl
                << ">;" << be_uidt << be_uidt_nl;
          }
        else
          {
            *os << "template class" << be_idt_nl 
                << "TAO_Bounded_Object_Sequence<" << be_idt << be_idt_nl
                << elem->name () << "," << be_nl
                << elem->name () << "_var," << be_nl
                << elem->fwd_helper_name () << "_life," << be_nl
                << elem->fwd_helper_name () << "_cast," << be_nl
                << node->max_size ()->ev ()->u.ulval << be_uidt_nl
                << ">;" << be_uidt << be_uidt_nl;
          }

        *os << "template class" << be_idt_nl
            << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            << "TAO_Object_Manager<" << be_idt << be_idt_nl
            << elem->name () << "," << be_nl
            << elem->name () << "_var," << be_nl
            << elem->fwd_helper_name () << "_life" << be_uidt_nl
            << ">" << be_uidt << be_uidt_nl
            << ">;" << be_uidt << be_uidt_nl;

        *os << "template class" << be_idt_nl
            << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
            << node->name () << "," << be_nl
            << node->name () << "_var," << be_nl
            << "TAO_Object_Manager<" << be_idt << be_idt_nl
            << elem->name () << "," << be_nl
            << elem->name () << "_var," << be_nl
            << elem->fwd_helper_name () << "_life" << be_uidt_nl
            << ">" << be_uidt << be_uidt_nl
            << ">;";
      }

      break;
    case be_sequence::MNG_ABSTRACT:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_PSEUDO:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_VALUE:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_STRING:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    default: // not a managed type
      if (bt->base_node_type () == AST_Decl::NT_array)
        {
          if (node->unbounded ())
            {
            }
          else
            {
            }
        }
      else
        {
          if (node->unbounded ())
            {
            }
          else
            {
            }
        }

      break;
    }

  *os << be_uidt << be_uidt << be_uidt_nl
      << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)" << be_nl;
     
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      {
        be_interface *elem = be_interface::narrow_from_decl (bt);

        if (node->unbounded ())
          {
            *os << "# pragma instantiate \\" << be_idt << be_idt_nl
                << "TAO_Unbounded_Object_Sequence< \\" << be_idt << be_idt_nl
                << elem->name () << ", \\" << be_nl
                << elem->name () << "_var, \\" << be_nl
                << elem->fwd_helper_name () << "_life, \\" << be_nl
                << elem->fwd_helper_name () << "_cast, \\" << be_uidt_nl
                << ">" << be_uidt << be_uidt << be_uidt_nl;
          }
        else
          {
            *os << "# pragma instantiate \\" << be_idt << be_idt_nl
                << "TAO_Bounded_Object_Sequence< \\" << be_idt << be_idt_nl
                << elem->name () << ", \\" << be_nl
                << elem->name () << "_var, \\" << be_nl
                << elem->fwd_helper_name () << "_life, \\" << be_nl
                << elem->fwd_helper_name () << "_cast \\" << be_nl
                << node->max_size ()->ev ()->u.ulval << " \\" << be_uidt_nl
                << ">" << be_uidt << be_uidt << be_uidt_nl;
          }

        *os << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
            << elem->name () << ", \\" << be_nl
            << elem->name () << "_var, \\" << be_nl
            << elem->fwd_helper_name () << "_life \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">" << be_uidt << be_uidt << be_uidt_nl;

        *os << "# pragma instantiate \\" << be_idt << be_idt_nl
            << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
            << node->name () << ", \\" << be_nl
            << node->name () << "_var, \\" << be_nl
            << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
            << elem->name () << ", \\" << be_nl
            << elem->name () << "_var, \\" << be_nl
            << elem->fwd_helper_name () << "_life \\" << be_uidt_nl
            << "> \\" << be_uidt << be_uidt_nl
            << ">";
      }

      break;
    case be_sequence::MNG_ABSTRACT:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_PSEUDO:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_VALUE:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_STRING:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (node->unbounded ())
        {
        }
      else
        {
        }

      break;
    default: // not a managed type
      if (bt->base_node_type () == AST_Decl::NT_array)
        {
          if (node->unbounded ())
            {
            }
          else
            {
            }
        }
      else
        {
          if (node->unbounded ())
            {
            }
          else
            {
            }
        }

      break;
    }

  *os << be_uidt << be_uidt << be_uidt_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */";

  node->cli_stub_gen (1);
  return 0;
}

