/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    marshal_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for marshalling.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_MARSHAL_CS_H_
#define _BE_VISITOR_VALUETYPE_MARSHAL_CS_H_

/**
 * @class be_visitor_valuetype_marshal_cs
 *
 * @brief be_visitor_valuetype_marshal_cs
 *
 * This is a concrete visitor for interface that generates the CDR operator
 * declarations
 */
class be_visitor_valuetype_marshal_cs : public be_visitor_scope
{

public:
  /// constructor
  be_visitor_valuetype_marshal_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_marshal_cs (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  void class_name (be_valuetype *node, TAO_OutStream *os);
  int gen_fields (be_valuetype *node, be_visitor_context &ctx);
};

#endif /* _BE_VISITOR_VALUETYPE_MARSHAL_CS_H_ */
