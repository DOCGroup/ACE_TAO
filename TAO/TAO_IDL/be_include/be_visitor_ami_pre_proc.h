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
  int create_sendc_method (be_operation *node);
  // create a method with "sendc_" prepended

  int create_excep_method (be_operation *node);
  // create a method with "_excep" appended

  int visit_scope (be_scope *node);
  // specialized visit_scope 

};

#endif // TAO_BE_VISITOR_AMI_PRE_PROC_H
