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

  // Set the base (AST_Interface) class member.
  this->set_valuetype ();
}

// Constructor used to build the AST.
be_valuetype::be_valuetype (UTL_ScopedName *n,
                            AST_Interface **ih,
                            long nih,
                            idl_bool set_abstract)
  : be_interface (n,
                  ih,
                  nih,
                  0,
                  0,
                  0,
                  set_abstract),
    AST_Interface (n,
                   ih,
                   nih,
                   0,
                   0,
                   0,
                   set_abstract),
    AST_Decl (AST_Decl::NT_interface,  // It's like an interface.
              n),
    UTL_Scope (AST_Decl::NT_interface),
    COMMON_Base (0,
                 set_abstract),
    full_obv_skel_name_ (0)
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

  // Set the base (AST_Interface) class member.
  this->set_valuetype ();
}

be_valuetype::~be_valuetype (void)
{

}

void
be_valuetype::redefine (AST_Interface *from)
{
  this->AST_Interface::redefine (from);

  this->is_abstract_ = from->is_abstract_valuetype ();
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
be_valuetype::gen_var_defn (char *)
{
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_var",
                   this->local_name ());

  TAO_OutStream *ch = tao_cg->client_header ();

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  // Start with whatever was our current indent level.
  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // Default constructor.
  *ch << namebuf << " (void); // default constructor" << be_nl;
  *ch << namebuf << " (" << this->local_name () << "*);" << be_nl;
  *ch << namebuf << " (const " << this->local_name ()
      << "*); // (TAO extension)" << be_nl;

  // Copy constructor.
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << be_nl;

  // Destructor.
  *ch << "~" << namebuf << " (void); // destructor" << be_nl;
  *ch << be_nl;

  // Assignment operator from a pointer.
  *ch << namebuf << " &operator= (" << this->local_name ()
      << "*);" << be_nl;

  // Assignment from _var.
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << be_nl;

  // Arrow operator.
  *ch << local_name () << "* operator-> (void) const;" << be_nl;

  *ch << be_nl;

  // Other extra types (cast operators, [] operator, and others).
  *ch << "operator const " << this->local_name ()
      << "* () const;" << be_nl;
  *ch << "operator " << this->local_name () << "* ();" << be_nl;

  *ch << "// in, inout, out, _retn " << be_nl;
  // The return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type.
  *ch << this->local_name () << "* in (void) const;" << be_nl;
  *ch << this->local_name () << "* &inout (void);" << be_nl;
  *ch << this->local_name () << "* &out (void);" << be_nl;
  *ch << this->local_name () << "* _retn (void);" << be_nl;

  // Generate an additional member function that returns
  // the underlying pointer.
  *ch << this->local_name () << "* ptr (void) const;";

  *ch << be_uidt_nl << be_nl;

  // Private.
  *ch << "private:" << be_idt_nl;
  *ch << this->local_name () << "* ptr_;" << be_uidt_nl;

  *ch << "};" << be_nl << be_nl;

  return 0;
}

