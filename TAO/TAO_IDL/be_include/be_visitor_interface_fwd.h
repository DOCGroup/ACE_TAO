/* -*- C++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_interface_fwd.h
//
// = DESCRIPTION
//    Concrete visitor for the interface_fwd class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_INTERFACE_FWD_H)
#define TAO_BE_VISITOR_INTERFACE_FWD_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_interface_fwd_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_interface_fwd_ch
  //
  // = DESCRIPTION
  //   This is the visitor for interface_fwd for the header file
  //
  //
public:
  be_visitor_interface_fwd_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_fwd_ch (void);
  // destructor

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface_fwd.

};

class be_visitor_interface_fwd_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_interface_fwd_ci
  //
  // = DESCRIPTION
  //   This is the visitor for interface_fwd for the inline file
  //
  //
public:
  be_visitor_interface_fwd_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_fwd_ci (void);
  // destructor

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface_fwd

};

#endif // TAO_BE_VISITOR_INTERFACE_FWD_H
