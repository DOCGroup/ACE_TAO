/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_dds_ex_base.h
 *
 *  Base class visitor for the DDS Connector node.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_DDS_EX_BASE_H_
#define _BE_CONNECTOR_CONNECTOR_DDS_EX_BASE_H_

#include <string>

class be_visitor_context;
class AST_Template_Module;

/// Base visitor for generating DDS connector executor implementations.
class be_visitor_connector_dds_ex_base
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_connector_dds_ex_base (be_visitor_context *ctx);

  ~be_visitor_connector_dds_ex_base () override = default;
  be_visitor_connector_dds_ex_base (be_visitor_connector_dds_ex_base const &) = delete;
  be_visitor_connector_dds_ex_base (be_visitor_connector_dds_ex_base &&) = delete;
  be_visitor_connector_dds_ex_base &operator= (be_visitor_connector_dds_ex_base const &) = delete;
  be_visitor_connector_dds_ex_base &operator= (be_visitor_connector_dds_ex_base &&) = delete;

protected:
  bool begin (be_connector *node);

  bool is_dds_type (be_connector *node, AST_Decl *d);

protected:
  /// Current template module instantiation and its template arguments.
  AST_Template_Module_Inst *t_inst_ {};
  AST_Template_Module_Ref *t_ref_ {};
  FE_Utils::T_ARGLIST t_args_;
  FE_Utils::T_PARAMLIST_INFO *t_params_ {};

  /// Traits name derived from the DDS datatype and calculated once.
  std::string dds_traits_name_;
  std::string base_tname_;

private:
  void process_template_args (AST_Connector *node);
  void match_template_args ();
};

#endif /* _BE_CONNECTOR_CONNECTOR_DDS_EX_BASE_H_ */

