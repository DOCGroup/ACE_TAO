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
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();


  d = cg->node (); // retrieve the node that was passed in via the CodeGen
  // object

  if (!d)
    return -1; // error, we cannot be free standing.

  // we generate a name for ourselves. Start by generating a local name

  ACE_OS::memset (namebuf, '\0', 200);
  ACE_OS::sprintf (namebuf, "_tao__seq_%s", d->local_name ()->get_string ());

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
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

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

      ch = cg->client_header ();

      // generate the ifdefined macro for the sequence type
      ch->gen_ifdef_macro (this->flatname ());

      *ch << "// *************************************************************"
          << nl;
      *ch << "// class " << this->local_name () << nl;
      *ch << "// *************************************************************\n\n";

      ch->indent (); // start with the current indentation level

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
          *ch << local_name () << " (CORBA::ULong max);" << nl;
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
      *ch << " *value, CORBA::Boolean release=CORBA::B_FALSE);" << nl;
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
      ch->indent ();
      *ch << "typedef " << this->local_name () << "* "
	  << this->local_name () << "_ptr;\n";

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

      ch->gen_endif (); // endif macro

      // generate the ifdefined macro for the var type
      ch->gen_ifdef_macro (this->flatname (), "_var");

      // generate the var and out types
      if (this->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_sequence::gen_client_hdr - _var defn failed\n"), -1);
        }
      ch->gen_endif ();

      // generate the ifdefined macro for the var type
      ch->gen_ifdef_macro (this->flatname (), "_out");

      if (this->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_sequence::gen_client_hdr - _out defn failed\n"), -1);
        }
      ch->gen_endif ();

      cg->pop ();

      this->cli_hdr_gen_ = I_TRUE;
    } // if (cli_hdr_gen_)
  return 0;
}

int
be_sequence::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_type *bt; // base type
  be_state *s; //state object

  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      cs = cg->client_stubs (); // retrieve the client stubs stream

      // retrieve base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        return -1;

      cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CS);
      s = cg->make_state ();

      // generate stubs for our base type if it itself is a sequence
      if (!s || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                "be_sequence::gen_client_stubs - base type code gen\n"), -1);
        }
      cg->pop ();

      // generate the methods of the sequence C++ mapping
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CS);
      s = cg->make_state ();

      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                "be_sequence::gen_client_stubs - invalid state\n"), -1);
        }

      *cs << "// *************************************************************"
          << nl;
      *cs << "// class " << this->name () << nl;
      *cs << "// *************************************************************\n\n";

      // copy constructor
      cs->indent ();
      *cs << "// copy constructor" << nl;
      *cs << this->name () << "::" << this->local_name () <<
        " (const " << this->name () << " &seq)" << nl;
      *cs << "\t: maximum_ (seq.maximum_)," << nl;
      *cs << "\t  length_ (seq.length_)," << nl;
      *cs << "\t  buffer_ (" << this->name () << "::allocbuf (seq.maximum_)),"
          << nl;
      *cs << "\t  release_ (1) // we always own it" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // copy each element
      *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
      *cs << "\tthis->buffer_[i] = seq.buffer_[i];\n";
      cs->decr_indent ();
      *cs << "}\n\n";

      // destructor
      cs->indent ();
      *cs << "// destructor" << nl;
      *cs << this->name () << "::~" << this->local_name () << " (void)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      *cs << "if (this->release_) // we own the buffer" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // only for obj references and strings, we need to free each individual
      // element
      switch (bt->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
        case AST_Decl::NT_string:
          {
            // XXXASG - TODO (tricky)
          }
          break;
        default:
          break;
        }
      // call freebuf
      *cs << this->name () << "::freebuf (this->buffer_);\n";
      cs->decr_indent ();
      *cs << "}\n";
      cs->decr_indent ();
      *cs << "}\n\n";

      // assignment operator
      cs->indent ();
      *cs << "// assignment operator" << nl;
      *cs << this->name () << "& " << nl;
      *cs << this->name () << "::operator=" <<
        " (const " << this->name () << " &seq)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // check for equality
      *cs << "if (this == &seq) return *this;" << nl;
      // otherwise, if release flag, free the buffer
      *cs << "if (this->release_)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      *cs << this->name () << "::freebuf (this->buffer_);\n";
      cs->decr_indent ();
      *cs << "}" << nl;

      *cs << "this->length_ = seq.length_;" << nl;
      *cs << "this->maximum_ = seq.maximum_;" << nl;
      *cs << "this->buffer_ = " << this->name () << "::allocbuf (seq.maximum_),"
          << nl;
      *cs << "this->release_ =1; // we always own it" << nl;
      // copy each element
      *cs << "for (CORBA::ULong i=0; i < seq.length_; i++)" << nl;
      *cs << "\tthis->buffer_[i] = seq.buffer_[i];" << nl;
      *cs << "return *this;\n";
      cs->decr_indent ();
      *cs << "}\n\n";

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

