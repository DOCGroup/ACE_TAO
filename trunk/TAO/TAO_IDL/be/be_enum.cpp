// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_enum.cpp
//
// = DESCRIPTION
//    Extension of class AST_Enum that provides additional means for C++
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
 * BE_Enum
 */
be_enum::be_enum (void)
{
}

be_enum::be_enum (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Enum (n, p),
    AST_Decl (AST_Decl::NT_enum, n, p),
    UTL_Scope (AST_Decl::NT_enum),
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
be_enum::compute_member_count (void)
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
be_enum::member_count (void)
{
  if (this->member_count_ == -1)
    this->compute_member_count ();

  return this->member_count_;
}


// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// Generates the client-side header information for the interface 
int 
be_enum::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line

  if (!this->cli_hdr_gen_)
    {
      UTL_ScopeActiveIterator *i = new UTL_ScopeActiveIterator(this, IK_decls);
      // scope iterator
      AST_Decl		    *d; // AST node

      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_ENUM_CH);

      ch = cg->client_header ();

      ch->indent (); // start from whatever indentation level we were at
      *ch << "enum " << this->local_name () << nl;
      *ch << "{\n";
      ch->incr_indent ();
      while (!(i->is_done ())) 
        {
          d = i->item ();
          *ch << d->local_name ();
          i->next ();
          if (!(i->is_done()))
            *ch << ", " << nl;
        }
      delete i;
      *ch << "\n";
      ch->decr_indent ();
      *ch << "};" << nl;
      // As per the ORBOS spec, we need the following typedef 
      *ch << "typedef " << this->local_name () << " &" << this->local_name () << 
        "_out;\n";
      cg->pop ();

      // Generate the typecode decl
      // All names in the root scope have length 2 (for the root and
      // ourself). The children have length greater than 2. Thus, if our name
      // length is 2 or less, we are outermost and our typecode decl must be
      // extern, else we are defined static inside the enclosing scope.
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
      this->cli_hdr_gen_ =  I_TRUE;
    }
  return 0;
}

// Generates the client-side stubs for the interface
int 
be_enum::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line

  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_ENUM_CS); // set current code gen state

      cs = cg->client_stubs ();
      cg->outstream (cs);

      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl; 
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_encapsulation () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_enum:Error generating encapsulation\n\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "};" << nl;

      *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () << 
        " (CORBA::tk_enum, sizeof (_oc_" <<  this->flatname () << 
        "), (unsigned char *) &_oc_" << this->flatname () << 
        ", CORBA::B_FALSE);" << nl;
      *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
        this->flatname () << ";\n\n";
      this->cli_stub_gen_;
      cg->pop ();
    }
  return 0;
}

// Generates the server-side header information for the interface 
int 
be_enum::gen_server_header (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side skeletons for the interface
int 
be_enum::gen_server_skeletons (void)
{
  // nothing to be done
  return 0;
}

// Generates the client-side inline information
int 
be_enum::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int 
be_enum::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generate typecode.
// Typecode for enum comprises the enumerated value followed by the
// encapsulation of the parameters

int
be_enum::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_enum, // typecode kind" << nl;
  *cs << this->tc_size () << ", // encapsulation length\n";
  // now emit the encapsulation
  return this->gen_encapsulation ();
}

int
be_enum::gen_encapsulation (void)
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
  if (be_scope::gen_encapsulation () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_structure: cannot generate code for members\n"));
      return -1;
    }
  cs->decr_indent (0);
  return 0;
}

// compute typecode size
long
be_enum::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation length
  return 4 + 4 + this->tc_encap_len ();
}

// return encapsulation length
long
be_enum::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      long slen;

      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += this->repoID_encap_len (); // repoID storage

      // do the same thing for the local name
      this->encap_len_ += this->name_encap_len ();

      this->encap_len_ += 4; // to hold the member count

      // compute encap length for members
      this->encap_len_ += be_scope::tc_encap_len ();
    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_enum, AST_Enum, be_scope, be_type)
  IMPL_NARROW_FROM_DECL (be_enum)
  IMPL_NARROW_FROM_SCOPE (be_enum)

  
