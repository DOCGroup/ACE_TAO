/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_attribute.h
//
// = DESCRIPTION
//    Concrete visitor for the Attribute class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_ATTRIBUTE_H)
#define TAO_BE_VISITOR_ATTRIBUTE_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

// ************************************************************
// Attribute visitor
// ************************************************************

class be_visitor_attribute : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_attribute
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate code for attributes. We have one
  //   class for mapping into the client and server.
  //
  //
public:
  be_visitor_attribute (be_visitor_context *ctx);
  // constructor

  ~be_visitor_attribute (void);
  // destructor

  virtual int visit_attribute (be_attribute *node);
  // visit attribute. We provide code for this method in the derived class

};


#endif
