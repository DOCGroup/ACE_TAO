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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_type, "$Id$")

be_type::be_type (void)
  : tc_name_ (0),
    nested_type_name_ (0)
{
}

be_type::be_type (AST_Decl::NodeType nt, 
                  UTL_ScopedName *n)
  : AST_Decl (nt, 
              n),
    tc_name_ (0),
    nested_type_name_ (0)
{
}

be_type::~be_type (void)
{
}

// Compute the typecode name. The idea is to use the fully scoped name,
// however, prepend a _tc_ to the last component. A slightly different approach
// is required of the predefined types. Hence this method is overridden for
// predefined types.

void
be_type::compute_tc_name (void)
{
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n = this->name ();

  this->tc_name_ = 0;

  ACE_OS::memset (namebuf, 
                  '\0', 
                  NAMEBUFSIZE);

  while (n->tail () != 0)
    {
      // Does not exist.
      if (this->tc_name_ == 0)
        {
          ACE_NEW (this->tc_name_,
                   UTL_ScopedName (n->head ()->copy (),
                                   0));
        }
      else
        {
          UTL_ScopedName *conc_name = 0;
          ACE_NEW (conc_name,
                   UTL_ScopedName (n->head ()->copy (),
                                   0));

          this->tc_name_->nconc (conc_name);
        }

      n = (UTL_ScopedName *)n->tail ();
    }

  ACE_OS::sprintf (namebuf, 
                   "_tc_%s", 
                   n->last_component ()->get_string ());

  Identifier *id = 0;
  ACE_NEW (id,
           Identifier (namebuf));

  // Does not exist.
  if (this->tc_name_ == 0)
    {
      ACE_NEW (this->tc_name_,
               UTL_ScopedName (id,
                               0));
    }
  else
    {
      UTL_ScopedName *conc_name = 0;
      ACE_NEW (conc_name,
               UTL_ScopedName (id,
                               0));

      this->tc_name_->nconc (conc_name);
    }
}

// Retrieve typecode name.
UTL_ScopedName *
be_type::tc_name (void)
{
  // Compute and init the member.
  if (this->tc_name_ == 0)
    {
      this->compute_tc_name ();
    }

  return this->tc_name_;
}

// This code works. However, whether we should generate the
// ACE_NESTED_CLASS macro or not should be based on an option to the
// compiler. The previous version generated a relative path.
// This version always generates ACE_NESTED_CLASS, (leave ace/ACE.h and friends
// do the porting)
//
// Caution: returns the same buffer pointer even if the contents may change
// in the next call.  (return std::string anyone?)
//
// Return the type name using the ACE_NESTED_CLASS macro

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
  // Our defining scope.
  be_decl *fu_scope = 0;

  char fu_name [NAMEBUFSIZE];
  char fl_name [NAMEBUFSIZE];
  
  ACE_OS::memset (fu_name, 
                  '\0', 
                  NAMEBUFSIZE);

  ACE_OS::memset (fl_name, 
                  '\0', 
                  NAMEBUFSIZE);
  
  fu_scope = this->defined_in ()
               ? be_scope::narrow_from_scope (this->defined_in ())->decl ()
               : 0;
  
  ACE_OS::strcat (fu_name, 
                  fu_scope->full_name ());

  ACE_OS::strcat (fu_name, 
                  "::TAO_");

  ACE_OS::strcat (fu_name, 
                  this->flat_name());
  
  ACE_OS::strcat (fl_name, 
                  "TAO_");

  ACE_OS::strcat (fl_name, 
                  this->flat_name());
  
  return this->nested_name (fl_name, 
                            fu_name,
                            use_scope, 
                            suffix, 
                            prefix);
}

