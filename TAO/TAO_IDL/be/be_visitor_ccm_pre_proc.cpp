/**
 *  @file   be_visitor_ccm_pre_proc.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_ccm_pre_proc.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_exception.h"
#include "be_structure.h"
#include "be_sequence.h"
#include "be_valuetype.h"
#include "be_module.h"
#include "be_field.h"
#include "be_typedef.h"
#include "be_component.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_home.h"
#include "be_extern.h"
#include "ast_generator.h"
#include "utl_exceptlist.h"
#include "utl_namelist.h"
#include "utl_err.h"
#include "fe_interface_header.h"
#include "global_extern.h"
#include "nr_extern.h"

ACE_RCSID (be,
           be_visitor_ccm_pre_proc,
           "$Id$")

const char *EXCEP_NAMES[] =
  {
    "AlreadyConnected",
    "InvalidConnection",
    "NoConnection",
    "ExceededConnectionLimit",
    "CreateFailure",
    "RemoveFailure",
    "FinderFailure",
    "InvalidKey",
    "UnknownKeyValue",
    "DuplicateKeyValue",
    "CreateFailure",
    "FinderFailure"
  };

const int N_EXCEPS = sizeof (EXCEP_NAMES) / sizeof (char *);
be_exception *EXCEPS[N_EXCEPS];

be_visitor_ccm_pre_proc::be_visitor_ccm_pre_proc (be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    module_id_ ("Components"),
    cookie_ (0),
    already_connected_ (0),
    invalid_connection_ (0),
    no_connection_ (0),
    exceeded_connection_limit_ (0),
    create_failure_ (0),
    remove_failure_ (0),
    finder_failure_ (0),
    invalid_key_ (0),
    unknown_key_value_ (0),
    duplicate_key_value_ (0)
{
}

be_visitor_ccm_pre_proc::~be_visitor_ccm_pre_proc (void)
{
  this->module_id_.destroy ();
}

int
be_visitor_ccm_pre_proc::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::visit_component (be_component *node)
{
  if (this->lookup_cookie (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "Components::Cookie lookup failed\n"),
                        -1);
    }

  if (this->lookup_exceptions (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "component exception lookups failed\n"),
                        -1);
    }

  if (this->gen_provides (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "code generation for provides declarations failed\n"),
                        -1);
    }

  if (this->gen_uses (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "code generation for uses declarations failed\n"),
                        -1);
    }

  if (this->gen_emits (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "code generation for emits declarations failed\n"),
                        -1);
    }

  if (this->gen_publishes (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "code generation for publishes declarations failed\n"),
                        -1);
    }

  if (this->gen_consumes (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_component - "
                         "code generation for consumes declarations failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::visit_home (be_home *node)
{
  AST_Interface *xplicit = this->create_explicit (node);

  if (xplicit == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_home - "
                         "code generation for explicit interface failed\n"),
                        -1);
    }

  AST_Interface *implicit = this->create_implicit (node);

  if (implicit == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_home - "
                         "code generation for implicit interface failed\n"),
                        -1);
    }

  if (this->gen_factories (node, xplicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_home - "
                         "code generation for factories declarations failed\n"),
                        -1);
    }

  if (this->gen_finders (node, xplicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_home - "
                         "code generation for finders declarations failed\n"),
                        -1);
    }

  if (this->gen_implicit_ops (node, implicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_home - "
                         "code generation for primary key "
                         "operations failed\n"),
                        -1);
    }

  if (this->create_equivalent (node, xplicit, implicit) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_home - "
                         "code generation for equivalent interface failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::visit_eventtype (be_eventtype *node)
{
  if (node->ccm_pre_proc_gen ())
    {
      return 0;
    }

  if (this->create_event_consumer (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "visit_eventtype - "
                         "code generation for consumer failed\n"),
                        -1);
    }

  node->ccm_pre_proc_gen (true);
  return 0;
}

int
be_visitor_ccm_pre_proc::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  be_eventtype *fd =
    be_eventtype::narrow_from_decl (node->full_definition ());

  return this->visit_eventtype (fd);
}

// ****************************************************************

int
be_visitor_ccm_pre_proc::gen_provides (be_component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s = node->provides ();
  AST_Component::port_description *pd = 0;
  AST_Operation *provides_op = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);
      UTL_ScopedName *op_name =
        this->create_scoped_name ("provide_",
                                  pd->id->get_string (),
                                  0,
                                  node);
      ACE_NEW_RETURN (provides_op,
                      be_operation (pd->impl,
                                    AST_Operation::OP_noflags,
                                    0,
                                    0,
                                    0),
                      -1);
      provides_op->set_defined_in (node);
      provides_op->set_imported (node->imported ());
      provides_op->set_name (op_name);

      if (0 == node->be_add_operation (provides_op))
        {
          return -1;
        }
        
      // Might as well clean up here instead of putting 5 loops in 
      // AST_Component::destroy ().
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_uses (be_component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s = node->uses ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      if (pd->is_multiple == false)
        {
          if (this->gen_connect_single (node, pd) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ccm_pre_proc::"
                                 "gen_uses - "
                                 "gen_connect_single failed\n"),
                                -1);
            }

          if (this->gen_disconnect_single (node, pd) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ccm_pre_proc::"
                                 "gen_uses - "
                                 "gen_disconnect_single failed\n"),
                                -1);
            }

          if (this->gen_get_connection_single (node, pd) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ccm_pre_proc::"
                                 "gen_uses - "
                                 "gen_get_connection_single failed\n"),
                                -1);
            }
        }
      else
        {
          if (this->gen_connect_multiple (node, pd) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ccm_pre_proc::"
                                 "gen_uses - "
                                 "gen_connect_multiple failed\n"),
                                -1);
            }

          if (this->gen_disconnect_multiple (node, pd) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ccm_pre_proc::"
                                 "gen_uses - "
                                 "gen_disconnect_multiple failed\n"),
                                -1);
            }

          if (this->gen_get_connection_multiple (node, pd) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_ccm_pre_proc::"
                                 "gen_uses - "
                                 "gen_get_connection_single failed\n"),
                                -1);
            }
        }
        
      // Might as well clean up here instead of putting 5 loops in 
      // AST_Component::destroy ().
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_emits (be_component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s = node->emits ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      if (this->gen_emits_connect (node, pd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_emits - "
                             "gen_emits_connect failed\n"),
                            -1);
        }

      if (this->gen_emits_disconnect (node, pd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_emits - "
                             "gen_emits_disconnect failed\n"),
                            -1);
        }
        
      // Might as well clean up here instead of putting 5 loops in 
      // AST_Component::destroy ().
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_publishes (be_component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s = node->publishes ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      if (this->gen_subscribe (node, pd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_publishes - "
                             "gen_subscribe failed\n"),
                            -1);
        }

      if (this->gen_unsubscribe (node, pd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_publishes - "
                             "gen_unsubscribe failed\n"),
                            -1);
        }
        
      // Might as well clean up here instead of putting 5 loops in 
      // AST_Component::destroy ().
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_consumes (be_component *node)
{
  ACE_Unbounded_Queue<AST_Component::port_description> &s = node->consumes ();
  AST_Component::port_description *pd = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> iter (s);
       ! iter.done ();
       iter.advance ())
    {
      iter.next (pd);

      if (this->gen_get_consumer (node, pd) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_consumes - "
                             "gen_consumes_get_connection failed\n"),
                            -1);
        }
              
      // Might as well clean up here instead of putting 5 loops in 
      // AST_Component::destroy ().
      pd->id->destroy ();
      delete pd->id;
      pd->id = 0;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_factories (be_home *node,
                                        AST_Interface *xplicit)
{
  AST_Operation **item = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->factories ());
       ! i.done ();
       i.advance ())
    {
      i.next (item);
      (*item)->set_defined_in (xplicit);
      UTL_ScopedName *new_name =
        this->create_scoped_name (0,
                                  (*item)->local_name ()->get_string (),
                                  0,
                                  xplicit);
      (*item)->set_name (new_name);

      if ((*item)->be_insert_exception (this->create_failure_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_factories - "
                             "exception insertion failed\n"),
                            -1);
        }

      if (0 == xplicit->be_add_operation (*item))
        {
          return -1;
        }
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_finders (be_home *node,
                                      AST_Interface *xplicit)
{
  AST_Operation **item = 0;

  for (ACE_Unbounded_Queue_Iterator<AST_Operation *> i (node->finders ());
       ! i.done ();
       i.advance ())
    {
      i.next (item);
      (*item)->set_defined_in (xplicit);
      UTL_ScopedName *new_name =
        this->create_scoped_name (0,
                                  (*item)->local_name ()->get_string (),
                                  0,
                                  xplicit);
      (*item)->set_name (new_name);

      if ((*item)->be_insert_exception (this->finder_failure_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_ccm_pre_proc::"
                             "gen_factories - "
                             "exception insertion failed\n"),
                            -1);
        }

      if (0 == xplicit->be_add_operation (*item))
        {
          return -1;
        }
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_implicit_ops (be_home *node,
                                           AST_Interface *implicit)
{
  if (this->gen_create (node, implicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_implicit_ops - "
                         "gen_create failed\n"),
                        -1);
    }

  AST_ValueType *pk = node->primary_key ();

  if (pk == 0)
    {
      return 0;
    }

  if (this->gen_find_by_primary_key (node, implicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_implicit_ops - "
                         "gen_find_by_primary_key failed\n"),
                        -1);
    }

  if (this->gen_remove (node, implicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_implicit_ops - "
                         "gen_remove failed\n"),
                        -1);
    }

  if (this->gen_get_primary_key (node, implicit) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_implicit_ops - "
                         "gen_get_primary_key failed\n"),
                        -1);
    }

  return 0;
}

// **************************************************************

int
be_visitor_ccm_pre_proc::gen_connect_single (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  if (pd == 0)
    {
      return -1;
    }

  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("connect_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  op->set_name (op_full_name);
  Identifier arg_id ("conxn");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               pd->impl,
                               &arg_name),
                  -1);
  arg_id.destroy ();
  op->be_add_argument (arg);

  UTL_ExceptList *tail = 0;
  ACE_NEW_RETURN (tail,
                  UTL_ExceptList (this->invalid_connection_,
                                  0),
                  -1);
  UTL_ExceptList *connect_single = 0;
  ACE_NEW_RETURN (connect_single,
                  UTL_ExceptList (this->already_connected_,
                                  tail),
                 -1);
  op->be_add_exceptions (connect_single);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_disconnect_single (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  if (pd == 0)
    {
      return -1;
    }

  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("disconnect_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (pd->impl,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_full_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  UTL_ExceptList *disconnect_single = 0;
  ACE_NEW_RETURN (disconnect_single,
                  UTL_ExceptList (this->no_connection_,
                                  0),
                 -1);
  op->be_add_exceptions (disconnect_single);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_get_connection_single (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  if (pd == 0)
    {
      return -1;
    }

  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("get_connection_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (pd->impl,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_full_name);
  op->set_defined_in (node);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_connect_multiple (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  if (pd == 0)
    {
      return -1;
    }

  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("connect_",
                              pd->id->get_string (),
                              0,
                              node);

  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (this->cookie_,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_full_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  Identifier arg_id ("connection");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               pd->impl,
                               &arg_name),
                  -1);
  arg_id.destroy ();
  op->be_add_argument (arg);
  UTL_ExceptList *tail = 0;
  ACE_NEW_RETURN (tail,
                  UTL_ExceptList (this->invalid_connection_,
                                  0),
                  -1);
  UTL_ExceptList *connect_multiple = 0;
  ACE_NEW_RETURN (connect_multiple,
                  UTL_ExceptList (this->exceeded_connection_limit_,
                                  tail),
                 -1);
  op->be_add_exceptions (connect_multiple);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_disconnect_multiple (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  if (pd == 0)
    {
      return -1;
    }

  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("disconnect_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (pd->impl,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_full_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  Identifier arg_id ("ck");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               this->cookie_,
                               &arg_name),
                  -1);
  arg_id.destroy ();
  op->be_add_argument (arg);
  UTL_ExceptList *disconnect_multiple = 0;
  ACE_NEW_RETURN (disconnect_multiple,
                  UTL_ExceptList (this->invalid_connection_,
                                  0),
                 -1);
  op->be_add_exceptions (disconnect_multiple);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_get_connection_multiple (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  if (pd == 0)
    {
      return -1;
    }

  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("get_connections_",
                              pd->id->get_string (),
                              0,
                              node);

  // Look up the implied IDL typedef created in the front end.
  // It will be the return type of the created operation.
  ACE_CString connections_string (pd->id->get_string ());
  connections_string += "Connections";
  Identifier connections_id (connections_string.c_str ());
  UTL_ScopedName connections_name (&connections_id,
                                   0);
  AST_Decl *d = node->lookup_by_name (&connections_name,
                                      true);
  be_typedef *td = be_typedef::narrow_from_decl (d);
  connections_id.destroy ();

  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (td,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_full_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_push_op (be_eventtype *node,
                                      AST_Interface *consumer)
{
  UTL_ScopedName *op_full_name =
    this->create_scoped_name ("push_",
                              node->local_name (),
                              0,
                              consumer);
  be_operation *push_op = 0;
  ACE_NEW_RETURN (push_op,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                0,
                                false,
                                false),
                  -1);
  push_op->set_defined_in (consumer);
  push_op->set_imported (node->imported ());
  push_op->set_name (op_full_name);
  ACE_CString arg_string ("the_",
                          0,
                          false);
  arg_string += node->local_name ();
  Identifier arg_id (arg_string.fast_rep ());
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               node,
                               &arg_name),
                  -1);
  arg_id.destroy ();
  push_op->be_add_argument (arg);

  if (0 == consumer->be_add_operation (push_op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_subscribe (be_component *node,
                                        AST_Component::port_description *pd)
{
  UTL_ScopedName *op_name =
    this->create_scoped_name ("subscribe_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (this->cookie_,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  op->set_name (op_name);

  AST_Interface *i = this->lookup_consumer (pd);

  if (i == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_subscribe - "
                         "consumer lookup failed\n"),
                        -1);
    }

  Identifier arg_id ("consumer");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               i,
                               &arg_name),
                  -1);
  op->be_add_argument (arg);
  UTL_ExceptList *subscribe = 0;
  ACE_NEW_RETURN (subscribe,
                  UTL_ExceptList (this->exceeded_connection_limit_,
                                  0),
                  -1);
  op->be_add_exceptions (subscribe);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_unsubscribe (be_component *node,
                                          AST_Component::port_description *pd)
{
  AST_Interface *i = this->lookup_consumer (pd);

  if (i == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_unsubscribe - "
                         "consumer lookup failed\n"),
                        -1);
    }

  UTL_ScopedName *op_name =
    this->create_scoped_name ("unsubscribe_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (i,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  op->set_name (op_name);
  Identifier arg_id ("ck");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               this->cookie_,
                               &arg_name),
                  -1);
  op->be_add_argument (arg);
  UTL_ExceptList *unsubscribe = 0;
  ACE_NEW_RETURN (unsubscribe,
                  UTL_ExceptList (this->invalid_connection_,
                                  0),
                  -1);
  op->be_add_exceptions (unsubscribe);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_emits_connect (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  UTL_ScopedName *op_name =
    this->create_scoped_name ("connect_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  AST_Interface *i = this->lookup_consumer (pd);

  if (i == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_emits_connect - "
                         "consumer lookup failed\n"),
                        -1);
    }

  Identifier arg_id ("consumer");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               i,
                               &arg_name),
                  -1);
  op->be_add_argument (arg);
  UTL_ExceptList *emits_connect = 0;
  ACE_NEW_RETURN (emits_connect,
                  UTL_ExceptList (this->already_connected_,
                                  0),
                  -1);
  op->be_add_exceptions (emits_connect);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_emits_disconnect (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  AST_Interface *i = this->lookup_consumer (pd);

  if (i == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_emits_disconnect - "
                         "consumer lookup failed\n"),
                        -1);
    }

  UTL_ScopedName *op_name =
    this->create_scoped_name ("disconnect_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (i,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());
  UTL_ExceptList *emits_disconnect = 0;
  ACE_NEW_RETURN (emits_disconnect,
                  UTL_ExceptList (this->no_connection_,
                                  0),
                  -1);
  op->be_add_exceptions (emits_disconnect);

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_get_consumer (
    be_component *node,
    AST_Component::port_description *pd
  )
{
  AST_Interface *i = this->lookup_consumer (pd);

  if (i == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ccm_pre_proc::"
                         "gen_consumes_get_connection - "
                         "consumer lookup failed\n"),
                        -1);
    }

  UTL_ScopedName *op_name =
    this->create_scoped_name ("get_consumer_",
                              pd->id->get_string (),
                              0,
                              node);
  be_operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (i,
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  op->set_defined_in (node);
  op->set_imported (node->imported ());

  if (0 == node->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_create (be_home *node,
                                     AST_Interface *implicit)
{
  UTL_ScopedName *op_name = this->create_scoped_name (0,
                                                      "create",
                                                      0,
                                                      implicit);
  AST_Operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (node->managed_component (),
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  AST_ValueType *pk = node->primary_key ();
  UTL_ExceptList *exceps = 0;
  ACE_NEW_RETURN (exceps,
                  UTL_ExceptList (this->create_failure_,
                                  0),
                  -1);

  if (pk != 0)
    {
      Identifier arg_id ("key");
      UTL_ScopedName arg_name (&arg_id,
                               0);
      AST_Argument *arg = 0;
      ACE_NEW_RETURN (arg,
                      be_argument (AST_Argument::dir_IN,
                                   pk,
                                   &arg_name),
                      -1);
      arg_id.destroy ();
      op->be_add_argument (arg);
      UTL_ExceptList *tail = 0;
      ACE_NEW_RETURN (tail,
                      UTL_ExceptList (this->invalid_key_,
                                      0),
                      -1);
      UTL_ExceptList *middle = 0;
      ACE_NEW_RETURN (middle,
                      UTL_ExceptList (this->duplicate_key_value_,
                                      tail),
                      -1);
      exceps->nconc (middle);
    }

  op->be_add_exceptions (exceps);
  op->set_defined_in (implicit);
  op->set_imported (node->imported ());

  if (0 == implicit->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_find_by_primary_key (be_home *node,
                                                  AST_Interface *implicit)
{
  UTL_ScopedName *op_name = this->create_scoped_name (0,
                                                      "find_by_primary_key",
                                                      0,
                                                      implicit);
  AST_Operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (node->managed_component (),
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  AST_ValueType *pk = node->primary_key ();
  Identifier arg_id ("key");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  AST_Argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               pk,
                               &arg_name),
                  -1);
  arg_id.destroy ();
  op->be_add_argument (arg);
  UTL_ExceptList *tail = 0;
  ACE_NEW_RETURN (tail,
                  UTL_ExceptList (this->invalid_key_,
                                  0),
                  -1);
  UTL_ExceptList *middle = 0;
  ACE_NEW_RETURN (middle,
                  UTL_ExceptList (this->unknown_key_value_,
                                  tail),
                  -1);
  UTL_ExceptList *exceps = 0;
  ACE_NEW_RETURN (exceps,
                  UTL_ExceptList (this->finder_failure_,
                                  middle),
                  -1);
  op->be_add_exceptions (exceps);
  op->set_defined_in (implicit);
  op->set_imported (node->imported ());

  if (0 == implicit->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_remove (be_home *node,
                                     AST_Interface *implicit)
{
  UTL_ScopedName *op_name = this->create_scoped_name (0,
                                                      "remove",
                                                      0,
                                                      implicit);
  AST_Operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (be_global->void_type (),
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  AST_ValueType *pk = node->primary_key ();
  Identifier arg_id ("key");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  AST_Argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               pk,
                               &arg_name),
                  -1);
  arg_id.destroy ();
  op->be_add_argument (arg);
  UTL_ExceptList *tail = 0;
  ACE_NEW_RETURN (tail,
                  UTL_ExceptList (this->invalid_key_,
                                  0),
                  -1);
  UTL_ExceptList *middle = 0;
  ACE_NEW_RETURN (middle,
                  UTL_ExceptList (this->unknown_key_value_,
                                  tail),
                  -1);
  UTL_ExceptList *exceps = 0;
  ACE_NEW_RETURN (exceps,
                  UTL_ExceptList (this->remove_failure_,
                                  middle),
                  -1);
  op->be_add_exceptions (exceps);
  op->set_defined_in (implicit);
  op->set_imported (node->imported ());

  if (0 == implicit->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::gen_get_primary_key (be_home *node,
                                              AST_Interface *implicit)
{
  UTL_ScopedName *op_name = this->create_scoped_name (0,
                                                      "get_primary_key",
                                                      0,
                                                      implicit);
  AST_Operation *op = 0;
  ACE_NEW_RETURN (op,
                  be_operation (node->primary_key (),
                                AST_Operation::OP_noflags,
                                0,
                                0,
                                0),
                  -1);
  op->set_name (op_name);
  Identifier arg_id ("comp");
  UTL_ScopedName arg_name (&arg_id,
                           0);
  AST_Argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (AST_Argument::dir_IN,
                               node->managed_component (),
                               &arg_name),
                  -1);
  arg_id.destroy ();
  op->be_add_argument (arg);
  op->set_defined_in (implicit);
  op->set_imported (node->imported ());

  if (0 == implicit->be_add_operation (op))
    {
      return -1;
    }

  return 0;
}

// ********************************************************************

int
be_visitor_ccm_pre_proc::lookup_cookie (be_component *node)
{
  if (this->cookie_ == 0)
    {
      Identifier local_id ("Cookie");
      UTL_ScopedName local_name (&local_id,
                                 0);
      UTL_ScopedName cookie_name (&this->module_id_,
                                  &local_name);
      AST_Decl *d = node->lookup_by_name (&cookie_name,
                                          true);
      local_id.destroy ();

      if (d == 0)
        {
          idl_global->err ()->lookup_error (&cookie_name);
          return -1;
        }

      this->cookie_ = be_valuetype::narrow_from_decl (d);

      if (this->cookie_ == 0)
        {
          idl_global->err ()->valuetype_expected (d);
          return -1;
        }
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::lookup_exceptions (be_component *node)
{
  int status = 0;

  for (int i = 0; i < N_EXCEPS; ++i)
    {
      status = this->lookup_one_exception (node,
                                           EXCEP_NAMES[i],
                                           EXCEPS[i]);

      if (status == -1)
        {
          return -1;
        }
    }

  this->already_connected_          = EXCEPS[0];
  this->invalid_connection_         = EXCEPS[1];
  this->no_connection_              = EXCEPS[2];
  this->exceeded_connection_limit_  = EXCEPS[3];
  this->create_failure_             = EXCEPS[4];
  this->remove_failure_             = EXCEPS[5];
  this->finder_failure_             = EXCEPS[6];
  this->invalid_key_                = EXCEPS[7];
  this->unknown_key_value_          = EXCEPS[8];
  this->duplicate_key_value_        = EXCEPS[9];

  return 0;
}

int
be_visitor_ccm_pre_proc::lookup_one_exception (be_component *node,
                                               const char *name,
                                               be_exception *&result)
{
  Identifier id (name);
  UTL_ScopedName local_name (&id,
                             0);
  UTL_ScopedName scoped_name (&this->module_id_,
                              &local_name);
  AST_Decl *d = node->lookup_by_name (&scoped_name,
                                      true);
  id.destroy ();

  if (d == 0)
    {
      idl_global->err ()->lookup_error (&scoped_name);
      return -1;
    }

  result = be_exception::narrow_from_decl (d);

  if (result == 0)
    {
      return -1;
    }

  return 0;
}

int
be_visitor_ccm_pre_proc::create_event_consumer (be_eventtype *node)
{
  AST_Interface *event_consumer = 0;
  UTL_Scope *s = node->defined_in ();
  AST_Module *m = AST_Module::narrow_from_scope (s);

  UTL_ScopedName *consumer_name =
    this->create_scoped_name (0,
                              node->local_name (),
                              "Consumer",
                              ScopeAsDecl (node->defined_in ()));

  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  idl_global->scopes ().push (s);

  Identifier parent_id ("EventConsumerBase");
  UTL_ScopedName parent_local_name (&parent_id,
                                    0);
  UTL_ScopedName parent_full_name (&this->module_id_,
                                    &parent_local_name);
  UTL_NameList parent_list (&parent_full_name,
                            0);
  FE_InterfaceHeader header (consumer_name,
                              &parent_list,
                              false,
                              false,
                              true);
  ACE_NEW_RETURN (event_consumer,
                  be_interface (header.name (),
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                false,
                                false),
                  -1);
  parent_id.destroy ();

  // Back to reality.
  idl_global->scopes ().pop ();

  event_consumer->set_defined_in (s);
  event_consumer->set_imported (node->imported ());
  event_consumer->set_name (consumer_name);

  // Set repo id to 0, so it will be recomputed on the next access,
  // and set the prefix to the eventtype's prefix. All this is
  // necessary in case the eventtype's prefix was modified after
  // its declaration. We assume 'implied IDL' means that the
  // derived event consumer interface should have the same prefix.
  event_consumer->repoID (0);
  event_consumer->prefix (const_cast<char*> (node->prefix ()));

  be_type::narrow_from_decl (event_consumer)->gen_fwd_helper_name ();
  m->be_add_interface (event_consumer);
  return this->gen_push_op (node,
                            event_consumer);
}

AST_Interface *
be_visitor_ccm_pre_proc::lookup_consumer (AST_Component::port_description *pd)
{
  ACE_CString rettype_string (pd->impl->local_name ()->get_string ());
  rettype_string += "Consumer";
  Identifier rettype_id (rettype_string.fast_rep ());
  AST_Decl *d = pd->impl->defined_in ()->lookup_by_name_local (&rettype_id,
                                                               0);
  rettype_id.destroy ();

  if (d == 0)
    {
      return 0;
    }

  AST_Interface *i = AST_Interface::narrow_from_decl (d);

  if (i == 0)
    {
      idl_global->err ()->interface_expected (d);
      return 0;
    }

  return i;
}

AST_Interface *
be_visitor_ccm_pre_proc::create_explicit (be_home *node)
{
  UTL_NameList *parent_list = this->compute_inheritance (node);
  FE_InterfaceHeader header (0,
                             parent_list,
                             false,
                             false,
                             true);

  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  idl_global->scopes ().push (node->defined_in ());

  UTL_ScopedName *explicit_name =
  this->create_scoped_name (0,
                            node->local_name (),
                            "Explicit",
                            ScopeAsDecl (node->defined_in ()));

  AST_Interface *i = 0;
  ACE_NEW_RETURN (i,
                  be_interface (explicit_name,
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                false,
                                false),
                  0);

  // Back to reality.
  idl_global->scopes ().pop ();

  i->set_name (explicit_name);
  i->set_defined_in (node->defined_in ());
  i->set_imported (node->imported ());
  be_interface::narrow_from_decl (i)->gen_fwd_helper_name ();

  // Reuse the home's decls in the explicit interface. No need
  // to check for name clashes, redefinition, etc. because it
  // has already been done in the home and the explicit interface
  // is empty at this point. Later addition of factory and finder
  // operations will do these checks to make sure they don't
  // clash with the other decls.
  for (UTL_ScopeActiveIterator iter (node, UTL_Scope::IK_decls);
       ! iter.is_done ();
       iter.next ())
    {
      AST_Decl *d = iter.item ();
      d->set_defined_in (i);
      UTL_ScopedName *new_name =
        this->create_scoped_name (0,
                                  d->local_name ()->get_string (),
                                  0,
                                  i);
      d->set_name (new_name);
      i->add_to_scope (d);
    }
    
  header.destroy ();
  parent_list->destroy ();
  delete parent_list;
  parent_list = 0;

  AST_Module *m = AST_Module::narrow_from_scope (node->defined_in ());
  m->be_add_interface (i);
  return i;
}

AST_Interface *
be_visitor_ccm_pre_proc::create_implicit (be_home *node)
{
  Identifier *parent_id = 0;
  ACE_NEW_RETURN (parent_id,
                  Identifier ("KeylessCCMHome"),
                  0);
                  
  UTL_ScopedName *parent_local_name = 0;
  ACE_NEW_RETURN (parent_local_name,
                  UTL_ScopedName (parent_id, 0),
                  0);
                  
  UTL_ScopedName *parent_full_name = 0;
  ACE_NEW_RETURN (parent_full_name,
                  UTL_ScopedName (this->module_id_.copy (),
                                  parent_local_name),
                  0);
                  
  UTL_NameList parent_list (parent_full_name, 0);
  
  UTL_NameList *parent_list_ptr = 0;

  if (node->primary_key () == 0)
    {
      parent_list_ptr = &parent_list;
    }

  FE_InterfaceHeader header (0,
                             parent_list_ptr,
                             false,
                             false,
                             true);

  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  idl_global->scopes ().push (node->defined_in ());

  UTL_ScopedName *implicit_name =
    this->create_scoped_name (0,
                              node->local_name (),
                              "Implicit",
                              ScopeAsDecl (node->defined_in ()));
                              
  AST_Interface *i = 0;
  ACE_NEW_RETURN (i,
                  be_interface (implicit_name,
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                false,
                                false),
                  0);

  // Back to reality.
  idl_global->scopes ().pop ();
  
  header.destroy ();
  parent_list.destroy ();

  i->set_name (implicit_name);
  i->set_defined_in (node->defined_in ());
  i->set_imported (node->imported ());
  
  be_interface::narrow_from_decl (i)->gen_fwd_helper_name ();
  AST_Module *m = AST_Module::narrow_from_scope (node->defined_in ());
  m->be_add_interface (i);
  
  return i;
}

AST_Interface *
be_visitor_ccm_pre_proc::create_equivalent (be_home *node,
                                            AST_Interface *xplicit,
                                            AST_Interface *implicit)
{
  UTL_Scope *s = node->defined_in ();
  UTL_ScopedName *equiv_name =
    this->create_scoped_name (0,
                              node->local_name (),
                              0,
                              ScopeAsDecl (s));
  UTL_NameList tail (implicit->name (),
                     0);
  UTL_NameList parent_list (xplicit->name (),
                            &tail);
  FE_InterfaceHeader header (0,
                             &parent_list,
                             false,
                             false,
                             true);

  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  idl_global->scopes ().push (node->defined_in ());

  AST_Interface *retval = 0;
  ACE_NEW_RETURN (retval,
                  be_interface (equiv_name,
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                false,
                                false),
                  0);

  // Back to reality.
  idl_global->scopes ().pop ();

  // So we can skip typecode generation.
  retval->home_equiv (true);

  retval->set_name (equiv_name);
  retval->set_defined_in (s);
  retval->set_imported (node->imported ());
  be_interface::narrow_from_decl (retval)->gen_fwd_helper_name ();
  UTL_ScopedName *unmangled_name =
    static_cast<UTL_ScopedName *> (node->name ()->copy ());
  UTL_ScopedName *mangled_name =
    this->create_scoped_name (0,
                              node->local_name (),
                              "_tao_home_name_extension",
                              ScopeAsDecl (s));
  node->set_name (mangled_name);
  AST_Module *m = AST_Module::narrow_from_scope (s);
  m->be_add_interface (retval);
  node->set_name (unmangled_name);
  return retval;
}

UTL_ScopedName *
be_visitor_ccm_pre_proc::create_scoped_name (const char *prefix,
                                             const char *local_name,
                                             const char *suffix,
                                             AST_Decl *parent)
{
  ACE_CString local_string (prefix,
                            0,
                            false);
  local_string += local_name;
  local_string += suffix;
  Identifier *local_id = 0;
  ACE_NEW_RETURN (local_id,
                  Identifier (local_string.fast_rep ()),
                  0);
  UTL_ScopedName *last_segment = 0;
  ACE_NEW_RETURN (last_segment,
                  UTL_ScopedName (local_id,
                                  0),
                  0);
  UTL_ScopedName *full_name =
    static_cast<UTL_ScopedName *> (parent->name ()->copy ());
  full_name->nconc (last_segment);
  return full_name;
}

UTL_NameList *
be_visitor_ccm_pre_proc::compute_inheritance (be_home *node)
{
  UTL_NameList *retval = 0;

  if (node->base_home () == 0)
    {
      Identifier *local_id = 0;
      ACE_NEW_RETURN (local_id,
                      Identifier ("CCMHome"),
                      0);
      UTL_ScopedName *local_name = 0;
      ACE_NEW_RETURN (local_name,
                      UTL_ScopedName (local_id,
                                      0),
                      0);
      UTL_ScopedName *full_name = 0;
      ACE_NEW_RETURN (full_name,
                      UTL_ScopedName (this->module_id_.copy (),
                                      local_name),
                      0);
      ACE_NEW_RETURN (retval,
                      UTL_NameList (full_name,
                                    0),
                      0);

    }
  else
    {
      ACE_CString new_local (
          node->base_home ()->local_name ()->get_string ()
        );
      new_local += "Explicit";
      UTL_ScopedName *parent_name =
        static_cast<UTL_ScopedName *> (node->base_home ()->name ()->copy ());
      parent_name->last_component ()->replace_string (new_local.c_str ());
      ACE_NEW_RETURN (retval,
                      UTL_NameList (parent_name,
                                    0),
                      0);
    }

  long n_supports = node->n_inherits ();
  UTL_ScopedName *supported_name = 0;
  UTL_NameList *conc_value = 0;

  for (long i = 0; i < n_supports; ++i)
    {
      supported_name =
        static_cast<UTL_ScopedName *> (node->inherits ()[i]->name ()->copy ());
      ACE_NEW_RETURN (conc_value,
                      UTL_NameList (supported_name,
                                    0),
                      0);
      retval->nconc (conc_value);
    }

  return retval;
}
