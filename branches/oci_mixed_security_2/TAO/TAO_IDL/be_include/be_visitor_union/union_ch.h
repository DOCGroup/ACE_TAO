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
//    union_ch.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_UNION_CH_H_
#define _BE_VISITOR_UNION_UNION_CH_H_

class be_visitor_union_ch : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for union
  //
  //
public:
  be_visitor_union_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_ch (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

};

#endif /* _BE_VISITOR_UNION_UNION_CH_H_ */
