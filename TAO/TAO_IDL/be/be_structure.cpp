// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_structure.cpp
//
// = DESCRIPTION
//    Extension of class AST_Structure that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_structure, "$Id$")

be_structure::be_structure (void)
{
}

be_structure::be_structure (UTL_ScopedName *n,
                            UTL_StrList *p,
                            idl_bool local,
                            idl_bool abstract)
  : AST_Decl (AST_Decl::NT_struct, 
              n, 
              p),
    UTL_Scope (AST_Decl::NT_struct),
    COMMON_Base (local, 
                 abstract)
{
}

// Generate the _var definition for ourself.
int
be_structure::gen_var_defn (char *)
{
  TAO_OutStream *ch = 0;
  TAO_NL nl;
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_var",
                   this->local_name ()->get_string ());

  ch = tao_cg->client_header ();

  // Generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which
  // we account for here.

  // Start with whatever was our current indent level.
  ch->indent ();
  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // Default constructor.
  *ch << namebuf << " (void); // default constructor" << nl;

  // Constructor.
  *ch << namebuf << " (" << this->local_name () << " *);" << nl;

  // Copy constructor.
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;

  // Fixed-size types only.
  if (this->size_type () == be_decl::FIXED)
    {
      *ch << namebuf << " (const " << this->local_name ()
          << " &); // fixed-size types only" << nl;
    }

  // Destructor.
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;

  // Assignment operator from a pointer.
  *ch << namebuf << " &operator= (" << this->local_name () << " *);" << nl;

  // Assignment from _var.
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;

  // Fixed-size types only.
  if (this->size_type () == be_decl::FIXED)
    {
      *ch << namebuf << " &operator= (const " << this->local_name ()
          << " &); // fixed-size types only" << nl;
    }

  // Arrow operator.
  *ch << local_name () << " *operator-> (void);" << nl;
  *ch << "const " << this->local_name ()
      << " *operator-> (void) const;" << nl;
  *ch << nl;

  // Other extra types (cast operators, [] operator, and others).
  *ch << "operator const " << this->local_name () << " &() const;" << nl;
  *ch << "operator " << this->local_name () << " &();" << nl;
  *ch << "operator " << this->local_name () << " &() const;" << nl;

  if (this->size_type () == be_decl::VARIABLE)
    {
      *ch << "operator " << this->local_name ()
          << " *&(); // variable-size types only" << nl;
    }

  *ch << nl;
  *ch << "// in, inout, out, _retn " << nl;

  // The return types of in, out, inout, and _retn are based on the
  // parameter passing rules and the base type.
  if (this->size_type () == be_decl::FIXED)
    {
      *ch << "const " << this->local_name () << " &in (void) const;" << nl;
      *ch << local_name () << " &inout (void);" << nl;
      *ch << local_name () << " &out (void);" << nl;
      *ch << local_name () << " _retn (void);" << nl;
    }
  else
    {
      *ch << "const " << this->local_name () << " &in (void) const;" << nl;
      *ch << local_name () << " &inout (void);" << nl;
      *ch << local_name () << " *&out (void);" << nl;
      *ch << local_name () << " *_retn (void);" << nl;
    }

  // Generate an additional member function
  // that returns the underlying pointer.
  *ch << this->local_name () << " *ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // Generate the private section.
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << this->local_name () << " *ptr_;\n";
  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// Implementation of the _var class, generated in the inline file.
