/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    attribute.h
 *
 *  Visitor for the Attribute class.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ATTRIBUTE_ATTRIBUTE_H
#define _BE_VISITOR_ATTRIBUTE_ATTRIBUTE_H

#include <string>

// ************************************************************
// Attribute visitor
// ************************************************************

/**
 * @class be_visitor_attribute
 *
 * @brief be_visitor_attribute
 *
 * This is a concrete visitor to generate code for attributes. We have one
 * class for mapping into the client and server.
 */
class be_visitor_attribute : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_attribute (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_attribute () override;
  be_visitor_attribute (be_visitor_attribute const &) = delete;
  be_visitor_attribute (be_visitor_attribute &&) = delete;
  be_visitor_attribute &operator= (be_visitor_attribute const &) = delete;
  be_visitor_attribute &operator= (be_visitor_attribute &&) = delete;

  /// visit attribute. We provide code for this method in the derived class
  int visit_attribute (be_attribute *node) override;

  void for_facets (bool val);
  void op_scope (be_decl *node);
  void exec_class_extension (std::string const& extension);

private:
  bool for_facets_ {};
  be_decl *op_scope_ {};

  /// Defaults to "exec_i" but in special cases we want to
  /// modify it.
  std::string exec_class_extension_ {"_exec_i"};
};

#endif /* _BE_VISITOR_ATTRIBUTE_ATTRIBUTE_H*/
