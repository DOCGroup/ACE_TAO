/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_sunsoft.h
//
// = DESCRIPTION
//    SunSoft specific backend output generation
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_HELPER_H
#define TAO_BE_HELPER_H

#include "be_helper.h"

class TAO_SunSoft_OutStream : public TAO_OutStream
{
  // =TITLE
  //  TAO_SunSoft_OutStream
  // =DESCRIPTION
  //  Backend specific to SunSoft AST nodes
public:
  TAO_SunSoft_OutStream (void);
  // constructor

  ~TAO_SunSoft_OutStream (void);
  // destuctor

  virtual TAO_OutStream &print (Identifier *id);
  // output the SunSoft IDL Identifier Node

  virtual TAO_OutStream &print (UTL_IdList *idl);
  // output the SunSoft IDL UTL_IdList Node which is usually used to maintain a
  // scoped name

  virtual TAO_OutStream &print (AST_Expression *expr);
  // output the contents of the AST_Expression node
};

#endif // if !defined
