
//=============================================================================
/**
 *  @file    serializer_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TAO::DCPS::Serializer operators for
 *  structures. This uses compiled marshaling.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#include "global_extern.h"

// ***************************************************************************
// Structure visitor for generating Serializer operator declarations in the client header
// ***************************************************************************

be_visitor_structure_serializer_op_ch::be_visitor_structure_serializer_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_serializer_op_ch::~be_visitor_structure_serializer_op_ch (void)
{
}

int
be_visitor_structure_serializer_op_ch::visit_structure (be_structure *node)
{
  if (node->cli_hdr_serializer_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  IDL_GlobalData::DCPS_Data_Type_Info* info;
  if (0 != (info =  idl_global->is_dcps_type(node->name())))
    {
      *os << be_global->stub_export_macro ()
          << " ::CORBA::Boolean _dcps_has_key(const "
          << node->name() << "& val);" << be_nl;

      *os << "// This structure supports use of std::map with a key" << be_nl
          << "// defined by one or more #pragma DCPS_DATA_KEY lines." << be_nl
          << "struct " << be_global->stub_export_macro () << " "
          << node->name()->last_component() << "KeyLessThan " << be_nl
          << "{" << be_idt_nl
          << "bool operator() (" << be_idt << be_idt_nl
          << "const " << node->name() << "& v1," << be_nl
          << "const " << node->name() << "& v2) const"  << be_uidt << be_uidt_nl
          << "{" << be_idt_nl;

      if (info->key_list_.is_empty())
        {
          *os << "ACE_UNUSED_ARG(v1);" << be_nl;
          *os << "ACE_UNUSED_ARG(v2);" << be_nl;
          *os << "// if no DCPS_DATA_KEY values then return false" << be_nl
              << "// to allow use of map with just one entry" << be_nl;
          *os << "return false;" << be_nl;
        }
      else
        {
          *os << "return ";
          IDL_GlobalData::DCPS_Data_Type_Info_Iter iter (info->key_list_);

          IDL_GlobalData::DCPS_Data_Type_Info_Iter begin = iter;

          for (ACE_TString *kp = 0;
               iter.next (kp) != 0;
               )
            {
              *os << "(";
              IDL_GlobalData::DCPS_Data_Type_Info_Iter prev = begin;
              for (ACE_TString *p = 0;
               prev.next (p) != 0 && p != kp;
               )
              {
                *os << "v1." << ACE_TEXT_ALWAYS_CHAR (p->c_str ()) << " == " << "v2." << ACE_TEXT_ALWAYS_CHAR (p->c_str ()) << " && ";
                prev.advance ();
              }

              *os << "v1." << ACE_TEXT_ALWAYS_CHAR (kp->c_str ()) << " < " << "v2." << ACE_TEXT_ALWAYS_CHAR (kp->c_str ());
              *os << ")";

              iter.advance ();
              if (iter.done ())
                *os << ";" << be_uidt_nl;
              else
                *os << " || " << be_nl;
            }
        }
      *os << "}" << be_uidt_nl
          << "};" << be_nl << be_nl;
    }

  //- _dcps_max_marshaled_size(type) method
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_max_marshaled_size (const "
      << node->name() << "& _tao_aggregate);" << be_nl;
  //- _tao_is_bounded_size(type) method
  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean _tao_is_bounded_size (const "
      << node->name() << "& _tao_aggregate);" << be_nl;
  //- _dcps_find_size(type) method
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_find_size (const "
      << node->name() << "& _tao_aggregate);" << be_nl;

  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (TAO::DCPS::Serializer &, const " << node->name ()
      << " &);" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (TAO::DCPS::Serializer &, "
      << node->name () << " &);" << be_nl;

  //FUTURE: This can be excluded if the OpenDDS is excluding the DDS
  //        content-subscription profile at compile-time.
  *os << be_nl << "#ifdef DDS_USE_QUERY_CONDITION_COMPARATOR" << be_nl
      << "namespace OpenDDS" << be_nl
      << "{" << be_idt_nl
      << "namespace DCPS" << be_nl
      << "{" << be_idt_nl
      << be_global->stub_export_macro () << be_nl
      << "ComparatorBase::Ptr create_qc_comparator (" << node->name ()
      << " *, const char *field, ComparatorBase::Ptr next);" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "#endif" << be_nl;

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_serializer_op_ch::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }


  node->cli_hdr_serializer_op_gen (1);
  return 0;
}
