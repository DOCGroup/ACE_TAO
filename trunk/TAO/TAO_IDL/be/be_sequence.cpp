// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_sequence.cpp
//
// = DESCRIPTION
//    Extension of class AST_Sequence that provides additional means for C++
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
 * BE_Sequence
 */
be_sequence::be_sequence (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_sequence::be_sequence (AST_Expression *v, AST_Type *t)
  : AST_Sequence (v, t),
    AST_Decl (AST_Decl::NT_sequence,
              NULL,
              NULL),
    seq_node_ (NULL)
{
  // check if we are bounded or unbounded. An expression value of 0 means
  // unbounded
  if (v->ev ()->u.ulval == 0)
    {
      this->unbounded_ = I_TRUE;
    }
  else
    {
      this->unbounded_ = I_FALSE;
    }

  this->size_type (be_decl::VARIABLE); // a sequence data type is always
                                       // VARIABLE
}

int
be_sequence::create_name (void)
{
  static char namebuf [200];
  UTL_ScopedName *n = NULL;
  be_decl *d;   // may point to a typedef node
  be_decl *scope; // scope in which we are defined
  be_type *t;  // our base type
  AST_Expression *v; // our bounds
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();


  d = cg->node (); // retrieve the node that was passed in via the CodeGen
                   // object

  if (!d)
    return -1; // error, we cannot be free standing.

  if (d->node_type () == AST_Decl::NT_typedef)
    {
      // we are a named sequence. We will assume the same name as the typedef
      // node
      this->set_name ((UTL_ScopedName *)d->name ()->copy ());
    }
  else
    {
      // we generate a name for ourselves. Start by generating a local name

      ACE_OS::memset (namebuf, '\0', 200);
      ACE_OS::sprintf (namebuf, "_seq_%s", d->local_name ()->get_string ());

      if (d->node_type () == AST_Decl::NT_sequence)
        {
          // this means that we are an anonymous sequence who happens to be a
          // base type of the sequence denoted by the node "d".
          // Hence we set our enclosing scope to be the node "d"
          this->set_defined_in (DeclAsScope (d));
        }

      // now set our fully scoped name

      // now see if we have a fully scoped name.
      scope = be_decl::narrow_from_decl (ScopeAsDecl (this->defined_in ()));
      if (scope != NULL)
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
    }
  return 0;
}

void
be_sequence::compute_scoped_name (void)
{
  UTL_ScopedName *n = (UTL_ScopedName *)this->seq_node_->name ()->copy ();
  n->nconc (this->name ());
  this->set_name (n);
}

int
be_sequence::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_decl *d;        // temporary
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      ch = cg->client_header ();

      ch->indent (); // start with the current indentation level

      // first create a name for ourselves. We defer name creation for
      // ourselves to this point since named sequences should get the name
      // of the typedef node, else some other technique of name generation
      // should be used.
      if (this->create_name () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_sequence::gen_client_header - name creation failed\n"),
                            -1);
        }

      *ch << "class " << this->local_name () << nl;
      *ch << "{" << nl;
      *ch << "public:\n";
      ch->incr_indent (0);

      // retrieve the base type since we may need to do some code
      // generation for the base type.
      bt = be_type::narrow_from_decl (this->base_type ());
      if (bt == NULL)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence: base type NULL\n"));
          return -1;
        }

      cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CH); // set current code gen
      // state
      s = cg->make_state ();
      if (!s || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_sequence: base type codegen failed\n"));
          return -1;
        }
      cg->pop ();

      // now generate the sequence body
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
      s = cg->make_state ();

      // generate constructors
      ch->indent ();
      *ch << this->local_name () << " (void); // default constructor" << nl;
      // check whether we are bounded or not. Depending on that the mapping is
      // slightly different as shown below
      if (this->unbounded_)
        {
          *ch << local_name () << " (const CORBA::ULong max);" << nl;
          *ch << local_name () << " (CORBA::ULong max, CORBA::ULong length, " << nl;
        }
      else
        {
          // bounded seq does not take the "max" argument
          *ch << local_name () << " (CORBA::ULong length, " << nl;
        }

      *ch << "\t";
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ch << " value, CORBA::Boolean release=CORBA::B_FALSE);" << nl;
      *ch << local_name () << "(const " << local_name () <<
        " &); // copy constructor" << nl;
      *ch << "~" << this->local_name () << " (void);" << nl;
      *ch << this->local_name () << " &operator= (const " <<
        this->local_name () << " &);" << nl;
      *ch << "CORBA::ULong maximum (void) const;" << nl;
      *ch << "void length (CORBA::ULong);" << nl;
      *ch << "CORBA::ULong length (void) const;" << nl;
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ch << " &operator[] (CORBA::ULong index);" << nl;
      *ch << "const ";
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ch << " &operator[] (CORBA::ULong index) const;" << nl;

      // generate the static allocbuf and freebuf methods
      *ch << "static ";
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ch << " *allocbuf (CORBA::ULong nelems);" << nl;
      *ch << "static void freebuf (";
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ch << " *);\n" ;
      ch->decr_indent ();
      *ch << "private:\n";
      ch->incr_indent ();
      *ch << "CORBA::ULong maximum_;" << nl;
      *ch << "CORBA::ULong length_;" << nl;
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ch << " *buffer_;" << nl;
      *ch << "CORBA::Boolean release_;\n";
      ch->decr_indent ();
      *ch << "};\n";

      // generate the var and out types
      if (this->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_sequence::gen_client_hdr - _var defn failed\n"), -1);
        }
      if (this->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_sequence::gen_client_hdr - _out defn failed\n"), -1);
        }
      cg->pop ();

      // Generate the typecode decl
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
      this->cli_hdr_gen_ = I_TRUE;
    } // if (cli_hdr_gen_)
  return 0;
}

