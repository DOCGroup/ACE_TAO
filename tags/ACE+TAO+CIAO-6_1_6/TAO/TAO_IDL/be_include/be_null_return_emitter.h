/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_null_return_emitter.h
 *
 *  $Id$
 *
 *  Emits 'return x;' for all CORBA types and ACE-supported
 *  platforms.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BE_NULL_RETURN_EMITTER_H
#define TAO_BE_NULL_RETURN_EMITTER_H

class be_visitor_context;
class be_type;
class TAO_OutStream;

class be_null_return_emitter
{
public:
  be_null_return_emitter (be_visitor_context *ctx);
  ~be_null_return_emitter (void);

  int emit (be_type *node);

private:
  be_visitor_context *ctx_;
  TAO_OutStream &os_;
};


#endif // if !defined

