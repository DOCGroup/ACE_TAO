/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetypes in the client stubs.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on code from Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_CS_H_
#define _BE_VALUETYPE_VALUETYPE_CS_H_

/**
 * @class be_visitor_valuetype_cs
 *
 * @brief be_visitor_valuetype_cs
 *
 * This is a concrete visitor to generate the client stubs for valuetype
 */
class be_visitor_valuetype_cs : public be_visitor_valuetype
{
public:
  be_visitor_valuetype_cs (be_visitor_context *ctx);

  ~be_visitor_valuetype_cs (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  /// Called only by AMH exceptions.
  virtual int visit_operation (be_operation *node);

private:
  /// Recursive to generate for inherited members.
  void gen_ostream_operator_r (be_valuetype *node, unsigned long &index);
};

#endif /* _BE_VALUETYPE_VALUETYPE_CS_H_ */
