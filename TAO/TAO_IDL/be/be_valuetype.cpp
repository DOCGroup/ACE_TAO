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

#include "be_valuetype.h"
#include "be_visitor.h"
#include "be_extern.h"
#include "be_helper.h"
#include "ast_module.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "global_extern.h"
#include "ace/Log_Msg.h"

ACE_RCSID (be,
           be_valuetype,
           "$Id$")

// Default constructor.
be_valuetype::be_valuetype (void)
  : full_obv_skel_name_ (0)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);

  AST_Module *m = AST_Module::narrow_from_scope (this->defined_in ());

  if (m != 0)
    {
      m->set_has_nested_valuetype ();
    }

  // Always the case.
  this->has_constructor (I_TRUE);
}

// Constructor used to build the AST.
be_valuetype::be_valuetype (UTL_ScopedName *n,
                            AST_Interface **inherits,
                            long n_inherits,
                            AST_ValueType *inherits_concrete,
                            AST_Interface **inherits_flat,
                            long n_inherits_flat,
                            AST_Interface **supports,
                            long n_supports,
                            AST_Interface *supports_concrete,
                            idl_bool abstract,
                            idl_bool truncatable)
  : be_interface (n,
                  inherits,
                  n_inherits,
                  inherits_flat,
                  n_inherits_flat,
                  0,
                  abstract),
    AST_ValueType (n,
                   inherits,
                   n_inherits,
                   inherits_concrete,
                   inherits_flat,
                   n_inherits_flat,
                   supports,
                   n_supports,
                   supports_concrete,
                   abstract,
                   truncatable),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   0,
                   abstract),
    AST_Decl (AST_Decl::NT_valuetype,
              n),
    UTL_Scope (AST_Decl::NT_valuetype),
    COMMON_Base (0,
                 abstract),
    full_obv_skel_name_ (0),
    supports_abstract_ (0)
{
  // Check that redefine() copies all members.

  // Always the case.
  this->size_type (AST_Type::VARIABLE);

  AST_Module *m = AST_Module::narrow_from_scope (this->defined_in ());

  if (m != 0)
    {
      m->set_has_nested_valuetype ();
    }

  // Always the case.
  this->has_constructor (I_TRUE);

  for (long i = 0; i < this->pd_n_supports; ++i)
    {
      if (this->pd_supports[i]->is_abstract ())
        {
          this->supports_abstract_ = I_TRUE;
          break;
        }
    }

  // Set the flag that says we have a valuetype in this IDL file.
  if (this->is_defined ())
    {
      ACE_SET_BITS (idl_global->decls_seen_info_,
                    idl_global->decls_seen_masks.valuetype_seen_);
    }
}

be_valuetype::~be_valuetype (void)
{
}

void
be_valuetype::redefine (AST_Interface *from)
{
  this->AST_ValueType::redefine (from);
}

// Is true if non-virtual accessor and modifier should be generated
// If #pragma TAO OBV opt_accessor (todo) is used or -Wb,obv_opt_accessor.
idl_bool
be_valuetype::opt_accessor (void)
{
  return be_global->obv_opt_accessor ();
}

// Compute stringified fully scoped skeleton name (OBV_name).
void
be_valuetype::compute_fullobvskelname (void)
{
  this->compute_full_skel_name ("OBV_",
                                this->full_obv_skel_name_);
}

// Retrieve the fully scoped skeleton name.
const char*
be_valuetype::full_obv_skel_name (void)
{
  if (!this->full_obv_skel_name_)
    {
      compute_fullobvskelname ();
    }

  return this->full_obv_skel_name_;
}

