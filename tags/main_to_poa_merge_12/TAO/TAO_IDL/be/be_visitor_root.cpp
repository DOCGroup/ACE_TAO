//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_root.cpp
//
// = DESCRIPTION
//    Visitors for generation of code for Root
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_root.h"

// Root visitor for client header
be_visitor_root_ch::be_visitor_root_ch (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->client_header ();
}

be_visitor_root_ch::~be_visitor_root_ch (void)
{
}

int be_visitor_root_ch::visit_root (be_root *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT_CH); // set the code generation state

  // open the client-side header file
  if (cg->client_header (idl_global->be_get_client_hdr_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening client header\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (node->be_scope::gen_client_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: code generation for scope failed\n"));
      return -1;
    }
  cg->pop ();

  // at this point, we must be in the initial state
  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_INITIAL);

  (void) cg->end_client_header (); // generate the last #endif
  return 0;
}

int be_visitor_root_ch::visit_sequence (be_sequence *node)
{
  return 0;
}

// Root visitor for client inline

be_visitor_root_ci::be_visitor_root_ci (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->client_inline ();
}

be_visitor_root_ci::~be_visitor_root_ci (void)
{
}

int be_visitor_root_ci::visit_root (be_root *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT_CI); // set the code generation state

  // open the client-side inline file
  if (cg->client_inline (idl_global->be_get_client_inline_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening client inline file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (node->be_scope::gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: code generation for scope failed\n"));
      return -1;
    }
  cg->pop ();

  // at this point, we must be in the initial state
  //  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_INITIAL);

  return 0;
}

int be_visitor_root_ci::visit_sequence (be_sequence *node)
{
  return 0;
}

// Root visitor for client stubs

be_visitor_root_cs::be_visitor_root_cs (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->client_stubs ();
}

be_visitor_root_cs::~be_visitor_root_cs (void)
{
}

int be_visitor_root_cs::visit_root (be_root *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT_CS); // set the code generation state

  // open the client-side stub file
  if (cg->client_stubs (idl_global->be_get_client_stub_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening client stubs file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (node->be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: code generation for scope failed\n"));
      return -1;
    }
  cg->pop ();

  // at this point, we must be in the initial state
  //  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_INITIAL);

  return 0;
}

int be_visitor_root_cs::visit_sequence (be_sequence *node)
{
  return 0;
}

// Root visitor for server header

be_visitor_root_sh::be_visitor_root_sh (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->server_header ();
}

be_visitor_root_sh::~be_visitor_root_sh (void)
{
}

int be_visitor_root_sh::visit_root (be_root *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT_SH); // set the code generation state

  // open the server-side header file
  if (cg->server_header (idl_global->be_get_server_hdr_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening server header\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (node->be_scope::gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "be_root: server code generation for scope failed\n"));
      return -1;
    }

  cg->pop ();

  // at this point, we must be in the initial state
  //  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_INITIAL);

  (void) cg->end_server_header (); // generate the last #endif statement
  return 0;
}

int be_visitor_root_sh::visit_sequence (be_sequence *node)
{
  return 0;
}

// Root visitor for server inline

be_visitor_root_si::be_visitor_root_si (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->server_inline ();
}

be_visitor_root_si::~be_visitor_root_si (void)
{
}

int be_visitor_root_si::visit_root (be_root *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT_SI); // set the code generation state

  // open the server-side inline file
  if (cg->server_inline (idl_global->be_get_server_inline_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening server inline file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (node->be_scope::gen_server_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: code generation for scope failed\n"));
      return -1;
    }
  cg->pop ();

  // at this point, we must be in the initial state
  //  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_INITIAL);

  return 0;
}

int be_visitor_root_si::visit_sequence (be_sequence *node)
{
  return 0;
}

// Root visitor for server server

be_visitor_root_ss::be_visitor_root_ss (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  this->os_ = cg->server_skeletons ();
}

be_visitor_root_ss::~be_visitor_root_ss (void)
{
}

int be_visitor_root_ss::visit_root (be_root *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT_SS); // set the code generation state

  // open the server-side skeleton file
  if (cg->server_skeletons (idl_global->be_get_server_skeleton_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening server skeletons file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (node->be_scope::gen_server_skeletons () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: code generation for scope failed\n"));
      return -1;
    }
  cg->pop ();

  // at this point, we must be in the initial state
  //  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_INITIAL);

  return 0;
}

int be_visitor_root_ss::visit_sequence (be_sequence *node)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<be_visitor_root_ch, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_root_ci, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_root_cs, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_root_sh, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_root_si, ACE_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Singleton<be_visitor_root_ss, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<be_visitor_root_ch, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_root_ci, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_root_cs, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_root_sh, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_root_si, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Singleton<be_visitor_root_ss, ACE_SYNCH_RECURSIVE_MUTEX>
#endif
