//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ci.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arrays in client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_CI_H_
#define _BE_VISITOR_ARRAY_CI_H_

// ************************************************************
// class be_visitor_array_ci
// ************************************************************

class be_visitor_array_ci : public be_visitor_array
{
  //
  // = TITLE
  //   be_visitor_array_ci
  //
  // = DESCRIPTION
  //   Code to be generated for arrays in the client inline file
  //
public:
  be_visitor_array_ci (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_array_ci (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit the array node

  virtual int gen_var_impl (be_array *node);
  // generate the var class definition

  virtual int gen_out_impl (be_array *node);
  // generate the out class definition

  virtual int gen_forany_impl (be_array *node);
  // generate the forany class definition
};

#endif /* _BE_VISITOR_ARRAY_CI_H_ */