// Generate the var definition.
int
be_valuetype::gen_var_defn (char *local_name)
{
  char namebuf [NAMEBUFSIZE];

  if (local_name == 0)
    {
      local_name = (char *) this->local_name ();
    }

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_var",
                   local_name);

  TAO_OutStream *ch = tao_cg->client_header ();

  *ch << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // Default constructor.
  *ch << namebuf << " (void);" << be_nl;
  *ch << namebuf << " (" << local_name << "*);" << be_nl;
  *ch << namebuf << " (const " << local_name
      << "*); // (TAO extension)" << be_nl;

  // Copy constructor.
  *ch << namebuf << " (const " << namebuf <<
    " &);" << be_nl;

  // Destructor.
  *ch << "~" << namebuf << " (void);" << be_nl;
  *ch << be_nl;

  // Assignment operator from a pointer.
  *ch << namebuf << " &operator= (" << local_name
      << "*);" << be_nl;

  // Assignment from _var.
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << be_nl;

  // Arrow operator.
  *ch << local_name << "* operator-> (void) const;" << be_nl;

  *ch << be_nl;

  // Other extra types (cast operators and others).
  *ch << "operator const " << local_name
      << "* () const;" << be_nl;
  *ch << "operator " << local_name << "*& ();" << be_nl;

  *ch << "// in, inout, out, _retn " << be_nl;
  // The return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type.
  *ch << local_name << "* in (void) const;" << be_nl;
  *ch << local_name << "*& inout (void);" << be_nl;
  *ch << local_name << "*& out (void);" << be_nl;
  *ch << local_name << "* _retn (void);" << be_nl;

  // Generate an additional member function that returns
  // the underlying pointer.
  *ch << local_name << "* ptr (void) const;" << be_nl << be_nl;

  // Hooks for non-defined forward declared interfaces.
  *ch << "// Hooks used by template sequence and valuetype manager classes"
      << be_nl
      << "// for non-defined forward declared valuetypes." << be_nl
      << "static void tao_add_ref (" << local_name << " *);" << be_nl
      << "static void tao_remove_ref (" << local_name << " *);"
      << be_uidt_nl << be_nl;

  // Private.
  *ch << "private:" << be_idt_nl;
  *ch << local_name << "* ptr_;" << be_uidt_nl;

  *ch << "};";

  return 0;
}

