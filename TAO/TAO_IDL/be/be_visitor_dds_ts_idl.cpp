
//=============================================================================
/**
 *  @file    be_visitor_dds_ts_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for DDS type support IDL files.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ********************************
// Visitor for DDS type support IDL
// ********************************

#include "be_visitor_dds_ts_idl.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_structure.h"
#include "be_union.h"
#include "be_valuetype.h"
#include "be_extern.h"
#include "be_util.h"

#include "utl_string.h"
#include "utl_identifier.h"

be_visitor_dds_ts_idl::be_visitor_dds_ts_idl (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    os_ptr_ (0)
{
}

be_visitor_dds_ts_idl::~be_visitor_dds_ts_idl (void)
{
}

int
be_visitor_dds_ts_idl::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_dds_ts_idl")
                         ACE_TEXT ("::visit_root - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_dds_ts_idl::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_dds_ts_idl")
                         ACE_TEXT ("::visit_module - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_dds_ts_idl::visit_structure (be_structure *node)
{
  return this->process_node (node);
}

int
be_visitor_dds_ts_idl::visit_union (be_union *node)
{
  return this->process_node (node);
}

int
be_visitor_dds_ts_idl::visit_valuetype (be_valuetype *node)
{
  return this->process_node (node);
}

int
be_visitor_dds_ts_idl::process_node (be_type *node)
{
  if (this->init_file (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_dds_ts_idl")
                         ACE_TEXT ("::process_node - ")
                         ACE_TEXT ("failed to initialize\n")),
                        -1);
    }

  be_util::gen_nesting_open (*this->os_ptr_, node);

  this->gen_datawriter (node);
  this->gen_datareader (node);

  be_util::gen_nesting_close (*this->os_ptr_, node);

  this->fini_file ();

  return 0;
}

void
be_visitor_dds_ts_idl::gen_datawriter (be_type *node)
{
  TAO_OutStream &os_ (*this->os_ptr_);
  const char *lname = node->local_name ()->get_string ();

  os_ << be_nl
      << "local interface " << lname
      << "DataWriter : ::DDS::DataWriter" << be_nl
      << "{" << be_idt;

  os_ << be_nl
      << "DDS::InstanceHandle_t register_instance (" << be_idt_nl
      << "in " << lname << " instance_data);" << be_uidt;

  os_ << be_nl_2
      << "DDS::InstanceHandle_t register_instance_w_timestamp ("
      << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::Time_t source_timestamp);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t unregister_instance (" << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::InstanceHandle_t handle);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t unregister_instance_w_timestamp ("
      << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::InstanceHandle_t handle," << be_nl
      << "in DDS::Time_t source_timestamp);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t write (" << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::InstanceHandle_t handle);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t write_w_timestamp (" << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::InstanceHandle_t handle," << be_nl
      << "in DDS::Time_t source_timestamp);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t dispose (" << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::InstanceHandle_t instance_handle);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t dispose_w_timestamp ("
      << be_idt_nl
      << "in " << lname << " instance_data," << be_nl
      << "in DDS::InstanceHandle_t instance_handle," << be_nl
      << "in DDS::Time_t source_timestamp);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t get_key_value (" << be_idt_nl
      << "inout " << lname << " key_holder," << be_nl
      << "in DDS::InstanceHandle_t handle);" << be_uidt;

  os_ << be_nl_2
      << "DDS::InstanceHandle_t lookup_instance ("
      << be_idt_nl
      << "in " << lname << " instance_data);" << be_uidt;

  os_ << be_uidt_nl
      << "};";
}

void
be_visitor_dds_ts_idl::gen_datareader (be_type *node)
{
  TAO_OutStream &os_ (*this->os_ptr_);
  const char *lname = node->local_name ()->get_string ();

  os_ << be_nl_2
      << "local interface " << lname
      << "DataReader : ::DDS::DataReader" << be_nl
      << "{" << be_idt;

  os_ << be_nl
      << "DDS::ReturnCode_t read (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::SampleStateMask sample_states," << be_nl
      << "in DDS::ViewStateMask view_states," << be_nl
      << "in DDS::InstanceStateMask instance_states);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t take (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::SampleStateMask sample_states," << be_nl
      << "in DDS::ViewStateMask view_states," << be_nl
      << "in DDS::InstanceStateMask instance_states);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t read_w_condition (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::ReadCondition a_condition);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t take_w_condition (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::ReadCondition a_condition);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t read_next_sample (" << be_idt_nl
      << "inout " << lname << " data_values," << be_nl
      << "inout DDS::SampleInfo sample_info);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t take_next_sample (" << be_idt_nl
      << "inout " << lname << " data_values," << be_nl
      << "inout DDS::SampleInfo sample_info);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t read_instance (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::InstanceHandle_t a_handle," << be_nl
      << "in DDS::SampleStateMask sample_states," << be_nl
      << "in DDS::ViewStateMask view_states," << be_nl
      << "in DDS::InstanceStateMask instance_states);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t take_instance (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::InstanceHandle_t a_handle," << be_nl
      << "in DDS::SampleStateMask sample_states," << be_nl
      << "in DDS::ViewStateMask view_states," << be_nl
      << "in DDS::InstanceStateMask instance_states);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t read_next_instance (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::InstanceHandle_t previous_handle," << be_nl
      << "in DDS::SampleStateMask sample_states," << be_nl
      << "in DDS::ViewStateMask view_states," << be_nl
      << "in DDS::InstanceStateMask instance_states);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t take_next_instance (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::InstanceHandle_t previous_handle," << be_nl
      << "in DDS::SampleStateMask sample_states," << be_nl
      << "in DDS::ViewStateMask view_states," << be_nl
      << "in DDS::InstanceStateMask instance_states);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t read_next_instance_w_condition ("
      << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::InstanceHandle_t previous_handle," << be_nl
      << "in DDS::ReadCondition a_condition);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t take_next_instance_w_condition ("
      << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos," << be_nl
      << "in long max_samples," << be_nl
      << "in DDS::InstanceHandle_t previous_handle," << be_nl
      << "in DDS::ReadCondition a_condition);" << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t return_loan (" << be_idt_nl
      << "inout " << lname << "Seq data_values," << be_nl
      << "inout DDS::SampleInfoSeq sample_infos);"
      << be_uidt;

  os_ << be_nl_2
      << "DDS::ReturnCode_t get_key_value (" << be_idt_nl
      << "inout " << lname << " key_holder," << be_nl
      << "in DDS::InstanceHandle_t handle);" << be_uidt;

  os_ << be_nl_2
      << "DDS::InstanceHandle_t lookup_instance ("
      << be_idt_nl
      << "in " << lname << " instance_data);" << be_uidt;

  os_ << be_uidt_nl
      << "};";
}

int
be_visitor_dds_ts_idl::init_file (be_decl *node)
{
  /// Open a uniquely-named IDL file for writing.
  ACE_CString fname_noext (node->flat_name ());
  fname_noext += "TypeSupport";

  ACE_CString fname (fname_noext);
  fname += ".idl";

  ACE_NEW_RETURN (this->os_ptr_,
                  TAO_OutStream,
                  -1);

  int status = this->os_ptr_->open (fname.c_str ());

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_root_ts_idl::init - ")
                         ACE_TEXT ("Error opening DDS type support ")
                         ACE_TEXT ("IDL file\n")),
                        -1);
    }

  this->os_ptr_->gen_ifdef_macro (fname_noext.c_str (),
                                  "IDL",
                                  false);

  *this->os_ptr_ << be_nl_2
                 << "#include <dds_rtf2_dcps.idl>" << be_nl_2
                 << "#include \""
                 << idl_global->filename ()->get_string ()
                 << "\"";

  return 0;
}

void
be_visitor_dds_ts_idl::fini_file (void)
{
  *this->os_ptr_ << be_nl_2
                 << "#endif /* ifndef */\n"
                 << "\n";

  delete this->os_ptr_;
  this->os_ptr_ = 0;
}
