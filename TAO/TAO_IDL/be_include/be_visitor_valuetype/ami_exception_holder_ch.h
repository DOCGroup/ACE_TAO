/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ami_exception_holder_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This one provides code generation for AMI exception holders
 *  in client headers.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CH_H_
#define _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CH_H_


/**
 * @class be_visitor_valuetype_ami_exception_holder_ch
 *
 * @brief be_visitor_valuetype_ami_exception_holder_ch
 *
 * This is a concrete visitor to generate the client header for
 * interface.
 */
class be_visitor_valuetype_ami_exception_holder_ch : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_ami_exception_holder_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_ami_exception_holder_ch (void);

  virtual int visit_operation (be_operation *node);

};

#endif /* _BE_VISITOR_VALUETYPE_AMI_EXCEPTIUON_HOLER_CH_H_ */
