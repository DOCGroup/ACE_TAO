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
//    eventtype_sh.h
//
// = DESCRIPTION
//    Concrete visitor for the Eventtype node.
//    This one provides code generation for eventtypes in the server header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_EVENTTYPE_EVENTTYPE_SH_H_
#define _BE_EVENTTYPE_EVENTTYPE_SH_H_

class be_visitor_eventtype_sh : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for eventtype
  //
  //
public:
  be_visitor_eventtype_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_sh (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_EVENTTYPE_EVENTTYPE_SH_H_ */
