/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Visitor_Factory.h
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ============================================================================

#ifndef TAO_PSDL_VISITOR_FACTORY_H
#define TAO_PSDL_VISITOR_FACTORY_H

#include "ace/pre.h"

#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PSDL_Export TAO_PSDL_Visitor_Factory
{
 public:

  TAO_PSDL_Visitor_Factory ();

  ~TAO_PSDL_Visitor_Factory ();

  TAO_PSDL_Node_Visitor *make_visitor (int type);
};

#include "ace/post.h"

#endif /* TAO_PSDL_VISITOR_FACTORY_H */
