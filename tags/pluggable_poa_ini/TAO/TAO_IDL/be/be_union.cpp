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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_union, "$Id$")


be_union::be_union (void)
{
  // Always the case.
  this->has_constructor (I_TRUE);
}

be_union::be_union (AST_ConcreteType *dt,
                    UTL_ScopedName *n,
                    UTL_StrList *p,
                    idl_bool local,
                    idl_bool abstract)
  : AST_Union (dt, 
               n, 
               p, 
               local, 
               abstract),
    AST_Structure (AST_Decl::NT_union, 
                   n, 
                   p, 
                   local, 
                   abstract),
    AST_Decl (AST_Decl::NT_union, 
              n, 
              p),
    UTL_Scope (AST_Decl::NT_union),
    COMMON_Base (local, 
                 abstract),
    default_index_ (-2)
{
  this->default_value_.computed_ = -2;

  // Always the case.
  this->has_constructor (I_TRUE);
}

// Compute total number of members.
int
be_union::compute_default_index (void)
{
  UTL_ScopeActiveIterator *si = 0;
  AST_Decl *d = 0;
  be_union_branch *bub = 0;
  int i = 0;

  // If default case does not exist, it will have a value of -1 according to
  // the spec.
  this->default_index_ = -1;

  // If there are elements in this scope...
  if (this->nmembers () > 0)
    {
      // Instantiate a scope iterator.
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      -1);

      while (!si->is_done ())
        {
          // Get the next AST decl node.
          d = si->item ();

          if (!d->imported ())
            {
              bub = be_union_branch::narrow_from_decl (d);

              for (unsigned long j = 0;
                   j < bub->label_list_length ();
                   ++j)
                {
                  // Check if we are printing the default case.
                  if (bub->label (j)->label_kind ()
                        == AST_UnionLabel::UL_default)
                    {
                      // Zero based indexing.
                      this->default_index_ = i;
                    }
                }

              // TAO's Typecode class keeps only a member count (not
              // a label count) so this increment has been moved
              // out of the inner loop.
              i++;
            }

          si->next ();
        }

      delete si;
    }

  return 0;
}

// Return the default_index.
int
be_union::default_index (void)
{
  if (this->default_index_ == -2)
    {
      this->compute_default_index ();
    }

  return this->default_index_;
}

// Generate the _var definition for ourself.
int
be_union::gen_var_defn (char *)
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
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ch->indent (); // start with whatever was our current indent level
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
  *ch << namebuf << " &operator= ("
      << this->local_name () << " *);" << nl;

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

  // The return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type.
  if (this->size_type () == be_decl::FIXED)
    {
      *ch << "const " << local_name () << " &in (void) const;" << nl;
      *ch << this->local_name () << " &inout (void);" << nl;
      *ch << this->local_name () << " &out (void);" << nl;
      *ch << this->local_name () << " _retn (void);" << nl;
    }
  else
    {
      *ch << "const " << this->local_name () << " &in (void) const;" << nl;
      *ch << this->local_name () << " &inout (void);" << nl;
      *ch << this->local_name () << " *&out (void);" << nl;
      *ch << this->local_name () << " *_retn (void);" << nl;
    }

  // Generate an additional member function that 
  // returns the underlying pointer.
  *ch << this->local_name () << " *ptr(void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // Generate the private section
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << this->local_name () << " *ptr_;\n";
  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// Implementation of the _var class. All of these get generated in the inline
