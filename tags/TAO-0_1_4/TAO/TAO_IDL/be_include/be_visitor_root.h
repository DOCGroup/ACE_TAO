/* -*- c++ -*- */
//
// $Id$
//

#if !defined (TAO_BE_VISITOR_ROOT_H)
#define TAO_BE_VISITOR_ROOT_H

#include "be_visitor.h"

class be_visitor_root_ch : public be_visitor
{
  //
  // = TITLE
  //   Main visitor for the generation of the client header file.
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header.
  //
  // = NOTE
  //   The current implementation only works for sequences.
  //
public:
  be_visitor_root_ch (void);
  ~be_visitor_root_ch (void);

  virtual int visit_root (be_root *node);
  // visit root

  virtual int visit_sequence (be_sequence *node);
};

typedef ACE_Singleton <be_visitor_root_ch, ACE_SYNCH_RECURSIVE_MUTEX> TAO_BE_VISITOR_ROOT_CH;

class be_visitor_root_cs : public be_visitor
{
  //
  // = TITLE
  //   Main visitor for the generation of the client header file.
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stub.
  //
  // = NOTE
  //   The current implementation only works for sequences.
  //
public:
  be_visitor_root_cs (void);
  ~be_visitor_root_cs (void);

  virtual int visit_root (be_root *node);
  // visit root

  virtual int visit_sequence (be_sequence *node);
};

typedef ACE_Singleton <be_visitor_root_cs, ACE_SYNCH_RECURSIVE_MUTEX> TAO_BE_VISITOR_ROOT_CS;

class be_visitor_root_ci : public be_visitor
{
  //
  // = TITLE
  //   Main visitor for the generation of the client inline file.
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline.
  //
  // = NOTE
  //   The current implementation only works for sequences.
  //
public:
  be_visitor_root_ci (void);
  ~be_visitor_root_ci (void);

  virtual int visit_root (be_root *node);
  // visit root

  virtual int visit_sequence (be_sequence *node);
};

typedef ACE_Singleton <be_visitor_root_ci, ACE_SYNCH_RECURSIVE_MUTEX> TAO_BE_VISITOR_ROOT_CI;

class be_visitor_root_sh : public be_visitor
{
  //
  // = TITLE
  //   Main visitor for the generation of the server header file
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header
  //
  // = NOTE
  //   The current implementation only works for sequences.
  //
public:
  be_visitor_root_sh (void);
  ~be_visitor_root_sh (void);

  virtual int visit_root (be_root *node);
  // visit root

  virtual int visit_sequence (be_sequence *node);
};

typedef ACE_Singleton <be_visitor_root_sh, ACE_SYNCH_RECURSIVE_MUTEX> TAO_BE_VISITOR_ROOT_SH;

class be_visitor_root_si : public be_visitor
{
  //
  // = TITLE
  //   Main visitor for the generation of the server inline file.
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline.
  //
  // = NOTE
  //   The current implementation only works for sequences.
  //
public:
  be_visitor_root_si (void);
  ~be_visitor_root_si (void);

  virtual int visit_root (be_root *node);
  // visit root

  virtual int visit_sequence (be_sequence *node);
};

typedef ACE_Singleton <be_visitor_root_si, ACE_SYNCH_RECURSIVE_MUTEX> TAO_BE_VISITOR_ROOT_SI;

class be_visitor_root_ss : public be_visitor
{
  //
  // = TITLE
  //   Main visitor for the generation of the server skeletons file.
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons
  //
  // = NOTE
  //   The current implementation only works for sequences.
  //
public:
  be_visitor_root_ss (void);
  ~be_visitor_root_ss (void);

  virtual int visit_root (be_root *node);
  // visit root

  virtual int visit_sequence (be_sequence *node);
};

typedef ACE_Singleton <be_visitor_root_ss, ACE_SYNCH_RECURSIVE_MUTEX> TAO_BE_VISITOR_ROOT_SS;

#endif // TAO_BE_VISITOR_ROOT_H
