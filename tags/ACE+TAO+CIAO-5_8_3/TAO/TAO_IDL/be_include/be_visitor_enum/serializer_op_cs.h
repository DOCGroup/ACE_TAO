/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    serializer_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for Enums generating code for the
 *  TAO::DCPS::Serializer operators
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ENUM_serializer_op_CS_H_
#define _BE_VISITOR_ENUM_serializer_op_CS_H_

/**
 * @class be_visitor_enum_serializer_op_cs
 *
 * @brief be_visitor_enum_serializer_op_cs
 *
 * This is a concrete visitor for enum that generates the Serializer operator
 * implementations
 */
class be_visitor_enum_serializer_op_cs : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_enum_serializer_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_enum_serializer_op_cs (void);

  /// visit enum
  virtual int visit_enum (be_enum *node);
};

#endif /* _BE_VISITOR_ENUM_serializer_op_CS_H_ */
