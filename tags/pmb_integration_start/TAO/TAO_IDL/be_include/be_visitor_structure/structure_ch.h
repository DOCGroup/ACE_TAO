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
//    structure_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//    This one provides code generation for the structure in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_STRUCTURE_CH_H_
#define _BE_VISITOR_STRUCTURE_STRUCTURE_CH_H_

class be_visitor_structure_ch : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for structure
  //
  //
public:
  be_visitor_structure_ch (be_visitor_context *ctx);
  // constructureor

  ~be_visitor_structure_ch (void);
  // destructureor

  virtual int visit_structure (be_structure *node);
  // visit structure. We provide code for this method in the derived class
};

#endif /* _BE_VISITOR_STRUCTURE_STRUCTURE_CH_H_ */
