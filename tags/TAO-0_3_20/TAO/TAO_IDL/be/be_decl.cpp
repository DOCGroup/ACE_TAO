// $Id$

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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_decl, "$Id$")

// Default Constructor
be_decl::be_decl (void)
  : cli_hdr_gen_ (I_FALSE),
    cli_stub_gen_ (I_FALSE),
    cli_inline_gen_ (I_FALSE),
    srv_hdr_gen_ (I_FALSE),
    impl_hdr_gen_ (I_FALSE),
    srv_skel_gen_ (I_FALSE),
    impl_skel_gen_ (I_FALSE),
    srv_inline_gen_ (I_FALSE),
    cli_hdr_any_op_gen_ (I_FALSE),
    cli_stub_any_op_gen_ (I_FALSE),
    cli_hdr_cdr_op_gen_ (I_FALSE),
    cli_stub_cdr_op_gen_ (I_FALSE),
    cli_inline_cdr_op_gen_ (I_FALSE),
    fullname_ (0),
    flatname_ (0),
    repoID_ (0),
    prefix_ (0),
    size_type_ (be_decl::SIZE_UNKNOWN),
    encap_len_ (-1)
{
}

// Constructor
be_decl::be_decl (AST_Decl::NodeType type,
		  UTL_ScopedName *n,
		  UTL_StrList *pragmas)
  : AST_Decl (type, n, pragmas),
    cli_hdr_gen_ (I_FALSE),
    cli_stub_gen_ (I_FALSE),
    cli_inline_gen_ (I_FALSE),
    srv_hdr_gen_ (I_FALSE),
    impl_hdr_gen_ (I_FALSE),
    srv_skel_gen_ (I_FALSE),
    impl_skel_gen_ (I_FALSE),
    srv_inline_gen_ (I_FALSE),
    cli_hdr_any_op_gen_ (I_FALSE),
    cli_stub_any_op_gen_ (I_FALSE),
    cli_hdr_cdr_op_gen_ (I_FALSE),
    cli_stub_cdr_op_gen_ (I_FALSE),
    fullname_ (0),
    flatname_ (0),
    repoID_ (0),
    prefix_ (0),
    size_type_ (be_decl::SIZE_UNKNOWN),
    encap_len_ (-1)
{
}

//destructor
be_decl::~be_decl (void)
{
}

int
be_decl::gen_encapsulation (void)
{
  // do nothing
  return 0;
}

long
be_decl::tc_encap_len (void)
{
  return -1;
}

// return our size type
be_decl::SIZE_TYPE
be_decl::size_type (void)
{
  if (this->size_type_ == be_decl::SIZE_UNKNOWN)
    (void) this->compute_size_type ();
  return this->size_type_;
}

// set our size type and that of all our ancestors
void
be_decl::size_type (be_decl::SIZE_TYPE st)
{
  // precondition - you cannot set somebody's sizetype to unknown
  ACE_ASSERT (st != be_decl::SIZE_UNKNOWN);

  // st can be VARIABLE or FIXED
  if (this->size_type_ == be_decl::SIZE_UNKNOWN) // not set yet
    this->size_type_ = st; // set it
  else if ((this->size_type_ == be_decl::FIXED) &&
           (st == be_decl::VARIABLE))
    // once we are VARIABLE, we cannot be FIXED. But if we were FIXED and then
    // get overwritten to VARIABLE, it is fine. Such a situation occurs only
    // when setting the sizes of structures and unions
    this->size_type_ = st;
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
      UTL_IdListActiveIterator *i;
      long first = I_TRUE;
      long second = I_FALSE;

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
      this->fullname_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;
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
  if (!this->fullname_)
    compute_fullname ();

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
      UTL_IdListActiveIterator *i;
      long first = I_TRUE;
      long second = I_FALSE;

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
      this->flatname_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;
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
  if (!this->flatname_)
    compute_flatname ();

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
      UTL_IdListActiveIterator *i;
      long first = I_TRUE;
      long second = I_FALSE;

      // in the first loop compute the total length
      namelen = 8; // for the prefix "IDL:" and suffix ":1.0"
      namelen += ACE_OS::strlen (this->prefix ()) + 1;
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
      this->repoID_[0] = '\0';
      ACE_OS::sprintf (this->repoID_, "%s", "IDL:");
      ACE_OS::strcat (this->repoID_, this->prefix ());

      // Add the "/" only if there is a prefix
      if (ACE_OS::strcmp (this->prefix (), "") != 0)
        ACE_OS::strcat (this->repoID_, "/");

      i = new UTL_IdListActiveIterator (this->name ());
      first = I_TRUE;
      second = I_FALSE;
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
  if (!this->repoID_)
    compute_repoID ();

  return this->repoID_;
}

