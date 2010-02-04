/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Field" node
 *  This generates code for structure members in the client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_FIELD_CS_H_
#define _BE_VISITOR_FIELD_CS_H_

/**
 * @class be_visitor_field_cs
 *
 * @brief be_visitor_field_cs
 *
 * This is a concrete visitor for the be_field node for the client stubs
 * file
 */
class be_visitor_field_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_field_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_field_cs (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a field can be and impl
  // code must be generated

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit enum type
  virtual int visit_enum (be_enum *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit typedefed type
  virtual int visit_typedef (be_typedef *node);

  /// visit union type
  virtual int visit_union (be_union *node);
};

#endif /*  _BE_VISITOR_FIELD_CS_H_ */
