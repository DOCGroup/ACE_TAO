// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_struct.cpp
//
// = DESCRIPTION
//    state based code generation for structs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

be_state_struct_ch::be_state_struct_ch (void)
{
}

// generate code for structure member
int
be_state_struct_ch::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_field *f;       // field node
  be_structure *bs;  // enclosing structure node

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (nl);

  os = cg->client_header (); // get client header stream
  f = be_field::narrow_from_decl (d); // downcast to field node
  if (!f)
    return -1;

  bs = be_structure::narrow_from_scope (f->defined_in ());
  if (bs == NULL)
    return -1;

  // pass the field node just incase it is needed
  cg->node (f);

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
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bs, "_var") << " " << f->local_name () <<
          ";\n\n";
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        os->indent (); // start from current indentation
        *os << bt->nested_type_name (bs) << " " << f->local_name () << ";\n\n";
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        os->indent (); // start from current indentation
        if (bt->node_type () == AST_Decl::NT_typedef)
          {
            *os << bt->nested_type_name (bs, "_var") << " " << f->local_name () << ";\n\n";
          }
        else
          {
            *os << "CORBA::String_var " << f->local_name () << ";\n\n";
          }
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        // We first need to generate code for this aggregate type. Check if we
        // are not called recursively thru a typedef
        if (bt->node_type () != AST_Decl::NT_typedef)
          if (bt->gen_client_header () == -1)
            return -1;

        os->indent ();
        *os << bt->nested_type_name (bs) << " " << f->local_name () << ";\n\n";
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
        return this->gen_code (t, f, temp);
      } // end of switch
      //break;  unreachable statement!
    }
  // the enclosing structure will be variable length the field is variable
  // length
  bs->size_type (type->size_type ());
  return 0;
}
