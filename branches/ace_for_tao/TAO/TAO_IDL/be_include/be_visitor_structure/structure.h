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
//    structure.h
//
// = DESCRIPTION
//    Concrete visitor for the STRUCTURE class
//    This one provides the generic visitor for the Structure node.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_STRUCTURE_H_
#define _BE_VISITOR_STRUCTURE_STRUCTURE_H_

class be_visitor_structure : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_structure
  //
  // = DESCRIPTION
  //   This is the base visitor for structure
  //
  //
public:
  be_visitor_structure (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_structure (void);
  // destructureor

  virtual int visit_structure (be_structure *node);
  // visit structure. We provide code for this method in the derived class

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_field (be_field *node);
  // visit field
};

#endif /* _BE_VISITOR_STRUCTURE_STRUCTURE_H_ */
