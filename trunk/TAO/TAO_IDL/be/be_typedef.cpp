// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_typedef.cpp
//
// = DESCRIPTION
//    Extension of class AST_Typedef that provides additional means for C++
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

be_typedef::be_typedef (void)
{
}

be_typedef::be_typedef (AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Typedef (bt, n, p),
    AST_Decl (AST_Decl::NT_typedef, n, p)
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

// given a typedef node, traverse the chain of base types until they are no
// more typedefs, and return that most primitive base type
be_type *
be_typedef::primitive_base_type (void)
{
  be_type *d;

  d = this;
  while (d->node_type () == AST_Decl::NT_typedef)
    {
      be_typedef *temp; // temporary

      temp = be_typedef::narrow_from_decl (d);
      d = be_type::narrow_from_decl (temp->base_type ());
    }
  return d;
}

int
be_typedef::gen_client_header (void)
{
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_TYPEDEF_CH);
      cg->node (this); // pass ourselves
      cg->outstream (cg->client_header ());
      s = cg->make_state ();

      bt = be_type::narrow_from_decl (this->base_type ());
      // first generate the mapping for our type
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_typedef: error generating code for base type\n"));
          return -1;
        }

      cg->pop ();
      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
}

int
be_typedef::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;


  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_STRUCT_CS); // set current code gen state

      cs = cg->client_stubs ();
      // pass info
      cg->outstream (cs);
      cg->node (this);

#if 0
      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl; 
      *cs << "{\n";
      cs->incr_indent (0);
      // note that we just need the parameters here and hence we generate the
      // encapsulation for the parameters
      bt = this->primitive_base_type ();
      if (bt->gen_encapsulation () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error generating encapsulation\n\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "};" << nl;

      *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () << 
        " (CORBA::tk_struct, sizeof (_oc_" <<  this->flatname () << 
        "), (unsigned char *) &_oc_" << this->flatname () << 
        ", CORBA::B_FALSE);" << nl;
      *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
        this->flatname () << ";\n\n";
#endif
      this->cli_stub_gen_;
      cg->pop ();
    }

  return 0;
}

int
be_typedef::gen_server_header (void)
{
  return 0;
}

int
be_typedef::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int 
be_typedef::gen_client_inline (void)
{
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_TYPEDEF_CI);
      cg->node (this); // pass ourselves
      cg->outstream (cg->client_inline ());
      s = cg->make_state ();

      bt = be_type::narrow_from_decl (this->base_type ());
      // first generate the mapping for our type
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_typedef: error generating code for base type\n"));
          return -1;
        }

      cg->pop ();
      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the server-side inline
int 
be_typedef::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_typedef::gen_typecode (void)
{
  return 0;
}

long
be_typedef::tc_size (void)
{
  return 0;
}

int 
be_typedef::gen_encapsulation  (void)
{
  return 0;
}

long
be_typedef::tc_encap_len (void)
{
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type)
IMPL_NARROW_FROM_DECL (be_typedef)

