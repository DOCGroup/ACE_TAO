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
be_typedef::gen_client_header (void)
{
  be_type *bt;       // type node
  be_state *s;       // state based code gen object
  TAO_OutStream *ch; // client header

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      ch = cg->client_header ();
      cg->push (TAO_CodeGen::TAO_TYPEDEF_CH);
      s = cg->make_state ();

      bt = be_type::narrow_from_decl (this->base_type ());
      // first generate the mapping for our type. As a side effect, also
      // generate the mapping for the typedef
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_typedef: error generating code for base type\n"));
          return -1;
        }

      // generate the typecode decl for this typedef node
      if (this->is_nested ())
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
  be_state *s;       // state based code gen object

  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_TYPEDEF_CS); // set current code gen state

      cs = cg->client_stubs ();

      s = cg->make_state ();

      bt = be_type::narrow_from_decl (this->base_type ());
      // first generate the mapping for our type. As a side effect, also
      // generate the mapping for the typedef
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_typedef: error generating code for base type\n"));
          return -1;
        }

      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl;
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_encapsulation () == -1)
        {
          ACE_ERROR ((LM_ERROR, "Error generating typecode\n\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "};" << nl;

      *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () <<
        " (CORBA::tk_alias, sizeof (_oc_" <<  this->flatname () <<
        "), (unsigned char *) &_oc_" << this->flatname () <<
        ", CORBA::B_FALSE);" << nl;
      *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
        this->flatname () << ";\n\n";


      this->cli_stub_gen_ = I_TRUE;
      cg->pop ();
    }

  return 0;
}

// Generates the client-side inline information
int
be_typedef::gen_client_inline (void)
{
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  if (!this->cli_inline_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_TYPEDEF_CI);
      s = cg->make_state ();

      bt = be_type::narrow_from_decl (this->base_type ());
      // first generate the mapping for our type
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_typedef: error generating code for base type\n"));
          return -1;
        }

      cg->pop ();
      this->cli_inline_gen_ = I_TRUE;
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
  long *arr;  // an array holding string names converted to array of longs
  be_type *bt; // base type

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "TAO_ENCAP_BYTE_ORDER, // byte order" << nl;
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

int be_typedef::write_as_return (TAO_OutStream *stream,
				 be_type *type)
{
  be_type *base = be_type::narrow_from_decl (this->base_type ());
  return base->write_as_return (stream, type);
}

int
be_typedef::accept (be_visitor *visitor)
{
  return visitor->visit_typedef (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type)
IMPL_NARROW_FROM_DECL (be_typedef)
