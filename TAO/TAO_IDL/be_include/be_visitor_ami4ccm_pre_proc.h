//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_ami4ccm_pre_proc.h
//
// = DESCRIPTION
//    This visitor creates for AMI4CCM implied IDL constructs
//    the appropriate AST (Abstract Syntax Tree) node,
//    sets the corresponding interface or operation strategy
//    on it and enteres the nodes into the AST.
//
// = AUTHOR
//    Johnny Willemsen  <jwillemsen@remedy.nl>
//
// ============================================================================

#ifndef TAO_BE_VISITOR_AMI4CCM_PRE_PROC_H
#define TAO_BE_VISITOR_AMI4CCM_PRE_PROC_H

#include "be_visitor_scope.h"
#include "ace/SString.h"

class AST_Type;

class be_visitor_ami4ccm_pre_proc : public be_visitor_scope
{
  //
  // = TITLE
  //   AMI4CCM pre processor
  //
  // = DESCRIPTION
  //   Adds AMI4CCM implied IDL code to the AST
  //
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

  virtual int visit_root (be_root *node);
  // visit a root

  virtual int visit_module (be_module *node);
  // visit module

  virtual int visit_component (be_component *node);
  // visit component

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_operation (be_operation *node);
  // visit an operation

  virtual int visit_attribute (be_attribute *node);
  // visit an attribute

  virtual int visit_uses (be_uses *node);
  // visit an uses

private:
  int create_raise_operation (be_decl *node,
                              be_valuetype *excep_holder,
                              Operation_Kind operation_kind);
  // Creates a raise operation from node and inserts it in
  // excep_holder, while obeying if it is a normal operation
  // or a set or get attribute.

  be_interface *create_reply_handler (be_interface *node,
                                      be_valuetype *excep_holder);
  // create the reply handler interface

  be_interface *create_sendc_interface (be_interface *node,
                                      be_valuetype *excep_holder);
  // create the sendc interface
  
  int visit_scope (be_scope *node);
  // specialized visit_scope

  be_operation *create_sendc_operation (be_operation *node,
                                        int for_arguments);
  // create a method with "sendc_" prepended, if for_arguments
  // equals one the sendc operation contains also the
  // object reference to the reply handler as the first argument,
  // but this should not be marhaled, therefor we need the switch

  int create_excep_operation (be_operation *node,
                              be_interface *reply_handler,
                              be_valuetype *excep_holder);
  // create a method with "_excep" appended

  int create_reply_handler_operation (be_operation *node,
                                      be_interface *reply_handler);
  // create an operation with only the OUT and INOUT arguments

  //int visit_scope (be_scope *node);
  // specialized visit_scope

  int generate_name (ACE_CString &desintation,
                     const char *prefix,
                     const char *middle_name,
                     const char *suffix);
  // concatenate the prefix, middle_name and suffix
  // and store the result in desination.

  be_operation *generate_get_operation (be_attribute *node);
  // generate a get operation out of the attribute.

  be_operation *generate_set_operation (be_attribute *node);
  // generate a set operation out of the attribute.

  AST_Type **create_inheritance_list (be_interface *node,
                                      long &n_parents);
  // Create an inheritance list for the reply handler.

  be_uses * create_sendc_uses (be_uses *node);
};


#endif // TAO_BE_VISITOR_AMI4CCM_PRE_PROC_H