int
be_sequence::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line


  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CS); // set current code gen state

      cs = cg->client_stubs ();
      // pass info
      cg->outstream (cs);
      cg->node (this);
      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl;
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_encapsulation () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence:Error generating encapsulation\n\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "};" << nl;

      *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () <<
        " (CORBA::tk_sequence, sizeof (_oc_" <<  this->flatname () <<
        "), (unsigned char *) &_oc_" << this->flatname () <<
        ", CORBA::B_FALSE);" << nl;
      *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
        this->flatname () << ";\n\n";

      cg->pop ();
      this->cli_stub_gen_ = I_TRUE;

    }
  return 0;
}

int
be_sequence::gen_server_header (void)
{
  return 0;
}

int
be_sequence::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int
be_sequence::gen_client_inline (void)
{
  if (!this->cli_inline_gen_)
    {
      be_type *bt;

      if (this->gen_var_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence: _var impl code gen failed\n"));
          return -1;
        }
      if (this->gen_out_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_sequence: _out impl code gen failed\n"));
          return -1;
        }

      // if our base type is an anonymous sequence, we generate its client
      // inline methods
      bt = be_type::narrow_from_decl (this->base_type ());
      if (bt->node_type () == AST_Decl::NT_sequence)
        {
          if (bt->gen_client_inline () == -1)
            {
              ACE_ERROR ((LM_ERROR,
                "be_sequence: inline code gen for base type seq failed\n"));
              return -1;
            }
        }
      this->cli_inline_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the server-side inline
int
be_sequence::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}
// generate typecode.
// Typecode for sequences comprises the enumerated value followed by the
// encapsulation of the parameters

int
be_sequence::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_sequence, // typecode kind" << nl;
  *cs << this->tc_size () << ", // encapsulation length\n";
  // now emit the encapsulation
  return this->gen_encapsulation ();
}

// generate encapsulation
// An encapsulation for ourselves will be necessary when we are part of some
// other IDL type and a typecode for that other type is being generated. This
// will comprise our typecode kind. IDL types with parameters will additionally
// have the encapsulation length and the entire typecode description

int
be_sequence::gen_encapsulation (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt; // base type

  os = cg->client_stubs ();
  os->indent (); // start from the current indentation level

  // XXXASG - byte order must be based on what m/c we are generating code -
  // TODO
  *os << "0, // byte order" << nl;

  // emit typecode of element type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt || (bt->gen_typecode () == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_sequence::gen_typecode - bad base type\n"));
      return -1;
    }

  //  emit the length
  os->indent ();
  *os << this->max_size () << ",\n";
  return 0;
}

// compute typecode size
long
be_sequence::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation length
  return 4 + 4 + this->tc_encap_len ();
}

long
be_sequence::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      be_type *bt; // base type

      this->encap_len_ = 4;  // holds the byte order flag
      // add the encapsulation length of our base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_sequence::tc_encap_len - bad base type\n"));
          return 0;
        }
      this->encap_len_ += bt->tc_encap_len ();
      this->encap_len_ += 4; // to hold the max size

    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)
