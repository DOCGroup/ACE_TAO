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
    fullname_ (0),
    flatname_ (0),
    repoID_ (0),
    size_type_ (be_decl::SIZE_UNKNOWN),
    encap_len_ (-1)
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
    fullname_ (0),
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


#if 0
  // if we are just a typedef, nothing else to do
  if (this->node_type () == AST_Decl::NT_typedef)
    return;

  // update our parent if it is of a specific type
  if (this->is_nested ())
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
          d->size_type (st); // call recursively to set the size type of our
                             // ancestors
        default:
          return; // we are done
        }
    } // end else
#endif
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
      this->repoID_[0] = '\0';
      ACE_OS::sprintf (this->repoID_, "%s", "IDL:");
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

// converts a string name into an array of 4 byte longs
int
be_decl::tc_name2long (const char *name, long *&larr, long &arrlen)
{
  static char buf [NAMEBUFSIZE];
  long slen;
  unsigned int i;

  slen = ACE_OS::strlen (name) + 1; // 1 for NULL terminating

  // compute the number of bytes necessary to hold the name rounded to the next
  // multiple of 4 (i.e., size of long)
  arrlen = slen/4 + (slen%4 ? 1:0);

  ACE_OS::memset (buf, '\0', arrlen*4);
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

  slen = ACE_OS::strlen (this->local_name ()->get_string ()) + 1; // + 1 for
                                                    // NULL terminating char
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

// narrowing methods
IMPL_NARROW_METHODS1 (be_decl, AST_Decl)
IMPL_NARROW_FROM_DECL (be_decl)
