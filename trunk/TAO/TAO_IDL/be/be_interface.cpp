// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.cpp
//
// = DESCRIPTION
//    Extension of class AST_Interface that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be_visitor_interface.h"
#include "be.h"

ACE_RCSID(be, be_interface, "$Id$")

/*
 * BE_Interface
 */

// default constructor
be_interface::be_interface (void)
  : full_skel_name_ (0),
    skel_count_ (0),
    full_coll_name_ (0),
    local_coll_name_ (0),
    in_mult_inheritance_ (-1)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// constructor used to build the AST
be_interface::be_interface (UTL_ScopedName *n, AST_Interface **ih, long nih,
                            UTL_StrList *p)
  : AST_Interface (n, ih, nih, p),
    AST_Decl (AST_Decl::NT_interface, n, p),
    UTL_Scope (AST_Decl::NT_interface),
    full_skel_name_ (0),
    skel_count_ (0),
    full_coll_name_ (0),
    local_coll_name_ (0),
    in_mult_inheritance_ (-1)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_interface::~be_interface (void)
{
  if (this->full_skel_name_ != 0)
    {
      delete[] this->full_skel_name_;
      this->full_skel_name_ = 0;
    }
  if (this->full_coll_name_ != 0)
    {
      delete[] this->full_coll_name_;
      this->full_coll_name_ = 0;
    }
  if (this->local_coll_name_ != 0)
    {
      delete[] this->local_coll_name_;
      this->local_coll_name_ = 0;
    }
}

// compute stringified fully qualified collocated class name.
void
be_interface::compute_coll_name (void)
{
  if (this->full_coll_name_ != 0)
    return;

  const char collocated[] = "_tao_collocated_";
  const char poa[] = "POA_";
  // Reserve enough room for the "POA_" prefix, the "_tao_collocated_"
  // prefix and the local name and the (optional) "::"
  int namelen = sizeof (collocated) + sizeof (poa);

  UTL_IdListActiveIterator *i;
  ACE_NEW (i, UTL_IdListActiveIterator (this->name ()));
  while (!i->is_done ())
    {
      // reserve 2 characters for "::".
      namelen += ACE_OS::strlen (i->item ()->get_string ()) + 2;
      i->next ();
    }
  delete i;

  ACE_NEW (this->full_coll_name_,
           char[namelen+1]);
  this->full_coll_name_[0] = 0; // null terminate the string...

  // Iterate again....
  ACE_NEW (i, UTL_IdListActiveIterator (this->name ()));

  // Only the first component get the "POA_" preffix.
  int poa_added = 0;
  while (!i->is_done ())
    {
      const char* item = i->item ()->get_string ();

      // Increase right away, so we can test for the final component
      // in the loop.
      i->next ();

      // We add the POA_ preffix only if the first component is not
      // the global scope...
      if (ACE_OS::strcmp (item, "") != 0)
        {
          if (!i->is_done ())
            {
              // We only add the POA_ preffix if there are more than
              // two components in the name, in other words, if the
              // class is inside some scope.
              if (!poa_added)
                {
                  ACE_OS::strcat (this->full_coll_name_, poa);
                  poa_added = 1;
                }
              ACE_OS::strcat (this->full_coll_name_, item);
              ACE_OS::strcat (this->full_coll_name_, "::");
            }
          else
            {
              ACE_OS::strcat (this->full_coll_name_, collocated);
              ACE_OS::strcat (this->full_coll_name_, item);
            }
        }
    }
  delete i;

  // Compute the local name for the collocated class.
  int localen = sizeof (collocated);
  localen += ACE_OS::strlen (this->local_name ()->get_string ());
  ACE_NEW (this->local_coll_name_, char[localen]);
  ACE_OS::strcpy(this->local_coll_name_, collocated);
  ACE_OS::strcat(this->local_coll_name_,
                 this->local_name ()->get_string ());
}

const char *
be_interface::full_coll_name (void)
{
  if (this->full_coll_name_ == 0)
    this->compute_coll_name ();

  return this->full_coll_name_;
}

const char*
be_interface::local_coll_name (void) const
{
  if (this->local_coll_name_ == 0)
    ACE_const_cast (be_interface*, this)->compute_coll_name ();

  return this->local_coll_name_;
}

// compute stringified fully scoped skel name
void
be_interface::compute_fullskelname (void)
{
  if (full_skel_name_)
    return;
  else
    {
      long namelen;
      UTL_IdListActiveIterator *i;
      long first = I_TRUE;
      long second = I_FALSE;

      // in the first loop compute the total length
      namelen = 4;
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ()))
        {
          if (!first)
            namelen += 2; // for "::"
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          namelen += ACE_OS::strlen (i->item ()->get_string ()); //
          // additional 4 for the POA_ characters
          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                // does not start with a ""
                first = I_FALSE;
              else
                second = I_TRUE;
            }
          i->next ();
        }
      delete i;

      this->full_skel_name_ = new char [namelen+1];
      this->full_skel_name_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;
      ACE_OS::strcat (this->full_skel_name_, "POA_");
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ()))
        {
          if (!first)
            ACE_OS::strcat (this->full_skel_name_, "::");
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          ACE_OS::strcat (this->full_skel_name_, i->item ()->get_string ());
          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                // does not start with a ""
                first = I_FALSE;
              else
                second = I_TRUE;
            }
          i->next ();
        }
      delete i;
    }
  return;
}

