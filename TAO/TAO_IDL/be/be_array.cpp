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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Array
 */
be_array::be_array (void)
{
}

be_array::be_array (UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims)
  : AST_Array (n, ndims, dims),
    AST_Decl (AST_Decl::NT_array, n, NULL)
{
#if 0
  // if we are inside of a union, we change our local name to have an
  // underscore before us
  AST_Decl *d = ScopeAsDecl (this->defined_in ());
  if (d && (d->node_type () == AST_Decl::NT_union))
    {
      static char namebuf [200];
      UTL_ScopedName *myname;

      ACE_OS::memset (namebuf, '\0', 200);

      // make a copy of our parent's name
      myname = (UTL_ScopedName *)d->name ()->copy ();
      ACE_OS::sprintf (namebuf, "_%s", this->local_name ()->get_string ());
      myname->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                         (namebuf), 1, 0,
                                                         I_FALSE), NULL));
      this->set_name (myname);
    }
#endif
}

// create a name for ourselves
int
be_array::create_name (void)
{
  char namebuf [NAMEBUFSIZE];
  be_type *bt; // base type;
  unsigned long i;
  UTL_ScopedName *n = NULL;
  be_decl *scope; // scope in which we are defined

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
  // retrieve the base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::"
                         "create_name - "
                         "bad base type\n"),
                        0);
    }
  // the name always starts this way
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
be_array::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // our base type
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_ARRAY_DEFN_CH); // set current code gen state

      ch = cg->client_header (); // retrieve client hdr stream

#if 0
      if (this->create_name () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "name creation failed\n"),
                            0);
        }
#endif
      s = cg->make_state (); // get the state-based code generation object
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "Bad state\n"),
                            -1);
        }

      // retrieve the base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "Bad type\n"),
                            -1);
        }

      // generate the ifdefined macro for the array type
      ch->gen_ifdef_macro (this->flatname ());

      ch->indent (); // start from current indentation
      // generate code for our base type if required and then print the type
      // name
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }

      // print our name
      *ch << " " <<  this->local_name ();
      // print our dimensions
      if (this->gen_dimensions (ch) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "dimension codegen failed\n"),
                            -1);
        }

      *ch << ";" << nl;

      // if our base type is VARIABLE length, then we are variable length too
      this->size_type (bt->size_type ());
      cg->pop ();

      cg->push (TAO_CodeGen::TAO_ARRAY_OTHER_CH); // rest of the array defn
      s = cg->make_state ();
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "bad state\n"),
                            -1);
        }

      // now generate the slice definition
      *ch << "typedef ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "state based codegen failed\n"),
                            -1);
        }

      *ch << " " <<  this->local_name () << "_slice";
      // print dimensions except first one
      if (this->gen_dimensions (ch, 1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "slice dimensions codegen failed\n"),
                            -1);
        }
      *ch << ";" << nl;

      // memory management functions

      // The T_alloc method
      // first find if we are inside a scope
      if (this->is_nested ()) // we were defined inside a scope. Hence the
                              // memory management functions become static
                              // members of the enclosing class
        *ch << "static ";

      // the return type is a pointer to slice
      *ch <<  this->local_name () << "_slice *" << this->local_name () <<
        "_alloc (void);" << nl;
      // the T_dup method
      if (this->is_nested ())
        *ch << "static ";
      *ch <<  this->local_name () << "_slice *" << this->local_name () <<
        "_dup (const " << this->local_name () << "_slice *);" << nl;
      // the T_free method
      if (this->is_nested ())
        *ch << "static ";
      *ch << "void " << this->local_name () << "_free (" << this->local_name ()
          << "_slice *);\n\n";

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
      ch->gen_endif ();

      // generate the ifdefined macro
      ch->gen_ifdef_macro (this->flatname (), "_var");
      // generate the _var, _out, and _forany definition
      if (this->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "error generating _var class\n"),
                            -1);
        }
      ch->gen_endif ();

      // generate the ifdefined macro
      ch->gen_ifdef_macro (this->flatname (), "_out");
      if (this->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "error generating _out class\n"),
                            -1);
        }
      ch->gen_endif ();

      // generate the ifdefined macro
      ch->gen_ifdef_macro (this->flatname (), "_forany");
      if (this->gen_forany_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_header - "
                             "error generating _forany class\n"),
                            -1);
        }
      ch->gen_endif ();

      this->cli_hdr_gen_ = I_TRUE;
      cg->pop ();
    }
  return 0;
}

