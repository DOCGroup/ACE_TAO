//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_cs.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arrays in client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_CS_H_
#define _BE_VISITOR_ARRAY_CS_H_

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

class be_visitor_array_cs : public be_visitor_array
{
  //
  // = TITLE
  //   be_visitor_array_cs
  //
  // = DESCRIPTION
  //   Code to be generated for arrays in the client stub file
  //
public:
  be_visitor_array_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_array_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit the array node
};

#endif /* _BE_VISITOR_ARRAY_CS_H_ */
