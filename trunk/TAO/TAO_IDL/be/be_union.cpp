// $Id$

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

#include "be_union.h"
#include "be_visitor.h"
#include "be_codegen.h"
#include "be_helper.h"
#include "be_extern.h"
#include "ast_union_branch.h"
#include "utl_identifier.h"
#include "idl_defines.h"

ACE_RCSID (be, 
           be_union, 
           "$Id$")


be_union::be_union (void)
{
  // Always the case.
  this->has_constructor (I_TRUE);
}

be_union::be_union (AST_ConcreteType *dt,
                    UTL_ScopedName *n,
                    idl_bool local,
                    idl_bool abstract)
  : AST_Union (dt,
               n,
               local,
               abstract),
    AST_Structure (AST_Decl::NT_union,
                   n,
                   local,
                   abstract),
    AST_Decl (AST_Decl::NT_union,
              n),
    UTL_Scope (AST_Decl::NT_union),
    COMMON_Base (local,
                 abstract)
{
  // Always the case.
  this->has_constructor (I_TRUE);
}

// Generate the _var definition for ourself.
int
be_union::gen_var_defn (char *)
{
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_var",
                   this->local_name ()->get_string ());

  TAO_OutStream *ch = tao_cg->client_header ();

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  *ch << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *ch << be_nl << be_nl
      << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // Default constructor.
  *ch << namebuf << " (void);" << be_nl;

  // Constructor.
  *ch << namebuf << " (" << this->local_name () << " *);" << be_nl;

  // Copy constructor.
  *ch << namebuf << " (const " << namebuf
      << " &);" << be_nl;

  // Fixed-size types only.
  if (this->size_type () == AST_Type::FIXED)
    {
      *ch << namebuf << " (const " << this->local_name ()
          << " &); // fixed-size types only" << be_nl;
    }

  // Destructor.
  *ch << "~" << namebuf << " (void);" << be_nl;
  *ch << be_nl;

  // Assignment operator from a pointer.
  *ch << namebuf << " &operator= ("
      << this->local_name () << " *);" << be_nl;

  // Assignment from _var.
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << be_nl;

  // Fixed-size types only.
  if (this->size_type () == AST_Type::FIXED)
    {
      *ch << namebuf << " &operator= (const " << this->local_name ()
          << " &); // fixed-size types only" << be_nl;
    }

  // Arrow operator.
  *ch << local_name () << " *operator-> (void);" << be_nl;
  *ch << "const " << this->local_name ()
      << " *operator-> (void) const;" << be_nl;
  *ch << be_nl;

  // Other extra types (cast operators, [] operator, and others).
  *ch << "operator const " << this->local_name () << " &() const;" << be_nl;
  *ch << "operator " << this->local_name () << " &();" << be_nl;
  *ch << "operator " << this->local_name () << " &() const;" << be_nl;

  if (this->size_type () == AST_Type::VARIABLE)
    {
      *ch << "  // Variable size types only." << be_nl;
      *ch << "operator " << this->local_name ()
          << " *&();" << be_nl;
    }

  *ch << be_nl;
  *ch << "// in, inout, out, _retn " << be_nl;

  // The return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type.
  if (this->size_type () == AST_Type::FIXED)
    {
      *ch << "const " << local_name () << " &in (void) const;" << be_nl;
      *ch << this->local_name () << " &inout (void);" << be_nl;
      *ch << this->local_name () << " &out (void);" << be_nl;
      *ch << this->local_name () << " _retn (void);" << be_nl;
    }
  else
    {
      *ch << "const " << this->local_name () << " &in (void) const;" << be_nl;
      *ch << this->local_name () << " &inout (void);" << be_nl;
      *ch << this->local_name () << " *&out (void);" << be_nl;
      *ch << this->local_name () << " *_retn (void);" << be_nl;
    }

  // Generate an additional member function that
  // returns the underlying pointer.
  *ch << this->local_name () << " *ptr (void) const;"
      << be_uidt_nl << be_nl;

  // Generate the private section
  *ch << "private:" << be_idt_nl;
  *ch << this->local_name () << " *ptr_;" << be_uidt_nl;
  *ch << "};";

  return 0;
}