int
be_array::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  unsigned long i;

  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_SEQUENCE_BODY_CS); // set current code gen state

      cs = cg->client_stubs ();

      // generate the typecode information here
      cs->indent (); // start from current indentation level
      *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
        nl;
      *cs << "{\n";
      cs->incr_indent (0);
      if (this->gen_encapsulation () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_array:Error generating encapsulation\n\n"));
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

      // T_dup method
      *cs << this->name () << "_slice *" << nl;
      *cs << this->name () << "_dup (" << this->name () << "_slice * s)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      *cs << this->name () << "_slice *temp;" << nl;
      *cs << "// alloc an array" << nl;
      *cs << "temp = " << this->name () << "_alloc ();" << nl;
      *cs << "// copy each individual elements" << nl;
      // generate nested loops for as many dimensions as there are
      for (i = 0; i < this->n_dims (); i++)
        {
          AST_Expression *expr = this->dims ()[i]; // retrieve the ith
                                                   // dimension value
          if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_array::"
                                 "gen_client_stubs - "
                                 "bad array dimension\n"),
                                -1);
            }
          if (expr->ev ()->et == AST_Expression::EV_ulong)
            {
              // generate a loop for each dimension
              *cs << "for (CORBA::ULong i" << i << " = 0; i" << i << " < " <<
                expr->ev ()->u.ulval << "; i" << i << "++)\n";
              cs->incr_indent ();
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_array::"
                                 "gen_client_stubs - "
                                 "bad array dimension value\n"),
                                -1);
            }
        }

      // now generate code such that every element of the array gets assigned
      // inside the innermost level of the  nested loops generated above
      *cs << "temp"; // generate the lvalue
      for (i = 0; i < this->n_dims (); i++)
        {
          *cs << "[i" << i << "]";
        }
      *cs << " = ";
      *cs << "s";  // generate the rvalue
      for (i = 0; i < this->n_dims (); i++)
        {
          *cs << "[i" << i << "]";
        }
      *cs << ";\n";
      for (i = 0; i < this->n_dims (); i++)
        {
          // decrement indentation as many times as the number of dimensions
          cs->decr_indent (0);
        }
      cs->indent ();
      *cs << "return temp;\n";
      // one more to get to the outermost level
      cs->decr_indent ();
      *cs << "}\n\n";
    }
  return 0;
}

// Generates the client-side inline information
int
be_array::gen_client_inline (void)
{
  if (!this->cli_inline_gen_)
    {
      TAO_OutStream *ci; // output stream
      TAO_NL  nl;        // end line
      be_type *bt;       // our base type
      be_state *s;       // state based code gen obj

      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      ci = cg->client_inline ();
      cg->push (TAO_CodeGen::TAO_ARRAY_DEFN_CI);
      s = cg->make_state ();
      if (!s)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_inline - "
                             "bad state\n"),
                            -1);
        }

      // retrieve our base type
      bt = be_type::narrow_from_decl (this->base_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_inline - "
                             "bad base type\n"),
                            -1);
        }

      // first define the _alloc, _dup and _free methods
      // If we are defined inside some scope, these methods become static
      // members of the enclosing scope

      // alloc method
      ci->indent (); // start from current indentation
      *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
      *ci << this->name () << "_alloc (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return new ";
      if (s->gen_code (bt, this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_inline - "
                             "state based codegen failed\n"),
                            -1);
        }
      if (this->gen_dimensions (ci) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_array::"
                             "gen_client_inline - "
                             "dimensions codegen failed\n"),
                            -1);
        }

      *ci << ";\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // free method
      *ci << this->name () << "_free (" << this->name () << "_slice *s)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "delete [] s;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      // emit implementations of methods of the _var, _out, and _forany classes
      if (this->gen_var_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_array: _var impl code gen failed\n"));
          return -1;
        }
      if (this->gen_out_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_array: _out impl code gen failed\n"));
          return -1;
        }
      if (this->gen_forany_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_array: _forany impl code gen failed\n"));
          return -1;
        }

      // generate client inline for base type
      if (bt->gen_client_inline () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_array: base type inline code gen failed\n"));
          return -1;
        }

      cg->pop ();
      this->cli_inline_gen_ = I_TRUE;
    }

  return 0;
}

