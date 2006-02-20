/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Enum_Type_Visitor.h
//
// = DESCRIPTION
//    This class, as the name suggests, is for the generated code for
//    the enum types.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_ENUM_TYPE_VISITOR_H
#define TAO_PSDL_ENUM_TYPE_VISITOR_H

#include /**/ "ace/pre.h"

#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Enum_Type_Visitor : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Enum_Type_Visitor ();

  ~TAO_PSDL_Enum_Type_Visitor ();

  // Override the necessary methods for the enum types.
  int visit_identifier (TAO_PSDL_Identifier *identifier);

  int visit_enumerator (TAO_PSDL_Enumerator *enumerator);

  int visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type);

 private:

  void print_class_for_enum_type (ACE_CString enum_type_name);

};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_ENUM_TYPE_VISITOR_H */
