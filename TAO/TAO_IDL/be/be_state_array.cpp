// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_array.cpp
//
// = DESCRIPTION
//    state based code generation for arrays.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"
#include	"be_state.h"

ACE_RCSID(be, be_state_array, "$Id$")


// return type for array
be_state_array::be_state_array (void)
{
}

// generate code for array type
int
be_state_array::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (nl);

  // get the appropriate stream
  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ARRAY_DEFN_CH:
    case TAO_CodeGen::TAO_ARRAY_OTHER_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ARRAY_DEFN_CI:
      os = cg->client_inline ();
      break;
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type. For every case, first downcast to the
  // appropriate type. If the downcast fails, return error, else proceed. In
  // some cases, the type itself may need code generation, e.g., anonymous
  // struct types.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        *os << bt->name () << "_var ";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        *os << bt->name ();
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        *os << "CORBA::String_var ";
      }
      break;
    case AST_Decl::NT_array:
      // type is an array. This is possible only if we
      // are here thru the typedef node
      {
        *os << bt->name () << "_slice *";
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_enum:     // type is an enum
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct:   // type is a struct
    case AST_Decl::NT_union:    // type is a union
      {
        // based on what state we are in, we may have to generate the definition
        // of the type first
        if (cg->state () == TAO_CodeGen::TAO_ARRAY_DEFN_CH)
          if (bt->gen_client_header () == -1)  // generate the defn
            return -1;

        *os << bt->name ();
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG TODO: is this allowed ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp;
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      } // end of switch
      //break; unreachable statement!
    }
  return 0;
}
