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


/*
 * BE_Sequence
 */
be_sequence::be_sequence (void)
  : mt_ (be_sequence::MNG_UNKNOWN)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_sequence::be_sequence (AST_Expression *v, AST_Type *t)
  : AST_Sequence (v, t),
    AST_Decl (AST_Decl::NT_sequence,
              NULL,
              NULL),
    mt_ (be_sequence::MNG_UNKNOWN)
{
  // check if we are bounded or unbounded. An expression value of 0 means
  // unbounded
  if (v->ev ()->u.ulval == 0)
    {
      this->unbounded_ = I_TRUE;
    }
  else
    {
      this->unbounded_ = I_FALSE;
    }

  this->size_type (be_decl::VARIABLE); // a sequence data type is always
                                       // VARIABLE
}

idl_bool
be_sequence::unbounded (void) const
{
  return this->unbounded_;
}

// helper to create_name
char *
be_sequence::gen_name (void)
{
  char namebuf [NAMEBUFSIZE];
  be_type *bt; // base type;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
  // retrieve the base type
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_name - "
                         "bad base type\n"),
                        0);
    }
  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      // our base type is an anonymous sequence
      be_sequence *seq;
      seq = be_sequence::narrow_from_decl (bt);
      if (!seq)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_name - "
                             "error converting base type to sequence\n"),
                            0);
        }
      seq->set_defined_in (this); // set ourselves as its parent
      this->fe_add_sequence (seq); // add the child to our scope
      ACE_OS::sprintf (namebuf, "_tao_seq_%s", seq->gen_name ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "_tao_seq_%s", bt->local_name ()->get_string ());
    }
  // append the size (if any)
  if (!this->unbounded_)
    {
      ACE_OS::sprintf (namebuf, "%s_%d", namebuf, this->max_size ()->ev
                       ()->u.ulval);
    }
  return ACE_OS::strdup (namebuf);
}

// create a name for ourselves
int
be_sequence::create_name (be_typedef *node)
{
  static char namebuf [NAMEBUFSIZE];
  UTL_ScopedName *n = NULL;
  be_decl *scope; // scope in which we are defined

  // if there is a typedef node, we use its name as our name
  if (node)
    {
      n = (UTL_ScopedName *)node->name ()->copy ();
      this->set_name (n); // set our name
    }
  else
    {

      ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);  // reset the buffer
      ACE_OS::strcpy (namebuf, this->gen_name ()); // generate a local name

      // now see if we have a fully scoped name and if so, generate one
      scope = be_scope::narrow_from_scope (this->defined_in ())->decl ();
      if (scope)
        {
          // make a copy of the enclosing scope's  name
          n = (UTL_ScopedName *)scope->name ()->copy () ;

          // add our local name as the last component
          n->nconc (new UTL_ScopedName (new Identifier (ACE_OS::strdup
                                                        (namebuf), 1,
                                                        0, I_FALSE),
                                        NULL));
          // set the fully scoped name
          this->set_name (n);
        }
      else
        {
          // We better be not here because we must be inside some scope,
          // atleast the ROOT scope.
          return -1;
        }
    }
  return 0;
}

// Does this sequence have a managed type sequence element?
be_sequence::MANAGED_TYPE
be_sequence::managed_type (void)
{
  if (this->mt_ == be_sequence::MNG_UNKNOWN) // not calculated yet
    {
      be_type  *bt, *prim_type; // base types

      bt = be_type::narrow_from_decl (this->base_type ());

      if (bt->node_type () == AST_Decl::NT_typedef)
        {
          // get the primitive base type of this typedef node
          be_typedef *t = be_typedef::narrow_from_decl (bt);
          prim_type = t->primitive_base_type ();
        }
      else
        prim_type = bt;

      // determine if we need a managed type and which one
      switch (prim_type->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          this->mt_ = be_sequence::MNG_OBJREF;
          break;
        case AST_Decl::NT_string:
          this->mt_ = be_sequence::MNG_STRING;
          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *bpd = be_predefined_type::narrow_from_decl
              (prim_type);
            if (bpd->pt () == AST_PredefinedType::PT_pseudo)
              {
                // if this pseudo is a CORBA::Object, then the managed type is
                // an objref
                if (!ACE_OS::strcmp (bpd->local_name ()->get_string (),
                                     "Object")) 
                  this->mt_ = be_sequence::MNG_OBJREF;
                else
                  this->mt_ = be_sequence::MNG_PSEUDO;
              }
            else
              {
                this->mt_ = be_sequence::MNG_NONE;
              }
          }
          break;
        default:
          this->mt_ = be_sequence::MNG_NONE;
        } // end of switch
    }
  return this->mt_;
}