// Implementation of the _var class. All of these get generated in the stubs
// file.
int
be_valuetype::gen_var_impl (char *local_name,
                            char *full_name)
{
  TAO_OutStream *cs = 0;
  TAO_NL be_nl;

  // Decide on the names to use.
  // Even if one argument is 0, there is no point using the
  // arguments. Let us then use the name in this node.
  if (local_name == 0 || full_name == 0)
    {
      local_name = (char *) this->local_name ();
      full_name = (char *) this->full_name ();
    }

  // To hold the full and local _var names.
  char fname [NAMEBUFSIZE];
  char lname [NAMEBUFSIZE];

  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (fname,
                   "%s_var",
                   full_name);

  ACE_OS::memset (lname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (lname,
                   "%s_var",
                   local_name);

  cs = tao_cg->client_stubs ();

  // Generate the var implementation in the stubs file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *cs << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *cs << "// *************************************************************"
      << be_nl;
  *cs << "// Operations for class " << fname << be_nl;
  *cs << "// *************************************************************"
      << be_nl << be_nl;

  // Default constructor.
  *cs << fname << "::" << lname <<
    " (void)" << be_nl;
  *cs << "  " << ": ptr_ (0)" << be_nl;
  *cs << "{}" << be_nl << be_nl;

  // Constructor from a pointer.
  *cs << fname << "::" << lname << " ("
      << local_name << "* p)" << be_nl;
  *cs << "  : ptr_ (p)" << be_nl;
  *cs << "{}" << be_nl << be_nl;

  // Constructor from a const pointer.
  // TAO extension - it appears that there are problems with at least g++
  // which reclaims amguity between T(T*) and T(const T_var &)
  *cs << fname << "::" << lname << " (const "
      << local_name << "* p)" << be_nl;
  *cs << "  : ptr_ (ACE_const_cast ("
      << local_name << " *, p))" << be_nl;
  *cs << "{}" << be_nl << be_nl;

  // The additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  *cs << full_name << " *" << be_nl;
  *cs << fname << "::ptr (void) const" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Copy constructor.
  *cs << fname << "::" << lname << " (const " << lname <<
    " &p)" << be_nl;
  *cs << "{" << be_idt_nl
      <<    "CORBA::add_ref (p.ptr ());" << be_nl
      <<    "this->ptr_ = p.ptr ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Destructor.
  *cs << fname << "::~" << lname << " (void) // destructor" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "CORBA::remove_ref (this->ptr_);" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Assignment operator.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (" << local_name
      << " *p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = p;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Assignment operator from _var.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const " << lname
      << " &p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "if (this != &p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl
      << local_name << " *tmp = p.ptr ();" << be_nl
      << "CORBA::add_ref (tmp);" << be_nl
      << "this->ptr_ = tmp;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;
  *cs << "return *this;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Other extra methods - cast operators.
  *cs << fname << "::operator const " << full_name
      << "* () const // cast" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  *cs << fname << "::operator " << full_name
      << "*& () // cast " << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // operator->
  *cs << full_name << " *" << be_nl;
  *cs << fname << "::operator-> (void) const" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // in, inout, out, and _retn.
  *cs << full_name << " *" << be_nl;
  *cs << fname << "::in (void) const" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  *cs << full_name << " *&" << be_nl;
  *cs << fname << "::inout (void)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  *cs << full_name << " *&" << be_nl;
  *cs << fname << "::out (void)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = 0;" << be_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  *cs << full_name << " *" << be_nl;
  *cs << fname << "::_retn (void)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "// yield ownership of managed obj reference" << be_nl;
  *cs << local_name << "* tmp = this->ptr_;" << be_nl;
  *cs << "this->ptr_ = 0;" << be_nl;
  *cs << "return tmp;" << be_uidt_nl;
  *cs << "}" << be_nl << be_nl;

  // Hooks for the flat name global functions used by references to
  // non-defined valuetypes.
  *cs << "void" << be_nl
      << fname << "::tao_add_ref (" << be_idt << be_idt_nl
      << local_name << " *p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::add_ref (p);"
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  *cs << "void" << be_nl
      << fname << "::tao_remove_ref (" << be_idt << be_idt_nl
      << local_name << " *p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::remove_ref (p);" << be_uidt_nl
      << "}";

  return 0;
}

// Generate the _out definition.
int
be_valuetype::gen_out_defn (char *)
{
  TAO_OutStream *ch = 0;
  TAO_NL be_nl;
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_out",
                   this->local_name ());

  ch = tao_cg->client_header ();

  *ch << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the out definition (always in the client header).

  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // No default constructor.

  // Constructor from a pointer.
  *ch << namebuf << " (" << this->local_name () << "* &);" << be_nl;

  // Constructor from a _var &.
  *ch << namebuf << " (" << this->local_name () << "_var &);" << be_nl;

  // Constructor from a _out &.
  *ch << namebuf << " (const " << namebuf << " &);" << be_nl;

  // Assignment operator from a _out &.
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << be_nl;

  // Assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators.
  // Only interface allows assignment from var &.
  *ch << namebuf << " &operator= (const " << this->local_name ()
      << "_var &);" << be_nl;
  *ch << namebuf << " &operator= (" << this->local_name ()
      << "*);" << be_nl;

  // Cast.
  *ch << "operator " << this->local_name () << "*& ();" << be_nl;

  // ptr function.
  *ch << this->local_name () << "*& ptr (void);" << be_nl;

  // operator ->
  *ch << this->local_name () << "* operator-> (void);" << be_nl;

  *ch << be_uidt_nl;
  *ch << "private:" << be_idt_nl;
  *ch << this->local_name () << "* &ptr_;" << be_uidt_nl;
  *ch << "};";

  return 0;
}

int
be_valuetype::gen_out_impl (char *,
                            char *)
{
  TAO_OutStream *cs = 0;
  TAO_NL be_nl;

  // To hold the full and local _out names.
  char fname [NAMEBUFSIZE];
  char lname [NAMEBUFSIZE];

  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (fname,
                   "%s_out",
                   this->full_name ());

  ACE_OS::memset (lname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (lname,
                   "%s_out",
                   this->local_name ());

  cs = tao_cg->client_stubs ();

  *cs << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *cs << "// *************************************************************"
      << be_nl;
  *cs << "// Operations for class " << fname << be_nl;
  *cs << "// *************************************************************"
      << be_nl << be_nl;

  // Constructor from a pointer.
  *cs << fname << "::" << lname << " (" << this->local_name ()
      << " *&p)" << be_nl;
  *cs << "  : ptr_ (p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "this->ptr_ = 0;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Constructor from _var &.
  *cs << fname << "::" << lname << " (" << this->local_name ()
      << "_var &p)" << be_nl;
  *cs << "  : ptr_ (p.out ())" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = 0;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Copy constructor.
  *cs << fname << "::" << lname << " (const " << lname
      << " &p)" << be_nl;
  *cs << "  : ptr_ (ACE_const_cast (" << lname << "&, p).ptr_)" << be_nl;
  *cs << "{}" << be_nl << be_nl;

  // Assignment operator from _out &.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const " << lname <<
    " &p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "this->ptr_ = ACE_const_cast (" << lname << "&, p).ptr_;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Assignment operator from _var.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const " << this->local_name ()
      << "_var &p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << this->local_name () << " *tmp = p.ptr ();" << be_nl
      << "CORBA::add_ref (tmp);" << be_nl
      << "this->ptr_ = tmp;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Assignment operator from *.
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (" << this->local_name ()
      << " *p)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "this->ptr_ = p;" << be_nl;
  *cs << "return *this;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // Other extra methods - cast operator ().
  *cs << fname << "::operator " << this->name ()
      << "*& () // cast" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // ptr function.
  *cs << this->name () << " *&" << be_nl;
  *cs << fname << "::ptr (void) // ptr" << be_nl;
  *cs << "{" << be_idt_nl;
  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  // operator->
  *cs << this->name () << " *" << be_nl;
  *cs << fname << "::operator-> (void)" << be_nl;
  *cs << "{" << be_idt_nl;

  *cs << "return this->ptr_;" << be_uidt_nl;

  *cs << "}" << be_nl << be_nl;

  *cs << "// *************************************************************";

  return 0;
}

int
be_valuetype::gen_helper_header (char*,
                                 char*)
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_header ();

  *os << be_nl << be_nl
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "TAO_NAMESPACE CORBA" << be_nl
      << "{"
      << be_idt_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void add_ref ("
      << this->full_name () << " *);" << be_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void remove_ref ("
      << this->full_name () << " *);";

  *os <<  be_uidt_nl
      << "}" << be_nl
      << "TAO_NAMESPACE_CLOSE";

  return 0;
}

int
be_valuetype::gen_helper_inline (char*,
                                 char*)
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_inline ();

  // There is a problem, here. Looks like the if defined __ACE_INLINE
  // is not getting generated... Actually this is a much bigger
  // problem. Just hacking  it up for the timebeing..

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "#if defined (__ACE_INLINE__)" << be_nl << be_nl
      << "TAO_NAMESPACE CORBA" << be_nl
      << "{"
      << be_idt_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void add_ref ("
      << this->full_name () << " *);" << be_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void remove_ref ("
      << this->full_name () << " *);"
      <<  be_uidt_nl
      << "}" << be_nl
      << "TAO_NAMESPACE_CLOSE" << be_nl << be_nl 
      << "#endif /*__ACE_INLINE__*/";

  return 0;
}


int
be_valuetype::gen_helper_stubs (char* ,
                                char* )
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_stubs ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "void" << be_nl
      << "CORBA::add_ref (" << this->full_name () << " * vt)" << be_nl
      << "{" << be_idt_nl
      << "if (vt != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "vt->_add_ref ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "void" << be_nl
      << "CORBA::remove_ref (" << this->full_name () << " * vt)" << be_nl
      << "{" << be_idt_nl
      << "if (vt != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "vt->_remove_ref ();" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}";

  return 0;
}

// For building the pre and postfix of private data fields.
const char *
be_valuetype::field_pd_prefix (void)
{
  return "_pd_";
}

const char *
be_valuetype::field_pd_postfix (void)
{
  return "";
}

be_valuetype *
be_valuetype::statefull_inherit (void)
{
  if (this->pd_inherits_concrete != 0)
    {
      return be_valuetype::narrow_from_decl (this->pd_inherits_concrete);
    }
  else
    {
      return 0;
    }
}

// Accept a visitor.
int
be_valuetype::accept (be_visitor *visitor)
{
  return visitor->visit_valuetype (this);
}

void
be_valuetype::destroy (void)
{
  this->be_interface::destroy ();
}

ACE_CDR::ULong
be_valuetype::data_members_count (AST_Field::Visibility vis)
{
  ACE_CDR::ULong count = 0;

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::data_members_count - "
                             "bad node in this scope\n"), 0);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      if (vis != AST_Field::vis_NA)
        {
          if (vis == field->visibility ()) ++count;
        }
      else
        {
          ++count;
        }
    } // end of for loop
  return count;
}

idl_bool
be_valuetype::in_recursion (AST_Type *node)
{
  if (node == 0)
    {
      node = this;
    }

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next())
    {
      AST_Decl *d = si.item ();

      if (!d)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::in_recursion - "
                             "bad node in this scope\n"),
                            0);
        }

      AST_Field *field = AST_Field::narrow_from_decl (d);

      if (!field)
        {
          continue;
        }

      AST_Type *type = field->field_type ();

      // A valuetype may contain itself as a member. This will not
      // cause a problem when checking if the valuetype itself is
      // recursive, but if another valuetype contains a recursive
      // one, the string compare below is not sufficient, and we
      // will go into an infinite recursion of calls to in_recursion ;-).
      // The check below will catch that use case.
      if (this == type)
        {
          return 1;
        }

      if (!type)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::in_recursion - "
                             "bad base type\n"),
                            0);
        }

      // IDL doesn't have such a feature as name reuse so
      // just compare fully qualified names.
      if (!ACE_OS::strcmp (node->full_name (),
                           type->full_name ()))
        {
          return 1;
        }

      // Now hand over to our field type.
      if (type->in_recursion (node))
        {
          return 1;
        }

    } // end of for loop

  return 0;
}

