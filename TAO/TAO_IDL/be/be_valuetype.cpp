// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_valuetype.cpp
//
// = DESCRIPTION
//    Extension of class AST_Interface and be_interface that provides
//    additional means for C++ mapping of an valuetype.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_interface.cpp
//
// ============================================================================


#include "idl.h"
#include "idl_extern.h"
// #include "be_visitor_valuetype.h"
#include "be.h"

#ifdef IDL_HAS_VALUETYPE

ACE_RCSID(be, be_valuetype, "$Id$")

/*
 * BE_Valuetype
 */

// default constructor
be_valuetype::be_valuetype (void)
  : full_obv_skel_name_ (0),
    abstract_ (0)
{
  this->size_type (be_decl::VARIABLE); // always the case
  (AST_Module::narrow_from_scope (this->defined_in ()))->set_has_nested_valuetype ();
}

// constructor used to build the AST
be_valuetype::be_valuetype (UTL_ScopedName *n, AST_Interface **ih, long nih,
                            UTL_StrList *p, idl_bool set_abstract)
  : be_interface (n, ih, nih, p),
    AST_Interface (n, ih, nih, p),
    AST_Decl (AST_Decl::NT_interface, n, p), // its like an interface
    UTL_Scope (AST_Decl::NT_interface),
    full_obv_skel_name_ (0),
    abstract_ (set_abstract)
{
  // check that redefine() copy all members !
  // ---
  this->size_type (be_decl::VARIABLE); // always the case
  (AST_Module::narrow_from_scope (this->defined_in ()))->set_has_nested_valuetype ();
}

be_valuetype::~be_valuetype (void)
{

}

void
be_valuetype::redefine (AST_Interface *from, UTL_StrList *p)
{
  this->AST_Interface::redefine (from, p);
  abstract_ = from->is_abstract_valuetype();
}

// Is true if no-virtual accessor and modifier should be generated
// If #pragma TAO OBV opt_accessor (todo) is used or -Wb,obv_opt_accessor
idl_bool
be_valuetype::opt_accessor ()
{
  return idl_global->obv_opt_accessor ();
}

idl_bool
be_valuetype::is_valuetype ()
{
  return 1;
}

idl_bool
be_valuetype::is_abstract_valuetype ()
{
  return abstract_;
}

void
be_valuetype::set_abstract_valuetype ()
{
  abstract_ = 1;
}

// compute stringified fully scoped skeleton name (OBV_name)
void
be_valuetype::compute_fullobvskelname (void)
{
  this->compute_fullskelname (this->full_obv_skel_name_, "OBV_");
}


// retrieve the fully scoped skeleton name
const char*
be_valuetype::full_obv_skel_name (void)
{
  if (!this->full_obv_skel_name_)
    compute_fullobvskelname ();

  return this->full_obv_skel_name_;
}



// generate the var definition
int
be_valuetype::gen_var_defn (void)
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
  *ch << "class " << idl_global->export_macro ()
      << " " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // default constr
  *ch << namebuf << " (void); // default constructor" << nl;
  *ch << namebuf << " (" << local_name () << "*);" << nl;
  *ch << namebuf << " (const " << local_name () << "*); // (TAO extension)" << nl;

  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;

  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;

  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << local_name () << "*);" << nl;

  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << nl;

  // arrow operator
  *ch << local_name () << "* operator-> (void) const;" << nl;

  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  *ch << "operator const " << local_name () << "* () const;" << nl;
  *ch << "operator " << local_name () << "* ();" << nl;

/* %! &() or () ?
  *ch << "operator const " << local_name () << "* &() const;" << nl;
  *ch << "operator " << local_name () << "* &();" << nl;
*/
  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  *ch << local_name () << "* in (void) const;" << nl;
  *ch << local_name () << "* &inout (void);" << nl;
  *ch << local_name () << "* &out (void);" << nl;
  *ch << local_name () << "* _retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << local_name () << "* ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // private
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "* ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_valuetype::gen_var_impl (void)
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
  *ci << "  " << ": ptr_ (0)" << nl;
  *ci << "{}\n\n";

  // constr from a pointer
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "* p)" << nl;
  *ci << "  : ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // constr from a const pointer
  // TAO extension - it appears that there are problems with at least g++
  // which reclaims amguity between T(T*) and T(const T_var &)
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << name () << "* p)" << nl;
  *ci << "  : ptr_ (ACE_const_cast(" << name() << "*, p))" << nl;
  *ci << "{}\n\n";

  // the additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "* " << nl;
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
  *ci << "{" << be_idt_nl
      <<    "CORBA::add_ref (p.ptr ());" << be_nl
      <<    "this->ptr_ = p.ptr ();" << be_uidt_nl
      << "}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::remove_ref (this->ptr_);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << name () <<
    "* p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::remove_ref (this->ptr_);" << nl;
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
  *ci << "CORBA::remove_ref (this->ptr_);" << nl
      << local_name() << "* tmp = p.ptr ();" << be_nl
      << "CORBA::add_ref (tmp);" << be_nl
      << "this->ptr_ = tmp;\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << name () <<
    "* () const // cast" << nl;  // %! &() ?
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << "* () // cast " << nl;  // %! &()?
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator->
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "* " << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "*" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "* &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "* &" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::remove_ref (this->ptr_);" << nl;
  *ci << "this->ptr_ = 0;" << nl;
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "* " << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// yield ownership of managed obj reference" << nl;
  *ci << local_name () << "* tmp = this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;" << nl;
  *ci << "return tmp;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// generate the _out definition
