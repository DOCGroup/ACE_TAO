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
//    eventtype_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the Eventtype node.
//    This one provides code generation for eventtypes in the server source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_EVENTTYPE_EVENTTYPE_SS_H_
#define _BE_EVENTTYPE_EVENTTYPE_SS_H_

class be_visitor_eventtype_ss : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server source for eventtype
  //
  //
public:
  be_visitor_eventtype_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_ss (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_EVENTTYPE_EVENTTYPE_SS_H_ */
