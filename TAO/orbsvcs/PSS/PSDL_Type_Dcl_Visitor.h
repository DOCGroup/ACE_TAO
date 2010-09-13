/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Type_Dcl_Visitor.h
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_TYPE_DCL_VISITOR_H
#define TAO_PSDL_TYPE_DCL_VISITOR_H

#include /**/ "ace/pre.h"
#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_PSDL_Type_Dcl_Visitor : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Type_Dcl_Visitor ();

  ~TAO_PSDL_Type_Dcl_Visitor ();

  // All type_dcl's except typedef (Refer to PSDL.yy for type_dcl's
  int visit_identifier (TAO_PSDL_Identifier *identifier);

  // For typedef s
  int visit_type_declarator (TAO_PSDL_Type_Declarator *node);

  int visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type);

private:

  void print_for_simple_typedefs (void);

  void print_for_sequence_typedefs (ACE_CString identifier_type,
                                    ACE_CString identifier);

  // Generate the code that should go into the stub implementation
  // file for the type_declarators.
  void gen_code_for_si (void);
};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_TYPE_DCL_VISITOR_H */