// Implementation of the _var class. All of these get generated in the stubs
// file.
int
be_valuetype::gen_var_impl (char *,
                            char *)
{
  TAO_OutStream *cs = 0;
  TAO_NL be_nl;

  // To hold the full and local _var names.
  char fname [NAMEBUFSIZE];
  char lname [NAMEBUFSIZE];

  ACE_OS::memset (fname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (fname,
                   "%s_var",
                   this->full_name ());

  ACE_OS::memset (lname,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (lname,
                   "%s_var",
                   this->local_name ());

  cs = tao_cg->client_stubs ();

  // Generate the var implementation in the stubs file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  cs->indent (); // start with whatever was our current indent level

  *cs << "// *************************************************************"
      << be_nl;
  *cs << "// Operations for class " << fname << be_nl;
  *cs << "// *************************************************************\n\n";

  // Default constructor.
  *cs << fname << "::" << lname <<
    " (void) // default constructor" << be_nl;
  *cs << "  " << ": ptr_ (0)" << be_nl;
  *cs << "{}\n\n";

  // Constructor from a pointer.
  cs->indent ();
  *cs << fname << "::" << lname << " ("
      << this->local_name () << "* p)" << be_nl;
  *cs << "  : ptr_ (p)" << be_nl;
  *cs << "{}\n\n";

  // Constructor from a const pointer.
  // TAO extension - it appears that there are problems with at least g++
  // which reclaims amguity between T(T*) and T(const T_var &)
  cs->indent ();
  *cs << fname << "::" << lname << " (const "
      << this->local_name () << "* p)" << be_nl;
  *cs << "  : ptr_ (ACE_const_cast("
      << this->local_name () << "*, p))" << be_nl;
  *cs << "{}\n\n";

  // The additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  cs->indent ();
  *cs << this->name () << "* " << be_nl;
  *cs << fname << "::ptr (void) const" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Copy constructor.
  cs->indent ();
  *cs << fname << "::" << lname << " (const " << lname <<
    " &p) // copy constructor" << be_nl;
  *cs << "{" << be_idt_nl
      <<    "CORBA::add_ref (p.ptr ());" << be_nl
      <<    "this->ptr_ = p.ptr ();" << be_uidt_nl
      << "}\n\n";

  // Destructor.
  cs->indent ();
  *cs << fname << "::~" << lname << " (void) // destructor" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::remove_ref (this->ptr_);\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Assignment operator.
  cs->indent ();
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (" << this->local_name ()
      << "* p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = p;" << be_nl;
  *cs << "return *this;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Assignment operator from _var.
  cs->indent ();
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const " << lname
      << " &p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "if (this != &p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl
      << this->local_name() << "* tmp = p.ptr ();" << be_nl
      << "CORBA::add_ref (tmp);" << be_nl
      << "this->ptr_ = tmp;\n";
  cs->decr_indent ();
  *cs << "}" << be_nl;
  *cs << "return *this;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Other extra methods - cast operator ().
  cs->indent ();
  *cs << fname << "::operator const " << this->name ()
      << "* () const // cast" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  cs->indent ();
  *cs << fname << "::operator " << this->name ()
      << "* () // cast " << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // operator->
  cs->indent ();
  *cs << this->name () << "* " << be_nl;
  *cs << fname << "::operator-> (void) const" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // in, inout, out, and _retn.
  cs->indent ();
  *cs << this->name () << "*" << be_nl;
  *cs << fname << "::in (void) const" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  cs->indent ();
  *cs << this->name () << "* &" << be_nl;
  *cs << fname << "::inout (void)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  cs->indent ();
  *cs << this->name () << "* &" << be_nl;
  *cs << fname << "::out (void)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = 0;" << be_nl;
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  cs->indent ();
  *cs << this->name () << "* " << be_nl;
  *cs << fname << "::_retn (void)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "// yield ownership of managed obj reference" << be_nl;
  *cs << this->local_name () << "* tmp = this->ptr_;" << be_nl;
  *cs << "this->ptr_ = 0;" << be_nl;
  *cs << "return tmp;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

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
  *ch << "operator " << this->local_name () << "* &();" << be_nl;

  // ptr function.
  *ch << this->local_name () << "* &ptr (void);" << be_nl;

  // operator ->
  *ch << this->local_name () << "* operator-> (void);" << be_nl;

  *ch << be_uidt_nl;
  *ch << "private:" << be_idt_nl;
  *ch << this->local_name () << "* &ptr_;" << be_uidt_nl;
  *ch << "};" << be_nl << be_nl;

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

  // Generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  // Start with whatever was our current indent level.
  cs->indent ();

  *cs << "// *************************************************************"
      << be_nl;
  *cs << "// Operations for class " << fname << be_nl;
  *cs << "// *************************************************************\n\n";

  // Constructor from a pointer.
  cs->indent ();
  *cs << fname << "::" << lname << " (" << this->local_name ()
      << "* &p)" << be_nl;
  *cs << "  : ptr_ (p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "this->ptr_ = 0;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Constructor from _var &.
  cs->indent ();
  *cs << fname << "::" << lname << " (" << this->local_name ()
      << "_var &p) // constructor from _var" << be_nl;
  *cs << "  : ptr_ (p.out ())" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::remove_ref (this->ptr_);" << be_nl;
  *cs << "this->ptr_ = 0;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Copy constructor.
  cs->indent ();
  *cs << fname << "::" << lname << " (const " << lname
      << " &p) // copy constructor" << be_nl;
  *cs << "  : ptr_ (ACE_const_cast (" << lname << "&,p).ptr_)" << be_nl;
  *cs << "{}\n\n";

  // Assignment operator from _out &.
  cs->indent ();
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const " << lname <<
    " &p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "this->ptr_ = ACE_const_cast (" << lname << "&,p).ptr_;" << be_nl;
  *cs << "return *this;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Assignment operator from _var.
  cs->indent ();
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (const " << this->local_name ()
      << "_var &p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << this->local_name () << "* tmp = p.ptr ();" << be_nl
      << "CORBA::add_ref (tmp);" << be_nl
      << "this->ptr_ = tmp;" << be_nl;
  *cs << "return *this;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Assignment operator from *.
  cs->indent ();
  *cs << fname << " &" << be_nl;
  *cs << fname << "::operator= (" << this->local_name ()
      << "* p)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "this->ptr_ = p;" << be_nl;
  *cs << "return *this;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // Other extra methods - cast operator ().
  cs->indent ();
  *cs << fname << "::operator " << this->name ()
      << "* &() // cast" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // ptr function.
  cs->indent ();
  *cs << this->name () << "* &" << be_nl;
  *cs << fname << "::ptr (void) // ptr" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // operator->
  cs->indent ();
  *cs << this->name () << "* " << be_nl;
  *cs << fname << "::operator-> (void)" << be_nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return this->ptr_;\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  return 0;
}

int
be_valuetype::gen_helper_header (char* ,
                                 char* )
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_header ();

  *os << "//@@ Boris: begin experimental" << be_nl
      << "TAO_NAMESPACE CORBA" << be_nl
      << "{"
      << be_idt_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void add_ref (" << this->full_name () << " *);" << be_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void remove_ref (" << this->full_name () << " *);"
      <<  be_uidt_nl
      << "}" << be_nl
      << "TAO_NAMESPACE_CLOSE" << be_nl
      << "//@@ Boris: end experimental" << be_nl << be_nl;

  return 0;
}

int
be_valuetype::gen_helper_inline (char* ,
                                 char* )
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_inline ();

  // There is a problem, here. Looks like the if defined __ACE_INLINE
  // is not getting generated... Actually this is a much bigger
  // problem. Just hacking  it up for the timebeing..

  *os << "#if defined (__ACE_INLINE__)" << be_nl
      << "//@@ Boris: begin experimental" << be_nl
      << "TAO_NAMESPACE CORBA" << be_nl
      << "{"
      << be_idt_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void add_ref (" << this->full_name () << " *);" << be_nl
      << "TAO_NAMESPACE_STORAGE_CLASS void remove_ref (" << this->full_name () << " *);"
      <<  be_uidt_nl
      << "}" << be_nl
      << "TAO_NAMESPACE_CLOSE" << be_nl
      << "//@@ Boris: end experimental" << be_nl
      << "#endif /*__ACE_INLINE__*/"<< be_nl;

  return 0;
}


int
be_valuetype::gen_helper_stubs (char* ,
                                char* )
{
  TAO_OutStream *os = 0;

  os = tao_cg->client_stubs ();

  *os << "//@@ Boris: begin experimental" << be_nl
    // add_ref
      << "void" << be_nl
      << "CORBA::add_ref (" << this->full_name () << " * vt)" << be_nl
      << "{"
      << be_idt_nl
      << "if (vt != 0)" << be_nl
      << "{"
      << be_idt_nl
      << "vt->_add_ref ();"
      << be_uidt_nl
      << "}"
      << be_uidt_nl
      << "}" << be_nl << be_nl
  // remove_ref
      << "void" << be_nl
      << "CORBA::remove_ref (" << this->full_name () << " * vt)" << be_nl
      << "{"
      << be_idt_nl
      << "if (vt != 0)" << be_nl
      << "{"
      << be_idt_nl
      << "vt->_remove_ref ();"
      << be_uidt_nl
      << "}"
      << be_uidt_nl
      << "}" << be_nl << be_nl
      << "//@@ Boris: end experimental" << be_nl;

  return 0;
}

// For building the pre and postfix of private data fields.
const char*
be_valuetype::field_pd_prefix (void)
{
  return "_pd_";
}

const char*
be_valuetype::field_pd_postfix (void)
{
  return "";
}

be_valuetype*
be_valuetype::statefull_inherit (void)
{
  be_valuetype *rval = 0;

  if (this->n_inherits () > 0)
    {
      rval = be_valuetype::narrow_from_decl (this->inherits ()[0]);

      if (rval && (rval->is_valuetype ())
          && (!rval->is_abstract_valuetype ()))
        {
          return rval;
        }
    }

  return 0;
}

// Accept a visitor.
int
be_valuetype::accept (be_visitor *visitor)
{
  return visitor->visit_valuetype (this);
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


// Narrowing.
IMPL_NARROW_METHODS1 (be_valuetype, be_interface)
IMPL_NARROW_FROM_DECL (be_valuetype)
IMPL_NARROW_FROM_SCOPE (be_valuetype)
