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
  be_type *bt;

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

  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();

}

// Code generation

int
be_array::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  unsigned long i;   // loop index
  TAO_NL  nl;        // end line
  be_type *bt;       // our base type
  be_decl *d;        // enclosing scope
  be_state *s;       // state based code gen object

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_ARRAY_DEFN_CH); // set current code gen state

      ch = cg->client_header (); // retrieve client hdr stream
      cg->outstream (ch); // set client hdr as the current stream
      cg->node (this);    // pass ourselves for type gen

      s = cg->make_state (); // get the state-based code generation object

      bt = be_type::narrow_from_decl (this->base_type ());

      // generate our base type
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_array: type generation failed\n"));
          return -1;
        }

      // print our name
      *ch << " " <<  this->local_name ();
      // print our dimensions
      for (i = 0; i < this->n_dims (); i++)
        {
          AST_Expression *expr = this->dims ()[i]; // retrieve the ith
                                                   // dimension value
          if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimensions\n\n"));
              return -1;
            }
          if (expr->ev ()->et == AST_Expression::EV_ulong)
            {
              *ch << "[" << ((int)expr->ev ()->u.ulval) << "]";
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimension value\n"));
              return -1;
            }
        }
      *ch << ";" << nl;

      // if our base type is VARIABLE length, then we are variable length too
      this->size_type (bt->size_type ());

      cg->pop ();

      cg->push (TAO_CodeGen::TAO_ARRAY_OTHER_CH); // rest of the array defn
      s = cg->make_state ();

      *ch << "typedef ";

      // now generate the slice definition
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_array: type generation failed\n"));
          return -1;
        }

      *ch << " " <<  this->local_name () << "_slice";
      // print dimensions except first one
      for (i = 1; i < this->n_dims (); i++)
        {
          // no error checking. Assumption is that everything is valid since we
          // already checked it before.
          AST_Expression *expr = this->dims ()[i]; // retrieve the ith
                                                   // dimension value
          *ch << "[" << ((int)expr->ev ()->u.ulval) << "]";
        }
      *ch << ";" << nl;

      // memory management functions
      // first find if we are inside a scope
      if (this->is_nested ()) // we were defined inside a scope. Hence the
        // memory management 
        // functions become static members of the enclosing class
        *ch << "static ";
      *ch <<  this->local_name () << "_slice *" << this->local_name () <<
        "_alloc (void);" << nl;
      if (this->is_nested ())
        *ch << "static ";
      *ch <<  this->local_name () << "_slice *" << this->local_name () <<
        "_dup (const " << this->local_name () << "_slice *);" << nl;
      if (this->is_nested ())
        *ch << "static ";
      *ch << "void " << this->local_name () << "_free (" << this->local_name ()
          << "_slice *);\n\n";

      // generate the _var, _out, and _forany definition
      this->gen_var_defn ();
      this->gen_out_defn ();
      this->gen_forany_defn ();

      // Generate the typecode decl
      // All names in the root scope have length 2 (for the root and
      // ourself). The children have length greater than 2. Thus, if our name
      // length is 2 or less, we are outermost and our typecode decl must be
      // extern, else we are defined static inside the enclosing scope.
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
      cg->pop ();
    }
  return 0;
}

int
be_array::gen_client_stubs (void)
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
      this->cli_stub_gen_ = 1;

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

