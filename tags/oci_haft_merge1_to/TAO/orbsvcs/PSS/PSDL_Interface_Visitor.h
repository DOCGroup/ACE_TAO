/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Interface_Visitor.h
//
// = DESCRIPTION
//    Visitor class for the generated code for interfaces.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_INTERFACE_VISITOR_H
#define TAO_PSDL_INTERFACE_VISITOR_H

#include "ace/pre.h"

#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Interface_Visitor : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Interface_Visitor ();

  ~TAO_PSDL_Interface_Visitor ();

  // Override the necessary methods for interfaces (including forward
  // declarations).
  int visit_identifier (TAO_PSDL_Identifier *identifier);

  int visit_forward_dcl (TAO_PSDL_Forward_Dcl *forward_dcl);

  int visit_interface_dcl (TAO_PSDL_Interface_Dcl *interface_dcl);

  int visit_interface_header (TAO_PSDL_Interface_Header *interface_header);

  // Generate the code that should go into the stub implementation
  // file for the type_declarators.
  void gen_code_for_si (void);

 private:

  void print_for_forward_dcl (void);
  void print_class_for_interface (void);
  void print_end_for_interface (void);

  CORBA::Boolean forward_dcl_;
  ACE_CString interface_name_;
};

#include "ace/post.h"

#endif /* TAO_PSDL_INTERFACE_VISITOR_H */