// Implementation of the _var class. All of these get generated in the inline
// file.
int
be_union::gen_var_impl (char *,
                        char *)
{
  TAO_OutStream *ci = 0;
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
                   this->local_name ()->get_string ());

  ci = tao_cg->client_inline ();

  *ci << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *ci << "// *************************************************************"
      << be_nl;
  *ci << "// Inline operations for class " << fname << be_nl;
  *ci << "// *************************************************************"
      << be_nl << be_nl;

  // Default constructor.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::" << lname <<
    " (void)" << be_nl;
  *ci << "  " << ": ptr_ (0)" << be_nl;
  *ci << "{}" << be_nl << be_nl;

  // Constructor from a pointer.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::" << lname << " (" << this->local_name ()
      << " *p)" << be_nl;
  *ci << "  : ptr_ (p)" << be_nl;
  *ci << "{}" << be_nl << be_nl;

  // Copy constructor.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::" << lname << " (const ::" << fname
      << " &p)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "if (p.ptr_)" << be_idt_nl
      << "{" << be_idt_nl;
  *ci << "ACE_NEW (this->ptr_, ::" << this->name ()
      << " (*p.ptr_));" << be_uidt_nl
      << "}" << be_uidt_nl;
  *ci << "else" << be_idt_nl
      << "{" << be_idt_nl;
  *ci << "this->ptr_ = 0;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Fixed-size types only.
  if (this->size_type () == AST_Type::FIXED)
    {
      *ci << "// Fixed-size types only." << be_nl;
      *ci << "ACE_INLINE" << be_nl;
      *ci << fname << "::" << lname << " (const ::"
          << this->name () << " &p)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "ACE_NEW (this->ptr_, ::" << this->name ()
          << " (p));" << be_uidt_nl;
      *ci << "}\n\n";
    }

  // Destructor.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::~" << lname << " (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "delete this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Assignment operator from a pointer.
  *ci << "ACE_INLINE" << be_nl
      << "::" << fname << " &" << be_nl;
  *ci << fname << "::operator= (" << this->local_name ()
      << " *_tao_union_var)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "delete this->ptr_;" << be_nl;
  *ci << "this->ptr_ = _tao_union_var;" << be_nl;
  *ci << "return *this;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Assignment operator from _var.
  *ci << "ACE_INLINE" << be_nl
      << "::" << fname << " &" << be_nl
      << fname << "::operator= (const ::" << fname
      << " &_tao_union_var)" << be_nl
      << "{" << be_idt_nl
      << "if (this != &_tao_union_var)" << be_idt_nl
      << "{" << be_idt_nl
      << "if (_tao_union_var.ptr_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "delete this->ptr_;" << be_nl
      << "this->ptr_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << this->local_name () << " *deep_copy = 0;" << be_nl
      << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
      << "deep_copy," << be_nl
      << this->local_name () << " (*_tao_union_var.ptr_)," << be_nl
      << "*this" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl
      << "if (deep_copy != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << this->local_name () << " *tmp = deep_copy;" << be_nl
      << "deep_copy = this->ptr_;" << be_nl
      << "this->ptr_ = tmp;" << be_nl
      << "delete deep_copy;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return *this;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Fixed-size types only.
  if (this->size_type () == AST_Type::FIXED)
    {
      *ci << "// Fixed-size types only." << be_nl;
      *ci << "ACE_INLINE" << be_nl
          << "::" << fname << " &" << be_nl;
      *ci << fname << "::operator= (const ::" << this->name ()
          << " &_tao_union_var)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "if (this->ptr_ != &_tao_union_var)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "delete this->ptr_;" << be_nl;
      *ci << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "this->ptr_," << be_nl
          << "::" << this->name () << " (_tao_union_var)," << be_nl
          << "*this" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl;
      *ci << "}" << be_nl;
      *ci << "return *this;" << be_uidt_nl;
      *ci << "}" << be_nl << be_nl;
    }

  // Two arrow operators.
  *ci << "ACE_INLINE" << be_nl
      << "const ::" << this->name () << " *" << be_nl;
  *ci << fname << "::operator-> (void) const" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << "ACE_INLINE" << be_nl
      << "::" << this->name () << " *" << be_nl;
  *ci << fname << "::operator-> (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Other extra methods - 3 cast operator ().
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::operator const ::" << this->name ()
      << " &() const // cast" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return *this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::operator ::" << this->name ()
      << " &() // cast " << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return *this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::operator ::" << this->name ()
      << " &() const// cast " << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return *this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Variable-size types only.
  if (this->size_type () == AST_Type::VARIABLE)
    {
      *ci << "// Variable-size types only." << be_nl;
      *ci << "ACE_INLINE" << be_nl;
      *ci << fname << "::operator ::" << this->name ()
          << " *&() // cast " << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "return this->ptr_;" << be_uidt_nl;
      *ci << "}" << be_nl << be_nl;
    }

  // in, inout, out, _retn, and ptr.
  *ci << "ACE_INLINE" << be_nl
      << "const ::" << this->name () << " &" << be_nl;
  *ci << fname << "::in (void) const" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return *this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  *ci << "ACE_INLINE" << be_nl
      << "::" << this->name () << " &" << be_nl;
  *ci << fname << "::inout (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return *this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // The out and _retn are handled differently based on our size type.
  if (this->size_type () == AST_Type::VARIABLE)
    {
      *ci << "// Mapping for variable size." << be_nl;
      *ci << "ACE_INLINE" << be_nl
          << "::" << this->name () << " *&" << be_nl;
      *ci << fname << "::out (void)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "delete this->ptr_;" << be_nl;
      *ci << "this->ptr_ = 0;" << be_nl;
      *ci << "return this->ptr_;" << be_uidt_nl;
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE" << be_nl
          << "::" << this->name () << " *" << be_nl;
      *ci << fname << "::_retn (void)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "::" << this->name () << " *tmp = this->ptr_;" << be_nl;
      *ci << "this->ptr_ = 0;" << be_nl;
      *ci << "return tmp;" << be_uidt_nl;
      *ci << "}" << be_nl << be_nl;

    }
  else
    {
      *ci << "// Mapping for fixed size. " << be_nl;
      *ci << "ACE_INLINE" << be_nl
          << "::" << this->name () << " &" << be_nl;
      *ci << fname << "::out (void)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "return *this->ptr_;" << be_uidt_nl;
      *ci << "}" << be_nl << be_nl;

      *ci << "ACE_INLINE" << be_nl
          << "::" << this->name () << be_nl;
      *ci << fname << "::_retn (void)" << be_nl;
      *ci << "{" << be_idt_nl;
      *ci << "return *this->ptr_;" << be_uidt_nl;
      *ci << "}" << be_nl << be_nl;
    }

  // The additional ptr () member function.
  *ci << "ACE_INLINE" << be_nl
      << "::" << this->name () << " *" << be_nl;
  *ci << fname << "::ptr (void) const" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}";

  return 0;
}

