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
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from the current indentation level

  switch (this->pt ())
    {
    case AST_PredefinedType::PT_void:
      *cs << "CORBA::tk_void,\n\n";
      break;
    case AST_PredefinedType::PT_short:
      *cs << "CORBA::tk_short,\n\n";
      break;
    case AST_PredefinedType::PT_ushort:
      *cs << "CORBA::tk_ushort,\n\n";
      break;
    case AST_PredefinedType::PT_long:
      *cs << "CORBA::tk_long,\n\n";
      break;
    case AST_PredefinedType::PT_ulong:
      *cs << "CORBA::tk_ulong,\n\n";
      break;
    case AST_PredefinedType::PT_longlong:
      *cs << "CORBA::tk_longlong,\n\n";
      break;
    case AST_PredefinedType::PT_ulonglong:
      *cs << "CORBA::tk_ulonglong,\n\n";
      break;
    case AST_PredefinedType::PT_float:
      *cs << "CORBA::tk_float,\n\n";
      break;
    case AST_PredefinedType::PT_double:
      *cs << "CORBA::tk_double,\n\n";
      break;
    case AST_PredefinedType::PT_longdouble:
      *cs << "CORBA::tk_longdouble,\n\n";
      break;
    case AST_PredefinedType::PT_boolean:
      *cs << "CORBA::tk_boolean,\n\n";
      break;
    case AST_PredefinedType::PT_char:
      *cs << "CORBA::tk_char,\n\n";
      break;
    case AST_PredefinedType::PT_octet:
      *cs << "CORBA::tk_octet,\n\n";
      break;
    case AST_PredefinedType::PT_any:
      *cs << "CORBA::tk_any,\n\n";
      break;
    case AST_PredefinedType::PT_wchar:
      *cs << "CORBA::tk_wchar,\n\n";
      break;
    }
  return 0;
}

long
be_predefined_type::tc_size (void)
{
  return 4; // for the enum value
}

// Narrowing
IMPL_NARROW_METHODS2 (be_predefined_type, AST_PredefinedType, be_type)
IMPL_NARROW_FROM_DECL (be_predefined_type)

