//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component Home node.
//    This one provides code generation for component home node.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_HOME_CS_H_
#define _BE_VISITOR_HOME_CS_H_

class be_visitor_home_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_home_cs
  //
  // = DESCRIPTION
  //   This is the visitor for component home for the source file
  //
  //
public:
  be_visitor_home_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_home_cs (void);
  // destructor

  virtual int visit_home (be_home *node);
  // visit home.
};

#endif /* _BE_VISITOR_HOME_CS_H_ */