// retrieve the fully scoped skeleton name
const char*
be_interface::full_skel_name (void)
{
  if (!this->full_skel_name_)
    compute_fullskelname ();

  return this->full_skel_name_;
}

// Am I in some kind of a multiple inheritance
int be_interface::in_mult_inheritance (void)
{
  if (this->in_mult_inheritance_ == -1)
    {
      // compute once for all
      // determine if we are in some form of a multiple inheritance
      if (this->traverse_inheritance_graph
          (be_interface::in_mult_inheritance_helper, 0) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_interface::in_mult_inheritance "
                             "error determining mult inheritance\n"),
                            -1);
        }
    }

  return this->in_mult_inheritance_;
}

void be_interface::in_mult_inheritance (int mi)
{
  if (this->in_mult_inheritance_ == -1)
    this->in_mult_inheritance_ = mi;
}

//gen copy constructors
void
be_interface::gen_copy_ctors (TAO_OutStream* os)
{
  this->traverse_inheritance_graph(be_interface::gen_copy_ctors_helper,os);
  return;
}

int
be_interface::gen_copy_ctors_helper (be_interface* node, be_interface* base, TAO_OutStream *os)
{
  static int first = 0;
  if(node != base)
    {
      if(first)
        {
          *os << idl_global->impl_class_prefix () << base->flatname () << idl_global->impl_class_suffix () << " (t)"
              << ", " << base->full_skel_name () << " (t)";
          first = 0;
        }
      else
        {
          *os << ", " << idl_global->impl_class_prefix () << base->flatname () << idl_global->impl_class_suffix () << " (t)"
              << ", " << base->full_skel_name () << " (t)";   ;

        }
    }
  else
    {
      *os << ":";
      first = 1;
    }

  return 1;

}

//generate default constructors
void
be_interface::gen_def_ctors (TAO_OutStream* os)
{
  this->traverse_inheritance_graph(be_interface::gen_def_ctors_helper,os);
  return;
}


int
be_interface::gen_def_ctors_helper (be_interface* node, be_interface* base, TAO_OutStream *os)
{

  static int first = 0;
  if(node != base)
    {
      if(first)
        {
          *os << idl_global->impl_class_prefix () << base->flatname () << idl_global->impl_class_suffix () << " ()";
          first = 0;
        }
      else
        {
          *os << ", " << idl_global->impl_class_prefix () << base->flatname () << idl_global->impl_class_suffix () << " ()";

        }
    }
  else
    {
      *os << ":";
      first = 1;
    }

   return 1;

}


// generate the var definition
int
be_interface::gen_var_defn (void)
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
be_interface::gen_var_impl (void)
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
  *ci << "  " << ": ptr_ (" << this->name () << "::_nil ())" << nl;
  *ci << "{}\n\n";

  // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr p)" << nl;
  *ci << "  : ptr_ (p)" << nl;
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
  *ci << "  : ptr_ (" << name () << "::_duplicate (p.ptr ()))" << nl;
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
be_interface::gen_out_defn (void)
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
  *ch << namebuf << " (" << local_name () << "_ptr &);" << nl;
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
be_interface::gen_out_impl (void)
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
  *ci << "  : ptr_ (p)" << nl;
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
  *ci << "  : ptr_ (p.out ())" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();\n";
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

// generate typecode.
// Typecode for interface comprises the enumerated value followed by the
// encapsulation of the parameters

