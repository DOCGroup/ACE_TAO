// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_decl.cpp
//
// = DESCRIPTION
//    Extension of class AST_Decl that provides additional means for C++
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

// Default Constructor
be_decl::be_decl (void)
  : cli_hdr_gen_ (I_FALSE),
    cli_stub_gen_ (I_FALSE),
    cli_inline_gen_ (I_FALSE),
    srv_hdr_gen_ (I_FALSE),
    srv_skel_gen_ (I_FALSE),
    srv_inline_gen_ (I_FALSE),
    seq_names_ (NULL),
    fullname_ (0),
    tc_name_ (0),
    encap_len_ (-1),
    size_type_ (be_decl::FIXED) // everybody is fixed size to start with
{
}

// Constructor
be_decl::be_decl (AST_Decl::NodeType type, UTL_ScopedName *n, UTL_StrList
                  *pragmas)
  : AST_Decl (type, n, pragmas),
    cli_hdr_gen_ (I_FALSE),
    cli_stub_gen_ (I_FALSE),
    cli_inline_gen_ (I_FALSE),
    srv_hdr_gen_ (I_FALSE),
    srv_skel_gen_ (I_FALSE),
    srv_inline_gen_ (I_FALSE),
    seq_names_ (NULL),
    fullname_ (0),
    tc_name_ (0),
    encap_len_ (-1),
    size_type_ (be_decl::FIXED) // everybody is fixed size to start with
{
}

//destructor
be_decl::~be_decl (void)
{
}

int
be_decl::gen_typecode (void)
{
  // do nothing
  return 0;
}

// return our size type
be_decl::SIZE_TYPE
be_decl::size_type (void) const
{
  return this->size_type_;
}

// set our size type and that of all our ancestors
void
be_decl::size_type (be_decl::SIZE_TYPE st)
{
  if (st == be_decl::FIXED) // does not affect ancestors
    return;

  if (this->size_type_ == st) // already of that type.
    return; // nothing to do

  this->size_type_ = st;

  // if we are just a typedef, nothing else to do
  if (this->node_type () == AST_Decl::NT_typedef)
    return;

  // update our parent if it is of a specific type
  if (!this->defined_in ()) // we are outermost
    return; // nothing to do
  else
    {
      // get the scope we are defined in
      be_decl *d = be_decl::narrow_from_decl (ScopeAsDecl (this->defined_in
                                                           ()));
      switch (d->node_type ())
        {
          // only these define valid scopes whose size needs to be set the same
          // as ours.
        case AST_Decl::NT_struct:
        case AST_Decl::NT_union:
          d->size_type (st); // call recursively to set the size type of
          // our ancestors
        default:
          return; // we are done
        }
    } // end else
}