idl_bool
be_valuetype::supports_abstract (void) const
{
  return this->supports_abstract_;
}

// Template method that traverses the inheritance graph in a breadth-first
// style. The actual work on each element in the inheritance graph is carried
// out by the function passed as argument.
int
be_valuetype::traverse_supports_list_graphs (
    be_interface::tao_code_emitter gen,
    TAO_OutStream *os,
    idl_bool abstract_paths_only
  )
{
  long n_supports = this->n_supports ();

  if (n_supports == 0)
    {
      return 0;
    }

  // Make sure the queues are empty.
  this->insert_queue.reset ();
  this->del_queue.reset ();

  be_interface *supported_interface = 0;

  for (long i = 0; i < n_supports; ++i)
    {
      if (abstract_paths_only
          && ! this->pd_supports[i]->is_abstract ())
        {
          continue;
        }

      supported_interface =
        be_interface::narrow_from_decl (this->pd_supports[i]);

      // Insert a supported interface in the queue.
      if (this->insert_queue.enqueue_tail (supported_interface) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_valuetype::"
                             "traverse_supports_graph_list - "
                             "error generating entries\n"),
                            -1);
        }
    }

  be_code_emitter_wrapper wrapper (gen);

  return this->traverse_inheritance_graph (wrapper,
                                           os,
                                           abstract_paths_only);
}

