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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_sequence.h"

ACE_RCSID(be, be_sequence, "$Id$")


be_sequence::be_sequence (void)
  : mt_ (be_sequence::MNG_UNKNOWN)
{
  // Always the case.
  this->size_type (be_decl::VARIABLE);
  this->has_constructor (I_TRUE);
}

be_sequence::be_sequence (AST_Expression *v,
                          AST_Type *t,
                          idl_bool local,
                          idl_bool abstract)
  : AST_Sequence (v, 
                  t, 
                  t->is_local () || local, 
                  abstract),
    AST_Decl (AST_Decl::NT_sequence,
              0,
              0),
    COMMON_Base (t->is_local () || local, 
                 abstract),
    mt_ (be_sequence::MNG_UNKNOWN)
{
  // Check if we are bounded or unbounded. An expression value of 0 means
  // unbounded.
  if (v->ev ()->u.ulval == 0)
    {
      this->unbounded_ = I_TRUE;
    }
  else
    {
      this->unbounded_ = I_FALSE;
    }

  // A sequence data type is always VARIABLE.
  this->size_type (be_decl::VARIABLE);

  // Always the case.
  this->has_constructor (I_TRUE);
}

idl_bool
be_sequence::unbounded (void) const
{
  return this->unbounded_;
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
  if (this->unbounded_ == 0)
    {
      ACE_OS::sprintf (namebuf,
                       "%s_%lu",
                       namebuf,
                       this->max_size ()->ev ()->u.ulval);
    }

  return ACE_OS::strdup (namebuf);
}

// Create a name for ourselves.
int
be_sequence::create_name (be_typedef *node)
{
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n = 0;

  // Scope in which we are defined.
  be_decl *scope = 0;

  // If there is a typedef node, we use its name as our name.
  if (node)
    {
      n = (UTL_ScopedName *)node->name ()->copy ();
      this->set_name (n);
    }
  else
    {
      // Reset the buffer.
      ACE_OS::memset (namebuf, 
                      '\0', 
                      NAMEBUFSIZE);

      // Generate a local name.
      ACE_OS::strcpy (namebuf, 
                      this->gen_name ());

      // Now see if we have a fully scoped name and if so, generate one.
      UTL_Scope *us = this->defined_in ();

      scope = be_scope::narrow_from_scope (us)->decl ();

      if (scope != 0)
        {
          // Make a copy of the enclosing scope's name.
          n = (UTL_ScopedName *) scope->name ()->copy ();

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier (ACE_OS::strdup (namebuf),
                                      1,
                                      0,
                                      I_FALSE),
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
          this->mt_ = be_sequence::MNG_OBJREF;
          break;
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

            if (bpd->pt () == AST_PredefinedType::PT_pseudo)
              {
                // If this pseudo is a CORBA::Object, then the managed type is
                // an objref.
                if (!ACE_OS::strcmp (bpd->local_name ()->get_string (),
                                     "Object"))
                  {
                    this->mt_ = be_sequence::MNG_OBJREF;
                  }
                else
                  {
                    this->mt_ = be_sequence::MNG_PSEUDO;
                  }
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

idl_bool
be_sequence::in_recursion (be_type *node)
{
  if (node == 0)
    {
      // There has to be a parameter
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_sequence::")
                         ACE_TEXT ("in_recursion - ")
                         ACE_TEXT ("bad parameter node\n")),
                        0);
    }

  be_type *type = be_type::narrow_from_decl (this->base_type ());

  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_sequence::")
                         ACE_TEXT ("in_recursion - ")
                         ACE_TEXT ("bad base type\n")),
                        0);
    }

  if (!ACE_OS::strcmp (node->full_name (), 
                       type->full_name ()))
    {
      // They match.
      return 1;
    }
  else
    {
      // Not in recursion.
      return 0;
    }
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
