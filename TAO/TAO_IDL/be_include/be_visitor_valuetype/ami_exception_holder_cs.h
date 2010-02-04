/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ami_exception_holder_cs.h
 *
 *  $Id$
 *
 *  Visitor to generate code for exception holders.
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CS_H_
#define _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CS_H_

/**
 * @class be_visitor_valuetype_ami_exception_holder_cs
 *
 * @brief be_visitor_valuetype_ami_exception_holder_cs
 *
 * This is a concrete visitor to generate the client stubs for interface
 */
class be_visitor_valuetype_ami_exception_holder_cs : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_ami_exception_holder_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_ami_exception_holder_cs (void);

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CS_H_ */
