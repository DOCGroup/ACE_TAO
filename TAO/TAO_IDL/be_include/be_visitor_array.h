//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_array.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arrays
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_H
#define _BE_VISITOR_ARRAY_H

#include "be_visitor_decl.h"


// ************************************************************
// class be_visitor_array_ch
// ************************************************************

class be_visitor_array_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_ch
  //
  // = DESCRIPTION
  //   Code to be generated for arrays in the client header file
  //
public:
  be_visitor_array_ch (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_array_ch (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit the array node

};

// ************************************************************
// class be_visitor_array_ci
// ************************************************************

class be_visitor_array_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_ci
  //
  // = DESCRIPTION
  //   Code to be generated for arrays in the client inline file
  //
public:
  be_visitor_array_ci (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_array_ci (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit the array node

};

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

class be_visitor_array_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_cs
  //
  // = DESCRIPTION
  //   Code to be generated for arrays in the client stub file
  //
public:
  be_visitor_array_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_array_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit the array node

};

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

class be_visitor_array_any_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the Any operator
  //   declarations
  //

public:
  be_visitor_array_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_any_op_ch (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array

};

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

class be_visitor_array_any_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the Any operator
  //   implementations
  //

public:
  be_visitor_array_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_any_op_cs (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array

};

#endif
