/* -*- C++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    enum_ch.h
//
// = DESCRIPTION
//    Concrete visitor for Enums generating code in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ENUM_ENUM_CH_H_
#define _BE_VISITOR_ENUM_ENUM_CH_H_

class be_visitor_enum_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_ch
  //
  // = DESCRIPTION
  //   This is the base visitor for enum for the header file
  //
  //
public:
  be_visitor_enum_ch (be_visitor_context *ctx);
  // conenumor

  ~be_visitor_enum_ch (void);
  // deenumor

  virtual int visit_enum (be_enum *node);
  // visit enum.

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_enum_val (be_enum_val *node);
  // visit enum_val

  // = helper

  virtual int post_process (be_decl *);
  // generate a comma after each element is processed
};

#endif /* _BE_VISITOR_ENUM_ENUM_CH_H_ */
