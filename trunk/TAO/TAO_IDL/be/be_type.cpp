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
  if (!this->tc_name_)
    compute_tc_name ();

  return this->tc_name_;
}

// return the type name using the ACE_NESTING macro
char *
be_type::nested_type_name (be_decl *d, char *suffix)
{
  // some compilers do not like generating a fully scoped name for a type that
  // was defined in the same enclosing scope in which it was defined. For such,
  // we emit a macro defined in the ACE library.
  //

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well. If that is the case, then we can generate a fully
  // scoped name for that type, else we use the ACE_NESTED_CLASS macro

  static char macro [NAMEBUFSIZE];
  be_decl *t = 0;  // our enclosing scope

  // d : This is the node in whose scope we are generating a declaration
  // t : This is our enclosing scope (if one exists)
  //

  ACE_OS::memset (macro, '\0', NAMEBUFSIZE);
  if (this->is_nested ()) // if we are nested
    {
	  // get our enclosing scope
	  t = be_decl::narrow_from_decl (ScopeAsDecl (this->defined_in ()));

	  // now check if the scope in which we were defined is the same
	  // as the current scope in which we are used or one of its ancestors
	  while (d->node_type () != AST_Decl::NT_root) // keep moving up
                                            	   // the chain
		{
		  // now we need to make sure that "t" is not the same as "d" i.e., the
		  // scope in which we are using ourselves.
		  if (!ACE_OS::strcmp (t->fullname (), d->fullname ()))
			{
			  // we are the same, generate the ACE_NESTED_CLASS macro
			  ACE_OS::sprintf (macro, "ACE_NESTED_CLASS (");
			  ACE_OS::strcat (macro, t->fullname ());
			  ACE_OS::strcat (macro, ",");
			  ACE_OS::strcat (macro, this->local_name ()->get_string ());
              if (suffix)
                {
                  ACE_OS::strcat (macro, suffix);
                }
			  ACE_OS::strcat (macro, ")");
			  return macro;
			}
		  d = be_decl::narrow_from_decl (ScopeAsDecl (d->defined_in ()));
		}
	} // end of if is_nested

  // not nested OR not defined in the same scope as "d" or its
  // ancestors
  ACE_OS::sprintf (macro, "%s", this->fullname ());
  if (suffix)
    {
      ACE_OS::strcat (macro, suffix);
    }
  return macro;
}

// Narrowing
IMPL_NARROW_METHODS2 (be_type, AST_Type, be_decl)
IMPL_NARROW_FROM_DECL (be_type)