// compute stringified fully scoped name
void
be_decl::compute_fullname (void)
{
  if (fullname_)
    return;
  else
    {
      long namelen;
      UTL_IdListActiveIterator	*i;
      long				first = I_TRUE;
      long				second = I_FALSE;

      // in the first loop compute the total length
      namelen = 0;
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ())) 
        {
          if (!first)
            namelen += 2; // for "::"
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          namelen += ACE_OS::strlen (i->item ()->get_string ());
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

      this->fullname_ = new char [namelen+1];
      first = I_TRUE;
      second - I_FALSE;
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ())) 
        {
          if (!first)
            ACE_OS::strcat (this->fullname_, "::");
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          ACE_OS::strcat (this->fullname_, i->item ()->get_string ());
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

const char*
be_decl::fullname (void)
{
  return this->fullname_;
}

// compute stringified flattened fully scoped name
void
be_decl::compute_flatname (void)
{
  if (flatname_)
    return;
  else
    {
      long namelen;
      UTL_IdListActiveIterator	*i;
      long				first = I_TRUE;
      long				second = I_FALSE;

      // in the first loop compute the total length
      namelen = 0;
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ())) 
        {
          if (!first)
            namelen += 1; // for "_"
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          namelen += ACE_OS::strlen (i->item ()->get_string ());
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

      this->flatname_ = new char [namelen+1];
      first = I_TRUE;
      second - I_FALSE;
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ())) 
        {
          if (!first)
            ACE_OS::strcat (this->flatname_, "_");
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          ACE_OS::strcat (this->flatname_, i->item ()->get_string ());
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

const char*
be_decl::flatname (void)
{
  return this->flatname_;
}

// compute stringified repository ID
void
be_decl::compute_repoID (void)
{
  if (repoID_)
    return;
  else
    {
      long namelen;
      UTL_IdListActiveIterator	*i;
      long				first = I_TRUE;
      long				second = I_FALSE;

      // in the first loop compute the total length
      namelen = 8; // for the prefix "IDL:" and suffix ":1.0"
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ())) 
        {
          if (!first)
            namelen += 1; // for "/"
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          namelen += ACE_OS::strlen (i->item ()->get_string ());
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

      this->repoID_ = new char [namelen+1];
      ACE_OS::sprintf (this->repoID_, "%s", "IDL:");
      i = new UTL_IdListActiveIterator (this->name ());
      first = I_TRUE;
      second - I_FALSE;
      while (!(i->is_done ())) 
        {
          if (!first)
            ACE_OS::strcat (this->repoID_, "/");
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          ACE_OS::strcat (this->repoID_, i->item ()->get_string ());
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
      ACE_OS::strcat (this->repoID_, ":1.0");
    }
  return;
}

const char *
be_decl::repoID (void)
{
  return this->repoID_;
}

// compute the typecode name. The idea is to use the fully scoped name,
// however, prepend a _tc_ to the last component. A slightly different approach
// is required of the predefined types. Hence this method is overridden for
// predefined types.

void
be_decl::compute_tc_name (void)
{
  static char namebuf [200];
  UTL_ScopedName *n;

  this->tc_name_ = NULL;
  ACE_OS::memset (namebuf, '\0', 200);
  n = this->name ();
  while (n->tail () != NULL)
    {
      if (!this->tc_name_)
        {
          // does not exist
          this->tc_name_ = new UTL_ScopedName (n->head (), NULL);
        }
      else
        {
          this->tc_name_->nconc (new UTL_ScopedName (n->head (), NULL));
        }
      n = (UTL_ScopedName *)n->tail ();
    }
  ACE_OS::sprintf (namebuf, "_tc_%s", n->last_component ()->get_string ());
  if (!this->tc_name_)
    {
      // does not exist
      this->tc_name_ = new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                           (namebuf), 1, 0, I_FALSE), NULL);
    }
  else
    {
      this->tc_name_->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                                 (namebuf), 1,
                                                                 0, I_FALSE), NULL));
    }
  return;
}

// retrieve typecode name
UTL_ScopedName *
be_decl::tc_name (void)
{
  return this->tc_name_;
}

// return encapsulation length. Don't do anything here.
long 
be_decl::tc_encap_len (void)
{
  return 0;
}

int 
be_decl::tc_name2long (const char *name, long *&larr, long &arrlen)
{
  static char buf [MAXNAMELEN];
  long slen;
  int i,j;

  slen = ACE_OS::strlen (name) + 1; // 1 for NULL terminating

  // compute the number of bytes necessary to hold the name rounded to the next
  // multiple of 4 (i.e., size of long)
  arrlen = slen/4 + (slen%4 ? 1:0);

  memset (buf, '\0', arrlen*4);
  larr = (long *)buf;

  for (i=0; i < ACE_OS::strlen (name); i++)
    {
      long shift; // num bytes to shift left

      shift = 3 - (i%4); 
      // store the computed shifted quantity in the appropriate byte of the
      // array to be returned
      larr [i/4] |= ((name[i] & 0xff) << (shift*8));
    }
  return 0;
}


// *****************************
// CODE GENERATION
// *****************************

