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
//    eventtype_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Eventtype node.
//    This one provides code generation for eventtypes in the (client) header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_EVENTTYPE_EVENTTYPE_CH_H_
#define _BE_EVENTTYPE_EVENTTYPE_CH_H_

class be_visitor_eventtype_ch : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for eventtype
  //
  //
public:
  be_visitor_eventtype_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_ch (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_EVENTTYPE_EVENTTYPE_CH_H_ */