// Generates the client-side inline information
int
be_sequence::gen_client_inline (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  be_state *s;       // code gen state
  be_type *bt;  // base type

  if (!this->cli_inline_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      ci = cg->client_inline ();

      // retrieve base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        return -1;

      cg->push (TAO_CodeGen::TAO_SEQUENCE_BASE_CI);
      s = cg->make_state ();

      // generate inline methods for our base type if it itself is a sequence
      if (!s || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                "be_sequence::gen_client_inline - base type code gen\n"), -1);
        }
      cg->pop ();

      // generate the methods of the sequence C++ mapping
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CI);
      s = cg->make_state ();

      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                "be_sequence::gen_client_inline - invalid state\n"), -1);
        }

      // the allocbuf method
      ci->indent ();
      *ci << "ACE_INLINE ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             "be_sequence - base type code gen failure\n"), -1);
        }
      *ci << " *" << nl;
      *ci << this->name () << "::allocbuf (CORBA::ULong nelems)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return new ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             "be_sequence - base type code gen failure\n"), -1);
        }
      *ci << "[nelems]; // allocate from heap\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // freebuf method
      ci->indent ();
      *ci << "ACE_INLINE void" << nl;
      *ci << this->name () << "::freebuf (";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             "be_sequence - base type code gen failure\n"), -1);
        }
      *ci << " *seq)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "delete [] seq;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // default constructor
      ci->indent ();
      *ci << "//default constructor" << nl;
      *ci << "ACE_INLINE " << nl;
      *ci << this->name () << "::" << this->local_name () << " (void)" << nl;
      // for bounded and unbounded, initialize the data members differently
      if (this->unbounded_)
        {
          *ci << "\t: maximum_ (0)," << nl;
          *ci << "\t  length_ (0)," << nl;
          *ci << "\t  buffer_ (0)," << nl;
          *ci << "\t  release_ (0) // does not own" << nl;
        }
      else
        {
          *ci << "\t: maximum_ (" << this->max_size () << ")," << nl;
          *ci << "\t  length_ (0)," << nl;
          *ci << "\t  buffer_ (" << this->name () << "::allocbuf (" <<
            this->max_size () << "))," << nl;
          *ci << "\t  release_ (1) // owns" << nl;
        }
      *ci << "{}\n\n";

      // constructor only for unbounded seq. This takes in "max length"
      if (this->unbounded_)
        {
          ci->indent ();
          *ci << "// constructor for unbounded seq" << nl;
          *ci << "ACE_INLINE " << nl;
          *ci << this->name () << "::" << this->local_name () <<
            "(CORBA::ULong max )" << nl;
          *ci << "\t: maximum_ (max)," << nl;
          *ci << "\t  length_ (0)," << nl;
          *ci << "\t  buffer_ (" << this->name () << "::allocbuf (max))," << nl;
          *ci << "\t  release_ (1) // owns" << nl;
          *ci << "{}\n\n";
        }

      // constructor that takes in the data buffer
      // XXXASG - may not work for seq of strings or obj refs
      ci->indent ();
      *ci << "// constructor from data buffer" << nl;
      *ci << "ACE_INLINE " << nl;
      *ci << this->name () << "::" << this->local_name ();
      // depending on whether we are bounded ot not, the constructor has
      // different sets of parameters
      if (this->unbounded_)
        {
          *ci << " (CORBA::ULong max, CORBA::ULong length, " << nl;
        }
      else
        {
          // bounded seq does not take the "max" argument
          *ci << " (CORBA::ULong length, " << nl;
        }
      *ci << "\t";
      if (s->gen_code (bt, this) == -1)
        return -1;
      *ci << " *value, CORBA::Boolean release)" << nl;
      // for unbounded we have the additional max parameter
      if (this->unbounded_)
        {
          *ci << "\t: maximum_ (max)," << nl;
        }
      else
        {
          *ci << "\t: maximum_ (" << this->max_size () << ")," << nl;
        }
      *ci << "\t  length_ (length)," << nl;
      *ci << "\t  buffer_ (value)," << nl;
      *ci << "\t  release_ (release) // ownership depends on release" << nl;
      *ci << "{}\n\n";

      // the maximum method
      ci->indent ();
      *ci << "ACE_INLINE CORBA::ULong" << nl;
      *ci << this->name () << "::maximum (void) const" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->maximum_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // the length method
      ci->indent ();
      *ci << "ACE_INLINE CORBA::ULong" << nl;
      *ci << this->name () << "::length  (void) const" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->length_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // set the length
      ci->indent ();
      *ci << "ACE_INLINE void" << nl;
      *ci << this->name () << "::length (CORBA::ULong length)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "this->length_ = length;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // subscript operators (1) read-only, (2) read/write
      ci->indent ();
      *ci << "ACE_INLINE ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             "be_sequence - base type code gen failure\n"), -1);
        }
      *ci << " &" << nl;
      *ci << this->name () << "::operator[] (CORBA::ULong index) // read/write"
         << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->buffer_[index];\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE const ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
             "be_sequence - base type code gen failure\n"), -1);
        }
      *ci << " &" << nl;
      *ci << this->name () << "::operator[] (CORBA::ULong index) const // read"
         << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->buffer_[index];\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // generate the implementations for the _var and _impl classes
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

      this->cli_inline_gen_ = I_TRUE;
      cg->pop ();
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

