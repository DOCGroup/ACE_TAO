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
//    eventtype_si.h
//
// = DESCRIPTION
//    Concrete visitor for the Eventtype node.
//    This one provides code generation for eventtypes in the server inline.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_EVENTTYPE_EVENTTYPE_SI_H_
#define _BE_EVENTTYPE_EVENTTYPE_SI_H_

class be_visitor_eventtype_si : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for eventtype
  //
  //
public:
  be_visitor_eventtype_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_si (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_EVENTTYPE_EVENTTYPE_SI_H_ */
