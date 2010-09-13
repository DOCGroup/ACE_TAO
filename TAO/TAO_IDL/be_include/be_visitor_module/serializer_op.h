/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    serializer_op.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Module class
 *  This one provides visitors to generate the
 *  TAO::DCPS::Serializer operators for elements in
 *  the Module's scope.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_serializer_op_H_
#define _BE_VISITOR_MODULE_serializer_op_H_

/**
 * @class be_visitor_module_serializer_op
 *
 * @brief be_visitor_module_serializer_op
 *
 * This is a concrete visitor for module that generates the Serializer operator
 * declarations and implementations
 */
class be_visitor_module_serializer_op : public be_visitor_module
{

public:
  /// constructor
  be_visitor_module_serializer_op (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_serializer_op (void);

  /// visit module
  virtual int visit_module (be_module *node);
};

#endif /* _BE_VISITOR_MODULE_serializer_op_H_ */