// Generates the server-side inline
int
be_sequence::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generate the _var definition for ourself
int
be_sequence::gen_var_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // names
  be_state *s;       // code gen state
  be_type *bt;  // base type


  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

  *ch << "// *************************************************************"
      << nl;
  *ch << "// class " << this->name () << "_var" << nl;
  *ch << "// *************************************************************\n\n";

  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_var_defn - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ch->indent (); // start with whatever was our current indent level
  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();
  // default constr
  *ch << namebuf << " (void); // default constructor" << nl;
  // constr
  *ch << namebuf << " (" << this->local_name () << " *);" << nl;
  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << this->local_name () << " *);" << nl;
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << nl;

  // arrow operator
  *ch << this->local_name () << " *operator-> (void);" << nl;
  *ch << "const " << this->local_name () << " *operator-> (void) const;" << nl;
  *ch << nl;

  // other extra types (cast operators, [] operator, and others)

  // cast operator
  *ch << "operator const " << this->local_name () << " &() const;" << nl;
  *ch << "operator " << this->local_name () << " &();" << nl;
  *ch << "operator " << this->local_name () << " &() const;" << nl;

  // overloaded [] operator. The const version is not required for sequences

  // gen code for base return type
  if (s->gen_code (bt, this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_var_impl - base type codegen failed\n"), -1);
    }
  *ch << " &operator[] (CORBA::ULong index);" << nl;

  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  *ch << "const " << this->local_name () << " &in (void) const;" << nl;
  *ch << this->local_name () << " &inout (void);" << nl;
  *ch << this->local_name () << " *&out (void);" << nl;
  *ch << this->local_name () << " *_retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << this->local_name () << " *ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << this->local_name () << " *ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  cg->pop ();

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_sequence::gen_var_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names
  be_state *s;       // code gen state
  be_type *bt;  // base type


  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CI);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_var_impl - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the var implementation in the inline file
  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // default constr
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname <<
    " (void) // default constructor" << nl;
  *ci << "\t" << ": ptr_ (0)" << nl;
  *ci << "{}\n\n";

  // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (p.ptr_)" << nl;
  *ci << "\tthis->ptr_ = new " << this->name () << "(*p.ptr_);" << nl;
  *ci << "else" << nl;
  *ci << "\tthis->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from a pointer
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << name () <<
    " *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from _var
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << fname <<
    " &p) // deep copy" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (this != &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = new " << this->name () << " (*p.ptr_);\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // two arrow operators
  ci->indent ();
  *ci << "ACE_INLINE const " << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - 3 cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << name () <<
    " &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << " &() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << " &() const// cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator []
  ci->indent ();
  *ci << "ACE_INLINE ";
  // gen code for base return type
  if (s->gen_code (bt, this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_var_impl - base type codegen failed\n"), -1);
    }
  *ci << "&" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_->operator[] (index);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE const " << name () << " &" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << " &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "// mapping for variable size " << nl;
  *ci << "ACE_INLINE " << name () << " *&" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;" << nl;
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << " *" << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << this->name () << " *tmp = this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;" << nl;
  *ci << "return tmp;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // the additional ptr () member function
  ci->indent ();
  *ci << "ACE_INLINE " << name () << " *" << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  cg->pop ();

  return 0;
}

// generate the _out definition
int
be_sequence::gen_out_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // to hold the _out name
  be_state *s;       // code gen state
  be_type *bt;  // base type

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_out", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();
  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CH);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_out_defn - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the out definition (always in the client header)
  ch->indent (); // start with whatever was our current indent level

  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // constructor from a pointer
  *ch << namebuf << " (" << this->local_name () << " *&);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << this->local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // assignment
  *ch << namebuf << " &operator= (" << this->local_name () << " *);" << nl;
  // operator ()
  *ch << "operator " << this->local_name () << " *&();" << nl;
  // ptr fn
  *ch << this->local_name () << " *&ptr (void);" << nl;
  // operator ->
  *ch << this->local_name () << " *operator-> (void);" << nl;

  // overloaded [] operator only for sequence. The const version is not
  // required

  // gen code for base return type
  if (s->gen_code (bt, this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_var_impl - base type codegen failed\n"), -1);
    }
  *ch << " &operator[] (CORBA::ULong index);" << nl;
  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();

  *ch << this->local_name () << " *&ptr_;" << nl;
  *ch << "// assignment from T_var not allowed" << nl;
  *ch << "void operator= (const " << this->local_name () << "_var &);\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  cg->pop ();
  return 0;
}

