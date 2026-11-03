/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_cs.h
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetypes in the client stubs.
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
  explicit be_visitor_valuetype_cs (be_visitor_context *ctx);

  ~be_visitor_valuetype_cs () override;
  be_visitor_valuetype_cs (be_visitor_valuetype_cs const &) = delete;
  be_visitor_valuetype_cs (be_visitor_valuetype_cs &&) = delete;
  be_visitor_valuetype_cs &operator= (be_visitor_valuetype_cs const &) = delete;
  be_visitor_valuetype_cs &operator= (be_visitor_valuetype_cs &&) = delete;

  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;

  /// Called only by AMH exceptions.
  int visit_operation (be_operation *node) override;

private:
  /// Recursive to generate for inherited members.
  void gen_ostream_operator_r (be_valuetype *node, unsigned long &index);

  void marshal_unmarshal_v (be_valuetype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_CS_H_ */
