
//=============================================================================
/**
 *  @file    root_ih.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  implementation header.
 *
 *
 *  @author Yamuna Krishnamurthy
 */
//=============================================================================


#ifndef _BE_VISITOR_ROOT_ROOT_IH_H_
#define _BE_VISITOR_ROOT_ROOT_IH_H_

/**
 * @class be_visitor_root_ih
 *
 * @brief be_visitor_root_ih
 *
 * This is a concrete visitor to generate the implementation header for root
 */
class be_visitor_root_ih : public be_visitor_root
{
public:
  /// constructor
  be_visitor_root_ih (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_root_ih (void);

  /// set the right context and make a visitor
  virtual int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_IH_H_ */
