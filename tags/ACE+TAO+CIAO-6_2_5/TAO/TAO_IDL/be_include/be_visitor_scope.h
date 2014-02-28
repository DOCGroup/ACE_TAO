/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_scope.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Scope" node
 *
 *
 *  @author Aniruddha Gokhale and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_BE_VISITOR_SCOPE_H
#define TAO_BE_VISITOR_SCOPE_H

#include "be_visitor_decl.h"


/**
 * @class be_visitor_scope
 *
 * @brief be_visitor_scope
 *
 * This is a concrete visitor for the be_scope node. Its only purpose is to
 * iterate over the elements of the scope
 */
class be_visitor_scope : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_scope (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_scope (void);

  /// visit scope
  int visit_scope (be_scope *node);

  /// do any processing after every element except the last one of the scope is
  /// processed
  virtual int post_process (be_decl *);

  /// do any processing prior to processing the element in the scope
  virtual int pre_process (be_decl *);

  /// return the element number that is being processed
  virtual int elem_number (void);

  /// get the successor to elem
  virtual int next_elem (be_decl *elem, be_decl *&);

  /// Is bd the last node in the scope
  virtual bool last_node (be_decl *bd);

  /// Is bd the last inout or out in the scope
  bool last_inout_or_out_node (be_decl *bd);

protected:
  int elem_number_;
};


#endif // TAO_BE_VISITOR_SCOPE_H
