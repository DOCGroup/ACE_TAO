//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_arglist_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for OBV factory. This generates the
//    operation signature.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_
#define _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_


class be_visitor_valuetype_init_arglist_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_valuetype_init_arglist_ch
  //
  // = DESCRIPTION
  //   This is a visitor to generate factory argument list
  //
  //
public:
  be_visitor_valuetype_init_arglist_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_init_arglist_ch (void);
  // destructor

  virtual int visit_factory (be_factory *node);
  // visit factory construct

  virtual int visit_argument (be_argument *node);
  // visit each argument

  virtual int post_process (be_decl *);
  // stuff to o/p after each element of the scope is handled

  virtual int gen_throw_spec (be_factory *);
  // IDL3 extension.
};

#endif /* _BE_VISITOR_VALUETYPE_INIT_ARGLIST_CH_H_ */
