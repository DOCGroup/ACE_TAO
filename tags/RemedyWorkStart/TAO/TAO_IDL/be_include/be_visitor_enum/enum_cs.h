/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    enum_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for Enums generating code in the client stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ENUM_ENUM_CS_H_
#define _BE_VISITOR_ENUM_ENUM_CS_H_

/**
 * @class be_visitor_enum_cs
 *
 * @brief be_visitor_enum_cs
 *
 * This is the visitor for enum for the impl file
 */
class be_visitor_enum_cs : public be_visitor_scope
{
public:
  /// conenumor
  be_visitor_enum_cs (be_visitor_context *ctx);

  /// deenumor
  ~be_visitor_enum_cs (void);

  /// visit enum and generate the typecode
  virtual int visit_enum (be_enum *node);
};

#endif /* _BE_VISITOR_ENUM_ENUM_CS_H_ */
