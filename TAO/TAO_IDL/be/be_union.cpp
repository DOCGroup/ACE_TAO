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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_union, "$Id$")


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
  this->default_value_.computed_ = -2;
}

// compute total number of members
int
be_union::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si;  // iterator

  this->member_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ()))
        {
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

  // if default case does not exist, it will have a value of -1 according to
  // the spec
  this->default_index_ = -1;

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
              for (unsigned long j = 0;
                   j < bub->label_list_length ();
                   ++j)
                {
                  // check if we are printing the default case
                  if (bub->label (j)->label_kind () 
                      == AST_UnionLabel::UL_default)
                    this->default_index_ = i; // zero based indexing
                  i++;
                }
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
  *ch << "class " << idl_global->export_macro ()
      << " " << namebuf << nl;
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
  *ci << "  " << ": ptr_ (0)" << nl;
  *ci << "{}\n\n";

  // constr from a pointer
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << " *p)" << nl;
  *ci << "  : ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (p.ptr_)" << nl;
  *ci << "  this->ptr_ = new " << this->name () << "(*p.ptr_);" << nl;
  *ci << "else" << nl;
  *ci << "  this->ptr_ = 0;\n";
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

  *ch << "class " << idl_global->export_macro ()
      << " " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // constructor from a pointer
  *ch << namebuf << " (" << local_name () << " *&);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (const " << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (const " << namebuf << " &);" << nl;
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
  *ci << "delete this->ptr_;" << nl;
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

// compute the size type of the node in question
int
be_union::compute_size_type (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          bd = be_decl::narrow_from_decl (d);
          if (bd != 0)
            {
              // our sizetype depends on the sizetype of our members. Although
              // previous value of sizetype may get overwritten, we are
              // guaranteed by the "size_type" call that once the value reached
              // be_decl::VARIABLE, nothing else can overwrite it.
              this->size_type (bd->size_type ());
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "WARNING (%N:%l) be_structure::compute_size_type - "
                          "narrow_from_decl returned 0\n"));
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// Are we or the parameter node involved in any recursion
idl_bool
be_union::in_recursion (be_type *node)
{
  if (!node)
    {
      // we are determining the recursive status for ourselves
      node = this;
    }

  // proceed if the number of members in our scope is greater than 0
  if (this->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      -1);
      // continue until each element is visited
      while (!si->is_done ())
        {
          be_union_branch *field = be_union_branch::narrow_from_decl (si->item ());
          if (!field)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ASYS_TEXT ("(%N:%l) be_union::")
                                 ASYS_TEXT ("in_recursion - ")
                                 ASYS_TEXT ("bad field node\n")),
                                0);
            }
          be_type *type = be_type::narrow_from_decl (field->field_type ());
          if (!type)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ASYS_TEXT ("(%N:%l) be_union::")
                                 ASYS_TEXT ("in_recursion - ")
                                 ASYS_TEXT ("bad field type\n")),
                                0);
            }
          if (type->in_recursion (node))
            {
              delete si;
              return 1;
            }
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

  // not in recursion
  return 0;
}

// return the default value
int
be_union::default_value (be_union::DefaultValue &dv)
{
  if (this->default_value_.computed_ == -2)
    {
      // we need to compute it
      if (this->compute_default_value () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("(%N:%l) be_union::")
                             ASYS_TEXT ("default_value - ")
                             ASYS_TEXT ("Error computing ")
                             ASYS_TEXT ("default value\n")),
                            -1);
        }
    }
  dv = this->default_value_;
  return 0;
}

