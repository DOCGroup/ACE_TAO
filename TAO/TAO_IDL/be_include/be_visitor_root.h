/* -*- c++ -*- */
//
// $Id$
//

#if !defined (TAO_BE_VISITOR_ROOT_H)
#define TAO_BE_VISITOR_ROOT_H

#include "be_visitor.h"

class be_visitor_root_ch
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
  virtual ~be_visitor_root_ch (void);

  virtual int visit_sequence (be_sequence *node);
};

class be_visitor_root_cs
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
  be_visitor_root_cs (void);
  virtual ~be_visitor_root_cs (void);

  virtual int visit_sequence (be_sequence *node);
};

class be_visitor_root_ci
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
  be_visitor_root_ci (void);
  virtual ~be_visitor_root_ci (void);

  virtual int visit_sequence (be_sequence *node);
};

#endif // TAO_BE_VISITOR_ROOT_H
