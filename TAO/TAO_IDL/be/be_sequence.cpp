// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_sequence.cpp
//
// = DESCRIPTION
//    Extension of class AST_Sequence that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_sequence.h"
#include "be_typedef.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_predefined_type.h"
#include "be_visitor.h"
#include "utl_identifier.h"

ACE_RCSID (be, 
           be_sequence, 
           "$Id$")


be_sequence::be_sequence (void)
  : mt_ (be_sequence::MNG_UNKNOWN)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (I_TRUE);
}

be_sequence::be_sequence (AST_Expression *v,
                          AST_Type *t,
                          UTL_ScopedName *n,
                          idl_bool local,
                          idl_bool abstract)
  : be_scope (AST_Decl::NT_sequence),
    be_type (AST_Decl::NT_sequence,
             n),
    be_decl (AST_Decl::NT_sequence,
             n),
    UTL_Scope (AST_Decl::NT_sequence),
    AST_Sequence (v,
                  t,
                  n,
                  t->is_local () || local,
                  abstract),
    AST_ConcreteType (AST_Decl::NT_sequence,
                      n),
    AST_Type (AST_Decl::NT_sequence,
              n),
    AST_Decl (AST_Decl::NT_sequence,
              n,
              I_TRUE),
    COMMON_Base (t->is_local () || local,
                 abstract),
    mt_ (be_sequence::MNG_UNKNOWN)
{
  // A sequence data type is always VARIABLE.
  this->size_type (AST_Type::VARIABLE);

  // Always the case.
  this->has_constructor (I_TRUE);
}

// Helper to create_name.
char *
be_sequence::gen_name (void)
{
  char namebuf [NAMEBUFSIZE];
  be_type *bt = 0; // Base type.

  // Reset the buffer.
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  // Retrieve the base type.
  bt = be_type::narrow_from_decl (this->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_name - "
                         "bad base type\n"),
                        0);
    }

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      // Our base type is an anonymous sequence.
      be_sequence *seq = be_sequence::narrow_from_decl (bt);

      if (seq == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_name - "
                             "error converting base type to sequence\n"),
                            0);
        }

      // Some platforms define IDL sequences as template classes
      // and some do not. If the nested sequence were defined in
      // the scope of the enclosing sequence, we would have to
      // not only define the nested class in two places, but also
      // deal with the fact that, for the template classes, the
      // enclosing sequence's template type is a class defined
      // inside it. So we define the nested sequence in the next
      // scope up, and the existing code generation works for both
      // template and non-template implementations of IDL sequences.
      UTL_Scope *parent = this->defined_in ();
      seq->set_defined_in (parent);
      parent->add_sequence (seq);
      ACE_OS::sprintf (namebuf,
                       "_tao_seq_%s",
                       seq->gen_name ());
    }
  else
    {
      ACE_OS::sprintf (namebuf,
                       "_tao_seq_%s",
                       bt->local_name ()->get_string ());
    }

  // Append the size (if any).
  if (this->unbounded () == I_FALSE)
    {
      char ulval_str [NAMEBUFSIZE];
      ACE_OS::sprintf (ulval_str,
                       "_%lu",
                       this->max_size ()->ev ()->u.ulval);
      ACE_OS::strcat (namebuf, 
                      ulval_str);
    }

  return ACE_OS::strdup (namebuf);
}

// Create a name for ourselves.
int
be_sequence::create_name (be_typedef *node)
{
  static char *namebuf = 0;
  UTL_ScopedName *n = 0;

  // Scope in which we are defined.
  be_decl *scope = 0;

  // If there is a typedef node, we use its name as our name.
  if (node)
    {
      this->set_name (node->name ());
    }
  else
    {
      // Generate a local name.
      namebuf = this->gen_name ();

      // Now see if we have a fully scoped name and if so, generate one.
      UTL_Scope *us = this->defined_in ();

      scope = be_scope::narrow_from_scope (us)->decl ();

      if (scope != 0)
        {
          // Make a copy of the enclosing scope's name.
          n = (UTL_ScopedName *) scope->name ()->copy ();

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier (namebuf),
                          -1);

          UTL_ScopedName *conc_name = 0;
          ACE_NEW_RETURN (conc_name,
                          UTL_ScopedName (id,
                                          0),
                          -1);

          // Add our local name as the last component.
          n->nconc (conc_name);

          // Set the fully scoped name.
          this->set_name (n);
        }
      else
        {
          // We better be not here because we must be inside some scope,
          // at least the ROOT scope.
          return -1;
        }

      ACE_OS::free (namebuf);
    }

  return 0;
}

