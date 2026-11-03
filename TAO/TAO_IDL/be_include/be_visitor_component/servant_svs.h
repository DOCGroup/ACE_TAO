/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    servant_svs.h
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation for the Servant
 *  class definition
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
  explicit be_visitor_servant_svs (be_visitor_context *ctx);

  ~be_visitor_servant_svs () override = default;
  be_visitor_servant_svs (be_visitor_servant_svs const &) = delete;
  be_visitor_servant_svs (be_visitor_servant_svs &&) = delete;
  be_visitor_servant_svs &operator= (be_visitor_servant_svs const &) = delete;
  be_visitor_servant_svs &operator= (be_visitor_servant_svs &&) = delete;

  int visit_component (be_component *node) override;
  int visit_connector (be_connector *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;
  int visit_provides (be_provides *node) override;
  int visit_uses (be_uses *node) override;
  int visit_publishes (be_publishes *node) override;
  int visit_emits (be_emits *node) override;
  int visit_consumes (be_consumes *node) override;

private:
  void gen_provides_top ();
  void gen_uses_top ();
  void gen_publishes_top ();
  void gen_emits_top ();

  /// This method gets generated (unless we are
  /// generating the LwCCM profile) whether the
  /// comonent has any emitters or not.
  void gen_get_all_emitters ();

private:
  be_interface *op_scope_ {};
};

// ======================================================

class be_visitor_obv_factory_reg
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_obv_factory_reg (be_visitor_context *ctx);

  ~be_visitor_obv_factory_reg () override;
  be_visitor_obv_factory_reg (be_visitor_obv_factory_reg const &) = delete;
  be_visitor_obv_factory_reg (be_visitor_obv_factory_reg &&) = delete;
  be_visitor_obv_factory_reg &operator= (be_visitor_obv_factory_reg const &) = delete;
  be_visitor_obv_factory_reg &operator= (be_visitor_obv_factory_reg &&) = delete;

  int visit_publishes (be_publishes *node) override;
  int visit_emits (be_emits *node) override;
  int visit_consumes (be_consumes *node) override;

private:
  void gen_obv_factory_registration (AST_Type *t);
};

// ======================================================

class be_visitor_attr_set : public be_visitor_component_scope
{
public:
  explicit be_visitor_attr_set (be_visitor_context *ctx);

  ~be_visitor_attr_set () override;
  be_visitor_attr_set (be_visitor_attr_set const &) = delete;
  be_visitor_attr_set (be_visitor_attr_set &&) = delete;
  be_visitor_attr_set &operator= (be_visitor_attr_set const &) = delete;
  be_visitor_attr_set &operator= (be_visitor_attr_set &&) = delete;

  int visit_attribute (be_attribute *node) override;

  // This override includes the supported
  // interfaces, so we can generate code to initialize attribute
  // inherited from them, if any.
  int visit_component_scope (be_component *node);
};

// ======================================================

class be_visitor_facet_executor_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_facet_executor_block (be_visitor_context *ctx);

  ~be_visitor_facet_executor_block () override;
  be_visitor_facet_executor_block (be_visitor_facet_executor_block const &) = delete;
  be_visitor_facet_executor_block (be_visitor_facet_executor_block &&) = delete;
  be_visitor_facet_executor_block &operator= (be_visitor_facet_executor_block const &) = delete;
  be_visitor_facet_executor_block &operator= (be_visitor_facet_executor_block &&) = delete;

  int visit_provides (be_provides *node) override;
};

// ======================================================

class be_visitor_connect_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_connect_block (be_visitor_context *ctx);

  ~be_visitor_connect_block () override;
  be_visitor_connect_block (be_visitor_connect_block const &) = delete;
  be_visitor_connect_block (be_visitor_connect_block &&) = delete;
  be_visitor_connect_block &operator= (be_visitor_connect_block const &) = delete;
  be_visitor_connect_block &operator= (be_visitor_connect_block &&) = delete;

  int visit_uses (be_uses *node) override;
};

// ======================================================

class be_visitor_disconnect_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_disconnect_block (be_visitor_context *ctx);

  ~be_visitor_disconnect_block () override;
  be_visitor_disconnect_block (be_visitor_disconnect_block const &) = delete;
  be_visitor_disconnect_block (be_visitor_disconnect_block &&) = delete;
  be_visitor_disconnect_block &operator= (be_visitor_disconnect_block const &) = delete;
  be_visitor_disconnect_block &operator= (be_visitor_disconnect_block &&) = delete;

  int visit_uses (be_uses *node) override;
};

// ======================================================

class be_visitor_receptacle_desc
  : public be_visitor_component_scope
{
public:
  be_visitor_receptacle_desc (be_visitor_context *ctx,
                              be_component *node);

  ~be_visitor_receptacle_desc () override;
  be_visitor_receptacle_desc (be_visitor_receptacle_desc const &) = delete;
  be_visitor_receptacle_desc (be_visitor_receptacle_desc &&) = delete;
  be_visitor_receptacle_desc &operator= (be_visitor_receptacle_desc const &) = delete;
  be_visitor_receptacle_desc &operator= (be_visitor_receptacle_desc &&) = delete;

  int visit_uses (be_uses *node) override;

private:
  ACE_CDR::ULong slot_ {};
  be_component *comp_;
};