// generate the _var definition for ourself
int
be_decl::gen_var_defn (void)
{
  TAO_OutStream *ch; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char namebuf [MAXNAMELEN];  // names

  ACE_OS::memset (namebuf, '\0', MAXNAMELEN);
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
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      *ch << namebuf << " (" << local_name () << "_ptr);" << nl;
      break;
    case AST_Decl::NT_array:
      *ch << namebuf << " (" << local_name () << "_slice *);" << nl;
      break;
    default: // others
      *ch << namebuf << " (" << local_name () << " *);" << nl;
    }
  // copy constructor
  *ch << namebuf << " (const " << namebuf << 
    " &); // copy constructor" << nl;
  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;
  // assignment operator from a pointer
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      *ch << namebuf << " &operator= (" << local_name () << "_ptr);" << nl;
      break;
    case AST_Decl::NT_array:
      *ch << namebuf << " &operator= (" << local_name () << "_slice *);" << nl;
      break;
    default: // others
      *ch << namebuf << " &operator= (" << local_name () << " *);" << nl;
    }
  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf << 
    " &);" << nl;

  // arrow operator
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      *ch << local_name () << "_ptr operator-> (void) const;" << nl;
      break;
    case AST_Decl::NT_array:
      // nothing here
      break; 
    default: // others
      *ch << local_name () << " *operator-> (void);" << nl;
      *ch << "const " << local_name () << " *operator-> (void) const;" << nl;
    }
  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      // cast operators
      *ch << "operator const " << local_name () << "_ptr &() const;" << nl;
      *ch << "operator " << local_name () << "_ptr &();" << nl;
      break;
    case AST_Decl::NT_array:
      // overloaded [] operator
      *ch << namebuf << "_slice &operator[] (CORBA::ULong index);" << nl;
      *ch << "const " << namebuf << 
        "_slice &operator[] (CORBA::ULong index) const;" << nl;

      // cast operators
      *ch << "operator const " << local_name () << "_slice *&() const;" << nl;
      *ch << "operator " << local_name () << "_slice *&();" << nl;
      break;
    case AST_Decl::NT_struct:
    case AST_Decl::NT_union:
      *ch << "operator const " << local_name () << " &() const;" << nl;
      *ch << "operator " << local_name () << " &();" << nl;
      *ch << "operator " << local_name () << " &() const;" << nl;
      break;
    case AST_Decl::NT_sequence:
      {
        be_sequence *bs = be_sequence::narrow_from_decl (this);
        be_type *bt = be_type::narrow_from_decl (bs->base_type ());
        
        // cast operator
        *ch << "operator const " << local_name () << " &() const;" << nl;
        *ch << "operator " << local_name () << " &();" << nl;
        *ch << "operator " << local_name () << " &() const;" << nl;
        
        // overloaded [] operator. The const version is not required
        bt->be_type::gen_client_header ();
        *ch << " &operator[] (CORBA::ULong index);" << nl;
      }
    }
  
  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  switch (this->node_type ())
    {
    case AST_Decl::NT_struct:
    case AST_Decl::NT_union:
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
      break;
    case AST_Decl::NT_sequence:
      *ch << "const " << local_name () << " &in (void) const;" << nl;
      *ch << local_name () << " &inout (void);" << nl;
      *ch << local_name () << " *&out (void);" << nl;
      *ch << local_name () << " *_retn (void);" << nl;
      break;
    case AST_Decl::NT_array:
      if (this->size_type () == be_decl::FIXED)
        {
          *ch << "const " << local_name () << " in (void) const;" << nl;
          *ch << local_name () << " inout (void);" << nl;
          *ch << local_name () << " out (void);" << nl;
          *ch << local_name () << "_slice *_retn (void);" << nl;
        }
      else
        {
          *ch << "const " << local_name () << " in (void) const;" << nl;
          *ch << local_name () << " inout (void);" << nl;
          *ch << local_name () << "_slice *&out (void);" << nl;
          *ch << local_name () << " *_retn (void);" << nl;
        }
      break;
    case AST_Decl::NT_interface:
      *ch << local_name () << "_ptr in (void) const;" << nl;
      *ch << local_name () << "_ptr &inout (void);" << nl;
      *ch << local_name () << "_ptr &out (void);" << nl;
      *ch << local_name () << "_ptr _retn (void);" << nl;
    default:
      break;
    }
  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      *ch << local_name () << "_ptr ptr_;\n";
      break;
    case AST_Decl::NT_array:
      *ch << local_name () << "_slice *ptr_;\n";
      break;
    default:
      *ch << local_name () << " *ptr_;\n";
    }

  ch->decr_indent ();
  *ch << "};\n\n";
  
  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file 
