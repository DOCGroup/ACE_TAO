
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_ami4ccm_pre_proc.h
 *
 *  $Id$
 *
 *  This visitor creates for AMI4CCM implied IDL constructs
 *  the appropriate AST (Abstract Syntax Tree) node,
 *  sets the corresponding interface or operation strategy
 *  on it and enteres the nodes into the AST.
 *
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_BE_VISITOR_AMI4CCM_PRE_PROC_H
#define TAO_BE_VISITOR_AMI4CCM_PRE_PROC_H

#include "be_visitor_scope.h"
#include "ace/SString.h"

class AST_Type;

/**
 * @class be_visitor_ami4ccm_pre_proc
 *
 * @brief AMI4CCM pre processor
 *
 * Adds AMI4CCM implied IDL code to the AST
 */
class be_visitor_ami4ccm_pre_proc : public be_visitor_scope
{
private:
 typedef enum
 {
   NORMAL,
   GET_OPERATION,
   SET_OPERATION
 } Operation_Kind;

public:
  be_visitor_ami4ccm_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_ami4ccm_pre_proc (void);

  /// visit a root
  virtual int visit_root (be_root *node);

  /// visit module
  virtual int visit_module (be_module *node);

  /// visit component
  virtual int visit_component (be_component *node);

  /// visit interface
  virtual int visit_interface (be_interface *node);

  /// visit an operation
  virtual int visit_operation (be_operation *node);

  /// visit an attribute
  virtual int visit_attribute (be_attribute *node);

  /// visit an uses
  virtual int visit_uses (be_uses *node);

private:
  /**
   * Creates a raise operation from node and inserts it in
   * excep_holder, while obeying if it is a normal operation
   * or a set or get attribute.
   */
  int create_raise_operation (be_decl *node,
                              Operation_Kind operation_kind);

  /// create the reply handler interface
  be_interface *create_reply_handler (be_interface *node);

  /// create the ami4ccm sendc interface
  be_interface *create_ami_sendc_interface (be_interface *node);

  /// create the sendc interface
  be_interface *create_sendc_interface (be_interface *node);
  
  /// specialized visit_scope
  int visit_scope (be_scope *node);

  /**
   * create a method with "sendc_" prepended, if for_arguments
   * equals one the sendc operation contains also the
   * object reference to the reply handler as the first argument,
   * but this should not be marhaled, therefor we need the switch
   */
  be_operation *create_sendc_operation (be_operation *node,
                                        bool for_arguments);

  /// create a method with "_excep" appended
  int create_excep_operation (be_operation *node,
                              be_interface *reply_handler);

  /// create an operation with only the OUT and INOUT arguments
  int create_reply_handler_operation (be_operation *node,
                                      be_interface *reply_handler);

  //int visit_scope (be_scope *node);
  // specialized visit_scope

  /// concatenate the prefix, middle_name and suffix
  /// and store the result in desination.
  int generate_name (ACE_CString &desintation,
                     const char *prefix,
                     const char *middle_name,
                     const char *suffix);

  /// generate a get operation out of the attribute.
  be_operation *generate_get_operation (be_attribute *node);

  /// generate a set operation out of the attribute.
  be_operation *generate_set_operation (be_attribute *node);

  /// Create an inheritance list for the reply handler.
  AST_Type **create_inheritance_list (be_interface *node,
                                      long &n_parents);

  be_uses * create_sendc_uses (be_uses *node);
};


#endif // TAO_BE_VISITOR_AMI4CCM_PRE_PROC_H
