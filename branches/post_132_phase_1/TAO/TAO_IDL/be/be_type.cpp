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

#include "be_type.h"
#include "be_scope.h"
#include "be_visitor.h"
#include "be_codegen.h"
#include "utl_identifier.h"
#include "idl_defines.h"

ACE_RCSID (be, 
           be_type, 
           "$Id$")

be_type::be_type (void)
  : tc_name_ (0)
{
}

be_type::be_type (AST_Decl::NodeType nt,
                  UTL_ScopedName *n)
  : be_decl (nt,
             n),
    AST_Type (nt,
              n),
    AST_Decl (nt,
              n),
    tc_name_ (0)
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
      delete [] this->nested_type_name_;
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