int
be_interface::gen_typecode (void)
{
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
be_interface::gen_encapsulation (void)
{
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
be_interface::tc_size (void)
{
   return 4 + 4 + this->tc_encap_len ();
}

// compute the encapsulation length
long
be_interface::tc_encap_len (void)
{
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

// helper.
int
be_interface::gen_operation_table (void)
{
  TAO_OutStream *ss; // output stream.
  TAO_NL  nl;        // end line.

  // Retrieve the singleton instance of the CodeGen.
  TAO_CodeGen *cg = 0;
  cg = TAO_CODEGEN::instance ();

  // Check out the op_lookup_strategy.
  switch (cg->lookup_strategy ())
    {
    case TAO_CodeGen::TAO_DYNAMIC_HASH:
      // Init the outstream appropriately.
      ss = cg->server_skeletons ();

      // start from current indentation level.
      ss->indent ();

      // Start the table generation.
      *ss << "static const TAO_operation_db_entry " << this->flatname () <<
        "_operations [] = {\n";
      ss->incr_indent (0);

      // Traverse the graph.
      if (this->traverse_inheritance_graph (be_interface::gen_optable_helper, ss) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::gen_operation_table - "
                             "inheritance graph traversal failed\n"), -1);
        }

      // generate the skeleton for the is_a method.
      ss->indent ();
      *ss << "{\"_is_a\", &" << this->full_skel_name () << "::_is_a_skel},\n";
      this->skel_count_++;

      ss->indent ();
      *ss << "{\"_non_existent\", &" << this->full_skel_name () << "::_non_existent_skel}\n";
      this->skel_count_++;

      ss->decr_indent ();
      *ss << "};" << nl << nl;

      // XXXASG - this code should be based on using different strategies for
      // demux - for next release
      *ss << "static const CORBA::Long _tao_" << this->flatname ()
          << "_optable_size = sizeof (ACE_Hash_Map_Entry<const char *,"
          << " TAO_Skeleton>) * (" << (3*this->skel_count_)
          << ");" << be_nl;
      *ss << "static char _tao_" << this->flatname () << "_optable_pool "
          << "[_tao_" << this->flatname () << "_optable_size];" << be_nl;
      *ss << "static ACE_Static_Allocator_Base _tao_" << this->flatname ()
          << "_allocator (_tao_" << this->flatname () << "_optable_pool, "
          << "_tao_" << this->flatname () << "_optable_size);" << be_nl;
      *ss << "static TAO_Dynamic_Hash_OpTable tao_"
          << this->flatname () << "_optable " << "(" << be_idt << be_idt_nl
          << this->flatname () << "_operations," << be_nl
          << this->skel_count_ << "," << be_nl
          << 2*this->skel_count_ << "," << be_nl
          << "&_tao_" << this->flatname () << "_allocator" << be_uidt_nl
          << ");" << be_uidt_nl;

      break;

    case TAO_CodeGen::TAO_LINEAR_SEARCH:
      // For generating linear search also, we are calling GPERF
      // only.
    case TAO_CodeGen::TAO_BINARY_SEARCH:
      // For generating binary search also, we are calling GPERF
      // only.
    case TAO_CodeGen::TAO_PERFECT_HASH:
      // For each interface in the IDL, have a new temp file to
      // collect the input for the gperf program.
      {
        // Temp file name.
        char *temp_file = 0;
        ACE_NEW_RETURN (temp_file,
                        char [ACE_OS::strlen (idl_global->temp_dir ()) +
                             ACE_OS::strlen (this->flatname ()) +
                             ACE_OS::strlen (".gperf") + 1],
                        -1);
        ACE_OS::sprintf (temp_file,
                         "%s%s.gperf",
                         idl_global->temp_dir (),
                         this->flatname ());

        // Save this file name with the codegen singleton.
        cg->gperf_input_filename (temp_file);

        // Make a new outstream to hold the gperf_temp_file for this
        // interface.

        // Retrieve the singleton instance to the outstream factory.
        TAO_OutStream_Factory *factory =
          TAO_OUTSTREAM_FACTORY::instance ();

        // Get a new instance for the temp file.
        ss = factory->make_outstream ();
        if (ss == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss",
                             "::",
                             "visit_interface-",
                             "make_outstream failed\n"),
                            -1);

        // Store the outstream with the codegen singleton.
        cg->gperf_input_stream (ss);

        // Open the temp file.
        if (ss->open (temp_file,
                      TAO_OutStream::TAO_GPERF_INPUT) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_ss",
                             "::",
                             "visit_interface-",
                             "gperf_input.tmp file open failed\n"),
                            -1);

        // Add the gperf input header.
        this->gen_gperf_input_header (ss);

        // Traverse the graph.
        if (this->traverse_inheritance_graph (be_interface::gen_optable_helper, ss) == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::gen_operation_table - "
                             "inheritance graph traversal failed\n"),
                            -1);

        // Generate the skeleton for the is_a method.
        ss->indent ();
        *ss << "_is_a" << ", &"
            << this->full_skel_name ()
            << "::_is_a_skel\n";
        this->skel_count_++;

        ss->indent ();
        *ss << "_non_existent, &"
            << this->full_skel_name ()
            << "::_non_existent_skel\n";
        this->skel_count_++;

        // Input to the gperf is ready. Run gperf and get things
        // done. This method also unlinks the temp file that we used
        // for the gperf.
        this->gen_gperf_things ();
      }
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_interface",
                         "::",
                         "gen_operation_table",
                         "unknown op_lookup_strategy\n"),
                        -1);
    }
  return 0;
}

// Output the header (type declaration and %%) to the gperf's input
// file.
void
be_interface::gen_gperf_input_header (TAO_OutStream *ss)
{
  *ss << "class TAO_operation_db_entry {\n"
      << "public:\n"
      << "\tchar *opname_;" << "\n"
      << "\tTAO_Skeleton skel_ptr_;" << "\n"
      << "};" << "\n"
      << "%%"
      << "\n";
}