/*
 * Add this be_sequence to the locally defined types in this scope
 */
AST_Sequence *
be_sequence::fe_add_sequence (AST_Sequence *t)
{
  if (t == NULL)
    return NULL;

  this->add_to_local_types(t);
  return t;
}

// overridden method
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
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  be_type *bt;
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_DEBUG ((LM_ERROR,
                  "(%N:%l) be_visitor_sequence_ch::"
                  "gen_instantiate_name - "
                  "Bad element type\n"));
      return namebuf;
    }

  // generate the class name

  // the base type after removing all the aliases
  be_type  *prim_type = bt;
  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // get the primitive base type of this typedef node
      be_typedef *t = be_typedef::narrow_from_decl (bt);
      prim_type = t->primitive_base_type ();
    }

  // generate the appropriate sequence type
  switch (this->managed_type ())
    {
    case be_sequence::MNG_PSEUDO:
    case be_sequence::MNG_OBJREF:
      if (this->unbounded ())
        ACE_OS::sprintf (namebuf, 
                         "_TAO_Unbounded_Object_Sequence_%s",
                         this->flatname());
      else
        ACE_OS::sprintf (namebuf, 
                         "_TAO_Bounded_Object_Sequence_%s_%d",
                         this->flatname(),
                         this->max_size ()->ev()->u.ulval);
      break;
    case be_sequence::MNG_STRING: // sequence of strings
      if (this->unbounded ())
        ACE_OS::sprintf (namebuf, 
                         "TAO_Unbounded_String_Sequence");
      else
        ACE_OS::sprintf (namebuf, 
                         "_TAO_Bounded_String_Sequence_%s",
                         this->flatname());
      break;
    default: // not a managed type
      if (this->unbounded ())
	{
	  // TAO provides extensions for octet sequences, first find out
	  // if the base type is an octet (or an alias for octet)
	  be_predefined_type *predef =
	    be_predefined_type::narrow_from_decl (prim_type);
	  if (predef != 0 && 
	      predef->pt() == AST_PredefinedType::PT_octet)
	    ACE_OS::sprintf (namebuf, 
			     "TAO_Unbounded_Sequence<CORBA::Octet>");
	  else
            ACE_OS::sprintf (namebuf, 
                             "_TAO_Unbounded_Sequence_%s",
                             this->flatname());
                             // or prim_type->flatname ());
	  // ACE_DEBUG ((LM_DEBUG, "testing.... %d, %d = <%s>\n",
	  // predef, predef->pt (), namebuf));
	}        
      else
        ACE_OS::sprintf (namebuf, 
                         "_TAO_Bounded_Sequence_%s_%d",
                          this->flatname(),
                          //prim_type->flatname (),
                          this->max_size()->ev()->u.ulval);
      break;
    }

  return namebuf; 
}


const char *
be_sequence::object_manager_name ()
{
  static char namebuf[NAMEBUFSIZE];
  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  be_type *bt;
  bt = be_type::narrow_from_decl (this->base_type ());
  if (!bt)
    {
      ACE_DEBUG ((LM_ERROR,
                  "(%N:%l) be_visitor_sequence_ch::"
                  "gen_instantiate_name - "
                  "Bad element type\n"));
      return namebuf;
    }

  be_scope * bs = be_scope::narrow_from_scope (this->defined_in());

  if (!bs)
    {
      ACE_DEBUG ((LM_ERROR,
                  "(%N:%l) be_visitor_sequence_ch::"
                  "gen_instantiate_name - "
                  "Bad element type\n"));
      return namebuf;
    }

  ACE_OS::sprintf (namebuf, 
                   "_TAO_Object_Manager_%s_%s",
                   bs->decl()->flatname(),
                   bt->flatname());  


  return namebuf; 
}


idl_bool
be_sequence::in_recursion (be_type *node)
{
  if (!node)
    {
      // there has to be a parameter
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_sequence::")
                         ASYS_TEXT ("in_recursion - ")
                         ASYS_TEXT ("bad parameter node\n")),
                        0);
    }

  be_type *type = be_type::narrow_from_decl (this->base_type ());
  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%N:%l) be_sequence::")
                         ASYS_TEXT ("in_recursion - ")
                         ASYS_TEXT ("bad base type\n")),
                        0);
    }
  
  if (!ACE_OS::strcmp (node->fullname (), type->fullname ()))
    // they match
    return 1;
  else
    // not in recursion
    return 0;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)
