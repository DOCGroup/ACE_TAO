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

#include "be_predefined_type.h"
#include "be_visitor.h"
#include "utl_identifier.h"
#include "ace/Log_Msg.h"
#include "ace/ACE.h"

ACE_RCSID (be, 
           be_predefined_type, 
           "$Id$")

be_predefined_type::be_predefined_type (void)
{
}

be_predefined_type::be_predefined_type (AST_PredefinedType::PredefinedType t,
                                        UTL_ScopedName *n)
  : AST_PredefinedType (t,
                        n),
    AST_Decl (AST_Decl::NT_pre_defined,
              n,
              I_TRUE)
{
  // Computes the repoID.
  this->compute_repoID ();

  // Computes the fully scoped typecode name.
  this->compute_tc_name ();

  // Compute the flattened fully scoped name.
  this->compute_flat_name ();
}

// Overriden method.
void
be_predefined_type::compute_tc_name (void)
{
  // Start with the head as the CORBA namespace.
  Identifier *corba_id = 0;
  ACE_NEW (corba_id,
           Identifier ("CORBA"));

  ACE_NEW (this->tc_name_,
           UTL_ScopedName (corba_id,
                           0));

  Identifier *id = 0;
  UTL_ScopedName *conc_name = 0;

  switch (this->pt ())
    {
    case AST_PredefinedType::PT_void:
      ACE_NEW (id,
               Identifier ("_tc_void"));
      break;
    case AST_PredefinedType::PT_long:
      ACE_NEW (id,
               Identifier ("_tc_long"));
      break;
    case AST_PredefinedType::PT_longlong:
      ACE_NEW (id,
               Identifier ("_tc_longlong"));
      break;
    case AST_PredefinedType::PT_ulong:
      ACE_NEW (id,
               Identifier ("_tc_ulong"));
      break;
    case AST_PredefinedType::PT_ulonglong:
      ACE_NEW (id,
               Identifier ("_tc_ulonglong"));
      break;
    case AST_PredefinedType::PT_short:
      ACE_NEW (id,
               Identifier ("_tc_short"));
      break;
    case AST_PredefinedType::PT_ushort:
      ACE_NEW (id,
               Identifier ("_tc_ushort"));
      break;
    case AST_PredefinedType::PT_float:
      ACE_NEW (id,
               Identifier ("_tc_float"));
      break;
    case AST_PredefinedType::PT_double:
      ACE_NEW (id,
               Identifier ("_tc_double"));
      break;
    case AST_PredefinedType::PT_longdouble:
      ACE_NEW (id,
               Identifier ("_tc_longdouble"));
      break;
    case AST_PredefinedType::PT_char:
      ACE_NEW (id,
               Identifier ("_tc_char"));
      break;
    case AST_PredefinedType::PT_wchar:
      ACE_NEW (id,
               Identifier ("_tc_wchar"));
      break;
    case AST_PredefinedType::PT_octet:
      ACE_NEW (id,
               Identifier ("_tc_octet"));
      break;
    case AST_PredefinedType::PT_boolean:
      ACE_NEW (id,
               Identifier ("_tc_boolean"));
      break;
    case AST_PredefinedType::PT_any:
      ACE_NEW (id,
               Identifier ("_tc_any"));
    break;
    case AST_PredefinedType::PT_pseudo:
      {
        char tcname [100];
        ACE_OS::sprintf (tcname,
                         "_tc_%s",
                         this->name ()->last_component ()->get_string ());

        ACE_NEW (id,
                 Identifier (tcname));
        break;
      }
    default:
      ACE_ERROR ((LM_WARNING, "Unknown or invalid predefined type"));
      break;
    }

  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           0));

  this->tc_name_->nconc (conc_name);
}

// Compute the size type of the node in question.
int
be_predefined_type::compute_size_type (void)
{
  if (this->pt () == AST_PredefinedType::PT_any
      || this->pt () == AST_PredefinedType::PT_pseudo)
    {
      this->size_type (AST_Type::VARIABLE);
    }
  else
    {
      this->size_type (AST_Type::FIXED);
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
        {
	        this->repoID_ = ACE::strnew ("IDL:omg.org/CORBA/Object:1.0");
        }
      else
        {
	        AST_Decl::compute_repoID ();
        }

      break;
    default:
      AST_Decl::compute_repoID ();
      break;
    }
}

int
be_predefined_type::accept (be_visitor *visitor)
{
  return visitor->visit_predefined_type (this);
}

void
be_predefined_type::destroy (void)
{
  this->AST_PredefinedType::destroy ();
  this->be_type::destroy ();
}

// Narrowing
IMPL_NARROW_METHODS2 (be_predefined_type, AST_PredefinedType, be_type)
IMPL_NARROW_FROM_DECL (be_predefined_type)
