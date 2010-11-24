/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_dds_ex_base.h
 *
 *  $Id$
 *
 *  Base class visitor for the DDS Connector node.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EX_BASE_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EX_BASE_H_

class be_visitor_context;

class be_visitor_connector_dds_ex_base
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_connector_dds_ex_base
  //
  // = DESCRIPTION
  //   This is a base class visitor to generate
  //   the executor implementation for DDS connectors.
  //
  //
public:
  be_visitor_connector_dds_ex_base (be_visitor_context *ctx);

  ~be_visitor_connector_dds_ex_base (void);

protected:
  bool begin (be_connector *node);

  bool is_dds_type (be_connector *node, AST_Decl *d);

protected:
  // Storage for the current template module instantiation we
  // are using, and its associated template args.
  AST_Template_Module_Inst *t_inst_;
  FE_Utils::T_ARGLIST *t_args_;

  // Derives from the DDS datatype, so we want to calculate it
  // once and store it.
  ACE_CString dds_traits_name_;
  const char *base_tname_;

private:
  void process_template_args (AST_Connector *node);
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EX_BASE_H_ */

