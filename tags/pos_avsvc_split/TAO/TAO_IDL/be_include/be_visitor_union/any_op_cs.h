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
//    any_op_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the Any operators for the Union in the client
//     header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_ANY_OP_CS_H_
#define _BE_VISITOR_UNION_ANY_OP_CS_H_

class be_visitor_union_any_op_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_any_op_cs
  //
  // = DESCRIPTION
  //   This is a base class for the visitors generate the Any
  //   operator implementations
  //

public:
  be_visitor_union_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_any_op_cs (void);
  // destructor

  // template methods that the derived visitors must implement.
  // They generate the body for the insertion and extraction
  // operators.
  virtual int gen_insertion_nocopy (TAO_OutStream* os,
                                    be_union *node) = 0;
  virtual int gen_insertion (TAO_OutStream* os,
                             be_union *node) = 0;
  virtual int gen_extraction (TAO_OutStream* os,
                              be_union *node) = 0;

  // = The be_visitor methods.
  virtual int visit_union (be_union *node);
  virtual int visit_union_branch (be_union_branch *node);
};

class be_visitor_union_any_op_compiled_cs : public be_visitor_union_any_op_cs
{
  //
  // = TITLE
  //   be_visitor_union_any_op_compiled_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Any
  //   operator implementations, using compiled marshaling.
  //

public:
  be_visitor_union_any_op_compiled_cs (be_visitor_context *ctx);
  // constructor

  // = See be_visitor_union_any_op_cs
  virtual int gen_insertion (TAO_OutStream* os,
                             be_union *node);
  virtual int gen_insertion_nocopy (TAO_OutStream* os,
                                    be_union *node);
  virtual int gen_extraction (TAO_OutStream* os,
                              be_union *node);
};

class be_visitor_union_any_op_interpretive_cs : public be_visitor_union_any_op_cs
{
  //
  // = TITLE
  //   be_visitor_union_any_op_interpretive_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Any
  //   operator implementations, using interpretive marshaling.
  //

public:
  be_visitor_union_any_op_interpretive_cs (be_visitor_context *ctx);
  // constructor

  // = See be_visitor_union_any_op_cs
  virtual int gen_insertion (TAO_OutStream* os,
                             be_union *node);
  virtual int gen_insertion_nocopy (TAO_OutStream* os,
                                    be_union *node);
  virtual int gen_extraction (TAO_OutStream* os,
                              be_union *node);
};

#endif /* _BE_VISITOR_UNION_ANY_OP_CS_H_ */
