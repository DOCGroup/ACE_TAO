// $Id$

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

ACE_RCSID(be, be_typedef, "$Id$")


be_typedef::be_typedef (void)
{
}

be_typedef::be_typedef (AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Typedef (bt, n, p),
    AST_Decl (AST_Decl::NT_typedef, n, p)
{
}

// given a typedef node, traverse the chain of base types until they are no
// more typedefs, and return that most primitive base type
be_type *
be_typedef::primitive_base_type (void)
{
  be_type *d;

  d = this;
  while (d && d->node_type () == AST_Decl::NT_typedef)
    {
      be_typedef *temp; // temporary

      temp = be_typedef::narrow_from_decl (d);
      d = be_type::narrow_from_decl (temp->base_type ());
    }
  return d;
}

int
be_typedef::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_alias, // typecode kind for typedefs" << nl;
  *cs << this->tc_encap_len () << ", // encapsulation length\n";
  // now emit the encapsulation
  cs->incr_indent (0);
  if (this->gen_encapsulation () == -1)
    {
      return -1;
    }

  cs->decr_indent (0);
  return 0;
}

// generate encapsulation. A typedef is an alias to its base type
int
be_typedef::gen_encapsulation  (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  long i, arrlen;
  ACE_UINT32 *arr;
  be_type *bt; // base type

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "TAO_ENCAP_BYTE_ORDER, // byte order" << nl;
  // generate repoID
  *cs << (ACE_OS::strlen (this->repoID ())+1) << ", ";
  (void)this->tc_name2long (this->repoID (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("ACE_NTOHL (0x%x), ", arr[i]);
    }
  *cs << " // repository ID = " << this->repoID () << nl;

  // generate name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("ACE_NTOHL (0x%x), ", arr[i]);
    }
  *cs << " // name = " << this->local_name () << "\n";

  // generate typecode for the base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt || (bt->gen_typecode () == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
       "be_typedef::gen_encapsulation failed for base type\n"),
                        -1);
    }
  return 0;
}

long
be_typedef::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation length
  return 4 + 4 + this->tc_encap_len ();
}

long
be_typedef::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      be_type *bt; // base type
      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += this->repoID_encap_len (); // repoID

      // do the same thing for the local name
      this->encap_len_ += this->name_encap_len ();

      // add the encapsulation length of our base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR ((LM_ERROR,
              "be_typedef::tc_encap_len - bad base type\n"));
          return 0;
        }
      this->encap_len_ += bt->tc_size ();

    }
  return this->encap_len_;
}

// compute the size type of the node in question
int
be_typedef::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->base_type ());
  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_typedef::compute_size_type - "
                         "bad base type\n"), -1);
    }

  // our size type is the same as our type
  this->size_type (type->size_type ());
  return 0;
}

AST_Decl::NodeType be_typedef::base_node_type (void) const
{
  be_type *base = be_type::narrow_from_decl (ACE_const_cast(be_typedef*, this)->base_type ());
  return base->base_node_type ();
}

int
be_typedef::accept (be_visitor *visitor)
{
  return visitor->visit_typedef (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type)
IMPL_NARROW_FROM_DECL (be_typedef)
