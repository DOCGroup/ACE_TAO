// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_constant.cpp
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
        case AST_PredefinedType::PT_any:
          {
            new_name->nconc (new UTL_ScopedName (new Identifier ("Any", 1, 0,
                                                                 I_FALSE), NULL));
            // we are variable length type
            this->size_type (be_decl::VARIABLE);
          }
          break;
        default:
          {
            new_name->nconc (this->name ());
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
        this->tc_name_->nconc (new UTL_ScopedName (new Identifier ("_tc_any", 1, 0,
                                                             I_FALSE), NULL));
      }
    }
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// Generates the client-side header information for the predefined type 
int 
be_predefined_type::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

#if 0
  if (idl_global->in_main_file ())
    {
      *ch << this->gen_corba_mapping ();
    }
#endif
  ch->indent ();
  *ch << this->name ();
  return 0;
}

// Generates the client-side stubs for the predefined type
int 
be_predefined_type::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();

  if (idl_global->in_main_file ())
    {
      *cs << this->gen_corba_mapping ();
    }
  return 0;
}

// Generates the server-side header information for the predefined type 
int 
be_predefined_type::gen_server_header (void)
{
  TAO_OutStream *sh; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  sh = cg->server_header ();

  if (idl_global->in_main_file ())
    {
      *sh << this->gen_corba_mapping ();
    }
  return 0;
}

// Generates the server-side skeletons for the predefined type
int 
be_predefined_type::gen_server_skeletons (void)
{
  TAO_OutStream *ss; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ss = cg->server_skeletons ();

  if (idl_global->in_main_file ())
    {
      *ss << this->gen_corba_mapping ();
    }
  return 0;
}

// Generates the client-side inline information
int 
be_predefined_type::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int 
be_predefined_type::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_predefined_type::gen_typecode (void)
{
  return 0;
}

// Generates the client-side header information for the predefined type 
const char *const
be_predefined_type::gen_corba_mapping (void)
{
  switch (this->pt ())
    {
    case AST_PredefinedType::PT_long:
      return "CORBA::Long";
    case AST_PredefinedType::PT_ulong:
      return "CORBA::ULong";
    case AST_PredefinedType::PT_longlong:
      return "CORBA::LongLong";
    case AST_PredefinedType::PT_short:
      return "CORBA::Short";
    case AST_PredefinedType::PT_ushort:
      return "CORBA::UShort";
    case AST_PredefinedType::PT_float:
      return "CORBA::Float";
    case AST_PredefinedType::PT_double:
      return "CORBA::Double";
    case AST_PredefinedType::PT_longdouble:
      return "CORBA::LongDouble";
    case AST_PredefinedType::PT_char:
      return "CORBA::Char";
    case AST_PredefinedType::PT_wchar:
      return "CORBA::WChar";
    case AST_PredefinedType::PT_boolean:
      return "CORBA::Boolean";
    case AST_PredefinedType::PT_octet:
      return "CORBA::Octet";
    case AST_PredefinedType::PT_any:
      return "CORBA::Any";
    default:
      return 0;
    }
}

// Narrowing
IMPL_NARROW_METHODS2 (be_predefined_type, AST_PredefinedType, be_type)
IMPL_NARROW_FROM_DECL (be_predefined_type)

