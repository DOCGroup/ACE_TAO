// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_structure.cpp
//
// = DESCRIPTION
//    Extension of class AST_Structure that provides additional means for C++
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

/*
 * BE_Structure
 */
be_structure::be_structure (void)
{
}

be_structure::be_structure (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (AST_Decl::NT_struct, n, p),
    UTL_Scope (AST_Decl::NT_struct),
    member_count_ (-1)
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

// compute total number of members
int
be_structure::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si;  // iterator
  AST_Decl *d;  // temp node

  this->member_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ())) 
	{
	  // get the next AST decl node
	  d = si->item ();
	  if (!d->imported ()) 
	    {
              this->member_count_++;
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// return the member count
int
be_structure::member_count (void)
{
  if (this->member_count_ == -1)
    this->compute_member_count ();

  return this->member_count_;
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// Generates the client-side header information for the structure 
int 
be_structure::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line


  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_STRUCT); // set current code gen state

      ch = cg->client_header ();

      ch->indent (); // start from whatever indentation level we were at
      *ch << "struct " << local_name () << nl;
      *ch << "{\n";
      ch->incr_indent (0);

      // generate code for field members
      if (be_scope::gen_client_header () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_structure::gen_client_header\n")); 
          ACE_ERROR ((LM_ERROR, "code generation for fields failed\n"));
          return -1;
        }

      ch->decr_indent ();
      *ch << "};\n\n";

      // generate var defn
      this->gen_var_defn ();

      // a class is generated for an out defn only for a variable length struct
      if (this->size_type () == be_decl::VARIABLE)
        {
          this->gen_out_defn ();
        }
      else
        {
          ch->indent ();
          *ch << "typedef " << this->local_name () << " &" << this->local_name
            () << "_out;\n\n";
        }

      // generate the typecode decl
      if (this->name ()->length () > 2)
        {
          // we have a scoped name
          ch->indent ();
          *ch << "static CORBA::TypeCode_ptr " << this->tc_name
            ()->last_component () << ";\n\n";
        }
      else
        {
          // we are in the ROOT scope
          ch->indent ();
          *ch << "extern CORBA::TypeCode_ptr " << this->tc_name
            ()->last_component () << ";\n\n";
        }
      this->cli_hdr_gen_ = I_TRUE;
      cg->pop ();
    }
  return 0;
}

// Generates the client-side stubs for the structure
int 
be_structure::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line


  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_STRUCT); // set current code gen state

      cs = cg->client_stubs ();
      cg->outstream (cs);

      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl; 
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_typecode () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error generating typecode\n\n"));
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
      this->cli_stub_gen_;
    }

  return 0;
}

// Generates the server-side header information for the structure 
int 
be_structure::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side skeletons for the structure
int 
be_structure::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side inline information
int 
be_structure::gen_client_inline (void)
{
  if (!this->cli_inline_gen_)
    {
      if (this->gen_var_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_structure: _var impl code gen failed\n"));
          return -1;
        }
      if (this->size_type () == be_decl::VARIABLE && this->gen_out_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_structure: _out impl code gen failed\n"));
          return -1;
        }
      if (be_scope::gen_client_inline () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_structure: code gen failed for scope\n"));
          return -1;
        }
      this->cli_inline_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the server-side inline
int 
be_structure::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_structure::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  long i, arrlen;
  long *arr;  // an array holding string names converted to array of longs

  cs = cg->outstream ();
  cs->indent (); // start from whatever indentation level we were at

  // XXXASG - byte order must be based on what m/c we are generating code -
  // TODO 
  *cs << "0, // byte order" << nl; 
  // generate repoID
  *cs << (ACE_OS::strlen (this->repoID ())+1) << ", ";
  (void)this->tc_name2long (this->repoID (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("0x%x, ", arr[i]);
    }
  *cs << " // repository ID = " << this->repoID () << nl;
  // generate name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("0x%x, ", arr[i]);
    }
  *cs << " // name = " << this->local_name () << nl;
  // generate the member count
  *cs << this->member_count () << ", // member count\n";
  cs->incr_indent (0);
  // hand over to the scope to generate the typecode for elements
  if (be_scope::gen_typecode () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_structure: cannot generate code for members\n"));
      return -1;
    }
  cs->decr_indent (0);
  return 0;
}

// compute encapsulation length
long
be_structure::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      long slen;

      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += 4; // store the size of repository ID
      // compute bytes reqd to store repoID
      slen = ACE_OS::strlen (this->repoID ()) + 1; // + 1 for NULL terminating char
      this->encap_len_ += 4 * (slen/4 + (slen%4 ? 1:0)); // storage for the repoID

      // do the same thing for the local name
      this->encap_len_ += 4; // store the size of name
      slen = ACE_OS::strlen (this->local_name ()->get_string ()) + 1; 
      // + 1 for  NULL 
      this->encap_len_ += 4 * (slen/4 + (slen%4 ? 1:0)); // storage for the name

      this->encap_len_ += 4; // to hold the member count

      // compute encap length for members
      this->encap_len_ += be_scope::tc_encap_len ();
    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_structure, AST_Structure, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_structure)
IMPL_NARROW_FROM_SCOPE (be_structure)

