/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_ss.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuetype node.
 *  This provides for code generation in the server skeleton
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_SS_H_
#define _BE_VALUETYPE_VALUETYPE_SS_H_

/**
 * @class be_visitor_valuetype_ss
 *
 * @brief be_visitor_valuetype_ss
 *
 * This is a concrete visitor to generate the server skeletons for
 * interface
 */
class be_visitor_valuetype_ss : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_ss (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

protected:
  ACE_CString generate_flat_name (be_valuetype *node);
  ACE_CString generate_local_name (be_valuetype *node);
  ACE_CString generate_full_skel_name (be_valuetype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_SS_H_ */
