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
    ami_handler_fullname_ (0),
    flatname_ (0),
    ami_handler_flatname_ (0),
    repoID_ (0),
    ami_handler_repoID_ (0),
    prefix_ (0),
    size_type_ (be_decl::SIZE_UNKNOWN),
    has_constructor_ (I_FALSE)
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
    ami_handler_fullname_ (0),
    flatname_ (0),
    ami_handler_flatname_ (0),
    repoID_ (0),
    ami_handler_repoID_ (0),
    prefix_ (0),
    size_type_ (be_decl::SIZE_UNKNOWN),
    has_constructor_ (I_FALSE)
{
}

//destructor
be_decl::~be_decl (void)
{
  if (this->fullname_ != 0)
    {
      delete[] this->fullname_;
      this->fullname_ = 0;
    }
  if (this->ami_handler_fullname_ != 0)
    {
      delete[] this->ami_handler_fullname_;
      this->ami_handler_fullname_ = 0;
    }
  if (this->flatname_ != 0)
    {
      delete[] this->flatname_;
      this->flatname_ = 0;
    }
  if (this->ami_handler_flatname_ != 0)
    {
      delete[] this->ami_handler_flatname_;
      this->ami_handler_flatname_ = 0;
    }
  if (this->repoID_ != 0)
    {
      delete[] this->repoID_;
      this->repoID_ = 0;
    }
  if (this->ami_handler_repoID_ != 0)
    {
      delete[] this->ami_handler_repoID_;
      this->ami_handler_repoID_ = 0;
    }
  if (this->prefix_ != 0)
    {
      delete[] this->prefix_;
      this->prefix_ = 0;
    }
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

const char*
be_decl::fullname (void)
{
  if (!this->fullname_)
    compute_fullname ();

  return this->fullname_;
}

const char*
be_decl::ami_handler_fullname (void)
{
  if (!this->ami_handler_fullname_)
    compute_ami_handler_name (this->fullname (),
                              this->ami_handler_fullname_);

  return this->ami_handler_fullname_;
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
be_decl::flatname (void)
{
  if (!this->flatname_)
    this->compute_flatname ();

  return this->flatname_;
}

const char*
be_decl::ami_handler_flatname (void)
{
  if (!this->ami_handler_flatname_)
    this->compute_flatname ("AMI_","_Handler");

  return this->ami_handler_flatname_;
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



char *
be_decl::compute_flatname  (const char *prefix, const char *suffix)
{
  if (prefix == 0 || suffix == 0)
    return 0;

  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);

  ACE_CString result_str;

  // Get parent.
  if (this->defined_in () == 0)
    {
      // Global scope.

      // Prefix.
      result_str = prefix_str;

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }
  else
    {
      // Get scope name.
      be_decl *parent = be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (parent == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_decl::"
                             "compute_flat_name - "
                             "scope name is nil\n"),
                            0);
        }

      // Parent name.
      result_str = ACE_CString (parent->fullname ());

      // _
      if (ACE_OS::strcmp (parent->fullname (), "") != 0)
        result_str += ACE_CString ("_");

      // Prefix.
      result_str += ACE_CString ("AMI_");

      // Local name.
      result_str += ACE_CString (this->local_name ()->get_string ());

      // Suffix.
      result_str += suffix_str;
    }

  this->ami_handler_flatname_ = result_str.rep ();
  return this->ami_handler_flatname_;
}

const char *
be_decl::repoID (void)
{
  if (!this->repoID_)
    this->compute_repoID ();

  return this->repoID_;
}


