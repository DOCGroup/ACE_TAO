// $Id$

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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_type, "$Id$")


/*
 * BE_Type
 */

be_type::be_type (void)
  : tc_name_ (0),
    type_name_ (0),
    nested_type_name_ (0)
{
}

be_type::be_type (AST_Decl::NodeType nt, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (nt, n, p),
    tc_name_ (0),
    type_name_ (0),
    nested_type_name_ (0)
{
}

be_type::~be_type (void)
{
  if (this->nested_type_name_ != 0)
    {
      delete[] this->nested_type_name_;
      this->nested_type_name_ = 0;
    }
}

// compute the typecode name. The idea is to use the fully scoped name,
// however, prepend a _tc_ to the last component. A slightly different approach
// is required of the predefined types. Hence this method is overridden for
// predefined types.

void
be_type::compute_tc_name (void)
{
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n;

  this->tc_name_ = NULL;
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
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

UTL_ScopedName *
be_type::compute_tc_name (const char *prefix, const char *suffix)
{
  // Both prefix and suffix has to be valid. Else return. 
  if (prefix == 0 || suffix == 0)
    return 0;
    
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n;

  UTL_ScopedName *result = NULL;
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  n = this->name ();
  while (n->tail () != NULL)
    {
      if (!result)
        {
          // does not exist
          result = new UTL_ScopedName (n->head (), NULL);
        }
      else
        {
          result->nconc (new UTL_ScopedName (n->head (), NULL));
        }
      n = (UTL_ScopedName *)n->tail ();
    }

  ACE_OS::sprintf (namebuf,
                   "_tc_%s%s%s",
                   prefix,
                   n->last_component ()->get_string (),
                   suffix);
  
  if (!result)
    {
      // does not exist
      result = new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                           (namebuf), 1, 0, I_FALSE), NULL);
    }
  else
    {
      result->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                                 (namebuf), 1,
                                                                 0, I_FALSE), NULL));
    }

  return result;
}

// retrieve typecode name
UTL_ScopedName *
be_type::tc_name (const char *prefix, const char *suffix)
{
  if (prefix != 0 && suffix != 0)
    {
      // Just compute and return the name.
      return compute_tc_name (prefix, suffix);
    }

  // Compute and init the member.
  if (!this->tc_name_)
    compute_tc_name ();

  return this->tc_name_;
}

// This code works. However, whether we should generate the
// ACE_NESTED_CLASS macro or not should be based on an option to the
// compiler. The previous version generated a relative path.
// This version always generates ACE_NESTED_CLASS, (leave ace/ACE.h and friends
// do the porting)
//
// caution: returns the same buffer pointer even if the contents may change
// in the next call.  (return std::string anyone?)
//
// return the type name using the ACE_NESTED_CLASS macro

const char *
be_type::nested_type_name (be_decl *use_scope, 
                           const char *suffix,
                           const char *prefix)
{
  return nested_name (this->local_name()->get_string(),
                     this->full_name(),
                     use_scope,
                     suffix,
                     prefix);
}

// This works for the "special" names generated for smart proxy
// classes. The form of these names is
// scope'TAO_'+flat_name+'_Smart_Proxy_Base'.
//
// We can use the flat_name() method for the local_name parm but have
// to construct the full name ourselves.
const char *
be_type::nested_sp_type_name (be_decl *use_scope, 
                              const char *suffix, 
                              const char *prefix)
{
  be_decl *fu_scope;  // our defining scope
  char fu_name [NAMEBUFSIZE],fl_name[NAMEBUFSIZE];
  
  ACE_OS::memset (fu_name, '\0', NAMEBUFSIZE);
  ACE_OS::memset (fl_name, '\0', NAMEBUFSIZE);
  
  fu_scope = ((this->defined_in ())?
              (be_scope::narrow_from_scope (this->defined_in ())->decl ()):
              0);
  
  ACE_OS::strcat (fu_name, fu_scope->full_name ());
  ACE_OS::strcat (fu_name, "::TAO_");
  ACE_OS::strcat (fu_name, this->flat_name());
  
  ACE_OS::strcat (fl_name, "TAO_");
  ACE_OS::strcat (fl_name, this->flat_name());
  
  return nested_name(fl_name, 
                     fu_name,
                     use_scope, 
                     suffix, 
                     prefix);
}

