// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_module.cpp
//
// = DESCRIPTION
//    Extension of class AST_Module that provides additional means for C++
//    mapping of a module
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

/*
 * BE_Module
 */
be_module::be_module (void)
{
}

be_module::be_module (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (AST_Decl::NT_module, n, p),
    UTL_Scope (AST_Decl::NT_module)
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// generate the client header 
int be_module::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_MODULE_CH); // set the current code generation state
  ch = cg->client_header (); // get the stream

  // XXXASG - Modules really map to namespace. We need to see if our target
  // compiler supports namespaces or not. For this release we opt to generate a
  // class definition for a module

  ch->indent (); // start with whatever indentation level we are at

  // now generate the class definition
  *ch << "class " << local_name ();

  // generate the body
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent (0);

  // generate code for the module definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (be_scope::gen_client_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_module::gen_client_header\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  ch->decr_indent ();
  *ch << "};\n\n";
  cg->pop ();
  return 0;
}

int be_module::gen_client_stubs (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_MODULE_CS); // set the current code generation state

  // gen code for elements in the scope
  if (be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_module::gen_client_stubs\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  cg->pop ();
  return 0;
}

int be_module::gen_server_header (void)
{
  TAO_OutStream *sh; // output stream
  TAO_NL  nl;        // end line
  AST_Decl *d;

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_MODULE_SH); // set the current code generation state

  sh = cg->server_header ();

  // generate the skeleton class name

  sh->indent (); // start with whatever indentation level we are at

  // now generate the class definition. The prefix POA_ is prepended to our
  // name only if we are the outermost module
  d = ScopeAsDecl (this->defined_in ());
  if (d->node_type () == AST_Decl::NT_root)
    // we are outermost module
    *sh << "class POA_" << local_name () << nl;
  else
    // we are inside another module
    *sh << "class " << local_name () << nl;

  *sh << "{" << nl;
  *sh << "public:\n";
  sh->incr_indent (0);

  if (be_scope::gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_module::gen_server_header\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  sh->decr_indent ();
  *sh << "};\n\n";
  cg->pop ();
  return 0;
}

int be_module::gen_server_skeletons (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_MODULE_SS); // set the current code generation state

  if (be_scope::gen_server_skeletons () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_module::gen_server_skeletons\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }
  cg->pop ();
  return 0;
}

// Generates the client-side inline information
int 
be_module::gen_client_inline (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_MODULE_CI); // set the current code generation state

  // gen code for elements in the scope
  if (be_scope::gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_module::gen_client_inline\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  cg->pop ();
  return 0;
}

// Generates the server-side inline
int 
be_module::gen_server_inline (void)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_MODULE_SI); // set the current code generation state

  // gen code for elements in the scope
  if (be_scope::gen_server_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_module::gen_server_inline\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  cg->pop ();
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_module, AST_Module, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_module)
IMPL_NARROW_FROM_SCOPE (be_module)

  