// Does this sequence have a managed type sequence element?
be_sequence::MANAGED_TYPE
be_sequence::managed_type (void)
{
  if (this->mt_ == be_sequence::MNG_UNKNOWN) // Not calculated yet.
    {
      // Base types.
      be_type *bt = 0;
      be_type *prim_type = 0;

      bt = be_type::narrow_from_decl (this->base_type ());

      if (bt->node_type () == AST_Decl::NT_typedef)
        {
          // Get the primitive base type of this typedef node.
          be_typedef *t = be_typedef::narrow_from_decl (bt);
          prim_type = t->primitive_base_type ();
        }
      else
        {
          prim_type = bt;
        }

      // Determine if we need a managed type and which one.
      switch (prim_type->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          {
            int is_valuetype = 0;
            be_interface *bf = be_interface::narrow_from_decl (prim_type);

            if (bf != 0)
              {
                is_valuetype = bf->is_valuetype ();
              }
            else
              {
                be_interface_fwd *bff = 
                  be_interface_fwd::narrow_from_decl (prim_type);

                if (bff != 0)
                  {
                    is_valuetype = bff->is_valuetype ();
                  }
              }
            if (is_valuetype)
              {
                this->mt_ = be_sequence::MNG_VALUE;
              }
            else
              {
                this->mt_ = be_sequence::MNG_OBJREF;
              }

          break;
          }
        case AST_Decl::NT_string:
          this->mt_ = be_sequence::MNG_STRING;
          break;
        case AST_Decl::NT_wstring:
          this->mt_ = be_sequence::MNG_WSTRING;
          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *bpd =
              be_predefined_type::narrow_from_decl (prim_type);
            AST_PredefinedType::PredefinedType pt = bpd->pt ();

            if (pt == AST_PredefinedType::PT_pseudo)
              {
                this->mt_ = be_sequence::MNG_PSEUDO;
              }
            else if (pt == AST_PredefinedType::PT_object)
              {
                this->mt_ = be_sequence::MNG_OBJREF;
              }
            else
              {
                this->mt_ = be_sequence::MNG_NONE;
              }
          }
          break;
        default:
          this->mt_ = be_sequence::MNG_NONE;
        }
    }

  return this->mt_;
}

// Add this be_sequence to the locally defined types in this scope
AST_Sequence *
be_sequence::fe_add_sequence (AST_Sequence *t)
{
  if (t == 0)
    {
      return 0;
    }

  this->add_to_local_types(t);
  return t;
}

// Overridden method
be_decl *
be_sequence::decl (void)
{
  return this;
}

int
be_sequence::accept (be_visitor *visitor)
{
  return visitor->visit_sequence (this);
}


const char *
be_sequence::instance_name ()
{
  static char namebuf[NAMEBUFSIZE];
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  be_type *bt = 0;
  bt = be_type::narrow_from_decl (this->base_type ());

  if (bt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_visitor_sequence_ch::"
                  "gen_instantiate_name - "
                  "Bad element type\n"));

      return namebuf;
    }

  // Generate the class name.

  // The base type after removing all the aliases.
  be_type  *prim_type = bt;

  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // Get the primitive base type of this typedef node.
      be_typedef *t = be_typedef::narrow_from_decl (bt);
      prim_type = t->primitive_base_type ();
    }

  // Generate the appropriate sequence type.
  switch (this->managed_type ())
    {
    case be_sequence::MNG_PSEUDO:
    case be_sequence::MNG_OBJREF:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Unbounded_Object_Sequence_%s",
                           this->flat_name ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Object_Sequence_%s_%lu",
                           this->flat_name (),
                           this->max_size ()->ev ()->u.ulval);
        }
      break;
    case be_sequence::MNG_VALUE:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Unbounded_Valuetype_Sequence_%s",
                           this->flat_name ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Valuetype_Sequence_%s_%lu",
                           this->flat_name (),
                           this->max_size ()->ev ()->u.ulval);
        }
      break;
    case be_sequence::MNG_STRING:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "TAO_Unbounded_String_Sequence");
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_String_Sequence_%s",
                           this->flat_name  ());
        }
      break;
    case be_sequence::MNG_WSTRING:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "TAO_Unbounded_WString_Sequence");
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_WString_Sequence_%s",
                           this->flat_name ());
        }
      break;
    default: // Not a managed type.
      if (this->unbounded ())
        {
          // TAO provides extensions for octet sequences, first find out
          // if the base type is an octet (or an alias for octet)
          be_predefined_type *predef =
            be_predefined_type::narrow_from_decl (prim_type);

          if (predef != 0
              && predef->pt() == AST_PredefinedType::PT_octet)
            {
              ACE_OS::sprintf (namebuf,
                               "TAO_Unbounded_Sequence<CORBA::Octet>");
            }
          else
            {
              ACE_OS::sprintf (namebuf,
                               "_TAO_Unbounded_Sequence_%s",
                               this->flat_name ());
            }
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Sequence_%s_%lu",
                            this->flat_name (),
                            this->max_size ()->ev ()->u.ulval);
        }
      break;
    }

  return namebuf;
}

void
be_sequence::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
}

// Narrowing
IMPL_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)
