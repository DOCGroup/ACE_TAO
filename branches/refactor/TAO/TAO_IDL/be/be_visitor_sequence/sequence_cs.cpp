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
  AST_Type::SIZE_TYPE st = bt->size_type ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from " << be_nl
      << "// "__FILE__ << ":" << __LINE__;

  os->gen_ifdef_AHETI ();

  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Unbounded_Object_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life," << be_nl
              << bt->fwd_helper_name () << "_cast" << be_uidt_nl
              << ">;" << be_uidt << be_uidt_nl;
        }
      else
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Bounded_Object_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life," << be_nl
              << bt->fwd_helper_name () << "_cast," << be_nl
              << node->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << "TAO_Object_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
              << "TAO_Object_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_ABSTRACT:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Unbounded_Abstract_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Bounded_Abstract_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life," << be_nl
              << node->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
              << "TAO_Abstract_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
            }

      break;
    case be_sequence::MNG_PSEUDO:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Unbounded_Pseudo_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Bounded_Pseudo_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << node->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
              << "TAO_Pseudo_Object_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
            }

      break;
    case be_sequence::MNG_VALUE:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Unbounded_Valuetype_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl 
              << "TAO_Bounded_Valuetype_Sequence<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life," << be_nl
              << node->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
              << "TAO_Valuetype_Manager<" << be_idt << be_idt_nl
              << bt->name () << "," << be_nl
              << bt->name () << "_var," << be_nl
              << bt->fwd_helper_name () << "_life" << be_uidt_nl
              << ">" << be_uidt << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
            }

      break;
    case be_sequence::MNG_STRING:
      if (! node->unbounded ())
        {
          *os << be_nl << be_nl
              << "template class TAO_Bounded_String_Sequence<" << be_nl
              << node->max_size ()->ev ()->u.ulval << ">;";
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              <<"TAO_SeqElem_String_Manager" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
              << "TAO_SeqElem_String_Manager" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (! node->unbounded ())
        {
          *os << be_nl << be_nl
              << "template class TAO_Bounded_WString_Sequence<"
              << node->max_size ()->ev ()->u.ulval << ">;";
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_VarSeq_Var_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              <<"TAO_SeqElem_WString_Manager" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->name () << "," << be_nl
              << node->name () << "_var," << be_nl
              << "TAO_SeqElem_WString_Manager" << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      break;
    default: // not a managed type
      if (bt->base_node_type () == AST_Decl::NT_array)
        {
          if (node->unbounded ())
            {
              *os << be_nl << be_nl
                  << "template class" << be_idt_nl
                  << "TAO_Unbounded_Array_Sequence<"
                  << be_idt << be_idt_nl
                  << bt->name () << "," << be_nl
                  << bt->fwd_helper_name () << "_life" << be_uidt_nl
                  << ">;" << be_uidt << be_uidt;
            }
          else
            {
              *os << be_nl << be_nl
                  << "template class" << be_idt_nl
                  << "TAO_Bounded_Array_Sequence<"
                  << be_idt << be_idt_nl
                  << bt->name () << "," << be_nl
                  << bt->fwd_helper_name () << "_life," << be_nl
                  << node->max_size ()->ev ()->u.ulval << be_uidt_nl
                  << ">;" << be_uidt << be_uidt;
            }
        }
      else
        {
          if (node->unbounded ())
            {
              *os << be_nl << be_nl
                  << "template class" << be_idt_nl
                  << "TAO_Unbounded_Sequence<"
                  << be_idt << be_idt_nl
                  << bt->name () << be_uidt_nl
                  << ">;" << be_uidt << be_uidt;
            }
          else
            {
              *os << be_nl << be_nl
                  << "template class" << be_idt_nl
                  << "TAO_Bounded_Sequence<"
                  << be_idt << be_idt_nl
                  << bt->name () << "," << be_nl
                  << node->max_size ()->ev ()->u.ulval << be_uidt_nl
                  << ">;" << be_uidt << be_uidt;
            }
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << (st == AST_Type::FIXED ? "TAO_FixedSeq_Var_T<" 
                                        : "TAO_VarSeq_Var_T<")
              << be_idt << be_idt_nl
              << node->local_name () << "," << be_nl
              << bt->name () << be_uidt_nl
              << ">;" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "template class" << be_idt_nl
              << "TAO_Seq_Out_T<" << be_idt << be_idt_nl
              << node->local_name () << "," << be_nl
              << node->local_name () << "_var," << be_nl
              << bt->name () << be_uidt_nl
              << ">;" << be_uidt << be_uidt;
        }

      break;
    }

  os->gen_elif_AHETI ();
     
  switch (node->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt << be_idt_nl
              << "TAO_Unbounded_Object_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life, \\" << be_nl
              << bt->fwd_helper_name () << "_cast, \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt << be_idt_nl
              << "TAO_Bounded_Object_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life, \\" << be_nl
              << bt->fwd_helper_name () << "_cast \\" << be_nl
              << node->max_size ()->ev ()->u.ulval << " \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt << be_idt_nl
              << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << node->name () << "_var, \\" << be_nl
              << "TAO_Object_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_ABSTRACT:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl 
              << "TAO_Unbounded_Abstract_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl 
              << "TAO_Bounded_Abstract_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life, \\" << be_nl
              << node->max_size ()->ev ()->u.ulval << " \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << "TAO_Abstract_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << node->name () << "_var, \\" << be_nl
              << "TAO_Abstract_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_PSEUDO:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl 
              << "TAO_Unbounded_Pseudo_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl 
              << "TAO_Bounded_Pseudo_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << node->max_size ()->ev ()->u.ulval << " \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << "TAO_Pseudo_Object_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << node->name () << "_var, \\" << be_nl
              << "TAO_Pseudo_Object_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_VALUE:
      if (node->unbounded ())
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl 
              << "TAO_Unbounded_Valuetype_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }
      else
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl 
              << "TAO_Bounded_Valuetype_Sequence< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life, \\" << be_nl
              << node->max_size ()->ev ()->u.ulval << " \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << "TAO_Valuetype_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << node->name () << "_var, \\" << be_nl
              << "TAO_Valuetype_Manager< \\" << be_idt << be_idt_nl
              << bt->name () << ", \\" << be_nl
              << bt->name () << "_var, \\" << be_nl
              << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
              << "> \\" << be_uidt << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_STRING:
      if (! node->unbounded ())
        {
          *os << be_nl << be_nl
              << "# pragma instantiate TAO_Bounded_String_Sequence< \\" << be_nl
              << node->max_size ()->ev ()->u.ulval << ">";
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              <<"TAO_SeqElem_String_Manager \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << node->name () << "_var, \\" << be_nl
              << "TAO_SeqElem_String_Manager \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (! node->unbounded ())
        {
          *os << be_nl << be_nl
              << "# pragma instantiate TAO_Bounded_WString_Sequence<"
              << node->max_size ()->ev ()->u.ulval << ">";
        }

      // Instantiate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_VarSeq_Var_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              <<"TAO_SeqElem_WString_Manager \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->name () << ", \\" << be_nl
              << node->name () << "_var, \\" << be_nl
              << "TAO_SeqElem_WString_Manager \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    default: // not a managed type
      if (bt->base_node_type () == AST_Decl::NT_array)
        {
          if (node->unbounded ())
            {
              *os << be_nl << be_nl
                  << "# pragma instantiate \\" << be_idt_nl
                  << "TAO_Unbounded_Array_Sequence< \\"
                  << be_idt << be_idt_nl
                  << bt->name () << ", \\" << be_nl
                  << bt->fwd_helper_name () << "_life \\" << be_uidt_nl
                  << ">" << be_uidt << be_uidt;
            }
          else
            {
              *os << be_nl << be_nl
                  << "# pragma instantiate \\" << be_idt_nl
                  << "TAO_Bounded_Array_Sequence< \\"
                  << be_idt << be_idt_nl
                  << bt->name () << ", \\" << be_nl
                  << bt->fwd_helper_name () << "_life, \\" << be_nl
                  << node->max_size ()->ev ()->u.ulval 
                  << " \\" << be_uidt_nl
                  << ">" << be_uidt << be_uidt;
            }
        }
      else
        {
          if (node->unbounded ())
            {
              *os << be_nl << be_nl
                  << "# pragma instantiate \\" << be_idt_nl
                  << "TAO_Unbounded_Sequence< \\"
                  << be_idt << be_idt_nl
                  << bt->name () << " \\" << be_uidt_nl
                  << ">" << be_uidt << be_uidt;
            }
          else
            {
              *os << be_nl << be_nl
                  << "# pragma instantiate \\" << be_idt_nl
                  << "TAO_Bounded_Sequence< \\"
                  << be_idt << be_idt_nl
                  << bt->name () << ", \\" << be_nl
                  << node->max_size ()->ev ()->u.ulval 
                  << " \\" << be_uidt_nl
                  << ">" << be_uidt << be_uidt;
            }
        }

      // Generate the _var and _out types only if we are not anonymous.
      if (this->ctx_->tdef () != 0)
        {
          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << (st == AST_Type::FIXED ? "TAO_FixedSeq_Var_T< \\" 
                                        : "TAO_VarSeq_Var_T< \\")
              << be_idt << be_idt_nl
              << node->local_name () << ", \\" << be_nl
              << bt->name () << " \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;

          *os << be_nl << be_nl
              << "# pragma instantiate \\" << be_idt_nl
              << "TAO_Seq_Out_T< \\" << be_idt << be_idt_nl
              << node->local_name () << ", \\" << be_nl
              << node->local_name () << "_var, \\" << be_nl
              << bt->name () << " \\" << be_uidt_nl
              << ">" << be_uidt << be_uidt;
        }

      break;
    }

  os->gen_endif_AHETI ();

  node->cli_stub_gen (1);
  return 0;
}