// determine the implicit default value (if any)
int
be_union::compute_default_value (void)
{
  // check if we really need a default value. This will be true if there is an
  // explicit default case OR if an implicit default exists because not all
  // values of the discriminant type are covered by the cases.

  // compute the total true "case" labels i.e., exclude the "default" case
  int total_case_members = 0;

  // instantiate a scope iterator.
  UTL_ScopeActiveIterator *si
    = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
  while (!(si->is_done ()))
    {
      // get the next AST decl node
      be_union_branch *ub = be_union_branch::narrow_from_decl (si->item ());
      if (ub)
        {
          // if the label is a case label, increment by 1
          for (unsigned long i = 0;
               i < ub->label_list_length ();
               ++i)
            {
              if (ub->label (i)->label_kind () ==
                  AST_UnionLabel::UL_label)
                total_case_members++;
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
      if (total_case_members == ACE_UINT16_MAX+1)
        this->default_value_.computed_ = 0;
      break;
    case AST_Expression::EV_long:
    case AST_Expression::EV_ulong:
      if (total_case_members == ACE_UINT32_MAX+1)
        this->default_value_.computed_ = 0;
      break;
    case AST_Expression::EV_longlong:
    case AST_Expression::EV_ulonglong:
      // error for now
      this->default_value_.computed_ = -1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_union::compute_default_value ")
                         ASYS_TEXT ("- unimplemented discriminant type ")
                         ASYS_TEXT ("(longlong or ulonglong)\n")),
                        -1);
      break;
    case AST_Expression::EV_char:
      if (total_case_members == ACE_OCTET_MAX+1)
        this->default_value_.computed_ = 0;
      break;
    case AST_Expression::EV_bool:
      if (total_case_members == 2)
        this->default_value_.computed_ = 0;
      break;
    case AST_Expression::EV_any:
      // has to be enum
      {
        be_decl *d = be_decl::narrow_from_decl (this->disc_type ());
        if (d->node_type () == AST_Decl::NT_typedef)
          {
            be_typedef *bt = be_typedef::narrow_from_decl (d);
            d = bt->primitive_base_type ();
          }
        be_enum *en = be_enum::narrow_from_decl (d);
        if (en)
          {
            if (total_case_members == en->member_count ())
              this->default_value_.computed_ = 0;
          }
        else
          {
            // error
            this->default_value_.computed_ = -1;
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("(%N:%l) be_union::")
                               ASYS_TEXT ("compute_default_value ")
                               ASYS_TEXT ("- disc type not an ENUM\n")),
                              -1);
          }
      }
      break;
    default:
      // error
      this->default_value_.computed_ = -1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_union::compute_default_value ")
                         ASYS_TEXT ("- Bad discriminant type\n")),
                        -1);
      break;
    } // end of switch
  
  // if we have determined that we don't need a default case and even then a
  // default case was provided, flag this off as error
  if ((this->default_value_.computed_ == 0) &&
      (this->default_index () != -1))
    {
      // error
      this->default_value_.computed_ = -1;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_union::compute_default_value ")
                         ASYS_TEXT ("- default clause is invalid here\n")),
                        -1);
    }
  
  // proceed only if necessary
  switch (this->default_value_.computed_)
    {
    case -1:
      // error. We should never be here because errors have already been caught
      // above 
      return -1;
    case 0:
      // nothing more to do
      return 0;
    default:
      // proceed further down
      break;
    }

  // initialization of the default value data member
  switch (this->udisc_type ())
    {
    case AST_Expression::EV_short:
      this->default_value_.u.short_val = ACE_INT16_MIN;
      break;
    case AST_Expression::EV_ushort:
      this->default_value_.u.ushort_val = 0;
      break;
    case AST_Expression::EV_long:
      this->default_value_.u.long_val = ACE_INT32_MIN;
      break;
    case AST_Expression::EV_ulong:
      this->default_value_.u.ulong_val = 0;
      break;
    case AST_Expression::EV_char:
      this->default_value_.u.char_val = 0;
      break;
    case AST_Expression::EV_bool:
      this->default_value_.u.bool_val = 0;
      break;
    case AST_Expression::EV_any:
      this->default_value_.u.enum_val = 0;
      break;
    case AST_Expression::EV_longlong:
    case AST_Expression::EV_ulonglong:
      // unimplemented
    default:
      // error caught earlier.
      break;
    } // end of switch

  // proceed until we have found the appropriate default value
  while (this->default_value_.computed_ == -2)
    {
      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.
      
      int break_loop = 0;
      unsigned long enum_member = 0;
      while (!(si->is_done ()) && !break_loop)
        {
          // get the next AST decl node
          be_union_branch *ub = be_union_branch::narrow_from_decl (si->item ());
          if (ub)
            {
              for (unsigned long i = 0;
                   i < ub->label_list_length () && !break_loop;
                   ++i)
                {
                  if (ub->label (i)->label_kind () == AST_UnionLabel::UL_label)
                    {
                      // not a default
                      AST_Expression *expr = ub->label (i)->label_val ();
                      if (!expr)
                        {
                          // error
                          this->default_value_.computed_ = -1;
                          ACE_ERROR_RETURN 
                            ((LM_ERROR,
                              ASYS_TEXT ("(%N:%l) be_union::")
                              ASYS_TEXT ("compute_default_value - ")
                              ASYS_TEXT ("Bad case label value\n")),
                             -1);
                        }
                      
                      switch (expr->ev ()->et)
                        {
                          // check if they match in which case this
                          // cannot be the implicit default value. So
                          // start with a new value and try the whole loop
                          // again because our case labels may not be sorted
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
                          // unimplemented. right now flag as error.
                        default:
                          // error
                          break;
                        } // end of switch
                    } // if label_Kind == label
                } // end of for loop going thru all labels
            } // if valid union branch
          si->next ();
        } // end of while scope iterator loop
      delete si; // free the iterator object
      
      // we have not aborted the inner loops which means we have found the
      // default value
      if (!break_loop)
        this->default_value_.computed_ = 1;
      
    } // end of outer while
  
  return 0;
}

// visitor method

int
be_union::accept (be_visitor *visitor)
{
  return visitor->visit_union (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_union)
IMPL_NARROW_FROM_SCOPE (be_union)
