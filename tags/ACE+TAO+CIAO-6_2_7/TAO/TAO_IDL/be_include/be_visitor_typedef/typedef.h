/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Typedef class
 *  This provides the generic visitors for the Typedef class.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_H_

/**
 * @class be_visitor_typedef
 *
 * @brief be_visitor_typedef
 *
 * This is a concrete visitor for typedef that abstracts all common tasks
 */
class be_visitor_typedef : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_typedef (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef (void);

  /// must be overridden
  virtual int visit_typedef (be_typedef *node);

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  virtual int visit_array (be_array *node);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit a sequence
  virtual int visit_sequence (be_sequence *node);

  /// visit a structure
  virtual int visit_structure (be_structure *node);

  /// visit a union
  virtual int visit_union (be_union *node);
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_H_ */