int
be_valuetype::gen_out_defn (void)
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

  *ch << "class " << idl_global->export_macro ()
      << " " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // constructor from a pointer
  *ch << namebuf << " (" << local_name () << "* &);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (const " << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // only interface allows assignment from var &
  *ch << namebuf << " &operator= (const " << local_name () << "_var &);" << nl;
  *ch << namebuf << " &operator= (" << local_name () << "*);" << nl;
  // cast
  *ch << "operator " << local_name () << "* &();" << nl;
  // ptr fn
  *ch << local_name () << "* &ptr (void);" << nl;
  // operator ->
  *ch << local_name () << "* operator-> (void);" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "* &ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_valuetype::gen_out_impl (void)
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

      // constr from a pointer
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "* &p)" << nl;
  *ci << "  : ptr_ (p)" << nl;
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
  *ci << "  : ptr_ (p.out ())" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::remove_ref (this->ptr_);" << nl;
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "  : ptr_ (ACE_const_cast (" << fname << "&,p).ptr_)" << nl;
  *ci << "{}\n\n";

      // assignment operator from _out &
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = ACE_const_cast (" << fname << "&,p).ptr_;" << nl;
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
  *ci << local_name() << "* tmp = p.ptr ();" << be_nl
      << "CORBA::add_ref (tmp);" << be_nl
      << "this->ptr_ = tmp;" << be_nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // assignment operator from *
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    "* p)" << nl;
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
    "* &() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "* &" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // operator->
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "* " << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

#if 0
// generate typecode.
// Typecode for interface comprises the enumerated value followed by the
// encapsulation of the parameters

int
be_valuetype::gen_typecode (void)
{
  ACE_ASSERT(0);

  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_objref, // typecode kind" << nl;
  *cs << this->tc_encap_len () << ", // encapsulation length\n";
  // now emit the encapsulation
  return this->gen_encapsulation ();
}

// generate encapsulation
// An encapsulation for ourselves will be necessary when we are part of some
// other IDL type and a typecode for that other type is being generated. This
// will comprise our typecode kind. IDL types with parameters will additionally
// have the encapsulation length and the entire typecode description
int
be_valuetype::gen_encapsulation (void)
{
  ACE_ASSERT (0);

  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  long i, arrlen;
  ACE_UINT32 *arr;

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  // XXXASG - byte order must be based on what m/c we are generating code -
  // TODO
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
  *cs << " // name = " << this->local_name () << ",\n";

  return 0;
}

// compute size of typecode
long
be_valuetype::tc_size (void)
{
  ACE_ASSERT (0);

   return 4 + 4 + this->tc_encap_len ();
}

// compute the encapsulation length
long
be_valuetype::tc_encap_len (void)
{
  ACE_ASSERT (0);

  if (this->encap_len_ == -1) // not computed yet
    {
      long slen;

      // Macro to avoid "warning: unused parameter" type warning.
      ACE_UNUSED_ARG (slen);

      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += this->repoID_encap_len (); // for repoID

      // do the same thing for the local name
      this->encap_len_ += this->name_encap_len ();

    }
  return this->encap_len_;
}
#endif

// for building the pre and postfix of private data fields
const char*
be_valuetype::field_pd_prefix ()
{
  return "_pd_";
}

const char*
be_valuetype::field_pd_postfix ()
{
  return "";
}

be_valuetype*
be_valuetype::statefull_inherit ()
{
  be_valuetype *rval;
  if (this->n_inherits () > 0)
    {
      rval = be_valuetype::narrow_from_decl (this->inherits ()[0]);
      if (rval && (rval->is_valuetype ()) && (!rval->is_abstract_valuetype ()))
        return rval;
    }
  return 0;
}

// accept a visitor
int
be_valuetype::accept (be_visitor *visitor)
{
  return visitor->visit_valuetype (this);
}

// Narrowing
IMPL_NARROW_METHODS1 (be_valuetype, be_interface)
IMPL_NARROW_FROM_DECL (be_valuetype)
IMPL_NARROW_FROM_SCOPE (be_valuetype)

#endif /* IDL_HAS_VALUETYPE */
