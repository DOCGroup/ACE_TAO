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
//    valuetype_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the valuetype node.
//    This provides for code generation in the server skeleton
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_SS_H_
#define _BE_VALUETYPE_VALUETYPE_SS_H_

class be_visitor_valuetype_ss : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   interface
  //
public:
  be_visitor_valuetype_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_ss (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // set the right context and make a visitor

protected:
  ACE_CString generate_flat_name (be_valuetype *node);
  ACE_CString generate_local_name (be_valuetype *node);
  ACE_CString generate_full_skel_name (be_valuetype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_SS_H_ */