// we separate the generation of operation table entries from the
// "gen_operation_table" method. This enables us to invoke generation of
// entries for interfaces from which we inherit without any additional
// code. The parameter "derived" is the one for which the entire operation
// table is being built.
int
be_interface::gen_optable_entries (be_interface *derived)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  TAO_OutStream *ss; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  switch (cg->lookup_strategy ())
    {
    case TAO_CodeGen::TAO_DYNAMIC_HASH:
      // Init the outstream.
      ss = cg->server_skeletons ();

      // The major stuff.
      if (this->nmembers () > 0)
        {
          // if there are elements in this scope i.e., any operations and
          // attributes defined by "this" which happens to be the same as "derived"
          // or one of its ancestors.

          si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
          // instantiate a scope iterator.

          while (!(si->is_done ()))
            {
              // get the next AST decl node
              d = si->item ();
              if (d->node_type () == AST_Decl::NT_op)
                {
                  ss->indent (); // start from current indentation level
                  // we are an operation node
                  *ss << "{\"" << d->local_name () << "\", &"
                      << derived->full_skel_name () << "::"
                      << d->local_name () << "_skel},\n";
                  derived->skel_count_++;
                }
              else if (d->node_type () == AST_Decl::NT_attr)
                {
                  AST_Attribute *attr;

                  ss->indent (); // start from current indentation level
                  // generate only the "get" entry if we are readonly
                  *ss << "{\"_get_" << d->local_name () << "\", &" <<
                    derived->full_skel_name () << "::_get_" << d->local_name () <<
                    "_skel},\n";
                  derived->skel_count_++;

                  attr = AST_Attribute::narrow_from_decl (d);
                  if (!attr)
                    return -1;

                  if (!attr->readonly ())
                    {
                      // the set method
                      ss->indent (); // start from current indentation level
                      *ss << "{\"_set_" << d->local_name () << "\", &" <<
                        derived->full_skel_name () << "::_set_" << d->local_name
                        () << "_skel},\n";
                      derived->skel_count_++;
                    }
                }
              si->next ();
            } // end of while
          delete si; // free the iterator object
        }
      break;

    case TAO_CodeGen::TAO_LINEAR_SEARCH:
    case TAO_CodeGen::TAO_BINARY_SEARCH:
    case TAO_CodeGen::TAO_PERFECT_HASH:
      // We call GPERF for all these three strategies.
      // Init the outstream.
      ss = cg->gperf_input_stream ();

      if (this->nmembers () > 0)
        {
          // if there are elements in this scope i.e., any operations and
          // attributes defined by "this" which happens to be the same as "derived"
          // or one of its ancestors.

          si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
          // instantiate a scope iterator.

          while (!(si->is_done ()))
            {
              // Get the next AST decl node.
              d = si->item ();
              if (d->node_type () == AST_Decl::NT_op)
                {
                  ss->indent (); // start from current indentation level
                  // we are an operation node
                  *ss << d->local_name () << ",\t&"
                      << derived->full_skel_name () << "::"
                      << d->local_name () << "_skel" << "\n";
                  derived->skel_count_++;
                }
              else if (d->node_type () == AST_Decl::NT_attr)
                {
                  AST_Attribute *attr;

                  ss->indent (); // start from current indentation level
                  // generate only the "get" entry if we are readonly
                  *ss << "_get_" << d->local_name () << ",\t&"
                      << derived->full_skel_name () << "::_get_"
                      << d->local_name () << "_skel\n";
                  derived->skel_count_++;

                  attr = AST_Attribute::narrow_from_decl (d);
                  if (!attr)
                    return -1;

                  if (!attr->readonly ())
                    {
                      // the set method
                      ss->indent (); // start from current indentation level
                      *ss << "_set_" << d->local_name () << ",\t&"
                          << derived->full_skel_name () << "::_set_"
                          << d->local_name () << "_skel\n";
                      derived->skel_count_++;
                    }
                }
              si->next ();
            } // end of while
          delete si; // free the iterator object
        }
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_interface",
                         "::",
                         "gen_optable_entries",
                         "unknown op_lookup_strategy\n"),
                        -1);
    }
  return 0;
}

// template method that traverses the inheritance graph in a breadth-first
// style. The actual work on each element in the inheritance graph is carried
// out by the function passed as argument
int
be_interface::traverse_inheritance_graph (be_interface::tao_code_emitter gen,
                                              TAO_OutStream *os)
{
  long i;            // loop index
  ACE_Unbounded_Queue <be_interface*> queue; // Queue data structure needed for
                                            // breadth-first traversal of
                                            // inheritance tree

  // For a special case of a deeply nested inheritance graph and one specific
  // way of inheritance in which a node that was already visited, but is not present in
  // the queue, gets inserted at the tail. This situation arises when a node
  // multiply inherits from two or more interfaces in which the first parent is
  // higher up in the tree than the second parent. In addition, if the second
  // parent turns out to be a child of the first .

  ACE_Unbounded_Queue <be_interface*> del_queue; // queue of dequeued nodes to
                                                 // be searched for the above case

  // insert ourselves in the Queue
  if (queue.enqueue_tail (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "(%N:%l) be_interface::gen_operation_table - "
                         "error generating entries\n"), -1);
    }

  // do until queue is empty
  while (!queue.is_empty ())
    {
      be_interface *bi;  // element inside the queue

      // use breadth-first strategy i.e., first generate entries for ourselves,
      // followed by nodes that we immediately inherit from, and so on. In the
      // process make sure that we do not generate code for the same node more
      // than once. Such a case may arise due to multiple inheritance forming a
      // diamond like inheritance graph.

      // dequeue the element at the head of the queue
      if (queue.dequeue_head (bi))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "dequeue_head failed\n"), -1);
        }

      // insert the dequeued element in the del_queue
      if (del_queue.enqueue_tail (bi) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "enqueue_head failed\n"), -1);
        }

      // use the helper method to generate code for ourself using the
      // properties of the element dequeued. For the first iteration, the
      // element dequeued and "this" will be the same i.e., ourselves
      if (gen (this, bi, os) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "helper code gen failed\n"), -1);
        }

      // now check if the dequeued element has any ancestors. If yes, insert
      // them inside the queue making sure that there are no duplicates
      for (i=0; i < bi->n_inherits (); i++)
        {
          be_interface *parent;  // parent of the dequeued element

          // initialize an iterator to search the queue for duplicates
          ACE_Unbounded_Queue_Iterator<be_interface*> q_iter (queue);

          // retrieve the next parent from which the dequeued element inherits
          parent = be_interface::narrow_from_decl (bi->inherits ()[i]);
          if (!parent)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_interface::gen_server_skeletons - "
                                 "bad inherited interface\n"), -1);
            }

          // now insert this node at the tail of the queue, but make sure that
          // it doesn't already exist in the queue
          int found = 0;
          while (!q_iter.done ())
            {
              be_interface **temp;  // queue element

              (void) q_iter.next (temp);
              if (!ACE_OS::strcmp (parent->fullname (), (*temp)->fullname ()))
                {
                  // we exist in this queue and cannot be inserted
                  found = 1;
                }
              if (found)
                break;
              (void) q_iter.advance ();
            } // end of while

          // initialize an iterator to search the del_queue for duplicates
          ACE_Unbounded_Queue_Iterator<be_interface*> del_q_iter (del_queue);

          while (!found && !del_q_iter.done ())
            {
              be_interface **temp;  // queue element

              (void) del_q_iter.next (temp);
              if (!ACE_OS::strcmp (parent->fullname (), (*temp)->fullname ()))
                {
                  // we exist in this del_queue and cannot be inserted
                  found = 1;
                }
              if (found)
                break;
              (void) del_q_iter.advance ();
            } // end of while

          if (!found)
            {
              // insert the parent in the queue
              if (queue.enqueue_tail (parent) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_interface::gen_server_skeletons - "
                                 "enqueue op failed\n"), -1);
                }
            }
        } // end of for loop
    } // end of while queue not empty
  return 0;
}

