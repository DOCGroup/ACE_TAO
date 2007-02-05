/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Module_Visitor.h
//
// = DESCRIPTION
//    Visitor class for the generated code for modules.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_MODULE_VISITOR_H
#define TAO_PSDL_MODULE_VISITOR_H

#include /**/ "ace/pre.h"

#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Module_Visitor : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Module_Visitor ();

  ~TAO_PSDL_Module_Visitor ();

  // Override the visit_module method.
  int visit_module (TAO_PSDL_Module *node);
};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_MODULE_VISITOR_H */