// Generate the _out definition
int
be_union::gen_out_defn (char *)
{
  TAO_OutStream *ch = 0;
  TAO_NL be_nl;

  // To hold the _out name.
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_out",
                   this->local_name ()->get_string ());

  ch = tao_cg->client_header ();

  // Generate the out definition (always in the client header).

  *ch << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *ch << be_nl << be_nl
      << "class " << be_global->stub_export_macro ()
      << " " << namebuf << be_nl;
  *ch << "{" << be_nl;
  *ch << "public:" << be_idt_nl;

  // No default constructor.

  // Constructor from a pointer.
  *ch << namebuf << " (" << this->local_name () << " *&);" << be_nl;

  // Constructor from a _var &.
  *ch << namebuf << " (" << this->local_name () << "_var &);" << be_nl;

  // Constructor from a _out &.
  *ch << namebuf << " (const " << namebuf << " &);" << be_nl;

  // Assignment operator from a _out &.
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << be_nl;

  // Assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators.

  // Assignment.
  *ch << namebuf << " &operator= (" << this->local_name ()
      << " *);" << be_nl;

  // operator ().
  *ch << "operator " << local_name () << " *&();" << be_nl;

  // ptr function.
  *ch << this->local_name () << " *&ptr (void);" << be_nl;

  // operator ->
  *ch << this->local_name () << " *operator-> (void);" << be_nl;

  *ch << be_uidt_nl;
  *ch << "private:" << be_idt_nl;
  *ch << this->local_name () << " *&ptr_;" << be_nl;
  *ch << "// assignment from T_var not allowed." << be_nl;
  *ch << "void operator= (const " << this->local_name ()
      << "_var &);" << be_uidt_nl;
  *ch << "};";

  return 0;
}

