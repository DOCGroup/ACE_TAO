/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Exception_Visitor.h
//
// = DESCRIPTION
//    Visitor class for the generated code for exceptions.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_EXCEPTION_VISITOR_H
#define TAO_PSDL_EXCEPTION_VISITOR_H

#include /**/ "ace/pre.h"

#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Exception_Visitor : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Exception_Visitor ();

  ~TAO_PSDL_Exception_Visitor ();

  // Override the necessary methods for the exceptions.
  int visit_identifier (TAO_PSDL_Identifier *identifier);

  int visit_except_dcl (TAO_PSDL_Except_Dcl *except_dcl);

  int visit_member_list (TAO_PSDL_Member_List *member_list);

  int visit_predefined_type (TAO_PSDL_Predefined_Type *predefined_type);

 private:

  void print_class_for_exception (void);
  ACE_CString exception_name_;
};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_EXCEPTION_VISITOR_H */
