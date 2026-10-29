/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_init.h
 *
 *  Coomon visitor for the Valuetype node.
 *  This one provides code generation for valuetype factory construct.
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_INIT_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_H_


/**
 * @class be_visitor_valuetype_init
 *
 * @brief be_visitor_valuetype_init
 *
 * This is a common visitor to generate valuetype factory constructs.
 */
class be_visitor_valuetype_init : public be_visitor_valuetype
{
public:
  /// constructor
  explicit be_visitor_valuetype_init (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_init () override = default;
  be_visitor_valuetype_init (be_visitor_valuetype_init const &) = delete;
  be_visitor_valuetype_init (be_visitor_valuetype_init &&) = delete;
  be_visitor_valuetype_init &operator= (be_visitor_valuetype_init const &) = delete;
  be_visitor_valuetype_init &operator= (be_visitor_valuetype_init &&) = delete;

public:
  void begin_public () override;
  void begin_private () override;
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_H_ */
