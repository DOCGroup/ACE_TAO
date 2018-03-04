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
  be_visitor_typedef_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef_cs (void);

  /// visit typedef. We provide code for this method in the derived class
  virtual int visit_typedef (be_typedef *node);

  /// visit a sequence
  virtual int visit_sequence (be_sequence *node);
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_*/
