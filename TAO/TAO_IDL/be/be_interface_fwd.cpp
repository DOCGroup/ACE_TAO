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

  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();

}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// generate the client header 
int be_interface_fwd::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();
  if (!this->cli_hdr_gen_) // not already generated
    {
      *ch << "\n#if !defined (_" << cg->upcase (this->flatname ()) <<
        "_DEFN_)\n"; 
      *ch << "#define _" << cg->upcase (this->flatname ()) << "_DEFN_\n";
      ch->indent (); // start from the current 
      // all we do in this is generate a forward declaration of the class
      *ch << "class " << this->local_name () << ";" << nl;

      // generate the _ptr declaration
      *ch << "typedef " << this->local_name () << " *" << this->local_name () <<
        "_ptr;" << nl;
      *ch << "\n#endif // !defined (_" << cg->upcase (this->flatname ()) <<
        "// _DEFN_)\n\n"; 
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

