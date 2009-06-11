/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_null_return_emitter.h
//
// = DESCRIPTION
//    Emits 'return x;' for all CORBA types and ACE-supported
//    platforms.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

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
  // No point in making this a visitor and overloading all the
  // operations - most of those variations are captured in the
  // spawned be_visitor_null_return_value, except for structs
  // and unions, where we delegate to the function below.
  void emit_struct_union (be_type *node);
  
private:
  be_visitor_context *ctx_;
  TAO_OutStream &os_;
};


#endif // if !defined

