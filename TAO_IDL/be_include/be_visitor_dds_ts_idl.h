/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_visitor_dds_ts_idl.h
 *
 *  $Id$
 *
 *  Concrete visitor.
 *  This one provides code generation for
 *  DDS type support IDL files.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_DDS_TS_IDL_H_
#define _BE_VISITOR_DDS_TS_IDL_H_

#include "be_visitor_scope.h"

/**
 * @class be_visitor_dds_ts_idl
 *
 * @brief be_visitor_dds_ts_idl
 *
 * This is a concrete visitor to generate the DDS type support IDL
 */
class be_visitor_dds_ts_idl : public be_visitor_scope
{
public:
  be_visitor_dds_ts_idl (be_visitor_context *ctx);

  ~be_visitor_dds_ts_idl (void);

  virtual int visit_root (be_root *node);
  virtual int visit_module (be_module *node);

  /// The only types recognized so far as DDS datatypes.
  virtual int visit_structure (be_structure *node);
  virtual int visit_union (be_union *node);
  virtual int visit_valuetype (be_valuetype *node);

/// Common code for processing all datatypes.
private:
  int process_node (be_type *node);
  void gen_datawriter (be_type *node);
  void gen_datareader (be_type *node);

  /// Manage streams and files.
  int init_file (void);
  void fini_file (void);

private:
  TAO_OutStream *os_ptr_;
};

#endif /* _BE_VISITOR_DDS_TS_IDL_H_ */
