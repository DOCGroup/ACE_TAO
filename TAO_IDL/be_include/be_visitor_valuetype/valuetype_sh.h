/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_sh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuetype node.
 *  This provides for code generation in the server header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_SH_H_
#define _BE_VALUETYPE_VALUETYPE_SH_H_

/**
 * @class be_visitor_valuetype_sh
 *
 * @brief be_visitor_valuetype_sh
 *
 * This is a concrete visitor to generate the server header for valuetype
 */
class be_visitor_valuetype_sh : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_sh (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_sh (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_SH_H_ */
