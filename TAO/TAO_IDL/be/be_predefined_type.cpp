// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_predefined_type.cpp
//
// = DESCRIPTION
//    Extension of class AST_PredefinedType that provides additional means for C++
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

ACE_RCSID(be, be_predefined_type, "$Id$")

/*
 * BE_PredefinedType
 */
be_predefined_type::be_predefined_type (void)
{
}

be_predefined_type::be_predefined_type (AST_PredefinedType::PredefinedType t,
                                        UTL_ScopedName *n, UTL_StrList *p)
  : AST_PredefinedType (t, n, p),
    AST_Decl (AST_Decl::NT_pre_defined, n, p)
{
  // generate a new Scoped Name for us such that we belong to the CORBA
  // namespace
  if (this->pt () != AST_PredefinedType::PT_void)
    {

      UTL_ScopedName *new_name = new UTL_ScopedName (new Identifier ("CORBA", 1, 0,
                                                                     I_FALSE),
                                                     NULL);
      switch (this->pt ())
        {
        case AST_PredefinedType::PT_long:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Long", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_ulong:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("ULong", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_short:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Short", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_ushort:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("UShort", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_float:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Float", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_double:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Double", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_char:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Char", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_octet:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Octet", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_wchar:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("WChar", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_boolean:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Boolean", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_longlong:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("LongLong", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_ulonglong:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("ULongLong", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_longdouble:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("LongDouble", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_any:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Any", 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        case AST_PredefinedType::PT_pseudo:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier
                                                 (n->last_component
                                                  ()->get_string (), 1, 0,
                                                                 I_FALSE), NULL));
          }
          break;
        default:
          {
            new_name->nconc (new UTL_ScopedName (this->local_name (), NULL));
          }
        }
      this->set_name (new_name);
    }

  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name
  compute_flatname ();

 
}

// overriden method
void
be_predefined_type::compute_tc_name (void)
{
  // start with the head as the CORBA namespace
  this->tc_name_ = new UTL_ScopedName (new Identifier ("CORBA", 1, 0, I_FALSE),
                                       NULL);

  switch (this->pt ())
    {
    case AST_PredefinedType::PT_void:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_void", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_long:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_long", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_longlong:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_longlong", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_ulong:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_ulong", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_ulonglong:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_ulonglong", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_short:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_short", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_ushort:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_ushort", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_float:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_float", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_double:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_double", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_longdouble:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_longdouble", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_char:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_char", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_wchar:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_wchar", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_octet:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_octet", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_boolean:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_boolean", 1, 0,
                                                             I_FALSE), NULL));
      }
      break;
    case AST_PredefinedType::PT_any:
      {
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier
						         ("_tc_any",
							  1, 0,
							  I_FALSE),
						   NULL));
      }
    break;
    case AST_PredefinedType::PT_pseudo:
      {
        char tcname [100];
        ACE_OS::sprintf (tcname, "_tc_%s",
                         this->name ()->last_component ()->get_string ());
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier
                                                   (ACE_OS::strdup (tcname),
                                                    1, 0,
                                                    I_FALSE), NULL));
      }
    break;
    default:
      ACE_ERROR ((LM_WARNING, "Unknown or invalid predefined type"));
      break;
    }
}

// compute the size type of the node in question
int
be_predefined_type::compute_size_type (void)
{
  switch (this->pt ())
    {
    case AST_PredefinedType::PT_any:
    case AST_PredefinedType::PT_pseudo:
      // we are variable length type
      this->size_type (be_decl::VARIABLE);
      break;
    default:
      this->size_type (be_decl::FIXED);
    }
  return 0;
}

void
be_predefined_type::compute_repoID (void)
{
  switch (this->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
      if (!ACE_OS::strcmp (this->local_name ()->get_string (), "Object"))
	this->repoID_ = ACE::strnew ("IDL:omg.org/CORBA/Object:1.0");
      else
	be_decl::compute_repoID ();
      break;
    default:
      be_decl::compute_repoID ();
      break;
    }
}

int
be_predefined_type::accept (be_visitor *visitor)
{
  return visitor->visit_predefined_type (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_predefined_type, AST_PredefinedType, be_type)
IMPL_NARROW_FROM_DECL (be_predefined_type)
