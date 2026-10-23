/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef_cs.h
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Typedef class in the client
 *  stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_

/**
 * @class be_visitor_typedef_cs
 *
 * @brief be_visitor_typedef_cs
 *
 * This is a concrete visitor to generate the client source for typedef
 */
class be_visitor_typedef_cs : public be_visitor_typedef
{
public:
  /// constructor
  explicit be_visitor_typedef_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef_cs () override;

  be_visitor_typedef_cs (be_visitor_typedef_cs const &) = delete;
  be_visitor_typedef_cs (be_visitor_typedef_cs &&) = delete;
  be_visitor_typedef_cs &operator= (be_visitor_typedef_cs const &) = delete;
  be_visitor_typedef_cs &operator= (be_visitor_typedef_cs &&) = delete;

  /// visit typedef. We provide code for this method in the derived class
  int visit_typedef (be_typedef *node) override;

  /// visit a sequence
  int visit_sequence (be_sequence *node) override;
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_*/
