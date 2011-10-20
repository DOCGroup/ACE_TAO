/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_init_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetype factory
 *  in the (client) inline.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_INIT_CI_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_CI_H_


/**
 * @class be_visitor_valuetype_init_ci
 *
 * @brief be_visitor_valuetype_init_ci
 *
 * This is a concrete visitor to generate valuetype factory in
 * the client header
 */
class be_visitor_valuetype_init_ci : public be_visitor_valuetype_init
{
public:
  /// constructor
  be_visitor_valuetype_init_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_init_ci (void);
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_CI_H_ */
