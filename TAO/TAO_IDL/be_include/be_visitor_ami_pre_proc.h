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
//    be_visitor_ami_pre_proc.h
//
// = DESCRIPTION
//    A visitor to generate the AMI implied IDL code.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef TAO_BE_VISITOR_AMI_PRE_PROC_H
#define TAO_BE_VISITOR_AMI_PRE_PROC_H

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_visitor_scope.h"


class be_visitor_ami_pre_proc : public be_visitor_scope
{
  //
  // = TITLE
  //   AMI pre processor
  //
  // = DESCRIPTION
  //   Adds AMI implied IDL code to the AST
  //
private:
 typedef AST_Interface *AST_Interface_ptr; 


public:
  be_visitor_ami_pre_proc (be_visitor_context *ctx);

  // constructor is protected
  virtual ~be_visitor_ami_pre_proc (void);

  virtual int visit_module (be_module *node);
  // visit module

  virtual int visit_interface (be_interface *node);
  // visit interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface_fwd

  virtual int visit_operation (be_operation *node);
  // visit an operation

  //virtual int visit_field (be_field *node);
  // visit a field

  virtual int visit_argument (be_argument *node);
  // visit argument

  virtual int visit_attribute (be_attribute *node);
  // visit an attribute

  virtual int visit_root (be_root *node);
  // visit a root

private:
  be_interface *create_reply_handler (be_interface *node);
  // create the reply handler interface

  be_valuetype *create_exception_holder (be_interface *node);
  // create the exception holder valuetype
  
  be_operation *create_sendc_operation (be_operation *node);
  // create a method with "sendc_" prepended

  be_operation *create_excep_operation (be_operation *node);
  // create a method with "_excep" appended

  be_operation *create_reply_handler_operation (be_operation *node);
  // create an operation with only the OUT and INOUT arguments

  int visit_scope (be_scope *node);
  // specialized visit_scope 

  int generate_name (ACE_CString &desintation,
                     const char *prefix,
                     const char *middle_name,
                     const char *suffix);
  // concatenate the prefix, middle_name and suffix
  // and store the result in desination.
};

#endif // TAO_BE_VISITOR_AMI_PRE_PROC_H
