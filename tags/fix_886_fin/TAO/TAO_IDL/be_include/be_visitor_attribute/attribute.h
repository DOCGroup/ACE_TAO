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
//    attribute.h
//
// = DESCRIPTION
//    Visitor for the Attribute class.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ATTRIBUTE_ATTRIBUTE_H
#define _BE_VISITOR_ATTRIBUTE_ATTRIBUTE_H

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

#endif /* _BE_VISITOR_ATTRIBUTE_ATTRIBUTE_H*/