int
be_array::gen_server_header (void)
{
  return 0;
}

int
be_array::gen_server_skeletons (void)
{
  return 0;
}

int
be_array::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generate the var defn
int
be_array::gen_var_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // names

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

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
  // constr from pointer to slice
  *ch << namebuf << " (" << this->local_name () << "_slice *);" << nl;
  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer to slice
  *ch << namebuf << " &operator= (" << this->local_name () << "_slice *);" <<
    nl;
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;

  // arrow operator
  // nothing here
  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  // overloaded [] operator
  *ch << this->local_name () << "_slice &operator[] (CORBA::ULong index);" <<
    nl;
  *ch << "const " << this->local_name () <<
    "_slice &operator[] (CORBA::ULong index) const;" << nl;

  // cast operators
  *ch << "operator const " << this->local_name () << "_slice *&() const;" <<
    nl;
  *ch << "operator " << this->local_name () << "_slice *&();" << nl;

  // in, inout, out and _retn
  *ch << "// in, inout, out, _retn " << nl;
  *ch << "const " << this->local_name () << "_slice *in (void) const;" << nl;
  *ch << this->local_name () << "_slice *inout (void);" << nl;
  *ch << this->local_name () << "_slice *&out (void);" << nl;
  *ch << this->local_name () << "_slice *_retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << this->local_name () << "_slice *ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_slice *ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_array::gen_var_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

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
  *ci << "\t" << ": ptr_ ((" << this->name () << "_slice *)0)" << nl;
  *ci << "{}\n\n";

  // constr from a _slice *
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_slice *p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // copy constructor (deep copy)
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = " << this->name () << "_dup (p.ptr_);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << this->name () << "_free (this->ptr_);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    "_slice *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// is what we own the same that is being assigned to us?" <<
    nl;
  *ci << "if (this->ptr_ != p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// delete our stuff and assume ownership of p" << nl;
  *ci << this->name () << "_free (this->ptr_);" << nl;
  *ci << "this->ptr_ = p;\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from _var
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (this != &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// not assigning to ourselves" << nl;
  *ci << this->name () << "_free (this->ptr_); // free old stuff" << nl;
  *ci << "this->ptr_ = " << this->name () <<
    "_dup (p.ptr_);// deep copy\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operators ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << this->name () <<
    "_slice *&() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << "_slice *&() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // two operator []s instead of ->
  ci->indent ();
  *ci << "ACE_INLINE const" << name () << "_slice &" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_[index];\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_slice &" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_[index];\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->val;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // the additional ptr () member function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// generate the _out definition
int
be_array::gen_out_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // to hold the _out name

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_out", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

  // generate the out definition (always in the client header)
  ch->indent (); // start with whatever was our current indent level

  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // constructor from a pointer to slice
  *ch << namebuf << " (" << this->local_name () << "_slice *&);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << this->local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment from slice *
  *ch << namebuf << " &operator= (" << this->local_name () << "_slice *);" <<
    nl;
  // cast
  *ch << "operator " << this->local_name () << "_slice *&();" << nl;
  // ptr fn
  *ch << this->local_name () << "_slice *&ptr (void);" << nl;
  // operator [] instead of ->
  *ch << this->local_name () << "_slice &operator[] (CORBA::ULong index);" << nl;
  *ch << "const " << this->local_name () << "_slice &operator[] " <<
    "(CORBA::ULong index) const;" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_slice *&ptr_;" << nl;
  *ch << "// assignment from T_var not allowed" << nl;
  *ch << "void operator= (const " << this->local_name () << "_var &);\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_array::gen_out_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_out", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_out", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  // generate the out implementation in the inline file

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // constr from a pointer to slice
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->name () << "_slice *&p)" << nl;
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
  *ci << this->name () << "_free (this->ptr_);" << nl;
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

  // assignment operator from _ptr
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    "_slice *p)" << nl;
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
    "_slice *&() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *&" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator [] instead of ->
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice &" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_[index];\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// generate the _var definition for ourself
int
be_array::gen_forany_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // names

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_forany", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

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
  // constr from pointer to slice
  *ch << namebuf << " (" << this->local_name () << "_slice *, " <<
    "CORBA::Boolean nocopy=0);" << nl;
  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer to slice
  *ch << namebuf << " &operator= (" << this->local_name () << "_slice *);" <<
    nl;
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;

  // arrow operator
  // nothing here
  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  // overloaded [] operator
  *ch << this->local_name () << "_slice &operator[] (CORBA::ULong index);" <<
    nl;
  *ch << "const " << this->local_name () <<
    "_slice &operator[] (CORBA::ULong index) const;" << nl;

  // cast operators
  *ch << "operator const " << this->local_name () << "_slice *&() const;" <<
    nl;
  *ch << "operator " << this->local_name () << "_slice *&();" << nl;

  // in, inout, out and _retn
  *ch << "// in, inout, out, _retn " << nl;
  *ch << "const " << this->local_name () << "_slice *in (void) const;" << nl;
  *ch << this->local_name () << "_slice *inout (void);" << nl;
  *ch << this->local_name () << "_slice *&out (void);" << nl;
  *ch << this->local_name () << "_slice *_retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << this->local_name () << "_slice *ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << "friend ACE_CORBA_1 (Any);" << nl;
  *ch << this->local_name () << "_slice *ptr_;" << nl;
  *ch << "CORBA::Boolean nocopy_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

int
be_array::gen_forany_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_forany", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_forany", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

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
  *ci << "\t" << ": ptr_ ((" << this->name () << "_slice *)0)," << nl;
  *ci << "\t  nocopy_ (0)" << nl;
  *ci << "{}\n\n";

  // constr from a _slice *
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_slice *p, " <<
    "CORBA::Boolean nocopy)" << nl;
  *ci << "\t: ptr_ (p)," << nl;
  *ci << "\t  nocopy_ (nocopy)" << nl;
  *ci << "{}\n\n";

  // copy constructor (deep copy)
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = " << this->name () << "_dup (p.ptr_);" << nl;
  *ci << "this->nocopy_ = p.nocopy_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << this->name () << "_free (this->ptr_);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    "_slice *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// is what we own the same that is being assigned to us?" <<
    nl;
  *ci << "if (this->ptr_ != p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// delete our stuff and assume ownership of p" << nl;
  *ci << this->name () << "_free (this->ptr_);" << nl;
  *ci << "this->ptr_ = p;\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from _forany
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (this != &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// not assigning to ourselves" << nl;
  *ci << this->name () << "_free (this->ptr_); // free old stuff" << nl;
  *ci << "this->ptr_ = " << this->name () <<
    "_dup (p.ptr_);// deep copy" << nl;
  *ci << "this->nocopy_ = p.nocopy_;\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operators ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << this->name () <<
    "_slice *&() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << "_slice *&() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // two operator []s instead of ->
  ci->indent ();
  *ci << "ACE_INLINE const" << name () << "_slice &" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_[index];\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_slice &" << nl;
  *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_[index];\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->val;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // the additional ptr () member function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

int
be_array::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_array, // typecode kind" << nl;
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
be_array::gen_encapsulation (void)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_type *bt; // base type

  os = cg->client_stubs ();
  os->indent (); // start from the current indentation level

  // XXXASG - byte order must be based on what m/c we are generating code -
  // TODO
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
  //  *os << this->max_size () << ",\n";
  return 0;
}

// compute typecode size
long
be_array::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation length
  return 4 + 4 + this->tc_encap_len ();
}

long
be_array::tc_encap_len (void)
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
                      "be_array::tc_encap_len - bad base type\n"));
          return 0;
        }
      this->encap_len_ += bt->tc_size ();
      this->encap_len_ += 4; // to hold the array size

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

int be_array::write_as_return (TAO_OutStream *stream,
			       be_type *type)
{
  *stream << type->name () << "_slice *";
  return 0;
}

// Visiting methods
int be_array::accept (be_visitor *visitor)
{
  return visitor->visit_array (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_array, AST_Array, be_type)
IMPL_NARROW_FROM_DECL (be_array)
