
//=============================================================================
/**
 *  @file    valuetype_init_arglist_ch.h
 *
 *  Visitor for generating code for OBV factory. This generates the
 *  operation signature.
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
  ~be_visitor_valuetype_init_arglist_ch ();

  /// visit factory construct
  int visit_factory (be_factory *node) override;

  /// visit each argument
  int visit_argument (be_argument *node) override;

  /// stuff to o/p after each element of the scope is handled
  int post_process (be_decl *) override;
};

#endif /* _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_ */
