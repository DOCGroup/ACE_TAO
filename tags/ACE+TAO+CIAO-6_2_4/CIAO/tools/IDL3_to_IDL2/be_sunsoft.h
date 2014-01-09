/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_sunsoft.h
 *
 *  $Id$
 *
 *  SunSoft specific backend output generation
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_BE_HELPER_H
#define TAO_BE_HELPER_H

#include "be_helper.h"

/**
 * @class TAO_SunSoft_OutStream
 *
 * Backend specific to SunSoft AST nodes
 */
class TAO_IDL3_TO_IDL2_BE_Export TAO_SunSoft_OutStream : public TAO_OutStream
{
public:
  /// constructor
  TAO_SunSoft_OutStream (void);

  /// destuctor
  ~TAO_SunSoft_OutStream (void);

  /// output the SunSoft IDL Identifier Node
  virtual TAO_OutStream &print (Identifier *id);

  /// output the contents of the AST_Expression node
  virtual TAO_OutStream &print (AST_Expression *expr);
};

#endif // if !defined