int
be_structure::gen_var_impl (char *,
                            char *)
{
  TAO_OutStream *ci = 0;
  TAO_NL nl;        // end line
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

  // Start with whatever was our current indent level.
  ci->indent ();

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // Default constructor.
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname
      << " (void) // default constructor" << nl;
  *ci << "  " << ": ptr_ (0)" << nl;
  *ci << "{}\n\n";

  // Constructor from a pointer.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->local_name ()
      << " *p)" << nl;
  *ci << "  : ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // Copy constructor.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const ::" << fname
      << " &p) // copy constructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (p.ptr_)" << nl;
  *ci << "  ACE_NEW (this->ptr_, " << "::" << this->name ()
      << " (*p.ptr_));" << nl;
  *ci << "else" << nl;
  *ci << "  this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Fixed-size types only.
  if (this->size_type () == be_decl::FIXED)
    {
      *ci << "// fixed-size types only" << nl;
      *ci << "ACE_INLINE" << nl;
      *ci << fname << "::" << lname << " (const "
          << "::" << this->name () << " &p)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "ACE_NEW (this->ptr_, " << "::" << this->name ()
          << " (p));\n";
      ci->decr_indent ();
      *ci << "}\n\n";
    }

  // Destructor.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Assignment operator from a pointer.
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->local_name ()
      << " *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Assignment operator from _var.
  ci->indent ();
  *ci << "ACE_INLINE ::" << fname << " &" << be_nl
      << fname << "::operator= (const ::" << fname
      << " &p)" << be_nl
      << "{" << be_idt_nl
      << "if (this != &p)" << be_idt_nl
      << "{" << be_idt_nl
      << "if (p.ptr_ == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "delete this->ptr_;" << be_nl
      << "this->ptr_ = 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "else" << be_idt_nl
      << "{" << be_idt_nl
      << this->name () << " *deep_copy =" << be_idt_nl
      << "new " << this->name () << " (*p.ptr_);" 
      << be_uidt_nl << be_nl
      << "if (deep_copy != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << this->name () << " *tmp = deep_copy;" << be_nl
      << "deep_copy = this->ptr_;" << be_nl
      << "this->ptr_ = tmp;" << be_nl
      << "delete deep_copy;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return *this;" << be_uidt_nl
      << "}\n\n";

  // Fixed-size types only.
  if (this->size_type () == be_decl::FIXED)
    {
      ci->indent ();
      *ci << "// fixed-size types only" << nl;
      *ci << "ACE_INLINE " << fname << " &" << nl;
      *ci << fname << "::operator= (const " << "::" << this->name ()
          << " &p)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "if (this->ptr_ != &p)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "delete this->ptr_;" << nl;
      *ci << "ACE_NEW_RETURN (this->ptr_, ::"
          << this->name () << " (p), *this);\n";
      ci->decr_indent ();
      *ci << "}" << nl;
      *ci << "return *this;\n";
      ci->decr_indent ();
      *ci << "}\n\n";
    }

  // Two arrow operators.
  ci->indent ();
  *ci << "ACE_INLINE const " << "::" << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << "::" << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Other extra methods - 3 cast operator ().
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::operator const " << "::" << this->name ()
      << " &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::operator " << "::" << this->name ()
      << " &() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::operator " << "::" << this->name ()
      << " &() const // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Variable-size types only.
  if (this->size_type () == be_decl::VARIABLE)
    {
      ci->indent ();
      *ci << "// variable-size types only" << nl;
      *ci << "ACE_INLINE" << nl;
      *ci << fname << "::operator " << "::" << this->name ()
          << " *&() // cast " << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";
    }

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE const " << "::" << this->name () << " &" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << "::" << this->name () << " &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // The out is handled differently based on our size type.
  ci->indent ();
  if (this->size_type () == be_decl::VARIABLE)
    {
      *ci << "// mapping for variable size " << nl;
      *ci << "ACE_INLINE " << "::" << this->name () << " *&" << nl;
      *ci << fname << "::out (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "delete this->ptr_;" << nl;
      *ci << "this->ptr_ = 0;" << nl;
      *ci << "return this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE " << "::" << this->name () << " *" << nl;
      *ci << fname << "::_retn (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "::" << this->name () << " *tmp = this->ptr_;" << nl;
      *ci << "this->ptr_ = 0;" << nl;
      *ci << "return tmp;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

    }
  else
    {
      *ci << "// mapping for fixed size " << nl;
      *ci << "ACE_INLINE " << "::" << this->name () << " &" << nl;
      *ci << fname << "::out (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE " << "::" << this->name () << nl;
      *ci << fname << "::_retn (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

    }

  // The additional ptr () member function.
  ci->indent ();
  *ci << "ACE_INLINE " << "::" << this->name () << " *" << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// Generate the _out definition.
int
be_structure::gen_out_defn (char *)
{
  TAO_OutStream *ch = 0;
  TAO_NL nl;
  char namebuf [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  ACE_OS::sprintf (namebuf,
                   "%s_out",
                   this->local_name ()->get_string ());

  ch = tao_cg->client_header ();

  // Generate the out definition (always in the client header).

  // Start with whatever was our current indent level.
  ch->indent ();

  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // Constructor from a pointer.
  *ch << namebuf << " (" << this->local_name () << " *&);" << nl;

  // Constructor from a _var &.
  *ch << namebuf << " (" << this->local_name () << "_var &);" << nl;

  // Constructor from a _out &.
  *ch << namebuf << " (const " << namebuf << " &);" << nl;

  // Assignment operator from a _out &.
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;

  // Assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators.

  // Assignment.
  *ch << namebuf << " &operator= ("
      << this->local_name () << " *);" << nl;

  // Operator ().
  *ch << "operator " << this->local_name () << " *&();" << nl;

  // ptr fn
  *ch << this->local_name () << " *&ptr (void);" << nl;

  // operator ->
  *ch << this->local_name () << " *operator-> (void);" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << " *&ptr_;" << nl;
  *ch << "// assignment from T_var not allowed" << nl;
  *ch << "void operator= (const " << this->local_name () << "_var &);\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_structure::gen_out_impl (char *,
                            char *)
{
  TAO_OutStream *ci = 0;
  TAO_NL nl;
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

  // Start with whatever was our current indent level.
  ci->indent ();

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // Constructor from a pointer.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << "::"
      << this->name () << " *&p)" << nl;
  *ci << "  : ptr_ (p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Constructor from _var &.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->local_name ()
      << "_var &p) // constructor from _var" << nl;
  *ci << "  : ptr_ (p.out ())" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "this->ptr_ = 0;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Copy constructor.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const ::" << fname
      << " &p) // copy constructor" << nl;
  *ci << "  : ptr_ (ACE_const_cast (" << lname << "&, p).ptr_)" << nl;
  *ci << "{}\n\n";

  // assignment operator from _out &.
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const ::" << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = ACE_const_cast (" << lname << "&, p).ptr_;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Assignment from _var is not allowed by a private declaration.

  // Assignment operator from pointer.
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->local_name () << " *p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Other extra methods - cast operator ().
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << "::" << this->name ()
      << " *&() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << "::" << this->name () << " *&" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator ->
  ci->indent ();
  *ci << "ACE_INLINE " << "::" << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";


  return 0;
}

// Compute the size type of the node in question.
int
be_structure::compute_size_type (void)
{
  UTL_ScopeActiveIterator *si = 0;
  AST_Decl *d = 0;
  be_decl *bd = 0;

  if (this->nmembers () > 0)
    {
      // If there are elements in this scope,
      // instantiate a scope iterator.
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      -1);

      while (!(si->is_done ()))
        {
          // Get the next AST decl node.
          d = si->item ();
          bd = be_decl::narrow_from_decl (d);
          if (bd != 0)
            {
              // Our sizetype depends on the sizetype of our members. Although
              // previous value of sizetype may get overwritten, we are
              // guaranteed by the "size_type" call that once the value reached
              // be_decl::VARIABLE, nothing else can overwrite it.
              this->size_type (bd->size_type ());

              // While we're iterating, we might as well do this one too.
              this->has_constructor (bd->has_constructor ());
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "WARNING (%N:%l) be_structure::compute_size_type - "
                          "narrow_from_decl returned 0\n"));
            }
          si->next ();
        }

      delete si;
    }

  return 0;
}

void
be_structure::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
  AST_Decl::destroy ();
}

int
be_structure::accept (be_visitor *visitor)
{
  return visitor->visit_structure (this);
}

// Narrowing.
IMPL_NARROW_METHODS3 (be_structure, AST_Structure, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_structure)
IMPL_NARROW_FROM_SCOPE (be_structure)
