/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.h
//
// = DESCRIPTION
//    Extension of class AST_Interface_Fwd that provides additional means for C++
//    mapping of an interface.
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
 * BE_InterfaceFwd
 */

be_interface_fwd::be_interface_fwd (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_interface_fwd::be_interface_fwd (UTL_ScopedName *n, UTL_StrList *p)
  : AST_InterfaceFwd (n, p),
    AST_Decl (AST_Decl::NT_interface_fwd, n, p)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// generate the client header
int be_interface_fwd::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line

  if (!this->cli_hdr_gen_) // not already generated
    {
      // retrieve a singleton instance of the code generator
      TAO_CodeGen *cg = TAO_CODEGEN::instance ();

      // get the client header
      ch = cg->client_header ();

      ch->indent (); // start from the current

      // all we do in this is generate a forward declaration of the class
      *ch << "class " << this->local_name () << ";" << nl;

      // generate the ifdefined macro for the _ptr type
      ch->gen_ifdef_macro (this->flatname (), "_ptr");

      // generate the _ptr declaration
      *ch << "typedef " << this->local_name () << " *" << this->local_name () <<
        "_ptr; \n";

      ch->gen_endif ();

      // enclose under an ifdef macro
      ch->gen_ifdef_macro (this->flatname (), "_var");

      // generate the _var declaration
      if (this->gen_var_defn () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_interface_fwd - error generating _var definition\n"));
          return -1;
        }
      // gen an endif
      ch->gen_endif ();

      // enclose under an ifdef macro
      ch->gen_ifdef_macro (this->flatname (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (this->gen_out_defn () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "be_interface_fwd - error generating _var definition\n"));
          return -1;
        }

      // generate the endif macro
      ch->gen_endif ();

      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
}

int
be_interface_fwd::gen_client_stubs (void)
{
  return 0;
}

int
be_interface_fwd::gen_client_inline (void)
{
  TAO_OutStream *ci; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  // generate the ifdefined macro for  the _var type
  ci->gen_ifdef_macro (this->flatname (), "_var");

  if (this->gen_var_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface_fwd: _var impl code gen failed\n"));
      return -1;
    }

  ci->gen_endif ();

  // generate the ifdefined macro for  the _out type
  ci->gen_ifdef_macro (this->flatname (), "_out");

  if (this->gen_out_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface_fwd: _out impl code gen failed\n"));
      return -1;
    }
  ci->gen_endif ();

  return 0;
}

int
be_interface_fwd::gen_server_header (void)
{
  return 0;
}

int
be_interface_fwd::gen_server_skeletons (void)
{
  return 0;
}

int
be_interface_fwd::gen_server_inline (void)
{
  return 0;
}

// generate the var definition
int
be_interface_fwd::gen_var_defn (void)
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
  *ch << namebuf << " (" << local_name () << "_ptr);" << nl;

  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;

  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;

  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << local_name () << "_ptr);" << nl;

  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << nl;

  // arrow operator
  *ch << local_name () << "_ptr operator-> (void) const;" << nl;

  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  *ch << "operator const " << local_name () << "_ptr &() const;" << nl;
  *ch << "operator " << local_name () << "_ptr &();" << nl;

  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  *ch << local_name () << "_ptr in (void) const;" << nl;
  *ch << local_name () << "_ptr &inout (void);" << nl;
  *ch << local_name () << "_ptr &out (void);" << nl;
  *ch << local_name () << "_ptr _retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << local_name () << "_ptr ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // private
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_ptr ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_interface_fwd::gen_var_impl (void)
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

  // generate the var implementation in the inline file
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
  *ci << "\t" << ": ptr_ (" << this->name () << "::_nil ())" << nl;
  *ci << "{}\n\n";

  // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // the additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr " << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "\t: ptr_ (" << name () << "::_duplicate (p.ptr ()))" << nl;
  *ci << "{}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << name () <<
    "_ptr p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
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
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << name () << "::_duplicate (p.ptr ());\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << name () <<
    "_ptr &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << "_ptr &() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator->
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr " << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr &" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();" << nl;
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr " << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// yield ownership of managed obj reference" << nl;
  *ci << this->name () << "_ptr val = this->ptr_;" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();" << nl;
  *ci << "return val;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// generate the _out definition
int
be_interface_fwd::gen_out_defn (void)
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
  *ch << namebuf << " (" << local_name () << "_ptr &);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // only interface allows assignment from var &
  *ch << namebuf << " &operator= (const " << local_name () << "_var &);" << nl;
  *ch << namebuf << " &operator= (" << local_name () << "_ptr);" << nl;
  // cast
  *ch << "operator " << local_name () << "_ptr &();" << nl;
  // ptr fn
  *ch << local_name () << "_ptr &ptr (void);" << nl;
  // operator ->
  *ch << local_name () << "_ptr operator-> (void);" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_ptr &ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

int
be_interface_fwd::gen_out_impl (void)
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
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

      // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr &p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = " << this->name () << "::_nil ();\n";
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
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();\n";
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

      // assignment operator from _var
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << this->name () <<
    "_var &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = " << this->name () << "::_duplicate (p.ptr ());" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // assignment operator from _ptr
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    "_ptr p)" << nl;
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
    "_ptr &() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_ptr &" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // operator->
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_ptr " << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}
int
be_interface_fwd::gen_typecode (void)
{
  return 0;
}

long
be_interface_fwd::tc_size (void)
{
  return 0;
}

// Visiting methods
int be_interface_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_interface_fwd (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_interface_fwd, AST_InterfaceFwd, be_type)
IMPL_NARROW_FROM_DECL (be_interface_fwd)
