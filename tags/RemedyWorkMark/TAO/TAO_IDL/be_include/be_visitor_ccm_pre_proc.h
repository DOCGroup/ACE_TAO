
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_ccm_pre_proc.h
 *
 *  $Id$
 *
 *  This visitor creates for components the appropriate AST
 *  (Abstract Syntax Tree) nodes, corresponding to provides, uses,
 *  emits, publishes and consumes declarations,
 *  and adds the nodes to the AST.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_BE_VISITOR_CCM_PRE_PROC_H
#define TAO_BE_VISITOR_CCM_PRE_PROC_H

#include "be_visitor_component_scope.h"
#include "ast_component.h"
#include "utl_identifier.h"

class be_valuetype;
class be_exception;
class UTL_ExceptList;

/**
 * @class be_visitor_ccm_pre_proc
 *
 * @brief be_visitor_ccm_pre_proc
 *
 * Adds CCM implied IDL code to the AST.
 */
class be_visitor_ccm_pre_proc : public be_visitor_component_scope
{
public:
  be_visitor_ccm_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_ccm_pre_proc (void);

  virtual int visit_root (be_root *node);
  virtual int visit_module (be_module *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_uses (be_uses *node);
  virtual int visit_publishes (be_publishes *node);
  virtual int visit_emits (be_emits *node);
  virtual int visit_consumes (be_consumes *node);
  virtual int visit_home (be_home *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

protected:
  // Utility methods to handle the corresponding IDL declarations.
  int gen_implicit_ops (be_home *node,
                        AST_Interface *implicit);

  // Utility methods which generate individual operation nodes.
  int gen_connect_single (be_uses *node);
  int gen_disconnect_single (be_uses *node);
  int gen_get_connection_single (be_uses *node);
  int gen_connect_multiple (be_uses *node);
  int gen_disconnect_multiple (be_uses *node);
  int gen_get_connection_multiple (be_uses *node);
  int gen_push_op (be_eventtype *node,
                   AST_Interface *consumer);
  int gen_subscribe (be_publishes *node);
  int gen_unsubscribe (be_publishes *node);
  int gen_emits_connect (be_emits *nodebe_emits);
  int gen_emits_disconnect (be_emits *node);
  int gen_get_consumer (be_consumes *node);
  int gen_create (be_home *node,
                  AST_Interface *implicit);
  int gen_find_by_primary_key (be_home *node,
                               AST_Interface *implicit);
  int gen_remove (be_home *node,
                  AST_Interface *implicit);
  int gen_get_primary_key (be_home *node,
                           AST_Interface *implicit);
  int gen_extended_port (be_porttype *port_type);

  // Utility functions to create and destroy the various things
  // needed by operations generated from CCM-related declarations.

  int lookup_cookie (void);
  int lookup_exceptions (void);
  int lookup_one_exception (const char *name,
                            be_exception *&result);

  int create_event_consumer (be_eventtype *node);
  AST_Interface *lookup_consumer (be_field *node);
  AST_Interface *create_explicit (be_home *node);
  AST_Interface *create_implicit (be_home *node);
  AST_Interface *create_equivalent (be_home *node,
                                    AST_Interface *xplicit,
                                    AST_Interface *implicit);
  UTL_ScopedName *create_scoped_name (const char *prefix,
                                      const char *local_name,
                                      const char *suffix,
                                      AST_Decl *parent);
  UTL_NameList *compute_inheritance (be_home *node);

protected:
  /// These are created for operations implied by 'uses multiple' declarations.
  Identifier module_id_;
  be_valuetype *cookie_;

  /// Exceptions thrown by implied CCM operations.
  be_exception *already_connected_;
  be_exception *invalid_connection_;
  be_exception *no_connection_;
  be_exception *exceeded_connection_limit_;
  be_exception *create_failure_;
  be_exception *remove_failure_;
  be_exception *finder_failure_;
  be_exception *invalid_key_;
  be_exception *unknown_key_value_;
  be_exception *duplicate_key_value_;

  /// Working nodes.
  be_component *comp_;
  be_home *home_;

  /// So we can look up Cookie and the CCM exceptions
  /// once when the first component is seen (then we
  /// know that Components.idl is included and the
  /// lookups will succeed).
  bool ccm_lookups_done_;

private:
  /// Generate a sendc_* receptacle for AMI4CCM.
  int generate_ami4ccm_uses (void);
};

#endif // TAO_BE_VISITOR_CCM_PRE_PROC_H