// helpers passed to the template method

int
be_interface::gen_optable_helper (be_interface *derived,
                                  be_interface *ancestor,
                                  TAO_OutStream * /*os*/)
{
  // generate entries for the derived class using the properties of its
  // ancestors
  if (ancestor->gen_optable_entries (derived) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_operation_table - "
                         "error generating entries for inherited"
                         "interfaces\n"), -1);
    }
  return 0;
}

// Run GPERF and get the correct lookup and other operations
// depending on which strategy we are using. Returns 0 on sucess, -1
// on error.
int
be_interface::gen_gperf_things (void)
{
  // GPERF can give Binary search, Linear search and Perfect Hash
  // methods. Generate the class defintion according to that.

  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Generate the correct class definition for the operation lookup
  // strategy. Then, get the lookup method from GPERF. And then,
  // instantiate the correct class for the operation lookup strategy
  // we are following.
  switch (cg->lookup_strategy ())
    {
    case TAO_CodeGen::TAO_PERFECT_HASH:
      // Output a class definition deriving from
      // TAO_Perfect_Hash_OpTable.
      gen_perfect_hash_class_definition ();

      // Call GPERF and get the methods defined.
      if (gen_gperf_lookup_methods () == -1)
        return -1;

      // Create an instance of the correct class corresponding the
      // operation lookup strategy we are following.
      gen_perfect_hash_instance ();

      break;

    case TAO_CodeGen::TAO_BINARY_SEARCH:
      // Output a class definition deriving from
      // TAO_Binary_Search_OpTable.
      this->gen_binary_search_class_definition ();

      // Call GPERF and get the methods defined.
      if (gen_gperf_lookup_methods () == -1)
        return -1;

      // Create an instance of the correct class corresponding the
      // operation lookup strategy we are following.
      gen_binary_search_instance ();

      break;

    case TAO_CodeGen::TAO_LINEAR_SEARCH:
      // Output a class definition deriving from
      // TAO_Linear_Search_OpTable.
      gen_linear_search_class_definition ();

      // Call GPERF and get the methods defined.
      if (gen_gperf_lookup_methods () == -1)
        return -1;

      // Create an instance of the correct class corresponding the
      // operation lookup strategy we are following.
      gen_linear_search_instance ();

      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "tao_idl:ERROR:%N:%l:Unknown Operation Lookup Strategy\n"),
                        -1);
    }

  return 0;
}


// Outputs the class definition for the perfect hashing. This class
// will inherit from the TAO_Perfect_Hash_OpTable.
void
be_interface::gen_perfect_hash_class_definition (void)
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  TAO_OutStream *ss = cg->server_skeletons ();

  *ss << "class " << "TAO_" << this->flatname () << "_Perfect_Hash_OpTable"
      << " : public TAO_Perfect_Hash_OpTable"
      << be_nl
      << "{"
      << be_nl
      << "private:"
      << be_nl
      << "  unsigned int hash (const char *str, unsigned int len);"
      << be_nl
      << "public:"
      << be_nl
      << " const TAO_operation_db_entry * lookup (const char *str, unsigned int len);"
      << be_nl
      << "};"
      << "\n";
}

// Outputs the class definition for the binary searching. This class
// will inherit from the TAO_Binary_Seach_OpTable.
void
be_interface::gen_binary_search_class_definition (void)
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  TAO_OutStream *ss = cg->server_skeletons ();

  *ss << "class " << "TAO_" << this->flatname () << "_Binary_Search_OpTable"
      << " : public TAO_Binary_Search_OpTable"
      << be_nl
      << "{"
      << be_nl
      << "public:"
      << be_nl
      << " const TAO_operation_db_entry * lookup (const char *str);"
      << be_nl
      << "};"
      << "\n";
}

