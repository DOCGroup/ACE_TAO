/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ex_idl.h
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO executor IDL.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_EX_IDL_H_
#define _BE_VISITOR_ROOT_ROOT_EX_IDL_H_

/**
 * @class be_visitor_root_ex_idl
 *
 * @brief be_visitor_root_ex_idl
 *
 * This is a concrete visitor to generate the CIAO executor
 * IDL for root
 */
class be_visitor_root_ex_idl : public be_visitor_root
{
public:
  explicit be_visitor_root_ex_idl (be_visitor_context *ctx);

  ~be_visitor_root_ex_idl () override = default;

  be_visitor_root_ex_idl (const be_visitor_root_ex_idl &) = delete;
  be_visitor_root_ex_idl (be_visitor_root_ex_idl &&) = delete;
  be_visitor_root_ex_idl &operator= (const be_visitor_root_ex_idl &) = delete;
  be_visitor_root_ex_idl &operator= (be_visitor_root_ex_idl &&) = delete;

  int visit_root (be_root *node) override;

private:
  /// Open file and initialize stream.
  int init ();
};

#endif /* _BE_VISITOR_ROOT_ROOT_EX_IDL_H_ */