void
be_decl::compute_prefix ()
{
  const char* pragma = 0;
  if (this->pragmas () != 0)
    {
      for (UTL_StrlistActiveIterator i (this->pragmas ());
	   !i.is_done ();
	   i.next ())
	{
	  const char* s = i.item ()->get_string ();

	  if (ACE_OS::strncmp (s, "#pragma prefix", 14) == 0)
	    {
	      pragma = s;
	    }
	}
    }

  if (pragma != 0)
    {
      // Skip the space and the " also...
      const char* tmp = pragma + 16;
      const char* end = ACE_OS::strchr (tmp, '"');

      if (end == 0)
	{
	  idl_global->err ()->syntax_error
	    (IDL_GlobalData::PS_PragmaPrefixSyntax);
	  this->prefix_ = ACE::strnew ("");
	  return;
	}
      int l = end - tmp;
      this->prefix_ = new char[l+1];
      ACE_OS::strncpy (this->prefix_, tmp, end - tmp);
      this->prefix_[l] = 0;
      return;
    }

  // Could not find it in the local scope, try to recurse to the top
  // scope...
  if (this->defined_in () == 0)
    this->prefix_ = ACE::strnew ("");
  else
    {
      be_scope* scope = 
	be_scope::narrow_from_scope (this->defined_in ());
      if (scope == 0)
	this->prefix_ = ACE::strnew ("");
      else
	this->prefix_ = ACE::strnew (scope->decl()->prefix ());
    }
}

const char*
be_decl::prefix (void)
{
  if (!this->prefix_)
    compute_prefix ();
  return this->prefix_;
}

// converts a string name into an array of 4 byte longs
int
be_decl::tc_name2long (const char *name, ACE_UINT32 *&larr, long &arrlen)
{
  const int bytes_per_word = sizeof(ACE_UINT32);
  static ACE_UINT32 buf [NAMEBUFSIZE];
  long slen;
  long i;

  slen = ACE_OS::strlen (name) + 1; // 1 for NULL terminating

  // compute the number of bytes necessary to hold the name rounded to
  // the next multiple of 4 (i.e., size of long)
  arrlen = slen / bytes_per_word + (slen % bytes_per_word ? 1:0);

  ACE_OS::memset (buf, 0, sizeof(buf));
  larr = buf;
  ACE_OS::memcpy (buf, name, slen);
  for (i = 0; i < arrlen; i++)
    larr [i] = ACE_HTONL (larr [i]);
  return 0;
}

idl_bool
be_decl::is_nested (void)
{
  be_decl *d;

  d = be_scope::narrow_from_scope (this->defined_in ())->decl ();
  // if we have an outermost scope and if that scope is not that of the Root,
  // then we are defined at some nesting level
  if (d && d->node_type () != AST_Decl::NT_root)
    return I_TRUE;

  return I_FALSE;
}

// return the length in bytes to hold the repoID inside a typecode. This
// comprises 4 bytes indicating the length of the string followed by the actual
// string represented as longs.
long
be_decl::repoID_encap_len (void)
{
  long slen;

  slen = ACE_OS::strlen (this->repoID ()) + 1; // + 1 for NULL terminating char
  // the number of bytes to hold the string must be a multiple of 4 since this
  // will be represented as an array of longs
  return 4 + 4 * (slen/4 + (slen%4 ? 1:0));
}

// return the length in bytes to hold the name inside a typecode. This
// comprises 4 bytes indicating the length of the string followed by the actual
// string represented as longs.
long
be_decl::name_encap_len (void)
{
  long slen;

  slen = ACE_OS::strlen (this->local_name ()->get_string ()) + 1;

  // the number of bytes to hold the string must be a multiple of 4 since this
  // will be represented as an array of longs
  return 4 + 4 * (slen/4 + (slen%4 ? 1:0));
}

// compute the size type of the node in question
int
be_decl::compute_size_type (void)
{
  return 0;
}