int
be_sequence::gen_out_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names
  be_state *s;       // code gen state
  be_type *bt;  // base type


  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_out", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_out", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CI);
  s = cg->make_state ();

  if (!s)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_out_impl - invalid state obj\n"), -1);
    }

  // retrieve base type
  bt = be_type::narrow_from_decl (this->base_type ());

  // generate the out implementation in the inline file

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // constr from a pointer
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << " *&p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // constructor from _var &
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->name () <<
    "_var &p) // constructor from _var" << nl;
  *ci << "\t: ptr_ (p.out ())" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << fname <<
    " &p) // copy constructor" << nl;
  *ci << "\t: ptr_ (p.ptr_)" << nl;
  *ci << "{}\n\n";

  // assignment operator from _out &
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p.ptr_;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment from _var is not allowed by a private declaration

  // assignment operator from pointer
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    " *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << this->name () <<
    " *&() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << " *&" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator ->
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // sequence has an additional method
  ci->indent ();
  *ci << "ACE_INLINE ";
  // gen code for base return type
  if (s->gen_code (bt, this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
          "be_sequence::gen_out_impl - base type codegen failed\n"), -1);
    }
  *ci << "& " << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_->operator[] (index);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  cg->pop ();
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

  *os << "TAO_ENCAP_BYTE_ORDER, // byte order" << nl;

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
