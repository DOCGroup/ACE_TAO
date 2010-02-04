/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    enum_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for Enums generating code in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ENUM_ENUM_CH_H_
#define _BE_VISITOR_ENUM_ENUM_CH_H_

/**
 * @class be_visitor_enum_ch
 *
 * @brief be_visitor_enum_ch
 *
 * This is the base visitor for enum for the header file
 */
class be_visitor_enum_ch : public be_visitor_scope
{
public:
  /// conenumor
  be_visitor_enum_ch (be_visitor_context *ctx);

  /// deenumor
  ~be_visitor_enum_ch (void);

  /// visit enum.
  virtual int visit_enum (be_enum *node);

  // =visit operations on syntactically valid elements in our scope

  /// visit enum_val
  virtual int visit_enum_val (be_enum_val *node);

  // = helper

  /// generate a comma after each element is processed
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_ENUM_ENUM_CH_H_ */
