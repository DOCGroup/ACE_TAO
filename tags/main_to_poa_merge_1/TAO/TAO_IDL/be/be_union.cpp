// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union.cpp
//
// = DESCRIPTION
//    Extension of class AST_Union that provides additional means for C++
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
 * BE_Union
 */

be_union::be_union (void)
{
}

be_union::be_union (AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Union (dt, n, p),
    AST_Structure (AST_Decl::NT_union, n, p),
    AST_Decl (AST_Decl::NT_union, n, p),
    UTL_Scope (AST_Decl::NT_union),
    member_count_ (-1),
    default_index_ (-2)
{
}

// compute total number of members
int
be_union::compute_member_count (void)
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
          this->member_count_++;
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// compute total number of members
int
be_union::compute_default_index (void)
{
  UTL_ScopeActiveIterator *si;  // iterator
  AST_Decl *d;  // temp node
  be_union_branch *bub; // union branch node
  int i = 0; // counter

  this->default_index_ = -1;  // if not used at all, this is the value it will
                              // take

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
              bub = be_union_branch::narrow_from_decl (d);
              if (bub->label ()->label_kind () == AST_UnionLabel::UL_default)
                this->default_index_ = i; // zero based indexing
              i++;
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// return the member count
int
be_union::member_count (void)
{
  if (this->member_count_ == -1)
    this->compute_member_count ();

  return this->member_count_;
}

// return the default_index
int
be_union::default_index (void)
{
  if (this->default_index_ == -2)
    this->compute_default_index ();

  return this->default_index_;
}


// ****************************
// code generation
// ****************************

int
be_union::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_state *s;       // code generation state


  if (!this->cli_hdr_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      ch = cg->client_header ();

      ch->indent (); // start with the current indentation level
      *ch << "class " << local_name () << nl;
      *ch << "{" << nl;
      *ch << "public:\n";
      ch->incr_indent ();

      // generate default and copy constructors
      *ch << local_name () << " (void); // default constructor" << nl;
      *ch << local_name () << " (const " << local_name () <<
        " &); // copy constructor" << nl;
      // generate destructor
      *ch << "~" << local_name () << " (void); // destructor" << nl;
      // generate assignment operator
      *ch << local_name () << " &operator= (const " << local_name () <<
        " &); // copy constructor\n\n";

      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      cg->push (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CH); // set current code gen state
      bt = be_type::narrow_from_decl (this->disc_type ());

      s  = cg->make_state (); // get the code gen object for the current state
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_header\n"));
          ACE_ERROR ((LM_ERROR, "Discriminant type generation failure\n"));
          return -1;
        }
      cg->pop ();

      // now generate the public defn for the union branch members
      cg->push (TAO_CodeGen::TAO_UNION_PUBLIC_CH); // set current code gen state

      if (be_scope::gen_client_header () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_header\n"));
          ACE_ERROR ((LM_ERROR, "member generation failure\n"));
          return -1;
        }
      cg->pop ();

      // now generate the private data members of the union
      cg->push (TAO_CodeGen::TAO_UNION_PRIVATE_CH); // set current code gen state
      ch->decr_indent ();
      *ch << "private:\n";
      ch->incr_indent ();
      *ch << bt->nested_type_name (this) << " disc_;" << nl; // emit the
                                           // ACE_NESTED_CLASS macro

      // the members are inside of a union
      *ch << "union" << nl;
      *ch << "{\n";
      ch->incr_indent (0);
      if (be_scope::gen_client_header () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_header\n"));
          ACE_ERROR ((LM_ERROR, "data member generation failure\n"));
          return -1;
        }
      ch->decr_indent ();
      *ch << "}; // end of union\n\n";

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
      cg->pop ();
      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
 }

int
be_union::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line

  if (!this->cli_stub_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      cg->push (TAO_CodeGen::TAO_UNION_PUBLIC_CS); // set current code gen state

      cs = cg->client_stubs ();

      *cs << "// *************************************************************"
          << nl;
      *cs << "// Operations for union " << this->name () << nl;
      *cs << "// *************************************************************\n\n";

      // generate the copy constructor and the assignment operator here
      cs->indent ();
      *cs << "// copy constructor" << nl;
      *cs << this->name () << "::" << this->local_name () << " (const " <<
        this->name () << " &u)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // first set the discriminant
      *cs << "this->disc_ = u.disc_;" << nl;
      // now switch based on the disc value
      *cs << "switch (this->disc_)" << nl;
      *cs << "{\n";
      cs->incr_indent (0);
      if (be_scope::gen_client_stubs () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_stubs\n"));
          ACE_ERROR ((LM_ERROR, "constructor codegen failure\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "}\n";
      cs->decr_indent ();
      *cs << "}\n\n";

      cs->indent ();
      *cs << "// assignment operator" << nl;
      *cs << this->name () << " &" << nl; // return type
      *cs << this->name () << "::operator= (const " <<
        this->name () << " &u)" << nl;
      *cs << "{\n";
      cs->incr_indent ();
      // first set the discriminant
      *cs << "this->disc_ = u.disc_;" << nl;
      // now switch based on the disc value
      *cs << "switch (this->disc_)" << nl;
      *cs << "{\n";
      cs->incr_indent (0);
      if (be_scope::gen_client_stubs () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_stubs\n"));
          ACE_ERROR ((LM_ERROR, "assignment op codegen failure\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "}" << nl;
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
          ACE_ERROR ((LM_ERROR, "be_union:Error generating encapsulation\n\n"));
          return -1;
        }
      cs->decr_indent ();
      *cs << "};" << nl;

      *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () <<
        " (CORBA::tk_union, sizeof (_oc_" <<  this->flatname () <<
        "), (unsigned char *) &_oc_" << this->flatname () <<
        ", CORBA::B_FALSE);" << nl;
      *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
        this->flatname () << ";\n\n";
      this->cli_stub_gen_ = I_TRUE;
      cg->pop ();
    }
  return 0;
}

int
be_union::gen_server_header (void)
{
  return 0;
}

int
be_union::gen_server_skeletons (void)
{
  return 0;
}

// Generates the client-side inline information
int
be_union::gen_client_inline (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_state *s;       // code generation state


  if (!this->cli_inline_gen_)
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();
      ci = cg->client_inline ();

      *ci << "// *************************************************************"
          << nl;
      *ci << "// Inline operations for union " << this->name () << nl;
      *ci << "// *************************************************************\n\n";

      // generate the default constructor and the destructor here
      ci->indent ();
      *ci << "// default constructor" << nl;
      *ci << "ACE_INLINE" << nl;
      *ci << this->name () << "::" << this->local_name () << " (void)" << nl;
      *ci << "{" << nl;
      *ci << "}" << nl << nl;

      *ci << "// destructor" << nl;
      *ci << "ACE_INLINE" << nl;
      *ci << this->name () << "::~" << this->local_name () << " (void)" << nl;
      *ci << "{" << nl;
      *ci << "}\n\n";

      // the discriminant type may have to be defined here if it was an enum
      // declaration inside of the union statement.

      cg->push (TAO_CodeGen::TAO_UNION_DISCTYPEDEFN_CI); // set current code gen state
      bt = be_type::narrow_from_decl (this->disc_type ());

      s  = cg->make_state (); // get the code gen object for the current state
      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_header\n"));
          ACE_ERROR ((LM_ERROR, "Discriminant type generation failure\n"));
          return -1;
        }
      cg->pop ();

      // now generate the implementation of the access methods
      cg->push (TAO_CodeGen::TAO_UNION_PUBLIC_CI); // set current code gen state
      if (be_scope::gen_client_inline () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union::gen_client_inline\n"));
          ACE_ERROR ((LM_ERROR, "accessor generation failure\n"));
          return -1;
        }
      cg->pop ();

      if (this->gen_var_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union: _var impl code gen failed\n"));
          return -1;
        }
      if (this->size_type () == be_decl::VARIABLE && this->gen_out_impl () == -1)
        {
          ACE_ERROR ((LM_ERROR, "be_union: _out impl code gen failed\n"));
          return -1;
        }
      this->cli_inline_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the server-side inline
int
be_union::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// generate typecode.
// Typecode for union comprises the enumerated value followed by the
// encapsulation of the parameters

// generate the _var definition for ourself
int
be_union::gen_var_defn (void)
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
  // constr
  *ch << namebuf << " (" << local_name () << " *);" << nl;
  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << local_name () << " *);" << nl;
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;

  // arrow operator
  *ch << local_name () << " *operator-> (void);" << nl;
  *ch << "const " << local_name () << " *operator-> (void) const;" << nl;
  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  *ch << "operator const " << local_name () << " &() const;" << nl;
  *ch << "operator " << local_name () << " &();" << nl;
  *ch << "operator " << local_name () << " &() const;" << nl;
  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  if (this->size_type () == be_decl::FIXED)
    {
      *ch << "const " << local_name () << " &in (void) const;" << nl;
      *ch << local_name () << " &inout (void);" << nl;
      *ch << local_name () << " &out (void);" << nl;
      *ch << local_name () << " _retn (void);" << nl;
    }
  else
    {
      *ch << "const " << local_name () << " &in (void) const;" << nl;
      *ch << local_name () << " &inout (void);" << nl;
      *ch << local_name () << " *&out (void);" << nl;
      *ch << local_name () << " *_retn (void);" << nl;
    }

  // generate an additional member function that returns the underlying pointer
  *ch << local_name () << " *ptr(void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << " *ptr_;\n";
  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_union::gen_var_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_var", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

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

  // constr from a pointer
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << " *p)" << nl;
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
    " &p)" << nl;
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

  // the out is handled differently based on our size type
  ci->indent ();
  if (this->size_type () == be_decl::VARIABLE)
    {
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

    }
  else
    {
      *ci << "// mapping for fixed size " << nl;
      *ci << "ACE_INLINE " << name () << " &" << nl;
      *ci << fname << "::out (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE " << name () << nl;
      *ci << fname << "::_retn (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
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

    }

  return 0;
}

// generate the _out definition
int
be_union::gen_out_defn (void)
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

  // constructor from a pointer
  *ch << namebuf << " (" << local_name () << " *&);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // assignment
  *ch << namebuf << " &operator= (" << local_name () << " *);" << nl;
  // operator ()
  *ch << "operator " << local_name () << " *&();" << nl;
  // ptr fn
  *ch << local_name () << " *&ptr (void);" << nl;
  // operator ->
  *ch << local_name () << " *operator-> (void);" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << " *&ptr_;" << nl;
  *ch << "// assignment from T_var not allowed" << nl;
  *ch << "void operator= (const " << local_name () << "_var &);\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_union::gen_out_impl (void)
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

  // generate the var implementation in the inline file

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


  return 0;
}

int
be_union::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_union, // typecode kind" << nl;
  *cs << this->tc_encap_len () << ", // encapsulation length\n";
  // now emit the encapsulation
  return this->gen_encapsulation ();
}