const char *
be_decl::ami_handler_repoID (void)
{
  if (!this->ami_handler_repoID_)
    this->compute_repoID ("AMI_","_Handler");

  return this->ami_handler_repoID_;
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

// Apply the prefix and suffix to the local name and compute the
// repoID.  Both the parameters should be non-null.
char *
be_decl::compute_repoID (const char *prefix, const char *suffix)
{
  // Prefix and suffix should be valid.
  if (prefix == 0 || suffix == 0)
    return 0;

  // First prepare the result without IDL: and  :1.0 strings.

  // repoID without IDL: and :1.0 strings.
  char *result = 0;

  long namelen;
  UTL_IdListActiveIterator *i;
  long first = I_TRUE;
  long second = I_FALSE;

  // In the first loop compute the total length.
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

  // Get the result.

  result = new char [namelen+1];
  result[0] = '\0';

  // Start the result with prefix.
  ACE_OS::sprintf (result, "%s", this->prefix ());

  // Add the "/" only if there is a prefix
  if (ACE_OS::strcmp (this->prefix (), "") != 0)
    ACE_OS::strcat (result, "/");

  i = new UTL_IdListActiveIterator (this->name ());
  first = I_TRUE;
  second = I_FALSE;
  while (!(i->is_done ()))
    {
      if (!first)
        ACE_OS::strcat (result, "/");
      else if (second)
        first = second = I_FALSE;
      // print the identifier
      ACE_OS::strcat (result, i->item ()->get_string ());
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

  // Add prefix and suffix.

  // Search where the last / is. If it is not there put AMI_ right
  // after IDL: and _Handler after that and then the :1.0 string.
  // Otherwise put AMI after the last / and _Handler at the end and
  // then the 1.0 string.

  // CStrings are cool.

  ACE_CString result_str (result);

  // Return val. Release = 0 so that we can return the <c_str>.
  ACE_CString repoID ("IDL:", 0, 0);

  // Useful CStrings.
  ACE_CString prefix_str (prefix);
  ACE_CString suffix_str (suffix);
  ACE_CString version_str (":1.0");

  // Find the last /.
  int last_name_pos = result_str.rfind ('/');

  if (last_name_pos == ACE_CString::npos)
    {
      // '/' not present.

      // Add prefix + result.
      repoID += prefix_str;

      repoID += result_str;

      // Add suffix.
      repoID += suffix_str;
    }
  else
    {
      // '/' present.

      // IDl:<result except the last name>'/'. Watch get the last /
      // also.
      repoID += result_str.substring (0, last_name_pos+1);

      // Put prefix.
      repoID += prefix_str;

      // Put the last name.
      repoID += result_str.substr (last_name_pos + 1);

      // Add suffix.
      repoID += suffix_str;
    }

  // Add 1.0.
  repoID += version_str;

  // Delete result.
  delete result;
  result = 0;

  this->ami_handler_repoID_ = repoID.rep ();

  return this->ami_handler_repoID_;
}


int
be_decl::compute_ami_handler_name (const char *name,
                                        char *&ami_handler_name)
{
  int name_length = ACE_OS::strlen (name);
  int ami_handler_length = ACE_OS::strlen ("AMI__HANDLER");

  ACE_NEW_RETURN (ami_handler_name,
                  char[name_length + ami_handler_length+1],
                  -1);

  // copy it in
  ACE_OS::strcpy (ami_handler_name, name);

  const char *interface_name = 0;
  int i = ACE_OS::strlen (name);
  for (;i >= 1; i--)
    {
      if (name[i-1] == ':' && name[i] == ':')
        {
          interface_name = &name[i+1];
          break;
        }
      else if (i >= 3)
        if (name[i-3] == 'P' &&
            name[i-2] == 'O' &&
            name[i-1] == 'A' &&
            name[i] == '_')
          {
            interface_name = &name[i+1];
            break;
          }
  }

  if (interface_name == 0)
    interface_name = name;

  ACE_OS::strcpy(&ami_handler_name[name_length-ACE_OS::strlen(interface_name)],"AMI_");
  ACE_OS::strcpy(&ami_handler_name[name_length-ACE_OS::strlen(interface_name)+4],
                 interface_name);
  ACE_OS::strcpy(&ami_handler_name[name_length+4],
                 "_Handler");

  return 0;
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

idl_bool
be_decl::has_constructor (void)
{
  return this->has_constructor_;
}

void
be_decl::has_constructor (idl_bool value)
{
  // Similarly to be_decl::size_type_, once this
  // gets set to I_TRUE, we don't want it to
  // change back.
  if (!this->has_constructor_)
    {
      this->has_constructor_ = value;
    }
}

// narrowing methods
IMPL_NARROW_METHODS1 (be_decl, AST_Decl)
IMPL_NARROW_FROM_DECL (be_decl)