int
be_valuetype::traverse_concrete_inheritance_graph (tao_code_emitter gen,
                                                   TAO_OutStream *os)
{
  AST_Interface *supported = this->supports_concrete ();

  if (supported == 0)
    {
      return 0;
    }

  be_interface *concrete = be_interface::narrow_from_decl (supported);

  // Make sure the queues are empty.
  this->insert_queue.reset ();
  this->del_queue.reset ();

  // Insert the supported concrete interface in the queue.
  if (this->insert_queue.enqueue_tail (concrete) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_valuetype::"
                         "traverse_concrete_inheritance_graph - "
                         "error generating entries\n"),
                        -1);
    }

  be_code_emitter_wrapper wrapper (gen);

  return this->traverse_inheritance_graph (wrapper,
                                           os);
}

int
be_valuetype::abstract_supports_helper (be_interface *,
                                        be_interface *base,
                                        TAO_OutStream *os)
{
  *os << "," << be_nl
      << "public virtual "
      << base->name ();
/*
  if (base->is_nested ())
    {
      UTL_Scope *parent_scope = base->defined_in ();
      AST_Decl *parent_decl = ScopeAsDecl (parent_scope);

      *os << "ACE_NESTED_CLASS ("
          << parent_decl->name () << ", "
          << base->local_name () << ")";
    }
  else
    {
      *os << base->local_name ();
    }
*/
  return 0;
}