// Generates the client-side inline information
int 
be_array::gen_client_inline (void)
{
  if (!this->cli_inline_gen_)
    {
      TAO_OutStream *ci; // output stream
      unsigned long i;   // loop index
      TAO_NL  nl;        // end line
      be_type *bt;       // our base type
      be_decl *d;        // enclosing scope
      be_state *s;       // state based code gen obj

      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  
      ci = cg->client_inline ();
      cg->outstream (ci);
      cg->node (this);
      cg->push (TAO_CodeGen::TAO_ARRAY_DEFN_CI);
      s = cg->make_state ();

      // retrieve our base type
      bt = be_type::narrow_from_decl (this->base_type ());

      // first define the _alloc, _dup and _free methods
      // If we are defined inside some scope, these methods become static
      // members of the enclosing scope

      ci->indent (); // start from current indentation

      // alloc method
      *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
      *ci << this->name () << "_alloc (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "new ";
      if (s->gen_code (bt, this) == -1)
        return -1;
      
      ci->decr_indent ();
      *ci << "}\n\n";
      
      // dup method
      *ci << "ACE_INLINE " << this->name () << "_slice *" << nl;
      *ci << this->name () << "_dup (" << this->name () << "_slice * s)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << this->name () << "_slice *temp;" << nl;
      *ci << "temp = " << this->name () << "_alloc ();" << nl;
      *ci << "// copy each individual elements" << nl;
      for (i = 0; i < this->n_dims (); i++)
        {
          AST_Expression *expr = this->dims ()[i]; // retrieve the ith
                                                   // dimension value
          if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimensions\n\n"));
              return -1;
            }
          if (expr->ev ()->et == AST_Expression::EV_ulong)
            {
              // generate a loop for each dimension
              *ci << "for (CORBA::ULong i" << i << " = 0; i" << i << " < " <<
                expr->ev ()->u.ulval << "; i" << i << "++)\n"; 
              ci->incr_indent ();
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimension value\n"));
              return -1;
            }
        }

      // the code below looks terribly complex. However, all it is trying to do
      // is to generate code of the form 
      // temp [i0][i1].... = 
      *ci << "temp ";
      for (i = 0; i < this->n_dims (); i++)
        {
          AST_Expression *expr = this->dims ()[i]; // retrieve the ith
                                                   // dimension value
          if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimensions\n\n"));
              return -1;
            }
          if (expr->ev ()->et == AST_Expression::EV_ulong)
            {
              *ci << "[" << ((int)expr->ev ()->u.ulval) << "]";
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimension value\n"));
              return -1;
            }
        }
      *ci << " = s";
      for (i = 0; i < this->n_dims (); i++)
        {
          AST_Expression *expr = this->dims ()[i]; // retrieve the ith
                                                   // dimension value
          if ((expr == NULL) || ((expr != NULL) && (expr->ev () == NULL)))
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimensions\n\n"));
              return -1;
            }
          if (expr->ev ()->et == AST_Expression::EV_ulong)
            {
              *ci << "[" << ((int)expr->ev ()->u.ulval) << "]";
            }
          else
            {
              ACE_ERROR ((LM_ERROR, "be_array: bad dimension value\n"));
              return -1;
            }
        }
      *ci << ";\n";
      for (i = 0; i < this->n_dims (); i++)
        {
          // decrement indentation as many times as the number of dimensions
          ci->decr_indent ();
        }

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

// Generates the server-side inline
int 
be_array::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generate the _var definition for ourself
int
be_array::gen_forany_defn (void)
{
  TAO_OutStream *ch; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char namebuf [MAXNAMELEN];  // to hold the _out name

  ACE_OS::memset (namebuf, '\0', MAXNAMELEN);
  ACE_OS::sprintf (namebuf, "%s_forany", local_name ()->get_string ());

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
  // constr
  *ch << namebuf << " (" << local_name () << "_slice *);" << nl;
  // copy constructor
  *ch << namebuf << " (const " << namebuf << 
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << local_name () << 
    "_slice *, CORBA::Boolean nocopy=0);" << nl;
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf << 
    " &);" << nl;

  // overloaded [] operator
  *ch << namebuf << "_slice &operator[] (CORBA::ULong index);" << nl;
  *ch << "const " << namebuf << 
    "_slice &operator[] (CORBA::ULong index) const;" << nl;
  
  // cast operators
  *ch << "operator const " << local_name () << "_slice *&() (void) const;" << nl;
  *ch << "operator " << local_name () << "_slice *&() (void);" << nl;
  
  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  if (this->size_type () == be_decl::FIXED)
    {
      *ch << "const " << local_name () << " in (void) const;" << nl;
      *ch << local_name () << " inout (void);" << nl;
      *ch << local_name () << " out (void);" << nl;
      *ch << local_name () << "_slice *_retn (void);" << nl;
    }
  else
    {
      *ch << "const " << local_name () << " in (void) const;" << nl;
      *ch << local_name () << " inout (void);" << nl;
      *ch << local_name () << "_slice *&out (void);" << nl;
      *ch << local_name () << " *_retn (void);" << nl;
    }
  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_slice *ptr_;\n";
  ch->decr_indent ();
  *ch << "};\n\n";
  
  return 0;
}

int
be_array::gen_forany_impl (void)
{
  TAO_OutStream *ci; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char fname [MAXNAMELEN];  // to hold the full and
  char lname [MAXNAMELEN];  // local _var names

  ACE_OS::memset (fname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (fname, "%s_forany", this->fullname ());

  ACE_OS::memset (lname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (lname, "%s_forany", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();
  cg->outstream (ci);

  // generate the forany implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************" 
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // default constr
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << 
    " (void) // default constructor" << nl;
  *ci << "\t" << ": ptr_ ((" << name () << "_slice *)0)" << nl;
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
  *ci << "this->ptr_ = " << this->name () << "_dup (p->ptr);\n";
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
    "_slice p)" << nl;
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
    "_dup (p->ptr);// deep copy\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operators ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << name () << 
    "_slice &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << "_slice &() // cast " << nl;
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
  *ci << "ACE_INLINE " << fname << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << fname << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << fname << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_slice " << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->val;\n";
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
      this->encap_len_ += bt->tc_encap_len ();
      this->encap_len_ += 4; // to hold the array size 

    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_array, AST_Array, be_type)
IMPL_NARROW_FROM_DECL (be_array)

