// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_root.cpp
//
// = DESCRIPTION
//    Extension of class AST_Root that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// Default Constructor
be_root::be_root (void)
{
}

// Constructor used to build the root of the abstract syntax tree (AST)
be_root::be_root (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Root (n, p),
    AST_Decl (AST_Decl::NT_root, n, p),
    UTL_Scope (AST_Decl::NT_root)
    
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

// =====================================================
//           CODE GENERATION METHODS
// =====================================================

// Starting point for the code generation. Called inside BE_produce ()
int be_root::gen_idl2cplusplus_mapping (void)
{
  int i;  // loop index
  UTL_ScopeActiveIterator	*si;  // instance of a scope iterator
  AST_Decl			*d;   // points to an individual node in the
				      // chain of nodes in the current scope

  // this is the main starting point from which the files are generated. We
  // delegate the task of code generation to a special Code Generation
  // object. This allows a number of different front ends to access the services
  // of a single code generator

  // C++ mapping involves producing 6 files. These include the client and
  // server side headers, the client and server side inlines, as well as client
  // stubs and server skeletons. 
  if (this->gen_client_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error generating client header file: %s\n",
		  idl_global->be_get_client_hdr_fname ()));
      return -1;
    }
  if (this->gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error generating client inline file: %s\n",
		  idl_global->be_get_client_inline_fname ()));
      return -1;
    }
  if (this->gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error generating client stubs file: %s\n",
		  idl_global->be_get_client_stub_fname ()));
      return -1;
    }
  if (this->gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error generating server header file: %s\n",
		  idl_global->be_get_server_hdr_fname ()));
      return -1;
    }
  if (this->gen_server_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error generating server inline file: %s\n",
		  idl_global->be_get_server_inline_fname ()));
      return -1;
    }
  if (this->gen_server_skeletons () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error generating server skeleton file: %s\n",
		  idl_global->be_get_server_skeleton_fname ()));
      return -1;
    }

  return 0;
}

// generate client header
int be_root::gen_client_header (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT); // set the code generation state

  // open the client-side header file
  if (cg->client_header (idl_global->be_get_client_hdr_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening client header\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (be_scope::gen_client_header () == -1)
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

// Generates the client-side stubs for the root
int be_root::gen_client_stubs (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT); // set the code generation state

  // open the client-side stub file
  if (cg->client_stubs (idl_global->be_get_client_stub_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening client stubs file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  return be_scope::gen_client_stubs ();

}

// Generates the client-side inlines for the root
int be_root::gen_client_inline (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT); // set the code generation state

  // open the client-side inline file
  if (cg->client_inline (idl_global->be_get_client_inline_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening client inline file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  return be_scope::gen_client_inline ();

}

// Generates the server-side header information for the root
int be_root::gen_server_header (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT); // set the code generation state

  // open the server-side header file
  if (cg->server_header (idl_global->be_get_server_hdr_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening server header\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  if (be_scope::gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, 
                  "be_root: server code generation for scope failed\n")); 
      return -1;
    }

  (void) cg->end_server_header (); // generate the last #endif statement
  return 0;
}

// Generates the server-side skeletons for the root
int be_root::gen_server_skeletons (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT); // set the code generation state

  // open the server-side skeleton file
  if (cg->server_skeletons (idl_global->be_get_server_skeleton_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening server skeletons file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  return be_scope::gen_server_skeletons ();
}

// Generates the server-side inlines for the root
int be_root::gen_server_inline (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_ROOT); // set the code generation state

  // open the server-side inline file
  if (cg->server_inline (idl_global->be_get_server_inline_fname ()) == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_root: Error opening server inline file\n"));
      return -1;
    }
  // delegate the task of code generation to the scope
  return be_scope::gen_server_inline ();

}

// We had to provide these since the AST_Root::fe_* method was setting the
// names of these three to "local type"

/*
 * Add this AST_Sequence to the locally defined types in this scope
 */
AST_Sequence *
be_root::fe_add_sequence (AST_Sequence *t)
{
  if (t == NULL)
    return NULL;

  add_to_local_types(t);
  return t;
}

/*
 * Add this AST_String to the locally defined types in this scope
 */
AST_String *
be_root::fe_add_string (AST_String *t)
{
  if (t == NULL)
    return NULL;

  add_to_local_types (t);

  return t;
}

/*
 * Add this AST_Array to the locally defined types in this scope
 */
AST_Array *
be_root::fe_add_array (AST_Array *t)
{
  if (t == NULL)
    return NULL;

  add_to_local_types (t);

  return t;
}

/*
 * Narrowing methods
 */
IMPL_NARROW_METHODS3 (be_root, AST_Root, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_root)
IMPL_NARROW_FROM_SCOPE (be_root)
