/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_init_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetype factory
 *  in the (client) source.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_INIT_CS_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_CS_H_


/**
 * @class be_visitor_valuetype_init_cs
 *
 * @brief be_visitor_valuetype_init_cs
 *
 * This is a concrete visitor to generate valuetype factory in
 * the client header
 */
class be_visitor_valuetype_init_cs : public be_visitor_valuetype_init
{
public:
  /// constructor
  be_visitor_valuetype_init_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_init_cs (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_CS_H_ */
