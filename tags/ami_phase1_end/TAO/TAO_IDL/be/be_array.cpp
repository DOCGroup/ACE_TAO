// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_array.cpp
//
// = DESCRIPTION
//    Extension of class AST_Array that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_array, "$Id$")

/*
 * BE_Array
 */
be_array::be_array (void)
  :  tao_name_ (0)
{
}

be_array::be_array (UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims)
  : AST_Array (n, ndims, dims),
    AST_Decl (AST_Decl::NT_array, n, NULL),
    tao_name_ (0)
{
}

be_array::~be_array (void)
{
  if (this->tao_name_ == 0)
    delete[] tao_name_;
}

// create a name for ourselves
const char*
be_array::tao_name (void)
{
  if (this->tao_name_ != 0)
    return this->tao_name_;

  be_type *bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "tao_name - "
                         "bad base type\n"),
                        0);
    }

  const char prefix[] = "_tc_tao_array_";

  int l = (ACE_OS::strlen (bt->local_name ()->get_string ())
           + ACE_OS::strlen (this->local_name ()->get_string ()) + 1
           + sizeof (prefix)
           + 5 * this->n_dims ());

  ACE_NEW_RETURN (this->tao_name_, char[l], 0);

  ACE_OS::sprintf (this->tao_name_, "%s%s_%s",
                   prefix, this->local_name ()->get_string (),
                   bt->local_name ()->get_string());

  for (unsigned int i = 0; i < this->n_dims (); ++i)
    {
      AST_Expression *expr = this->dims ()[i]; // retrieve the ith

      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "tao_name - "
                             "bad array dimension\n"),
                            0);
        }

      if (expr->ev ()->et != AST_Expression::EV_ulong)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "tao_name - "
                             "bad dimension value\n"),
                            0);
        }

      char buf[16];
      ACE_OS::sprintf (buf, "_%04.4x", ((int)expr->ev ()->u.ulval));
      ACE_OS::strcat (this->tao_name_, buf);
    }
  return this->tao_name_;
}

int
be_array::create_name (void)
{
  char namebuf [NAMEBUFSIZE];
  unsigned long i;
  UTL_ScopedName *n = NULL;
  be_decl *scope; // scope in which we are defined

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
  // retrieve the base type
  // the name always starts this way

  be_type *bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "create_name - "
                         "bad base type\n"),
                        0);
    }

  ACE_OS::sprintf (namebuf, "_tao_array_%s", bt->local_name ()->get_string ());
  // now append dimensions
  for (i = 0; i < this->n_dims (); i++)
    {
      AST_Expression *expr = this->dims ()[i]; // retrieve the ith

      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          ACE_OS::sprintf (namebuf, "%s_%d", namebuf, ((int)expr->ev ()->u.ulval));
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "create_name - "
                             "bad dimension value\n"),
                            -1);
        }
    }

  // now see if we have a fully scoped name and if so, generate one
  scope = be_scope::narrow_from_scope (this->defined_in ())->decl ();
  if (scope)
    {
      // make a copy of the enclosing scope's  name
      n = (UTL_ScopedName *)scope->name ()->copy () ;

      // add our local name as the last component
      n->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                    (namebuf), 1,
                                                    0, I_FALSE),
                                    NULL));
      // set the fully scoped name
      this->set_name (n);
    }
  else
    {
      // We better be not here because we must be inside some scope,
      // atleast the ROOT scope.
      return -1;
    }
  return 0;
}

// Code generation

int
be_array::gen_dimensions (TAO_OutStream *os, unsigned short slice)
{
  unsigned long i;   // loop index

  // print our dimensions
  for (i = (slice?1:0); i < this->n_dims (); i++)
    {
      AST_Expression *expr = this->dims ()[i]; // retrieve the ith

      // dimension value
      if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_dimensions - "
                             "bad array dimension\n"),
                            -1);
        }
      if (expr->ev ()->et == AST_Expression::EV_ulong)
        {
          *os << "[" << ((int)expr->ev ()->u.ulval) << "]";
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_dimensions - "
                             "bad dimension value\n"),
                            -1);
        }
    }
  return 0;
}

int
be_array::gen_typecode (void)
{
  TAO_OutStream *os; // output stream

  os = tao_cg->client_stubs ();
  os->indent (); // start from whatever indentation level we were at

  *os << "CORBA::tk_array, // typecode kind" << be_nl;
  *os << this->tc_encap_len () << ", // encapsulation length" << be_idt << "\n";
  // now emit the encapsulation
  if (this->gen_encapsulation () == -1)
    {
    }
  //  *os << (this->dims () [0]) << "," << be_nl;
  return 0;
}

// generate encapsulation
// An encapsulation for ourselves will be necessary when we are part of some
// other IDL type and a typecode for that other type is being generated. This
// will comprise our typecode kind. IDL types with parameters will additionally
// have the encapsulation length and the entire typecode description

int
be_array::gen_encapsulation (void)
{
  TAO_OutStream *os; // output stream
  be_type *bt; // base type
  unsigned long i;

  os = tao_cg->client_stubs ();
  os->indent (); // start from the current indentation level

  // retrieve the base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::gen_encapsulation - "
                         "bad base type\n"),
                        -1);
    }

  for (i=0; i < (this->n_dims () - 1); i++)
    {
      unsigned long rem_encap_len;

      *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << be_nl;
      *os << "CORBA::tk_array, // typecode kind" << be_nl;
      rem_encap_len
        = (this->n_dims () - (i+1))*(4+4)
        + (this->n_dims () - (i+2))*(4+4)
        + bt->tc_size ();
      *os << rem_encap_len << ", // encapsulation length" << be_idt_nl;
    }
  *os << "TAO_ENCAP_BYTE_ORDER, // byte order\n";
  if (bt->gen_typecode () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::gen_encapsulation - "
                         "base type tyepcode gen failed\n"),
                        -1);
    }
  os->indent ();
  for (i = (this->n_dims ()-1); i > 0; i--)
    {
      *os << this->dims ()[i] << "," << be_uidt_nl;
    }
  *os << this->dims ()[0] << be_uidt << ",\n";
  return 0;
}

// compute typecode size
long
be_array::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation
  return 4 + 4 + this->tc_encap_len ();
}

long
be_array::tc_encap_len (void)
{
  // Suppose "N" is the number of dimensions, then for a N dimensional array,
  // we will have N encapsulations. The innermost encapsulation will hold the
  // typecode of the real base type.
  // Thus, we will have N byte order flags and dimensions, and N-1 tk_array
  // enumerations, encapsulation lengths, and dimensions.

  if (this->encap_len_ == -1) // not computed yet
    {
      be_type *bt; // base type

      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_array::tc_encap_len - bad base type\n"));
          return 0;
        }
      this->encap_len_ =
        this->n_dims () * (4+4) // N byte order flags and dims
        + (this->n_dims ()-1)* (4+4); // N-1 of Enum and encap lengths
      // to this you add the typecode size of the underlying type
      this->encap_len_ += bt->tc_size ();
    }
  return this->encap_len_;
}

// compute the size type of the node in question
int
be_array::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->base_type ());
  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::compute_size_type - "
                         "bad base type\n"), -1);
    }

  // our size type is the same as our type
  this->size_type (type->size_type ());
  return 0;
}

int
be_array::accept (be_visitor *visitor)
{
  return visitor->visit_array (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_array, AST_Array, be_type)
IMPL_NARROW_FROM_DECL (be_array)