int
be_decl::gen_var_impl (void)
{
  TAO_OutStream *ci; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char fname [MAXNAMELEN];  // to hold the full and
  char lname [MAXNAMELEN];  // local _var names

  ACE_OS::memset (fname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (lname, "%s_var", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();
  cg->outstream (ci);

  // generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************" 
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // due to some differences with the way with which the _var is defined, we
  // handle code generation in a switch statement
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
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

      // copy constructor
      ci->indent ();
      *ci << "ACE_INLINE" << nl;
      *ci << fname << "::" << lname << " (const " << fname << 
        " &p) // copy constructor" << nl;
      *ci << "\t: ptr_ (" << name () << "::_duplicate (p))" << nl; 
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
      *ci << "this->ptr_ = " << name () << "::_duplicate (p);\n";
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

      break;
    case AST_Decl::NT_array:
      { 
        be_array *b;

        b = be_array::narrow_from_decl (this);
        // default constr
        *ci << "ACE_INLINE" << nl;
        *ci << fname << "::" << lname << 
          " (void) // default constructor" << nl;
        *ci << "\t" << ": ptr_ ((" << name () << "_slice *)0)" << nl;
        *ci << "{}\n\n";

        // constr from a _slice *
        ci->indent ();
        *ci << "ACE_INLINE" << nl;
        *ci << fname << "::" << lname << " (" << name () << "_slice *p)" << nl;
        *ci << "\t: ptr_ (p)" << nl;
        *ci << "{}\n\n";

        // copy constructor (deep copy)
        ci->indent ();
        *ci << "ACE_INLINE" << nl;
        *ci << fname << "::" << lname << " (const " << fname << 
          " &p) // copy constructor" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "this->ptr_ = " << this->name () << "_dup (p->ptr);\n";
        ci->decr_indent ();
        *ci << "}\n\n";
        
        // destructor
        ci->indent ();
        *ci << "ACE_INLINE" << nl;
        *ci << fname << "::~" << lname << " (void) // destructor" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << this->name () << "_free (this->ptr_);\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // assignment operator
        ci->indent ();
        *ci << "ACE_INLINE " << fname << " &" << nl;
        *ci << fname << "::operator= (" << this->name () <<
          "_slice p)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "// is what we own the same that is being assigned to us?" <<
          nl;
        *ci << "if (this->ptr_ != p)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "// delete our stuff and assume ownership of p" << nl;
        *ci << this->name () << "_free (this->ptr_);" << nl;
        *ci << "this->ptr_ = p;\n";
        ci->decr_indent ();
        *ci << "}" << nl;
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
        *ci << "// not assigning to ourselves" << nl;
        *ci << this->name () << "_free (this->ptr_); // free old stuff" << nl;
        *ci << "this->ptr_ = " << this->name () << 
          "_dup (p->ptr);// deep copy\n";
        ci->decr_indent ();
        *ci << "}" << nl;
        *ci << "return *this;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // other extra methods - cast operators ()
        ci->indent ();
        *ci << "ACE_INLINE " << nl;
        *ci << fname << "::operator const " << name () << 
          "_slice *&() const // cast" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        ci->indent ();
        *ci << "ACE_INLINE " << nl;
        *ci << fname << "::operator " << name () << "_slice *&() // cast " << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // two operator []s instead of ->
        ci->indent ();
        *ci << "ACE_INLINE const" << name () << "_slice &" << nl;
        *ci << fname << "::operator[] (CORBA::ULong index) const" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_[index];\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        ci->indent ();
        *ci << "ACE_INLINE " << name () << "_slice &" << nl;
        *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_[index];\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // in, inout, out, and _retn
        ci->indent ();
        *ci << "ACE_INLINE " << fname << nl;
        *ci << fname << "::in (void) const" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        ci->indent ();
        *ci << "ACE_INLINE " << fname << nl;
        *ci << fname << "::inout (void)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        ci->indent ();
        *ci << "ACE_INLINE " << fname << nl;
        *ci << fname << "::out (void)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        ci->indent ();
        *ci << "ACE_INLINE " << name () << "_slice " << nl;
        *ci << fname << "::_retn (void)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->val;\n";
        ci->decr_indent ();
        *ci << "}\n\n";
        
      } // end of case ARRAY
    break;
    case AST_Decl::NT_struct:
    case AST_Decl::NT_union:
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
        }
      break;
    case AST_Decl::NT_sequence:
      {
        be_sequence *bs = be_sequence::narrow_from_decl (this);
        be_type *bt = be_type::narrow_from_decl (bs->base_type ());

      // default constr
      *ci << "ACE_INLINE" << nl;
      *ci << fname << "::" << lname << 
        " (void) // default constructor" << nl;
      *ci << "\t" << ": ptr_ (0)" << nl;
      *ci << "{}\n\n";

      // constr from a _ptr
      ci->indent ();
      *ci << "ACE_INLINE" << nl;
      *ci << fname << "::" << lname << " (" << name () << "_ptr p)" << nl;
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
        "_var &p) // deep copy" << nl;
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
      *ci << "ACE_INLINE const " << fname << " *" << nl;
      *ci << fname << "::operator-> (void) const" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->ptr_;\n";
      ci->decr_indent ();
      *ci << "}\n\n";

      ci->indent ();
      *ci << "ACE_INLINE " << fname << " *" << nl;
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

      // operator []
      ci->indent ();
      *ci << "ACE_INLINE ";
      bt->be_type::gen_client_header ();
      *ci << " " << nl;
      *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
      *ci << "{\n";
      ci->incr_indent ();
      *ci << "return this->ptr_->operator[] (index);\n";
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

      ci->indent ();
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

      } // end of case for sequence
    } // end of switch
  return 0;
}