// ======================================================

class be_visitor_subscribe_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_subscribe_block (be_visitor_context *ctx);

  ~be_visitor_subscribe_block () override;
  be_visitor_subscribe_block (be_visitor_subscribe_block const &) = delete;
  be_visitor_subscribe_block (be_visitor_subscribe_block &&) = delete;
  be_visitor_subscribe_block &operator= (be_visitor_subscribe_block const &) = delete;
  be_visitor_subscribe_block &operator= (be_visitor_subscribe_block &&) = delete;

  int visit_publishes (be_publishes *node) override;
};

// ======================================================

class be_visitor_unsubscribe_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_unsubscribe_block (be_visitor_context *ctx);

  ~be_visitor_unsubscribe_block () override;
  be_visitor_unsubscribe_block (be_visitor_unsubscribe_block const &) = delete;
  be_visitor_unsubscribe_block (be_visitor_unsubscribe_block &&) = delete;
  be_visitor_unsubscribe_block &operator= (be_visitor_unsubscribe_block const &) = delete;
  be_visitor_unsubscribe_block &operator= (be_visitor_unsubscribe_block &&) = delete;

  int visit_publishes (be_publishes *node) override;
};

// ======================================================

class be_visitor_event_source_desc
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_event_source_desc (be_visitor_context *ctx);

  ~be_visitor_event_source_desc () override;
  be_visitor_event_source_desc (be_visitor_event_source_desc const &) = delete;
  be_visitor_event_source_desc (be_visitor_event_source_desc &&) = delete;
  be_visitor_event_source_desc &operator= (be_visitor_event_source_desc const &) = delete;
  be_visitor_event_source_desc &operator= (be_visitor_event_source_desc &&) = delete;

  int visit_publishes (be_publishes *node) override;

private:
  ACE_CDR::ULong slot_ {};
};

// ======================================================

class be_visitor_connect_consumer_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_connect_consumer_block (be_visitor_context *ctx);

  ~be_visitor_connect_consumer_block () override;
  be_visitor_connect_consumer_block (be_visitor_connect_consumer_block const &) = delete;
  be_visitor_connect_consumer_block (be_visitor_connect_consumer_block &&) = delete;
  be_visitor_connect_consumer_block &operator= (be_visitor_connect_consumer_block const &) = delete;
  be_visitor_connect_consumer_block &operator= (be_visitor_connect_consumer_block &&) = delete;

  int visit_emits (be_emits *node) override;
};

// ======================================================

class be_visitor_disconnect_consumer_block
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_disconnect_consumer_block (be_visitor_context *ctx);

  ~be_visitor_disconnect_consumer_block () override;
  be_visitor_disconnect_consumer_block (be_visitor_disconnect_consumer_block const &) = delete;
  be_visitor_disconnect_consumer_block (be_visitor_disconnect_consumer_block &&) = delete;
  be_visitor_disconnect_consumer_block &operator= (be_visitor_disconnect_consumer_block const &) = delete;
  be_visitor_disconnect_consumer_block &operator= (be_visitor_disconnect_consumer_block &&) = delete;

  int visit_emits (be_emits *node) override;
};

// ======================================================

class be_visitor_emitter_desc
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_emitter_desc (be_visitor_context *ctx);

  ~be_visitor_emitter_desc () override;
  be_visitor_emitter_desc (be_visitor_emitter_desc const &) = delete;
  be_visitor_emitter_desc (be_visitor_emitter_desc &&) = delete;
  be_visitor_emitter_desc &operator= (be_visitor_emitter_desc const &) = delete;
  be_visitor_emitter_desc &operator= (be_visitor_emitter_desc &&) = delete;

  int visit_emits (be_emits *node) override;

private:
  ACE_CDR::ULong slot_ {};
};

// ======================================================

class be_visitor_populate_port_tables
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_populate_port_tables (be_visitor_context *ctx);

  ~be_visitor_populate_port_tables () override;
  be_visitor_populate_port_tables (be_visitor_populate_port_tables const &) = delete;
  be_visitor_populate_port_tables (be_visitor_populate_port_tables &&) = delete;
  be_visitor_populate_port_tables &operator= (be_visitor_populate_port_tables const &) = delete;
  be_visitor_populate_port_tables &operator= (be_visitor_populate_port_tables &&) = delete;

  int visit_provides (be_provides *node) override;

  int visit_consumes (be_consumes *node) override;
};

// ======================================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Component_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  explicit Component_Op_Attr_Generator (be_visitor_scope * visitor);

  int emit (be_interface * derived_interface,
            TAO_OutStream * os,
            be_interface * base_interface) override;

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_SERVANT_SVS_H_ */