int
be_valuetype::gen_abstract_init_helper (be_interface *,
                                        be_interface *,
                                        TAO_OutStream *)
{
  return 0;
}

int
be_valuetype::gen_skel_helper (be_interface *concrete,
                               be_interface *ancestor,
                               TAO_OutStream *os)
{
  // If derived and ancestor are same, skip it.
  if (concrete == ancestor)
    {
      return 0;
    }

  // If an operation or an attribute is abstract (declared in an
  // abstract interface), we will either generate the full
  // definition (if there are no concrete interfaces between the
  // abstract ancestor and us) or, if there is a concrete ancestor
  // in between, we will catch its definition elsewhere in this
  // traversal.
  if (ancestor->is_abstract ())
    {
      return 0;
    }

  // Else generate code that does the cast to the appropriate type.

  if (ancestor->nmembers () > 0)
    {
      // If there are elements in ancestor scope i.e., any operations and
      // attributes defined by "ancestor", become methods on the derived class
      // which call the corresponding method of the base class by doing the
      // proper casting.

      for (UTL_ScopeActiveIterator si (ancestor, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // Get the next AST decl node
          AST_Decl *d = si.item ();

          if (d->node_type () == AST_Decl::NT_op)
            {
              // Start from current indentation level.
              os->indent ();

              *os << "// TAO_IDL - Generated from" << be_nl
                  << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // Generate the static method corresponding to this method.
                  *os << "static void " << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req, " << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
                      << ");" << be_uidt << "\n\n";
                }
              else
                { // Generate code in the inline file.
                  // Generate the static method corresponding to this method.
                  *os << "ACE_INLINE void "
                      << concrete->full_skel_name () << "::"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "TAO_ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context" << be_nl
                      << "ACE_ENV_ARG_DECL" << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl;
                  *os << ancestor->full_skel_name ()
                      << "_ptr impl = ("
                      << concrete->full_skel_name ()
                      << "_ptr) obj;" << be_nl;
                  *os << ancestor->full_skel_name ()
                      << "::" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "(" << ancestor->full_skel_name ()
                      << "_ptr) impl," << be_nl
                      << "context" << be_nl
                      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
                      << ");" << be_uidt << be_uidt_nl
                      << "}\n";
                }
            }
        } // End of scope iterator.
    }

  return 0;
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_valuetype, be_interface, AST_ValueType)
IMPL_NARROW_FROM_DECL (be_valuetype)
IMPL_NARROW_FROM_SCOPE (be_valuetype)