// file.
int
be_union::gen_var_impl (char *,
                        char *)
{
  TAO_OutStream *ci = 0;
  TAO_NL nl;

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

  // Start with whatever was our current indent level.
  ci->indent ();

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // Default constructor.
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname <<
    " (void) // default constructor" << nl;
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
  *ci << "  ACE_NEW (this->ptr_, ::" << this->name ()
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
      *ci << fname << "::" << lname << " (const ::"
          << this->name () << " &p)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "ACE_NEW (this->ptr_, ::" << this->name ()
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
  *ci << "ACE_INLINE ::" << fname << " &" << nl;
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
  *ci << "ACE_INLINE ::" << fname << " &" << nl;
  *ci << fname << "::operator= (const ::" << fname 
      << " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (this != &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "delete this->ptr_;" << nl;
  *ci << "ACE_NEW_RETURN (this->ptr_, ::" << this->name ()
      << " (*p.ptr_), *this);\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Fixed-size types only.
  if (this->size_type () == be_decl::FIXED)
    {
      ci->indent ();
      *ci << "// fixed-size types only" << nl;
      *ci << "ACE_INLINE ::" << fname << " &" << nl;
      *ci << fname << "::operator= (const ::" << this->name ()
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
  *ci << "ACE_INLINE const ::" << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE ::" << this->name () << " *" << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Other extra methods - 3 cast operator ().
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const ::" << this->name ()
      << " &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator ::" << this->name ()
      << " &() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator ::" << this->name ()
      << " &() const// cast " << nl;
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
      *ci << fname << "::operator ::" << this->name ()
          << " *&() // cast " << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";
    }

  // in, inout, out, _retn, and ptr.
  ci->indent ();
  *ci << "ACE_INLINE const ::" << this->name () << " &" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE ::" << this->name () << " &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return *this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // The out and _retn are handled differently based on our size type.
  ci->indent ();
  if (this->size_type () == be_decl::VARIABLE)
    {
      *ci << "// mapping for variable size " << nl;
      *ci << "ACE_INLINE ::" << this->name () << " *&" << nl;
      *ci << fname << "::out (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "delete this->ptr_;" << nl;
      *ci << "this->ptr_ = 0;" << nl;
      *ci << "return this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE ::" << this->name () << " *" << nl;
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
      *ci << "ACE_INLINE ::" << this->name () << " &" << nl;
      *ci << fname << "::out (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE ::" << this->name () << nl;
      *ci << fname << "::_retn (void)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return *this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";
    }

  // The additional ptr () member function.
  ci->indent ();
  *ci << "ACE_INLINE ::" << this->name () << " *" << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// Generate the _out definition
int
be_union::gen_out_defn (char *)
{
  TAO_OutStream *ch = 0;
  TAO_NL nl;

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

  // Start with whatever was our current indent level.
  ch->indent ();

  *ch << "class " << be_global->stub_export_macro ()
      << " " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor.

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
  *ch << namebuf << " &operator= (" << this->local_name () << " *);" << nl;

  // operator ().
  *ch << "operator " << local_name () << " *&();" << nl;

  // ptr function.
  *ch << this->local_name () << " *&ptr (void);" << nl;

  // operator ->
  *ch << this->local_name () << " *operator-> (void);" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << this->local_name () << " *&ptr_;" << nl;
  *ch << "// assignment from T_var not allowed" << nl;
  *ch << "void operator= (const " << this->local_name () << "_var &);\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_union::gen_out_impl (char *,
                        char *)
{
  TAO_OutStream *ci = 0;
  TAO_NL nl;

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

  // Start with whatever was our current indent level.
  ci->indent ();

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // Constructor from a pointer.
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->local_name ()
      << " *&p)" << nl;
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

  // Assignment operator from _out &.
  ci->indent ();
  *ci << "ACE_INLINE ::" << fname << " &" << nl;
  *ci << fname << "::operator= (const ::" << fname
      << " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = ACE_const_cast (" << lname << "&, p).ptr_;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // Assignment from _var is not allowed by a private declaration.

  // Assignment operator from pointer.
  ci->indent ();
  *ci << "ACE_INLINE ::" << fname << " &" << nl;
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
  *ci << fname << "::operator ::" << this->name ()
      << " *&() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function.
  ci->indent ();
  *ci << "ACE_INLINE ::" << this->name () << " *&" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator ->
  ci->indent ();
  *ci << "ACE_INLINE ::" << this->name () << " *" << nl;
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
be_union::compute_size_type (void)
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

      while (!si->is_done ())
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
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "WARNING (%N:%l) be_union::compute_size_type - "
                          "narrow_from_decl returned 0\n"));
            }

          si->next ();
        }

      delete si;
    }

  return 0;
}

// Are we or the parameter node involved in any recursion?
idl_bool
be_union::in_recursion (be_type *node)
{
  if (node == 0)
    {
      // We are determining the recursive status for ourselves.
      node = this;
    }

  // Proceed if the number of members in our scope is greater than 0.
  if (this->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope.
      UTL_ScopeActiveIterator *si = 0;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      0);
      // Continue until each element is visited.
      while (!si->is_done ())
        {
          be_union_branch *field = 
            be_union_branch::narrow_from_decl (si->item ());

          if (field == 0)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) be_union::")
                                 ACE_TEXT ("in_recursion - ")
                                 ACE_TEXT ("bad field node\n")),
                                0);
            }

          be_type *type = be_type::narrow_from_decl (field->field_type ());

          if (type == 0)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) be_union::")
                                 ACE_TEXT ("in_recursion - ")
                                 ACE_TEXT ("bad field type\n")),
                                0);
            }

          if (type->in_recursion (node))
            {
              delete si;
              return 1;
            }

          si->next ();
        }

      delete si;
    }

  // Not in recursion.
  return 0;
}

// Return the default value.
int
be_union::default_value (be_union::DefaultValue &dv)
{
  if (this->default_value_.computed_ == -2)
    {
      // We need to compute it.
      if (this->compute_default_value () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N:%l) be_union::")
                             ACE_TEXT ("default_value - ")
                             ACE_TEXT ("Error computing ")
                             ACE_TEXT ("default value\n")),
                            -1);
        }
    }

  dv = this->default_value_;
  return 0;
}