// Outputs the class definition for the linear search. This class
// will inherit from the TAO_Linear_Search_OpTable.
void
be_interface::gen_linear_search_class_definition (void)
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  TAO_OutStream *ss = cg->server_skeletons ();

  *ss << "class " << "TAO_" << this->flatname () << "_Linear_Search_OpTable"
      << " : public TAO_Linear_Search_OpTable"
      << be_nl
      << "{"
      << be_nl
      << "public:"
      << be_nl
      << " const TAO_operation_db_entry * lookup (const char *str);"
      << be_nl
      << "};"
      << "\n";
}

// We have collected the input (Operations and the corresponding
// skeleton pointers) for the gperf program. Now let us execute gperf
// and get things done.
// GPERF reads from our temp file and write to the Server Skeleton
// file.
int
be_interface::gen_gperf_lookup_methods (void)
{
  // Using ACE_Process.
  ACE_Process process_manager;
  ACE_Process_Options process_options;

  // Codegen's singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Adjust the offset of the underlying file pointer.
  ACE_OS::rewind (cg->gperf_input_stream ()->file ());

  // Set the stdin and stdout appropriately for the gperf program.

  // Stdin is our temp file. Close the temp file and open. We will use
  // <open_temp_file> to  open the file now, so that the file will get
  // deleted once when we close the file.

  // Close the file.
  if (ACE_OS::fclose (cg->gperf_input_stream ()->file ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p:File close failed on temp gperf's input file\n"),
                      -1);

  // Open the temp file.
  ACE_HANDLE input =  ACE::open_temp_file (cg->gperf_input_filename (),
                                           O_RDONLY);

  if (input == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p:File open failed on gperf's temp input file\n"),
                      -1);

  // Stdout is server skeleton. Do *not* close the file, just open
  // again with ACE_OS::open with WRITE + APPEND option.. After this,
  // remember to update the file offset to the correct location.

  ACE_HANDLE output =  ACE_OS::open (idl_global->be_get_server_skeleton_fname (),
                                     O_WRONLY | O_APPEND);
  if (output == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p:File open failed on server skeleton file\n"),
                      -1);

  // Set the handles now in the process options.
  process_options.set_handles (input, output);

  // Set the command line for the gperf program. Give the right
  // arguments for the operation lookup strategy that we are using.
  switch (cg->lookup_strategy ())
    {
      // Perfect Hashing.
    case TAO_CodeGen::TAO_PERFECT_HASH:
      process_options.command_line ("%s"
                                    " "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname_ -L C++"
                                    " "
                                    "-Z TAO_%s_Perfect_Hash_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    this->flatname ());
      break;

      // Binary search methods from GPERF. Everythis and the -B flag.
    case TAO_CodeGen::TAO_BINARY_SEARCH:
      process_options.command_line ("%s"
                                    " "
                                    "-B"
                                    "  "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname_ -L C++"
                                    " "
                                    "-Z TAO_%s_Binary_Search_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    this->flatname ());
      break;

      // Linear search methods from GPERF. Everything and the -z flag.
    case TAO_CodeGen::TAO_LINEAR_SEARCH:
      process_options.command_line ("%s"
                                    " "
                                    "-b"
                                    "  "
                                    "-m -M -J -c -C"
                                    " "
                                    "-D -E -T -f 0"
                                    " "
                                    "-a -o -t -p -K"
                                    " "
                                    "opname_ -L C++"
                                    " "
                                    "-Z TAO_%s_Linear_Search_OpTable"
                                    " "
                                    "-N lookup",
                                    idl_global->gperf_path (),
                                    this->flatname ());
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "tao_idl:ERROR:%N:%l:Unknown Operation Lookup Strategy\n"),
                        -1);
    }


  // Spawn a process for gperf.
  if (process_manager.spawn (process_options) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Couldnt spawn a process for gperf program\n"),
                      -1);

  // Wait for gperf to complete.
  if (process_manager.wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Error on wait'ing for completion of gperf program.\n"),
                      -1);

  // Adjust the file offset to the EOF for the server skeleton file.
  ACE_OS::fseek (cg->server_skeletons ()->file (), 0, SEEK_END);

  return 0;
}

// Create an instance of this perfect hash table.
void
be_interface::gen_perfect_hash_instance ()
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  TAO_OutStream *ss = cg->server_skeletons ();

  *ss << "static TAO_" << this->flatname () << "_Perfect_Hash_OpTable"
      << " "
      << "tao_" << this->flatname () << "_optable"
      << ";\n" << be_nl;
}

// Create an instance of the binary search optable.
void
be_interface::gen_binary_search_instance ()
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  TAO_OutStream *ss = cg->server_skeletons ();

  *ss << "static TAO_" << this->flatname () << "_Binary_Search_OpTable"
      << " "
      << "tao_" << this->flatname () << "_optable"
      << ";\n" << be_nl;
}

// Create an instance of this perfect hash table.
void
be_interface::gen_linear_search_instance ()
{
  // Codegen singleton.
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Outstream.
  TAO_OutStream *ss = cg->server_skeletons ();

  *ss << "static TAO_" << this->flatname () << "_Linear_Search_OpTable"
      << " "
      << "tao_" << this->flatname () << "_optable"
      << ";\n" << be_nl;
}

