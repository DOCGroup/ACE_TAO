/* -*- c++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    orbsvcs/PSS
//
// = FILENAME
//    PSDL_Typecode_Defn.h
//
// = DESCRIPTION
//    Helper for the generation of the typecodes.
//    This one generates the definitions.
//
// = AUTHOR
//    Priyanka Gontla <pgontla@doc.ece.uci.edu>
//
// ============================================================================

#ifndef PSDL_TYPECODE_DEFN_H
#define PSDL_TYPECODE_DEFN_H

#include /**/ "ace/pre.h"
#include "PSDL_Node_Visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_PSDL_Typecode_Defn : public TAO_PSDL_Node_Visitor
{
 public:

  TAO_PSDL_Typecode_Defn ();

  ~TAO_PSDL_Typecode_Defn ();

  int visit_type_declarator (TAO_PSDL_Type_Declarator *);
};

#include /**/ "ace/post.h"
#endif /* PSDL_TYPECODE_DEFN_H */
