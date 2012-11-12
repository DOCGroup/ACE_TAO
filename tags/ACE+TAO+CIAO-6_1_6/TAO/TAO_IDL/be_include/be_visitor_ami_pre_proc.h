
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_ami_pre_proc.h
 *
 *  $Id$
 *
 *  This visitor creates for AMI implied IDL constructs
 *  the appropriate AST (Abstract Syntax Tree) node,
 *  sets the corresponding interface or operation strategy
 *  on it and enteres the nodes into the AST.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_AMI_PRE_PROC_H
#define TAO_BE_VISITOR_AMI_PRE_PROC_H

#include "be_visitor_scope.h"
#include "ace/SString.h"

class AST_Type;

/**
 * @class be_visitor_ami_pre_proc
 *
 * @brief AMI pre processor
 *
 * Adds AMI implied IDL code to the AST
 */
class be_visitor_ami_pre_proc : public be_visitor_scope
{
private:
 typedef enum
 {
   NORMAL,
   GET_OPERATION,
   SET_OPERATION
 } Operation_Kind;

public:
  be_visitor_ami_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_ami_pre_proc (void);

  virtual int visit_root (be_root *node);

  virtual int visit_module (be_module *node);

  virtual int visit_interface (be_interface *node);

  virtual int visit_operation (be_operation *node);

  virtual int visit_attribute (be_attribute *node);

private:
  /// create the reply handler interface
  be_interface *create_reply_handler (be_interface *node);

  /**
   * create a method with "sendc_" prepended, if for_arguments
   * equals one the sendc operation contains also the
   * object reference to the reply handler as the first argument,
   * but this should not be marhaled, therefor we need the switch
   */
  be_operation *create_sendc_operation (be_operation *node);

  /// create a method with "_excep" appended
  int create_excep_operation (be_operation *node,
                              be_interface *reply_handler);

  /// create an operation with only the OUT and INOUT arguments
  int create_reply_handler_operation (be_operation *node,
                                      be_interface *reply_handler);

  /// specialized visit_scope
  int visit_scope (be_scope *node);

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

  /// Generate an *A.idl file for AMI4CCM.
  int generate_ami4ccm_idl (void);
};

#endif // TAO_BE_VISITOR_AMI_PRE_PROC_H
