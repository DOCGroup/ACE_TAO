//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_ccm_pre_proc.h
//
// = DESCRIPTION
//    This visitor creates for components the appropriate AST
//    (Abstract Syntax Tree) nodes, corresponding to provides, uses,
//    emits, publishes and consumes declarations,
//    and adds the nodes to the AST.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef TAO_BE_VISITOR_CCM_PRE_PROC_H
#define TAO_BE_VISITOR_CCM_PRE_PROC_H

#include "be_visitor_scope.h"
#include "ast_component.h"
#include "utl_identifier.h"

class be_valuetype;
class be_exception;
class UTL_ExceptList;

class be_visitor_ccm_pre_proc : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_ccm_pre_proc
  //
  // = DESCRIPTION
  //   Adds CCM implied IDL code to the AST.
  //
public:
  be_visitor_ccm_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_ccm_pre_proc (void);

  virtual int visit_root (be_root *node);
  // visit a root

  virtual int visit_module (be_module *node);
  // visit module

  virtual int visit_component (be_component *node);
  // visit component

  virtual int visit_home (be_home *node);
  // visit component home

  virtual int visit_eventtype (be_eventtype *node);
  // visit eventtype

private:
  // Utility methods to handle the corresponding IDL declarations.
  int gen_provides (be_component *node);
  int gen_uses (be_component *node);
  int gen_emits (be_component *node);
  int gen_publishes (be_component *node);
  int gen_consumes (be_component *node);
  int gen_factories (be_home *node,
                     AST_Interface *xplicit);
  int gen_finders (be_home *node,
                   AST_Interface *xplicit);
  int gen_implicit_ops (be_home *node,
                        AST_Interface *implicit);

  // Utility methods which generate individual operation nodes.
  int gen_connect_single (be_component *node,
                          AST_Component::port_description *pd);
  int gen_disconnect_single (be_component *node,
                             AST_Component::port_description *pd);
  int gen_get_connection_single (be_component *node,
                                 AST_Component::port_description *pd);
  int gen_connect_multiple (be_component *node,
                            AST_Component::port_description *pd);
  int gen_disconnect_multiple (be_component *node,
                               AST_Component::port_description *pd);
  int gen_get_connection_multiple (be_component *node,
                                   AST_Component::port_description *pd);
  int gen_push_op (be_eventtype *node,
                   AST_Interface *consumer);
  int gen_subscribe (be_component *node,
                     AST_Component::port_description *pd);
  int gen_unsubscribe (be_component *node,
                       AST_Component::port_description *pd);
  int gen_emits_connect (be_component *node,
                         AST_Component::port_description *pd);
  int gen_emits_disconnect (be_component *node,
                            AST_Component::port_description *pd);
  int gen_get_consumer (be_component *node,
                        AST_Component::port_description *pd);
  int gen_create (be_home *node,
                  AST_Interface *implicit);
  int gen_find_by_primary_key (be_home *node,
                               AST_Interface *implicit);
  int gen_remove (be_home *node,
                  AST_Interface *implicit);
  int gen_get_primary_key (be_home *node,
                           AST_Interface *implicit);

  // Called if there is at least one component in the IDL file, saves
  // lookup in be_global for later use by component skeleton visitors.
  int lookup_ccmobject (void);

  // Utility functions to create and destroy the various things
  // needed by operations generated from CCM-related declarations.

  int lookup_cookie (be_component *node);
  int lookup_exceptions (be_component *node);
  int lookup_one_exception (be_component *node,
                            const char *name,
                            be_exception *&result);

  int create_uses_multiple_stuff (be_component *node,
                                  AST_Component::port_description *pd);
  int create_uses_multiple_struct (be_component *node,
                                   AST_Component::port_description *pd);
  int create_uses_multiple_sequence (be_component *node,
                                     AST_Component::port_description *pd);

  int create_event_consumer (be_eventtype *node);
  AST_Interface *lookup_consumer (AST_Component::port_description *pd);
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

private:
  // These are created for operations implied by 'uses multiple' declarations.
  Identifier module_id_;
  be_valuetype *cookie_;
  be_structure *connection_;
  be_sequence *connections_;

  // Exceptions thrown by implied CCM operations.
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
};


#endif // TAO_BE_VISITOR_CCM_PRE_PROC_H