// This is the real thing used by the two other methods above.
const char *
be_type::nested_name (const char* local_name,
                      const char* full_name,
                      be_decl *use_scope, 
                      const char *suffix, 
                      const char *prefix)
{
  // Some compilers do not like generating a fully scoped name for a type that
  // was defined in the same enclosing scope in which it was defined. For such,
  // we emit a macro defined in the ACE library.
  //

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well. If that is the case, then we can generate a fully
  // scoped name for that type, else we use the ACE_NESTED_CLASS macro.

  // Thus we need some sort of relative name to be generated.

  if (this->nested_type_name_ == 0)
    {
      ACE_NEW_RETURN (this->nested_type_name_, 
                      char[NAMEBUFSIZE], 
                      0);
    }

  // Our defining scope.
  be_decl *def_scope = 0;

  // Hold the fully scoped name.
  char def_name [NAMEBUFSIZE];
  char use_name [NAMEBUFSIZE];

  // These point to the prev, curr and next component in the scope.
  char *def_curr = def_name;
  char *def_next = 0;
  char *use_curr = use_name;
  char *use_next = 0;

  // How many chars to compare.
  int len_to_match = 0;

  // Initialize the buffers.
  ACE_OS::memset (this->nested_type_name_, 
                  '\0', 
                  NAMEBUFSIZE);

  ACE_OS::memset (def_name, 
                  '\0', 
                  NAMEBUFSIZE);

  ACE_OS::memset (use_name, 
                  '\0', 
                  NAMEBUFSIZE);

  // Traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed. This forms the first argument to the ACE_NESTED_CLASS
  // macro. Whenever there is no match, the remaining components of the
  // def_scope form the second argument.

  def_scope = this->defined_in ()
                ? be_scope::narrow_from_scope (this->defined_in ())->decl ()
                : 0;

  if (def_scope 
      && def_scope->node_type () != AST_Decl::NT_root 
      && use_scope)
    // If both scopes exist and that we are not in the root scope.
    {
      ACE_OS::strcpy (def_name, 
                      def_scope->full_name ());

      ACE_OS::strcpy (use_name, 
                      use_scope->full_name ());

      // Find the first occurrence of a :: and advance 
      // the next pointers accordingly.
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      // If the scopes are identical, don't supply them.
      if  (ACE_OS::strcmp (def_name, 
                         use_name)
             == 0)
        {
          if (prefix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_, 
                              prefix);
            }

          ACE_OS::strcat (this->nested_type_name_, 
                          local_name);
          if (suffix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_, 
                              suffix);
            }
 
          return this->nested_type_name_;
        }
       
      if (def_next != 0)
        {
          len_to_match = 
            ACE_OS::strlen (def_curr) - ACE_OS::strlen (def_next);
        }
      else
        {
          len_to_match = ACE_OS::strlen (def_curr);
        }

      if (use_next != 0)
        {
          int len = 
            ACE_OS::strlen (use_curr) - ACE_OS::strlen (use_next);

          if (len > len_to_match)
            {
              len_to_match = len;
            }
        }
      else
        {
          int len = ACE_OS::strlen (use_curr);

          if (len > len_to_match)
            {
              len_to_match = len;
            }
        }
      
      if (ACE_OS::strncmp (def_curr, 
                           use_curr, 
                           len_to_match)
            == 0)
        {
          // Initial prefix matches i.e., they have a common root.
          // Start by initializing the macro.
          ACE_OS::sprintf (this->nested_type_name_, 
                           "ACE_NESTED_CLASS (");

          // Initialize the first argument.
          ACE_OS::strncat (this->nested_type_name_, 
                           def_curr, 
                           len_to_match);

          // Shift the current scopes to the next level.
          def_curr = (def_next ? (def_next + 2) : 0); // Skip the ::
          use_curr = (use_next ? (use_next + 2) : 0); // Skip the ::

          while (def_curr && use_curr)
            {
              // Find the first occurrence of a :: and advance the 
              // next pointers accordingly.
              def_next = ACE_OS::strstr (def_curr, "::");
              use_next = ACE_OS::strstr (use_curr, "::");

              if (def_next != 0)
                {
                  len_to_match = 
                    ACE_OS::strlen (def_curr) - ACE_OS::strlen (def_next);
                }
              else
                {
                  len_to_match = ACE_OS::strlen (def_curr);
                }

              if (use_next != 0)
                {
                  int len  = 
                    ACE_OS::strlen (use_curr) - ACE_OS::strlen (use_next);

                  if (len > len_to_match)
                    {
                      len_to_match = len;
                    }
                }
              else
                {
                  int len = ACE_OS::strlen (use_curr);

                  if (len > len_to_match)
                    {
                      len_to_match = len;
                    }
                }
      
              if (ACE_OS::strncmp (def_curr, 
                                   use_curr,
                                   len_to_match)
                    == 0)
                {
                  // They have same prefix, append to arg1.
                  ACE_OS::strcat (this->nested_type_name_, 
                                  "::");

                  ACE_OS::strncat (this->nested_type_name_, 
                                   def_curr, 
                                   len_to_match);

                  def_curr = (def_next ? (def_next + 2) : 0); // Skip the ::
                  use_curr = (use_next ? (use_next + 2) : 0); // Skip the ::
                }
              else
                {
                  // No match. This is the end of the first argument. Get out
                  // of the loop as no more comparisons are necessary.
                  break;
                }
            }

          // Start the 2nd argument of the macro.
          ACE_OS::strcat (this->nested_type_name_, ", ");

          // Copy the remaining def_name (if any are left).
          if (def_curr != 0)
            {
              ACE_OS::strcat (this->nested_type_name_, 
                              def_curr);

              ACE_OS::strcat (this->nested_type_name_, 
                              "::");
            }

          // Append our local name.
          if (prefix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_, prefix);
            }

          ACE_OS::strcat (this->nested_type_name_, 
                          local_name);

          if (suffix != 0)
            {
              ACE_OS::strcat (this->nested_type_name_, 
                              suffix);
            }

          ACE_OS::strcat (this->nested_type_name_, 
                          ")");

          return this->nested_type_name_;
        } // End of if the root prefixes match.
    }

  // Otherwise just emit our full_name.
  if (prefix != 0)
    {
      ACE_OS::strcat (this->nested_type_name_, prefix);
    }

  ACE_OS::strcat (this->nested_type_name_, 
                  full_name);

  if (suffix != 0)
    {
      ACE_OS::strcat (this->nested_type_name_, 
                      suffix);
    }

  return this->nested_type_name_;
}

// *****************************
// CODE GENERATION
// *****************************

// Generate the _var definition for ourself.
int
be_type::gen_var_defn (char *)
{
  return 0;
}

// Implementation of the _var class. All of these get generated
// in the inline file
int
be_type::gen_var_impl (char *, 
                       char *)
{
  return 0;
}

// Generate the _out definition.
int
be_type::gen_out_defn (char *)
{
  return 0;
}

int
be_type::gen_out_impl (char *, 
                       char *)
{
  return 0;
}

AST_Decl::NodeType
be_type::base_node_type (void) const
{
  return ACE_const_cast (be_type*, this)->node_type ();
}

// Cleanup method
void
be_type::destroy (void)
{
  if (this->tc_name_ != 0)
    {
      this->tc_name_->destroy ();
      delete this->tc_name_;
      this->tc_name_ = 0;
    }

  if (this->nested_type_name_ != 0)
    {
      delete this->nested_type_name_;
      this->nested_type_name_ = 0;
    }
}

int
be_type::accept (be_visitor *visitor)
{
  return visitor->visit_type (this);
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_type, AST_Type, be_decl)
IMPL_NARROW_FROM_DECL (be_type)