// generate encapsulation.
// An encapsulation for ourselves will be necessary when we are part of some
// other IDL type and a typecode for that other type is being generated. This
// will comprise our typecode kind. IDL types with parameters will additionally
// have the encapsulation length and the entire typecode description

int
be_union::gen_encapsulation (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  long i, arrlen;
  long *arr;  // an array holding string names converted to array of longs
  be_type *discrim;

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
  *cs << " // name = " << this->local_name () << ",\n";

  // generate typecode for discriminant
  discrim = be_type::narrow_from_decl (this->disc_type ());
  if (discrim->gen_typecode () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_union: cannot generate typecode for discriminant\n"));
      return -1;
    }

  // generate the default used flag
  cs->indent ();
  *cs << this->default_index () << ", // default used index" << nl;
  // generate the member count
  *cs << this->member_count () << ", // member count\n";
  cs->incr_indent (0);
  // hand over to the scope to generate the typecode for elements
  if (be_scope::gen_encapsulation () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_union: cannot generate code for members\n"));
      return -1;
    }
  cs->decr_indent (0);
  return 0;
}

// compute typecode size
long
be_union::tc_size (void)
{
  // 4 bytes for enumeration, 4 bytes for storing encap length val, followed by the
  // actual encapsulation length
  return 4 + 4 + this->tc_encap_len ();
}

long
be_union::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      long slen;
      be_type *discrim;

      // Macro to avoid "warning: unused parameter" type warning.
      ACE_UNUSED_ARG (slen);

      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += this->repoID_encap_len (); // for repoID

      // do the same thing for the local name
      this->encap_len_ += this->name_encap_len (); // for name

      // add encapsulation size of discriminant typecode
      discrim = be_type::narrow_from_decl (this->disc_type ());
      this->encap_len_ += discrim->tc_size ();

      this->encap_len_ += 4; // to hold the "default used" flag
      this->encap_len_ += 4; // to hold the member count

      // compute encap length for members
      this->encap_len_ += be_scope::tc_encap_len ();
    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_union)
IMPL_NARROW_FROM_SCOPE (be_union)