// return the scope created by this node (if one exists, else NULL)
be_scope *
be_decl::scope (void)
{
  be_decl *d = this;

   switch (this->node_type()) {
   case AST_Decl::NT_interface_fwd:
      /*
       * Resolve forward declared interface by looking at full_definition()
       * field and iterating
       */
      d = be_interface::narrow_from_decl ((be_interface_fwd::narrow_from_decl
                                           (this))->full_definition ());
      /*
       * Fall through
       */
   case AST_Decl::NT_interface:
      return be_interface::narrow_from_decl (d);
   case AST_Decl::NT_module:
      return be_module::narrow_from_decl (d);
   case AST_Decl::NT_root:
      return be_root::narrow_from_decl (d);
   case AST_Decl::NT_except:
      return be_exception::narrow_from_decl (d);
   case AST_Decl::NT_union:
      return be_union::narrow_from_decl (d);
   case AST_Decl::NT_struct:
      return be_structure::narrow_from_decl (d);
   case AST_Decl::NT_enum:
      return be_enum::narrow_from_decl (d);
   case AST_Decl::NT_op:
      return be_operation::narrow_from_decl (d);
   case AST_Decl::NT_sequence:
      return be_sequence::narrow_from_decl (d);
   default:
      return (be_scope *)0;
   }
}

// boolean methods to test if code was already generated
idl_bool
be_decl::cli_hdr_gen (void)
{
  return this->cli_hdr_gen_;
}

idl_bool
be_decl::cli_stub_gen (void)
{
  return this->cli_stub_gen_;
}

idl_bool
be_decl::cli_hdr_any_op_gen (void)
{
  return this->cli_hdr_any_op_gen_;
}

idl_bool
be_decl::cli_stub_any_op_gen (void)
{
  return this->cli_stub_any_op_gen_;
}

idl_bool
be_decl::cli_hdr_cdr_op_gen (void)
{
  return this->cli_hdr_cdr_op_gen_;
}

idl_bool
be_decl::cli_stub_cdr_op_gen (void)
{
  return this->cli_stub_cdr_op_gen_;
}

idl_bool
be_decl::cli_inline_cdr_op_gen (void)
{
  return this->cli_inline_cdr_op_gen_;
}

idl_bool
be_decl::cli_inline_gen (void)
{
  return this->cli_inline_gen_;
}

idl_bool
be_decl::srv_hdr_gen (void)
{
  return this->srv_hdr_gen_;
}

idl_bool
be_decl::impl_hdr_gen (void)
{
  return this->impl_hdr_gen_;
}

idl_bool
be_decl::srv_skel_gen (void)
{
  return this->srv_skel_gen_;
}

idl_bool
be_decl::impl_skel_gen (void)
{
  return this->impl_skel_gen_;
}

idl_bool
be_decl::srv_inline_gen (void)
{
  return this->srv_inline_gen_;
}

// set the flag indicating that code generation is done
void
be_decl::cli_hdr_gen (idl_bool val)
{
  this->cli_hdr_gen_ = val;
}

void
be_decl::cli_stub_gen (idl_bool val)
{
  this->cli_stub_gen_ = val;
}

void
be_decl::cli_hdr_any_op_gen (idl_bool val)
{
  this->cli_hdr_any_op_gen_ = val;
}

void
be_decl::cli_stub_any_op_gen (idl_bool val)
{
  this->cli_stub_any_op_gen_ = val;
}

void
be_decl::cli_hdr_cdr_op_gen (idl_bool val)
{
  this->cli_hdr_cdr_op_gen_ = val;
}

void
be_decl::cli_stub_cdr_op_gen (idl_bool val)
{
  this->cli_stub_cdr_op_gen_ = val;
}

void
be_decl::cli_inline_cdr_op_gen (idl_bool val)
{
  this->cli_inline_cdr_op_gen_ = val;
}

void
be_decl::cli_inline_gen (idl_bool val)
{
  this->cli_inline_gen_ = val;
}

void
be_decl::srv_hdr_gen (idl_bool val)
{
  this->srv_hdr_gen_ = val;
}

void
be_decl::impl_hdr_gen (idl_bool val)
{
  this->impl_hdr_gen_ = val;
}


void
be_decl::srv_skel_gen (idl_bool val)
{
  this->srv_skel_gen_ = val;
}

void
be_decl::srv_inline_gen (idl_bool val)
{
  this->srv_inline_gen_ = val;
}

int
be_decl::accept (be_visitor *visitor)
{
  return visitor->visit_decl (this);
}

idl_bool
be_decl::is_child (be_decl *node)
{
  if (this->defined_in ())
    {
      be_decl *bd;

      bd = be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (!bd)
        return 0;

      if (!ACE_OS::strcmp (bd->fullname (), node->fullname ()))
        return 1; // true
    }
  return 0; // not a child
}

// narrowing methods
IMPL_NARROW_METHODS1 (be_decl, AST_Decl)
IMPL_NARROW_FROM_DECL (be_decl)
