//=============================================================================
/**
 *  @file   be_visitor_amh_pre_proc.h
 *
 *  $Id$
 *
 *  This visitor creates for AMH implied IDL constructs the appropriate AST 
 *  (Abstract Syntax Tree) node, sets the corresponding interface or operation 
 *  strategy on it and enteres the nodes into the AST.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_AMH_PRE_PROC_H
#define TAO_BE_VISITOR_AMH_PRE_PROC_H

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_visitor_scope.h"

/**
 * @class be_visitor_amh_pre_proc
 *
 * @brief Adds AMH implied IDL code into the AST
 */
class be_visitor_amh_pre_proc : public be_visitor_scope
{
private:
 typedef AST_Interface *AST_Interface_ptr;

 typedef enum {
   NORMAL,
   GET_OPERATION,
   SET_OPERATION
 } Operation_Kind;

public:
  be_visitor_amh_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_amh_pre_proc (void);

  virtual int visit_root (be_root *node);

  virtual int visit_module (be_module *node);

  virtual int visit_interface (be_interface *node);

  virtual int visit_operation (be_operation *node);

  virtual int visit_attribute (be_attribute *node);

private:

  /// Creates a raise operation from node and inserts it in
  /// excep_holder, while obeying if it is a normal operation
  /// or a set or get attribute.
  int create_raise_operation (be_decl *node,
                              be_valuetype *excep_holder,
                              Operation_Kind operation_kind);

  /// Creates the AMH version of the interface
  be_interface *create_amh_class (be_interface *node);

  /// Create a method in the AMH tree
  int create_amh_operation (be_operation *node,
                            be_interface *amh_class);

  /// Create the reponse handler interface
  be_interface *create_response_handler (be_interface *node,
                                         be_valuetype *excep_holder);
 
  /// Create the exception holder valuetype
  be_valuetype *create_exception_holder (be_interface *node);

  /// Create a method with "sendc_" prepended, if for_arguments
  /// equals one the sendc operation contains also the
  /// object reference to the response handler as the first argument,
  /// but this should not be marhaled, therefore we need the switch
//  be_operation *create_sendc_operation (be_operation *node,
//                                        int for_arguments);

  /// Create a method with "_excep" appended
//  int create_excep_operation (be_operation *node,
//                              be_interface *response_handler,
//                              be_valuetype *excep_holder);

  /// Create an operation with return, OUT and INOUT arguments
  int create_response_handler_operation (be_operation *node,
                                         be_interface *response_handler);

  /// Specialized visit_scope
  int visit_scope (be_scope *node);

  /// Concatenate 3 strings into one.
  int generate_name (ACE_CString &destination,
                     const char *prefix,
                     const char *middle_name,
                     const char *suffix);

  /// Generate a get operation out of the attribute.
  be_operation *generate_get_operation (be_attribute *node);

  /// Generate a set operation out of the attribute.
  be_operation *generate_set_operation (be_attribute *node);
};


#endif // TAO_BE_VISITOR_AMH_PRE_PROC_H
