/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.h
//
// = DESCRIPTION
//    Extension of class AST_Interface_Fwd that provides additional means for C++
//    mapping of an interface.
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
 * BE_InterfaceFwd
 */

be_interface_fwd::be_interface_fwd (void)
{
}

be_interface_fwd::be_interface_fwd (UTL_ScopedName *n, UTL_StrList *p)
  : AST_InterfaceFwd (n, p),
    AST_Decl (AST_Decl::NT_interface_fwd, n, p)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// generate the client header
int be_interface_fwd::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      // get the client header
      ch = cg->client_header ();

      ch->indent (); // start from the current

      // all we do in this is generate a forward declaration of the class
      *ch << "class " << this->local_name () << ";" << nl;

      // generate the ifdefined macro for the _ptr type
      ch->gen_ifdef_macro (this->flatname (), "_ptr");

      // generate the _ptr declaration
      *ch << "typedef " << this->local_name () << " *" << this->local_name () <<
        "_ptr; \n";

      ch->gen_endif ();

      // enclose under an ifdef macro
      ch->gen_ifdef_macro (this->flatname (), "_var");

      // generate the _var declaration
      if (this->gen_var_defn () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_interface_fwd - error generating _var definition\n"));
          return -1;
        }
      // gen an endif
      ch->gen_endif ();

      // enclose under an ifdef macro
      ch->gen_ifdef_macro (this->flatname (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (this->gen_out_defn () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_interface_fwd - error generating _var definition\n"));
          return -1;
        }

      // generate the endif macro
      ch->gen_endif ();

      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
}

int
be_interface_fwd::gen_client_stubs (void)
{
  return 0;
}

int
be_interface_fwd::gen_client_inline (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  // generate the ifdefined macro for  the _var type
  ci->gen_ifdef_macro (this->flatname (), "_var");

  if (this->gen_var_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface_fwd: _var impl code gen failed\n"));
      return -1;
    }

  ci->gen_endif ();

  // generate the ifdefined macro for  the _out type
  ci->gen_ifdef_macro (this->flatname (), "_out");

  if (this->gen_out_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface_fwd: _out impl code gen failed\n"));
      return -1;
    }
  ci->gen_endif ();

  return 0;
}

int
be_interface_fwd::gen_server_header (void)
{
  return 0;
}

int
be_interface_fwd::gen_server_skeletons (void)
{
  return 0;
}

int
be_interface_fwd::gen_server_inline (void)
{
  return 0;
}

int
be_interface_fwd::gen_typecode (void)
{
  return 0;
}

long
be_interface_fwd::tc_size (void)
{
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_interface_fwd, AST_InterfaceFwd, be_type)
IMPL_NARROW_FROM_DECL (be_interface_fwd)
