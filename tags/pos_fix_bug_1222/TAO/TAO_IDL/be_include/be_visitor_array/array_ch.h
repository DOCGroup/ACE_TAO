//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array_ch.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arrays in client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_CH_H_
#define _BE_VISITOR_ARRAY_CH_H_

// ************************************************************
// class be_visitor_array_ch
// ************************************************************

class be_visitor_array_ch : public be_visitor_array
{
  //
  // = TITLE
  //   be_visitor_array_ch
  //
  // = DESCRIPTION
  //   Code to be generated for arrays in the client header file
  //
public:
  be_visitor_array_ch (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_array_ch (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit the array node

  virtual int gen_var_defn (be_array *node);
  // generate the var class definition

  virtual int gen_out_defn (be_array *node);
  // generate the out class definition

  virtual int gen_forany_defn (be_array *node);
  // generate the forany class definition
};

#endif /* _BE_VISITOR_ARRAY_CH_H_ */