// Determine the implicit default value (if any).
int
be_union::compute_default_value (void)
{
  // Check if we really need a default value. This will be true if there is an
  // explicit default case OR if an implicit default exists because not all
  // values of the discriminant type are covered by the cases.

  // Compute the total true "case" labels i.e., exclude the "default" case.
  int total_case_members = 0;

  // Instantiate a scope iterator.
  UTL_ScopeActiveIterator *si = 0;
  ACE_NEW_RETURN (si,
                  UTL_ScopeActiveIterator (this,
                                           UTL_Scope::IK_decls),
                  -1);

  while (!si->is_done ())
    {
      // Get the next AST decl node.
      be_union_branch *ub =
        be_union_branch::narrow_from_decl (si->item ());

      if (ub != 0)
        {
          // If the label is a case label, increment by 1.
          for (unsigned long i = 0;
               i < ub->label_list_length ();
               ++i)
            {
              if (ub->label (i)->label_kind () == AST_UnionLabel::UL_label)
                {
                  total_case_members++;
                }
            }
        }

      si->next ();
    }

  delete si;

  // Check if the total_case_members cover the entire
  // range of values that are permitted by the discriminant type. If they do,
  // then a default value is not necessary. However, if such an explicit
  // default case is provided, it must be flagged off as an error. Our
  // front-end is not able to handle such a case since it is a semantic error
  // and not a syntax error. Such an error is caught here.

  switch (this->udisc_type ())
    {
    case AST_Expression::EV_short:
    case AST_Expression::EV_ushort:
      if (total_case_members == ACE_UINT16_MAX + 1)
        {
          this->default_value_.computed_ = 0;
        }

      break;
    case AST_Expression::EV_long:
    case AST_Expression::EV_ulong:
      if ((unsigned int) total_case_members > ACE_UINT32_MAX)
        {
          this->default_value_.computed_ = 0;
        }

      break;
    case AST_Expression::EV_longlong:
    case AST_Expression::EV_ulonglong:
      // Error for now.
      this->default_value_.computed_ = -1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_union::compute_default_value ")
                         ACE_TEXT ("- unimplemented discriminant type ")
                         ACE_TEXT ("(longlong or ulonglong)\n")),
                        -1);
      ACE_NOTREACHED (break;)
    case AST_Expression::EV_char:
      if (total_case_members == ACE_OCTET_MAX + 1)
        {
          this->default_value_.computed_ = 0;
        }

      break;
    case AST_Expression::EV_wchar:
      if (total_case_members == ACE_WCHAR_MAX + 1)
        {
          this->default_value_.computed_ = 0;
        }

      break;
    case AST_Expression::EV_bool:
      if (total_case_members == 2)
        {
          this->default_value_.computed_ = 0;
        }

      break;
    case AST_Expression::EV_any:
      // Has to be enum.
      {
        be_decl *d = be_decl::narrow_from_decl (this->disc_type ());

        if (d->node_type () == AST_Decl::NT_typedef)
          {
            be_typedef *bt = be_typedef::narrow_from_decl (d);
            d = bt->primitive_base_type ();
          }

        be_enum *en = be_enum::narrow_from_decl (d);

        if (en != 0)
          {
            if (total_case_members == en->member_count ())
              {
                this->default_value_.computed_ = 0;
              }
          }
        else
          {
            // Error.
            this->default_value_.computed_ = -1;
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%N:%l) be_union::")
                               ACE_TEXT ("compute_default_value ")
                               ACE_TEXT ("- disc type not an ENUM\n")),
                              -1);
          }
      }
      break;
    default:
      // Error.
      this->default_value_.computed_ = -1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_union::compute_default_value ")
                         ACE_TEXT ("- Bad discriminant type\n")),
                        -1);
      ACE_NOTREACHED (break;)
    } // End of switch

  // If we have determined that we don't need a default case and even then a
  // default case was provided, flag this off as error.
  if ((this->default_value_.computed_ == 0) 
      && (this->default_index () != -1))
    {
      // Error.
      this->default_value_.computed_ = -1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_union::compute_default_value ")
                         ACE_TEXT ("- default clause is invalid here\n")),
                        -1);
    }

  // Proceed only if necessary.
  switch (this->default_value_.computed_)
    {
    case -1:
      // Error. We should never be here because errors have already been caught
      // above.
      return -1;
    case 0:
      // Nothing more to do.
      return 0;
    default:
      // Proceed further down.
      break;
    }

  // Initialization of the default value data member.
  switch (this->udisc_type ())
    {
    case AST_Expression::EV_short:
      this->default_value_.u.short_val = ACE_INT16_MIN;
      break;
    case AST_Expression::EV_ushort:
      this->default_value_.u.ushort_val = 0;
      break;
    case AST_Expression::EV_long:
      // The +1 is to avert a warning on many compilers.
      this->default_value_.u.long_val = ACE_INT32_MIN + 1;
      break;
    case AST_Expression::EV_ulong:
      this->default_value_.u.ulong_val = 0;
      break;
    case AST_Expression::EV_char:
      this->default_value_.u.char_val = 0;
      break;
    case AST_Expression::EV_wchar:
      this->default_value_.u.wchar_val = 0;
      break;
    case AST_Expression::EV_bool:
      this->default_value_.u.bool_val = 0;
      break;
    case AST_Expression::EV_any:
      this->default_value_.u.enum_val = 0;
      break;
    case AST_Expression::EV_longlong:
    case AST_Expression::EV_ulonglong:
      // Unimplemented.
    default:
      // Error caught earlier.
      break;
    }

  // Proceed until we have found the appropriate default value.
  while (this->default_value_.computed_ == -2)
    {
      // Instantiate a scope iterator.
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this, 
                                               UTL_Scope::IK_decls),
                      -1);

      int break_loop = 0;

      while (!si->is_done () && break_loop == 0)
        {
          // Get the next AST decl node
          be_union_branch *ub = 
            be_union_branch::narrow_from_decl (si->item ());

          if (ub != 0)
            {
              for (unsigned long i = 0;
                   i < ub->label_list_length () && !break_loop;
                   ++i)
                {
                  if (ub->label (i)->label_kind () == AST_UnionLabel::UL_label)
                    {
                      // Not a default.
                      AST_Expression *expr = ub->label (i)->label_val ();

                      if (expr == 0)
                        {
                          // Error.
                          this->default_value_.computed_ = -1;
                          ACE_ERROR_RETURN
                            ((LM_ERROR,
                              ACE_TEXT ("(%N:%l) be_union::")
                              ACE_TEXT ("compute_default_value - ")
                              ACE_TEXT ("Bad case label value\n")),
                             -1);
                        }

                      switch (expr->ev ()->et)
                        {
                          // Check if they match in which case this
                          // cannot be the implicit default value. So
                          // start with a new value and try the whole loop
                          // again because our case labels may not be sorted.
                        case AST_Expression::EV_short:
                          if (this->default_value_.u.short_val
                                == expr->ev ()->u.sval)
                            {
                              this->default_value_.u.short_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_ushort:
                          if (this->default_value_.u.ushort_val
                                == expr->ev ()->u.usval)
                            {
                              this->default_value_.u.ushort_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_long:
                          if (this->default_value_.u.long_val
                                == expr->ev ()->u.lval)
                            {
                              this->default_value_.u.long_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_ulong:
                          if (this->default_value_.u.ulong_val
                                == expr->ev ()->u.ulval)
                            {
                              this->default_value_.u.ulong_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_char:
                          if (this->default_value_.u.char_val
                                == expr->ev ()->u.cval)
                            {
                              this->default_value_.u.char_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_wchar:
                          if (this->default_value_.u.wchar_val
                                == expr->ev ()->u.wcval)
                            {
                              this->default_value_.u.wchar_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_bool:
                          if (this->default_value_.u.bool_val
                                == expr->ev ()->u.bval)
                            {
                              this->default_value_.u.bool_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_any:
                          // this is the case of enums. We maintain
                          // evaluated values which always start with 0
                          if (this->default_value_.u.enum_val
                                == expr->ev ()->u.eval)
                            {
                              this->default_value_.u.enum_val++;
                              break_loop = 1;
                            }

                          break;
                        case AST_Expression::EV_longlong:
                        case AST_Expression::EV_ulonglong:
                          // Unimplemented. right now - flag as error.
                        default:
                          // Error.
                          break;
                        } // End of switch.
                    } // if label_Kind == label
                } // End of for loop going thru all labels.
            } // If valid union branch.

          si->next ();
        } // End of while scope iterator loop.

      delete si;

      // We have not aborted the inner loops which means we have found the
      // default value.
      if (break_loop == 0)
        {
          this->default_value_.computed_ = 1;
        }

    } // End of outer while (default_value.computed == -2).

  return 0;
}

idl_bool
be_union::has_duplicate_case_labels (void)
{
  // Instantiate a scope iterator.
  UTL_ScopeActiveIterator *si = 0;
  ACE_NEW_RETURN (si,
                  UTL_ScopeActiveIterator (this,
                                           UTL_Scope::IK_decls),
                  0);

  while (!si->is_done ())
    {
      AST_Decl *d = si->item ();
      AST_UnionBranch *ub = AST_UnionBranch::narrow_from_decl (d);

      if (ub->label_list_length () > 1)
        {
          delete si;
          return I_TRUE;
        }

      si->next ();
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
