/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_si.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuetype node.
 *  This provides for code generation in the server inline
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_SI_H_
#define _BE_VALUETYPE_VALUETYPE_SI_H_

/**
 * @class be_visitor_valuetype_si
 *
 * @brief be_visitor_valuetype_si
 *
 * This is a concrete visitor to generate the server inline for valuetype
 */
class be_visitor_valuetype_si : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_si (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_si (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_SI_H_ */
