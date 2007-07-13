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
#include "be_helper.h"
#include "utl_identifier.h"
#include "global_extern.h"

#include "ace/Log_Msg.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (be,
           be_predefined_type,
           "$Id$")

be_predefined_type::be_predefined_type (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    AST_PredefinedType (),
    be_decl (),
    be_type ()
{
}

be_predefined_type::be_predefined_type (AST_PredefinedType::PredefinedType t,
                                        UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_pre_defined,
              n,
              true),
    AST_Type (AST_Decl::NT_pre_defined,
              n),
    AST_ConcreteType (AST_Decl::NT_pre_defined,
                      n),
    AST_PredefinedType (t,
                        n),
    be_decl (AST_Decl::NT_pre_defined,
             n),
    be_type (AST_Decl::NT_pre_defined,
             n)
{
  // Computes the repoID.
  this->compute_repoID ();

  // Computes the fully scoped typecode name.
  this->compute_tc_name ();

  // Compute the flattened fully scoped name.
  this->AST_Decl::compute_flat_name ();

  if (t == AST_PredefinedType::PT_object)
    {
      this->fwd_helper_name_ = "::CORBA::tao_Object";
    }
  else if (t == AST_PredefinedType::PT_value)
    {
      this->fwd_helper_name_ = "::CORBA::tao_ValueBase";
    }
  else if (t == AST_PredefinedType::PT_abstract)
    {
      this->fwd_helper_name_ = "::CORBA::tao_AbstractBase";
    }

  switch (t)
    {
      case AST_PredefinedType::PT_char:
      case AST_PredefinedType::PT_wchar:
      case AST_PredefinedType::PT_boolean:
      case AST_PredefinedType::PT_octet:
        idl_global->ambiguous_type_seen_ = true;
        break;
      case AST_PredefinedType::PT_any:
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_value:
      case AST_PredefinedType::PT_void:
      case AST_PredefinedType::PT_pseudo:
      case AST_PredefinedType::PT_abstract:
        break;
      default:
        idl_global->basic_type_seen_ = true;
        break;
    }
}

// Overridden method.
void
be_predefined_type::gen_member_ostream_operator (TAO_OutStream *os,
                                                 const char *instance_name,
                                                 bool accessor)
{
  switch (this->pt ())
    {
      case AST_PredefinedType::PT_boolean:
        *os << "ACE_OutputCDR::from_boolean (" << instance_name
            << (accessor ? " ()" : "") << ")";
        break;
      case AST_PredefinedType::PT_char:
        *os << "ACE_OutputCDR::from_char (" << instance_name
            << (accessor ? " ()" : "") << ")";
        break;
      case AST_PredefinedType::PT_octet:
        *os << "ACE_OutputCDR::from_octet (" << instance_name
            << (accessor ? " ()" : "") << ")";
        break;
      case AST_PredefinedType::PT_wchar:
        *os << "ACE_OutputCDR::from_wchar (" << instance_name
            << (accessor ? " ()" : "") << ")";
        break;
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_abstract:
      case AST_PredefinedType::PT_pseudo:
        *os << instance_name << (accessor ? " ()" : ".in ()");
        break;
      default:
        this->be_type::gen_member_ostream_operator (os,
                                                    instance_name,
                                                    accessor);
        break;
    }
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
    case AST_PredefinedType::PT_object:
      ACE_NEW (id,
               Identifier ("_tc_Object"));
    break;
    case AST_PredefinedType::PT_value:
      ACE_NEW (id,
               Identifier ("_tc_ValueBase"));
    break;
    case AST_PredefinedType::PT_abstract:
      ACE_NEW (id,
               Identifier ("_tc_AbstractBase"));
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

void
be_predefined_type::compute_repoID (void)
{
  switch (this->pt ())
    {
    case AST_PredefinedType::PT_object:
      ACE::strdelete (this->repoID_);
      this->repoID_ = ACE::strnew ("IDL:omg.org/CORBA/Object:1.0");
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



IMPL_NARROW_FROM_DECL (be_predefined_type)
