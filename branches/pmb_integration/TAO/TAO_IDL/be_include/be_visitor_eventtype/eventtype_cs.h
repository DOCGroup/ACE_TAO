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
//    eventtype_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Eventtype node.
//    This one provides code generation for eventtypes in the (client) source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_EVENTTYPE_EVENTTYPE_CS_H_
#define _BE_EVENTTYPE_EVENTTYPE_CS_H_

class be_visitor_eventtype_cs : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client source for eventtype
  //
  //
public:
  be_visitor_eventtype_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_cs (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_EVENTTYPE_EVENTTYPE_CS_H_ */