int
be_interface::is_a_helper (be_interface * /*derived*/,
                           be_interface *bi,
                           TAO_OutStream *os)
{
  // emit the comparison code
  os->indent ();
  *os << "(!ACE_OS::strcmp ((char *)value, \"" << bi->repoID () <<
    "\")) ||\n";

  return 0;
}

int
be_interface::downcast_helper (be_interface * /* derived */,
                               be_interface *base,
                               TAO_OutStream *os)
{
  *os << "if (ACE_OS::strcmp (logical_type_id, \""
      << base->repoID () << "\") == 0)" << be_idt_nl
      << "return ACE_static_cast ("
      << base->full_skel_name () << "_ptr, this);" << be_uidt_nl;
  return 0;
}

int
be_interface::gen_skel_helper (be_interface *derived,
                               be_interface *ancestor,
                               TAO_OutStream *os)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  TAO_NL  nl;        // end line

  // if derived and ancestor are same, skip it
  if (derived == ancestor)
    return 0;

  // else generate code that does the cast to the appropriate type

  if (ancestor->nmembers () > 0)
    {
      // if there are elements in ancestor scope i.e., any operations and
      // attributes defined by "ancestor", become methods on the derived class
      // which call the corresponding method of the base class by doing the
      // proper casting

      si = new UTL_ScopeActiveIterator (ancestor, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (d->node_type () == AST_Decl::NT_op)
            {
              os->indent (); // start from current indentation level
              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // generate the static method corresponding to this method
                  *os << "static void " << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "CORBA::ServerRequest &req, " << be_nl
                      << "void *obj," << be_nl
                      << "void *context," << be_nl
                      << "CORBA::Environment &env =" << be_idt_nl
                      << "CORBA::Environment::default_environment ()"
                      << be_uidt << be_uidt_nl
                      << ");" << be_uidt << "\n\n";
                }
              else
                { // generate code in the inline file
                  // generate the static method corresponding to this method
                  *os << "ACE_INLINE void "
                      << derived->full_skel_name () << "::"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "CORBA::ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context," << be_nl
                      << "CORBA::Environment &env" << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl;
                  *os << ancestor->full_skel_name ()
                      << "_ptr impl = ("
                      << derived->full_skel_name ()
                      << "_ptr) obj;" << be_nl;
                  *os << ancestor->full_skel_name ()
                      << "::" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "(" << ancestor->full_skel_name ()
                      << "_ptr) impl," << be_nl
                      << "context," << be_nl
                      << " env" << be_uidt_nl
                      << ");" << be_uidt << be_uidt_nl
                      << "}\n";
                }
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr;

              attr = AST_Attribute::narrow_from_decl (d);
              if (!attr)
                return -1;

              os->indent (); // start from current indentation level
              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // generate the static method corresponding to this method
                  *os << "static void _get_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "CORBA::ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context," << be_nl
                      << "CORBA::Environment &env =" << be_idt_nl
                      << "CORBA::Environment::default_environment ()"
                      << be_uidt << be_uidt_nl
                      << ");" << be_uidt << "\n\n";
                }
              else
                { // generate code in the inline file
                  // generate the static method corresponding to this method
                  *os << "ACE_INLINE void "
                      << derived->full_skel_name () << "::_get_"
                      << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "CORBA::ServerRequest &req," << be_nl
                      << "void *obj," << be_nl
                      << "void *context," << be_nl
                      << "CORBA::Environment &env" << be_uidt_nl
                      << ")" << be_uidt_nl
                      << "{" << be_idt_nl
                      << ancestor->full_skel_name ()
                      << "_ptr impl = ("
                      << derived->full_skel_name ()
                      << "_ptr) obj;" << nl;
                  *os << ancestor->full_skel_name ()
                      << "::_get_" << d->local_name ()
                      << "_skel (" << be_idt << be_idt_nl
                      << "req," << be_nl
                      << "(" << ancestor->full_skel_name ()
                      << "_ptr) impl," << be_nl
                      << "context," << be_nl
                      << "env" << be_uidt_nl
                      << ");" << be_uidt << be_uidt_nl
                      << "}\n";
                }

              if (!attr->readonly ())
                {
                  // the set method
                  os->indent (); // start from current indentation level
                  if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                    {
                      // generate the static method corresponding to
                      // this method
                      *os << "static void _set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "CORBA::ServerRequest &req," << be_nl
                          << "void *obj," << be_nl
                          << "void *context," << be_nl
                          << "CORBA::Environment &env = " << be_idt_nl
                          << "CORBA::Environment::default_environment ()"
                          << be_uidt << be_uidt_nl
                          << ");" << be_uidt << "\n\n";
                    }
                  else
                    { // generate code in the inline file
                      // generate the static method corresponding to
                      // this method
                      *os << "ACE_INLINE void "
                          << derived->full_skel_name ()
                          << "::_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "CORBA::ServerRequest &req," << be_nl
                          << "void *obj," << be_nl
                          << "void *context," << be_nl
                          << "CORBA::Environment &env" << be_uidt_nl
                          << ")" << be_uidt_nl
                          << "{" << be_idt_nl
                          << ancestor->full_skel_name ()
                          << "_ptr impl = ("
                          << derived->full_skel_name ()
                          << "_ptr) obj;" << be_nl;
                      *os << ancestor->full_skel_name ()
                          << "::_set_" << d->local_name ()
                          << "_skel (" << be_idt << be_idt_nl
                          << "req," << be_nl
                          << "(" << ancestor->full_skel_name ()
                          << "_ptr) impl," << be_nl
                          << "context," << be_nl
                          << "env" << be_uidt_nl
                          << ");" << be_uidt << be_uidt_nl
                          << "}\n";
                    }

                }
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_interface::collocated_ctor_helper (be_interface *derived,
                                      be_interface *base,
                                      TAO_OutStream *os)
{
  if (derived == base)
    // we are the same. Don't do anything, otherwise we will end up calling
    // ourself
    return 0;

  if (base->is_nested ())
    {
      be_decl *scope;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();
      *os << "  ACE_NESTED_CLASS (POA_" << scope->name () << ","
          << base->local_coll_name () << ") (servant, stub)," << be_nl;
    }
  else
    {
      *os << "  " << base->full_coll_name () << " (servant, stub)," << be_nl;
    }

  return 0;
}

int
be_interface::copy_ctor_helper (be_interface *derived,
                                be_interface *base,
                                TAO_OutStream *os)
{
  if (derived == base)
    // we are the same. Don't do anything, otherwise we will end up calling
    // ourself
    return 0;

  if (base->is_nested ())
    {
      be_decl *scope;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();
      *os << "  ACE_NESTED_CLASS (POA_" << scope->name () << ","
          << base->local_name () << ") (rhs)," << be_nl;
    }
  else
    {
      *os << "  " << base->full_skel_name () << " (rhs)," << be_nl;
    }

  return 0;
}

int
be_interface::in_mult_inheritance_helper (be_interface *derived,
                                          be_interface *base,
                                          TAO_OutStream *)
{
  switch (derived->n_inherits ())
    {
    case 0:
      // no parent
      derived->in_mult_inheritance (0);
      break;
    case 1:
      if (derived == base)
        // prevent indefinite recursion
        derived->in_mult_inheritance (-1);
      else
        // one parent. We have the same characteristics as our base
        derived->in_mult_inheritance (base->in_mult_inheritance ());
      break;
    default:
      // direct multiple inheritance
      derived->in_mult_inheritance (1);
    }
  return 0;
}

const char*
be_interface::relative_coll_name (const char *collname)
{
  return be_interface::relative_name (this->full_coll_name (),
                                      collname);
}

// return the relative skeleton name (needed due to NT compiler insanity)
const char *
be_interface::relative_skel_name (const char *skelname)
{
  return be_interface::relative_name (this->full_skel_name (),
                                      skelname);
}

const char*
be_interface::relative_name (const char *localname,
                             const char *othername)
{
  // some compilers do not like generating a fully scoped name for a
  // type that was defined in the same enclosing scope in which it was
  // defined.  We have to emit just the partial name, relative to our
  // "localname"

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well. If that is the case, then we can generate a fully
  // scoped name for that type, else we use the ACE_NESTED_CLASS macro

  // thus we need some sort of relative name to be generated

  static char macro [NAMEBUFSIZE];
  // UNUSED: be_decl *def_scope = 0;  // our defining scope
  char // hold the fully scoped name
    def_name [NAMEBUFSIZE],
    use_name [NAMEBUFSIZE];
  char // these point to the curr and next component in the scope
    *def_curr = def_name,
    *def_next,
    *use_curr = use_name,
    *use_next;

  ACE_OS::memset (macro, '\0', NAMEBUFSIZE);
  ACE_OS::memset (def_name, '\0', NAMEBUFSIZE);
  ACE_OS::memset (use_name, '\0', NAMEBUFSIZE);

  // traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed. This forms the first argument to the ACE_NESTED_CLASS
  // macro. Whenever there is no match, the remaining components of the
  // def_scope form the second argument

  ACE_OS::strcpy (def_name, localname);
  ACE_OS::strcpy (use_name, othername);

  while (def_curr && use_curr)
    {
      // find the first occurrence of a :: and advance the next pointers accordingly
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      if (def_next)
        *def_next = 0;

      if (use_next)
        *use_next = 0;

      if (!ACE_OS::strcmp (def_curr, use_curr))
        {
          // they have same prefix, append to arg1
          def_curr = (def_next ? (def_next+2) : 0); // skip the ::
          use_curr = (use_next ? (use_next+2) : 0); // skip the ::
        }
      else
        {
          // we had overwritten a ':' by a '\0' for string comparison. We
          // revert back because we want the rest of the relative name to be
          // used
          if (def_next)
            *def_next = ':';

          if (use_next)
            *use_next = ':';

          // no match. This is the end of the first argument. Get out
          // of the loop as no more comparisons are necessary
          break;
        }
    }

  // start the 2nd argument of the macro

  // copy the remaining def_name (if any left)
  if (def_curr)
    ACE_OS::strcat (macro, def_curr);

  return macro;
}

int
be_interface::accept (be_visitor *visitor)
{
  return visitor->visit_interface (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_interface)
IMPL_NARROW_FROM_SCOPE (be_interface)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node <be_interface*>;
template class ACE_Unbounded_Queue <be_interface*>;
template class ACE_Unbounded_Queue_Iterator <be_interface*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<be_interface*>
#pragma instantiate ACE_Unbounded_Queue<be_interface*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<be_interface*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
