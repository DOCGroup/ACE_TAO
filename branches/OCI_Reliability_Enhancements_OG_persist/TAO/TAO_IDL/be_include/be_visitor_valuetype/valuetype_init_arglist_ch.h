
//=============================================================================
/**
 *  @file    valuetype_init_arglist_ch.h
 *
 *  $Id$
 *
 *  Visitor for generating code for OBV factory. This generates the
 *  operation signature.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_
#define _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_


/**
 * @class be_visitor_valuetype_init_arglist_ch
 *
 * @brief be_visitor_valuetype_init_arglist_ch
 *
 * This is a visitor to generate factory argument list
 */
class be_visitor_valuetype_init_arglist_ch : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_valuetype_init_arglist_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_init_arglist_ch (void);

  /// visit factory construct
  virtual int visit_factory (be_factory *node);

  /// visit each argument
  virtual int visit_argument (be_argument *node);

  /// stuff to o/p after each element of the scope is handled
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_ */