// This is the real thing used by the two other methods above
const char *
be_type::nested_name (const char* local_name,
                      const char* full_name,
                      be_decl *use_scope, 
                      const char *suffix, 
                      const char *prefix)
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

  if (this->nested_type_name_ == 0)
    ACE_NEW_RETURN (this->nested_type_name_, char[NAMEBUFSIZE], 0);

  be_decl *def_scope = 0;  // our defining scope
  char // hold the fully scoped name
    def_name [NAMEBUFSIZE],
    use_name [NAMEBUFSIZE];
  char // these point to the prev, curr and next component in the scope
    *def_curr = def_name,
    *def_next = 0,
    *use_curr = use_name,
    *use_next = 0;

  int len_to_match = 0; // how many chars to compare

  // initialize the buffers
  ACE_OS::memset (this->nested_type_name_, '\0', NAMEBUFSIZE);
  ACE_OS::memset (def_name, '\0', NAMEBUFSIZE);
  ACE_OS::memset (use_name, '\0', NAMEBUFSIZE);

  // traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed. This forms the first argument to the ACE_NESTED_CLASS
  // macro. Whenever there is no match, the remaining components of the
  // def_scope form the second argument.

  def_scope = ((this->defined_in ())?
               (be_scope::narrow_from_scope (this->defined_in ())->decl ()):
               0);

  if (def_scope && def_scope->node_type () != AST_Decl::NT_root && use_scope)
    // if both scopes exist and that we are not in the root scope
    {
      ACE_OS::strcpy (def_name, def_scope->full_name ());
      ACE_OS::strcpy (use_name, use_scope->full_name ());

      // find the first occurrence of a :: and advance the next pointers accordingly
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      // If the scopes are identical, don't supply them.
      if(!ACE_OS::strcmp (def_name, use_name))
        {
          if (prefix)
            ACE_OS::strcat (this->nested_type_name_, prefix);

          ACE_OS::strcat (this->nested_type_name_, 
                          local_name);
          if (suffix)
            ACE_OS::strcat (this->nested_type_name_, suffix);
 
          return this->nested_type_name_;
        }
       
      if (def_next)
        len_to_match = ACE_OS::strlen (def_curr) 
          - ACE_OS::strlen (def_next);
      else
        len_to_match = ACE_OS::strlen (def_curr);

      if (use_next)
        {
          int len  = ACE_OS::strlen (use_curr) 
            - ACE_OS::strlen (use_next);
          if (len > len_to_match)
            len_to_match = len;
        }
      else
        {
          int len = ACE_OS::strlen (use_curr);
          if (len > len_to_match)
            len_to_match = len;
        }
      
      if (!ACE_OS::strncmp (def_curr, use_curr, len_to_match))
        {
          // initial prefix matches i.e., they have a common root
          // start by initializing the macro

          ACE_OS::sprintf (this->nested_type_name_, "ACE_NESTED_CLASS (");
          ACE_OS::strncat (this->nested_type_name_, 
                           def_curr, 
                           len_to_match); // initialize the first argument 

          // shift the curr scopes to the next level
          def_curr = (def_next ? (def_next + 2) : 0); // skip the ::
          use_curr = (use_next ? (use_next + 2) : 0); // skip the ::

          while (def_curr && use_curr)
            {
              // find the first occurrence of a :: and advance the next pointers accordingly
              def_next = ACE_OS::strstr (def_curr, "::");
              use_next = ACE_OS::strstr (use_curr, "::");

              if (def_next)
                len_to_match = ACE_OS::strlen (def_curr) 
                  - ACE_OS::strlen (def_next);
              else
                len_to_match = ACE_OS::strlen (def_curr);

              if (use_next)
                {
                  int len  = ACE_OS::strlen (use_curr) 
                    - ACE_OS::strlen (use_next);
                  if (len > len_to_match)
                    len_to_match = len;
                }
              else
                {
                  int len = ACE_OS::strlen (use_curr);
                  if (len > len_to_match)
                    len_to_match = len;
                }
      
              if (!ACE_OS::strncmp (def_curr, use_curr, len_to_match))
                {
                  // they have same prefix, append to arg1
                  ACE_OS::strcat (this->nested_type_name_, "::");
                  ACE_OS::strncat (this->nested_type_name_, 
                                   def_curr, 
                                   len_to_match); 
                  def_curr = (def_next ? (def_next + 2) : 0); // skip the ::
                  use_curr = (use_next ? (use_next + 2) : 0); // skip the ::
                }
              else
                {
                  // no match. This is the end of the first argument. Get out
                  // of the loop as no more comparisons are necessary
                  break;
                }
            }

          // start the 2nd argument of the macro
          ACE_OS::strcat (this->nested_type_name_, ", ");

          // copy the remaining def_name (if any left)
          if (def_curr)
            {
              ACE_OS::strcat (this->nested_type_name_, def_curr);
              ACE_OS::strcat (this->nested_type_name_, "::");
            }

          // append our local name
          if (prefix)
            ACE_OS::strcat (this->nested_type_name_, prefix);
          ACE_OS::strcat (this->nested_type_name_, local_name);
          if (suffix)
            ACE_OS::strcat (this->nested_type_name_, suffix);
          ACE_OS::strcat (this->nested_type_name_, ")");
          return this->nested_type_name_;
        } // end of if the root prefixes match
    }

  // otherwise just emit our full_name
  if (prefix)
    ACE_OS::strcat (this->nested_type_name_, prefix);
  ACE_OS::strcat (this->nested_type_name_, full_name);
  if (suffix)
    ACE_OS::strcat (this->nested_type_name_, suffix);

  return this->nested_type_name_;
}

// *****************************
// CODE GENERATION
// *****************************

// generate the _var definition for ourself
int
be_type::gen_var_defn (char *)
{
  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_type::gen_var_impl (char *, char *)
{
  return 0;
}

// generate the _out definition
int
be_type::gen_out_defn (char *)
{
  return 0;
}

int
be_type::gen_out_impl (char *, char *)
{
  return 0;
}

AST_Decl::NodeType
be_type::base_node_type (void) const
{
  return ACE_const_cast(be_type*, this)->node_type ();
}

idl_bool
be_type::in_recursion (be_type *)
{
  // be default we are not involved in recursion
  return 0;
}

int
be_type::accept (be_visitor *visitor)
{
  return visitor->visit_type (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_type, AST_Type, be_decl)
IMPL_NARROW_FROM_DECL (be_type)
