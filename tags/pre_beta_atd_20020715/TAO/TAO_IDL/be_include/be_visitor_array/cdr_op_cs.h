//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.h
//
// = DESCRIPTION
//    The array's own CDR operators are generated in the .i file, but if the
//    array contains an anonymous sequence, those CDR operators will not
//    get generated on their own, and, since the sequence may contain a
//    recursive type, they must also be generated in the .cpp file. That is
//    the sole purpose of this visitor type.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_CDR_OP_CS_H_
#define _BE_VISITOR_ARRAY_CDR_OP_CS_H_

// ************************************************************
// class be_visitor_array_cdr_op_cs
// ************************************************************

class be_visitor_array_cdr_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the CDR operator
  //   implementations for the array's element type, if that type is
  //   an anonymous sequence.
  //

public:
  be_visitor_array_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_cdr_op_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array
};

#endif /* _BE_VISITOR_ARRAY_CDR_OP_CS_H_ */
