/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Struct_Visitor.h
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_STRUCT_VISITOR_H
#define TAO_PSDL_STRUCT_VISITOR_H

#include /**/ "ace/pre.h"

#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Struct_Visitor : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Struct_Visitor ();

  ~TAO_PSDL_Struct_Visitor ();

  int visit_identifier (TAO_PSDL_Identifier *identifier);

  int visit_type_spec (TAO_PSDL_Type_Spec *type_spec);

  int visit_declarator (TAO_PSDL_Declarator *declarator);

  // For structs
  int visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type);

  // Generate the code that should go into the stub implementation
  // file for the type_declarators.
  void gen_code_for_si (void);

  // Generate the code that should go into the stub inline
  // file for the type_declarators.
  void gen_code_for_sinline (void);

 private:
  void print_class_for_structure (ACE_CString struct_name);

  // Counter to help print the type_specs and declarators of a
  // structure.
  int identifiers_count_;

  // structure_name
  const char *struct_name_;
};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_STRUCT_VISITOR_H */
