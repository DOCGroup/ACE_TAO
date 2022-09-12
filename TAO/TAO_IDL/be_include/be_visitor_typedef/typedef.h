/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef.h
 *
 *  Concrete visitor for the Typedef class
 *  This provides the generic visitors for the Typedef class.
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
  ~be_visitor_typedef ();

  /// must be overridden
  int visit_typedef (be_typedef *node) override;

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  int visit_array (be_array *node) override;

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit a sequence
  int visit_sequence (be_sequence *node) override;

  /// visit a structure
  int visit_structure (be_structure *node) override;

  /// visit a union
  int visit_union (be_union *node) override;
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_H_ */
