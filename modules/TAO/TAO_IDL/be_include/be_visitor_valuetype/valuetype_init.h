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
//    valuetype_init.h
//
// = DESCRIPTION
//    Coomon visitor for the Valuetype node.
//    This one provides code generation for valuetype factory construct.
//
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_INIT_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_H_


class be_visitor_valuetype_init : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_init
  //
  // = DESCRIPTION
  //   This is a common visitor to generate valuetype factory constructs.
  //
  //

public:
  be_visitor_valuetype_init (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_init (void);
  // destructor

public:

  void begin_public (void);
  void begin_private (void);
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_H_ */