int
be_union::gen_out_impl (char *,
                        char *)
{
  TAO_OutStream *ci = 0;
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
                   this->local_name ()->get_string ());

  ci = tao_cg->client_inline ();

  // Generate the var implementation in the inline file.

  *ci << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *ci << "// *************************************************************"
      << be_nl;
  *ci << "// Inline operations for class " << fname << be_nl;
  *ci << "// *************************************************************"
      << be_nl << be_nl;

  // Constructor from a pointer.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::" << lname << " (" << this->local_name ()
      << " *&p)" << be_nl;
  *ci << "  : ptr_ (p)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_ = 0;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Constructor from _var &.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::" << lname << " (" << this->local_name ()
      << "_var &p)" << be_nl;
  *ci << "  : ptr_ (p.out ())" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "delete this->ptr_;" << be_nl;
  *ci << "this->ptr_ = 0;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Copy constructor.
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::" << lname << " (const ::" << fname
      << " &p)" << be_nl;
  *ci << "  : ptr_ (ACE_const_cast (" << lname << "&, p).ptr_)" << be_nl;
  *ci << "{}" << be_nl << be_nl;

  // Assignment operator from _out &.
  *ci << "ACE_INLINE" << be_nl
      << "::" << fname << " &" << be_nl;
  *ci << fname << "::operator= (const ::" << fname
      << " &p)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_ = ACE_const_cast (" << lname << "&, p).ptr_;" << be_nl;
  *ci << "return *this;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Assignment from _var is not allowed by a private declaration.

  // Assignment operator from pointer.
  *ci << "ACE_INLINE" << be_nl
      << "::" << fname << " &" << be_nl;
  *ci << fname << "::operator= (" << this->local_name () 
      << " *_tao_union_out)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "this->ptr_ = _tao_union_out;" << be_nl;
  *ci << "return *this;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // Other extra methods - cast operator ().
  *ci << "ACE_INLINE" << be_nl;
  *ci << fname << "::operator ::" << this->name ()
      << " *&() // cast" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // ptr function.
  *ci << "ACE_INLINE" << be_nl
      << "::" << this->name () << " *&" << be_nl;
  *ci << fname << "::ptr (void) // ptr" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}" << be_nl << be_nl;

  // operator ->
  *ci << "ACE_INLINE" << be_nl
      << "::" << this->name () << " *" << be_nl;
  *ci << fname << "::operator-> (void)" << be_nl;
  *ci << "{" << be_idt_nl;
  *ci << "return this->ptr_;" << be_uidt_nl;
  *ci << "}";

  return 0;
}

idl_bool
be_union::has_duplicate_case_labels (void)
{
  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_UnionBranch *ub =
        AST_UnionBranch::narrow_from_decl (d);

      if (ub->label_list_length () > 1)
        {
          return I_TRUE;
        }
    }

  return I_FALSE;
}

void
be_union::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
}

// Visitor method.
int
be_union::accept (be_visitor *visitor)
{
  return visitor->visit_union (this);
}

// Narrowing.
IMPL_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_union)
IMPL_NARROW_FROM_SCOPE (be_union)