// generate the _out definition
int
be_decl::gen_out_defn (void)
{
  TAO_OutStream *ch; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char namebuf [MAXNAMELEN];  // to hold the _out name

  ACE_OS::memset (namebuf, '\0', MAXNAMELEN);
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
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      *ch << namebuf << " (" << local_name () << "_ptr &);" << nl;
      break;
    case AST_Decl::NT_array:
      *ch << namebuf << " (" << local_name () << "_slice *&);" << nl;
      break;
    default: // others
      *ch << namebuf << " (" << local_name () << " *&);" << nl;
    }
  // constructor from a _var &
  *ch << namebuf << " (" << local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      // only interface allows assignment from var &
      *ch << namebuf << " &operator= (const " << local_name () << "_var &);" << nl;
      *ch << namebuf << " &operator= (" << local_name () << "_ptr);" << nl;
      // cast
      *ch << "operator " << local_name () << "_ptr &();" << nl;
      // ptr fn
      *ch << local_name () << "_ptr &ptr (void);" << nl;
      // operator ->
      *ch << local_name () << "_ptr operator-> (void);" << nl;
      break;
    case AST_Decl::NT_array:
      // assignment from slice *
      *ch << namebuf << " &operator= (" << local_name () << "_slice *);" << nl;
      // cast
      *ch << "operator " << local_name () << "_slice *&();" << nl;
      // ptr fn
      *ch << local_name () << "_slice *&ptr (void);" << nl;
      // operator [] instead of ->
      *ch << namebuf << "_slice &operator[] (CORBA::ULong index);" << nl;
      *ch << "const " << namebuf << 
        "_slice &operator[] (CORBA::ULong index) const;" << nl; 
      break;
    default: // others
      // assignment
      *ch << namebuf << " &operator= (" << local_name () << " *);" << nl;
      // operator ()
      *ch << "operator " << local_name () << " *&();" << nl;
      // ptr fn
      *ch << local_name () << " *&ptr (void);" << nl;
      // operator ->
      *ch << local_name () << " *operator-> (void);" << nl;

      if (this->node_type () == AST_Decl::NT_sequence)
        {
          be_sequence *bs = be_sequence::narrow_from_decl (this);
          be_type *bt = be_type::narrow_from_decl (bs->base_type ());

          // overloaded [] operator only for sequence. The const version is not
          // required
          bt->be_type::gen_client_header ();
          *ch << " &operator[] (CORBA::ULong index);" << nl;
        }
    }
  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
      *ch << local_name () << "_ptr &ptr_;\n";
      break;
    case AST_Decl::NT_array:
      *ch << local_name () << "_slice *&ptr_;" << nl;
      *ch << "// assignment from T_var not allowed" << nl;
      *ch << "void operator= (const " << local_name () << "_var &);\n";
      break;
    default:
      *ch << local_name () << " *&ptr_;" << nl;
      *ch << "// assignment from T_var not allowed" << nl;
      *ch << "void operator= (const " << local_name () << "_var &);\n";
    }

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_decl::gen_out_impl (void)
{
  TAO_OutStream *ci; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char fname [MAXNAMELEN];  // to hold the full and
  char lname [MAXNAMELEN];  // local _out names

  ACE_OS::memset (fname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (fname, "%s_out", this->fullname ());

  ACE_OS::memset (lname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (lname, "%s_out", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();
  cg->outstream (ci);

  // generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************" 
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // due to some differences with the way with which the _var is defined, we
  // handle code generation in a switch statement
  switch (this->node_type ())
    {
    case AST_Decl::NT_interface:
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
      *ci << "this->ptr_ = " << this->name () << "::_duplicate (p);" << nl;
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

      break;
    case AST_Decl::NT_array:
      { 
        be_array *b;

        b = be_array::narrow_from_decl (this);
        // constr from a pointer to slice
        ci->indent ();
        *ci << "ACE_INLINE" << nl;
        *ci << fname << "::" << lname << " (" << name () << "_slice *&p)" << nl;
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
        *ci << this->name () << "_free (this->ptr_);" << nl;
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

        // assignment operator from _ptr
        ci->indent ();
        *ci << "ACE_INLINE " << fname << " &" << nl;
        *ci << fname << "::operator= (" << this->name () <<
          "_slice *p)" << nl;
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
          "_slice *&() // cast" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // ptr function
        ci->indent ();
        *ci << "ACE_INLINE " << this->name () << "_slice *&" << nl;
        *ci << fname << "::ptr (void) // ptr" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // operator [] instead of ->
        ci->indent ();
        *ci << "ACE_INLINE " << this->name () << "_slice &" << nl;
        *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_[index];\n";
        ci->decr_indent ();
        *ci << "}\n\n";
      }
    break;
    default: // others
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
        *ci << "ACE_INLINE " << this->name () << " *&" << nl;
        *ci << fname << "::operator-> (void)" << nl;
        *ci << "{\n";
        ci->incr_indent ();
        *ci << "return this->ptr_;\n";
        ci->decr_indent ();
        *ci << "}\n\n";

        // sequence has an additional method
        if (this->node_type () == AST_Decl::NT_sequence)
          {
            be_type *bt;
            bt = be_type::narrow_from_decl (be_sequence::narrow_from_decl
                                            (this)->base_type ()); 

            ci->indent ();
            *ci << "ACE_INLINE ";
            *ci << bt->name ();
            bt->be_type::gen_client_header ();
            *ci << nl;
            *ci << fname << "::operator[] (CORBA::ULong index)" << nl;
            *ci << "{\n";
            ci->incr_indent ();
            *ci << "return this->ptr_->operator[] (index);\n";
            ci->decr_indent ();
            *ci << "}\n\n";
          }
    } // end of switch

  return 0;
}

idl_bool
be_decl::lookup_seq_name (Identifier *id)
{
  UTL_IdListActiveIterator	*i;

  if (!this->seq_names_) // no list
    return I_FALSE;

  i = new UTL_IdListActiveIterator (this->seq_names_);
  while (!(i->is_done ())) 
    {
      if (ACE_OS::strcmp (i->item ()->get_string (), id->get_string ()) == 0)
        return I_TRUE;  // found
      i->next ();
    }
  delete i;
  return I_FALSE; // not found
}

idl_bool
be_decl::add_seq_name (Identifier *id)
{
  // we do not check if the name exists or not. This is based on the assumption
  // that callers have made sure to call a lookup
  this->seq_names_ = new UTL_IdList (id, this->seq_names_);
  return I_TRUE;
}

// narrowing methods
IMPL_NARROW_METHODS1 (be_decl, AST_Decl)
IMPL_NARROW_FROM_DECL (be_decl)

  
