// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_type.cpp
//
// = DESCRIPTION
//    Extension of class AST_Type that provides additional means for C++
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

/*
 * BE_Type
 */

be_type::be_type (void)
{
}

be_type::be_type (AST_Decl::NodeType nt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (nt, n, p)
{
}

// compute the typecode name. The idea is to use the fully scoped name,
// however, prepend a _tc_ to the last component. A slightly different approach
// is required of the predefined types. Hence this method is overridden for
// predefined types.

void
be_type::compute_tc_name (void)
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
be_type::tc_name (void)
{
  return this->tc_name_;
}

// return the type name using the ACE_NESTING macro
char *
be_type::nested_type_name (be_decl *d)
{
  // some compilers do not like generating a fully scoped name for a type that
  // was defined in the same enclosing scope in which it was defined. For such,
  // we emit a macro defined in the ACE library.
  //
  // However, this is not straightforward. A type may have been defined in one
  // of our ancestor scopes  in which case even that type will not have a fully
  // scoped name

  static char macro [TAO_CodeGen::MAXNAMELEN];
  be_decl *bd = 0;  // enclosing scope

  // d : This is the node in whose scope we are generating a declaration
  // bd: This is the node in whose scope we i.e., the type were defined
  //
  // verify if
  if (this->is_nested ()) // if we are nested
    {
      bd = be_decl::narrow_from_decl (ScopeAsDecl (this->defined_in ()));

      ACE_OS::memset (macro, '\0', TAO_CodeGen::MAXNAMELEN);
      ACE_OS::sprintf (macro, "ACE_NESTED_CLASS (");
      ACE_OS::strcat (macro, bd->fullname ());
      ACE_OS::strcat (macro, ",");
      ACE_OS::strcat (macro, this->local_name ()->get_string ());
      return macro;
    }
  else
    {
      // not nested, return whatever
      return this->fullname ();
    }

}

// Narrowing
IMPL_NARROW_METHODS2 (be_type, AST_Type, be_decl)
IMPL_NARROW_FROM_DECL (be_type)
