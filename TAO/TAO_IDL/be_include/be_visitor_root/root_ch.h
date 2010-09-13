/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_CH_H_
#define _BE_VISITOR_ROOT_ROOT_CH_H_

/**
 * @class be_visitor_root_ch
 *
 * @brief be_visitor_root_ch
 *
 * This is a concrete visitor to generate the client
 * header for root
 */
class be_visitor_root_ch : public be_visitor_root
{
public:
  be_visitor_root_ch (be_visitor_context *ctx);

  ~be_visitor_root_ch (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);

  /// Encapsulating various bits of code generation peculiar
  /// to the client header file, done either before or after
  /// the main scope traversal.
  void gen_fwd_decls (void);
  void gen_proxy_broker_factory_pointers (void);
  void gen_ref_counting_overrides (void);
  void gen_static_corba_overrides (void);
  int gen_obv_decls (be_root *node);
  int gen_object_traits (be_root *node);
  int gen_template_exports (be_root *node);
  int gen_any_ops (be_root *node);
  int gen_cdr_ops (be_root *node);
  int gen_dds_serializer_ops (be_root *node);

private:
  /// Can't use base class be_visitor_decl's member since
  /// its constructor is called too soon to initialize the
  /// stream. This class is the only root visitor that
  /// contains filestream output.
  TAO_OutStream *o_;
};

#endif /* _BE_VISITOR_ROOT_ROOT_CH_H_ */
