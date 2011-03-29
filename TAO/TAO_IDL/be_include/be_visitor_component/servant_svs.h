/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    servant_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation for the Servant
 *  class definition
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_SERVANT_SVS_H_
#define _BE_COMPONENT_SERVANT_SVS_H_

/**
 * This is a concrete visitor to generate the servant class decl
 * for a component.
 */
class be_visitor_servant_svs
  : public be_visitor_component_scope
{
public:
  be_visitor_servant_svs (be_visitor_context *ctx);

  ~be_visitor_servant_svs (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);

private:
  void gen_provides_top (void);
  void gen_uses_top (void);
  void gen_publishes_top (void);
  void gen_emits_top (void);

  /// This method gets generated (unless we are
  /// generating the LwCCM profile) whether the
  /// comonent has any emitters or not.
  void gen_get_all_emitters (void);

private:
  be_interface *op_scope_;
};

// ======================================================

class be_visitor_obv_factory_reg
  : public be_visitor_component_scope
{
public:
  be_visitor_obv_factory_reg (be_visitor_context *ctx);

  ~be_visitor_obv_factory_reg (void);

  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);

private:
  void gen_obv_factory_registration (AST_Type *t);
};

// ======================================================

class be_visitor_attr_set : public be_visitor_component_scope
{
public:
  be_visitor_attr_set (be_visitor_context *ctx);

  ~be_visitor_attr_set (void);

  virtual int visit_attribute (be_attribute *node);
};

// ======================================================

class be_visitor_facet_executor_block
  : public be_visitor_component_scope
{
public:
  be_visitor_facet_executor_block (be_visitor_context *ctx);

  ~be_visitor_facet_executor_block (void);

  virtual int visit_provides (be_provides *node);
};

// ======================================================

class be_visitor_connect_block
  : public be_visitor_component_scope
{
public:
  be_visitor_connect_block (be_visitor_context *ctx);

  ~be_visitor_connect_block (void);

  virtual int visit_uses (be_uses *node);
};

// ======================================================

class be_visitor_disconnect_block
  : public be_visitor_component_scope
{
public:
  be_visitor_disconnect_block (be_visitor_context *ctx);

  ~be_visitor_disconnect_block (void);

  virtual int visit_uses (be_uses *node);
};

// ======================================================

class be_visitor_receptacle_desc
  : public be_visitor_component_scope
{
public:
  be_visitor_receptacle_desc (be_visitor_context *ctx,
                              be_component *node);

  ~be_visitor_receptacle_desc (void);

  virtual int visit_uses (be_uses *node);

private:
  ACE_CDR::ULong slot_;
  be_component *comp_;
};

// ======================================================

class be_visitor_subscribe_block
  : public be_visitor_component_scope
{
public:
  be_visitor_subscribe_block (be_visitor_context *ctx);

  ~be_visitor_subscribe_block (void);

  virtual int visit_publishes (be_publishes *node);
};

// ======================================================

class be_visitor_unsubscribe_block
  : public be_visitor_component_scope
{
public:
  be_visitor_unsubscribe_block (be_visitor_context *ctx);

  ~be_visitor_unsubscribe_block (void);

  virtual int visit_publishes (be_publishes *node);
};

// ======================================================

class be_visitor_event_source_desc
  : public be_visitor_component_scope
{
public:
  be_visitor_event_source_desc (be_visitor_context *ctx);

  ~be_visitor_event_source_desc (void);

  virtual int visit_publishes (be_publishes *node);

private:
  ACE_CDR::ULong slot_;
};

// ======================================================

class be_visitor_connect_consumer_block
  : public be_visitor_component_scope
{
public:
  be_visitor_connect_consumer_block (be_visitor_context *ctx);

  ~be_visitor_connect_consumer_block (void);

  virtual int visit_emits (be_emits *node);
};

// ======================================================

class be_visitor_disconnect_consumer_block
  : public be_visitor_component_scope
{
public:
  be_visitor_disconnect_consumer_block (be_visitor_context *ctx);

  ~be_visitor_disconnect_consumer_block (void);

  virtual int visit_emits (be_emits *node);
};

// ======================================================

class be_visitor_emitter_desc
  : public be_visitor_component_scope
{
public:
  be_visitor_emitter_desc (be_visitor_context *ctx);

  ~be_visitor_emitter_desc (void);

  virtual int visit_emits (be_emits *node);

private:
  ACE_CDR::ULong slot_;
};

// ======================================================

class be_visitor_populate_port_tables
  : public be_visitor_component_scope
{
public:
  be_visitor_populate_port_tables (be_visitor_context *ctx);

  ~be_visitor_populate_port_tables (void);

  virtual int visit_provides (be_provides *node);

  virtual int visit_consumes (be_consumes *node);
};

// ======================================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Component_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Component_Op_Attr_Generator (be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_SERVANT_SVS_H_ */

