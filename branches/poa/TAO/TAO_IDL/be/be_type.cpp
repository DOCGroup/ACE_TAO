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
  : tc_name_ (0),
    type_name_ (0)
{
}

be_type::be_type (AST_Decl::NodeType nt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (nt, n, p),
    tc_name_ (0),
    type_name_ (0)
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

// XXXASG - This code works. However, whether we should generate the
// ACE_NESTED_CLASS macro or not should be based on an option to the
// compiler. In this version, we choose to generate a relative path.

// return the type name using the ACE_NESTED_CLASS macro
char *
be_type::nested_type_name (be_decl *use_scope, char *suffix)
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

  // thus we need some sort of relative name to be generated

  static char macro [NAMEBUFSIZE];
  be_decl *def_scope = 0;  // our defining scope
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

  def_scope = ((this->defined_in ())?
               (be_scope::narrow_from_scope (this->defined_in ())->decl ()):
               0);

  if (def_scope && def_scope->node_type () != AST_Decl::NT_root && use_scope)
    // if both scopes exist and that we are not in the root scope
    {
      ACE_OS::strcpy (def_name, def_scope->fullname ());
      ACE_OS::strcpy (use_name, use_scope->fullname ());

      // find the first occurrence of a :: and advance the next pointers accordingly
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      if (def_next)
        *def_next = 0;

      if (use_next)
        *use_next = 0;

      if (!ACE_OS::strcmp (def_curr, use_curr))
        {
          // initial prefix matches i.e., they have a common root
          // start by initializing the macro

          //@@          ACE_OS::sprintf (macro, "ACE_NESTED_CLASS (");
          //@@          ACE_OS::strcat (macro, def_curr); // initialize the first argument

          def_curr = (def_next ? (def_next+2) : 0); // skip the ::
          use_curr = (use_next ? (use_next+2) : 0); // skip the ::

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
                  //@@    ACE_OS::strcat (macro, "::");
                  //@@ ACE_OS::strcat (macro, def_curr);
                  def_curr = (def_next ? (def_next+2) : 0); // skip the ::
                  use_curr = (use_next ? (use_next+2) : 0); // skip the ::
                }
              else
                {
                  // no match. This is the end of the first argument. Get out
                  // of the loop as no more comparisons are necessary
                  break;
                }
            }

          // start the 2nd argument of the macro
          //@@          ACE_OS::strcat (macro, ", ");

          // copy the remaining def_name (if any left)
          if (def_curr)
            ACE_OS::strcat (macro, def_curr);

          // append our local name
          ACE_OS::strcat (macro, this->local_name ()->get_string ());
          if (suffix)
            ACE_OS::strcat (macro, suffix);
          //@@          ACE_OS::strcat (macro, ")");
          return macro;
        } // end of if the root prefixes match
    }

  // otherwise just emit our fullname
  ACE_OS::sprintf (macro, this->fullname ());
  if (suffix)
    ACE_OS::strcat (macro, suffix);

  return macro;
}

// *****************************
// CODE GENERATION
// *****************************

// generate the _var definition for ourself
int
be_type::gen_var_defn (void)
{
  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_type::gen_var_impl (void)
{
  return 0;
}

// generate the _out definition
int
be_type::gen_out_defn (void)
{
  return 0;
}

int
be_type::gen_out_impl (void)
{
  return 0;
}

AST_Decl::NodeType be_type::base_node_type (void) const
{
  return ACE_const_cast(be_type*, this)->node_type ();
}

int be_type::write_as_return (TAO_OutStream *, be_type *)
{
  return -1;
}

// Visiting methods
int be_type::accept (be_visitor *visitor)
{
  return visitor->visit_type (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_type, AST_Type, be_decl)
IMPL_NARROW_FROM_DECL (be_type)
