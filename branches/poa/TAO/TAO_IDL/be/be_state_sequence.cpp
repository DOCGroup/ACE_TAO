// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_sequence.cpp
//
// = DESCRIPTION
//    state based code generation for sequences.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// return type for sequence
be_state_sequence::be_state_sequence (void)
{
}

// generate code for sequence type
int
be_state_sequence::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_sequence *seq;

  seq = be_sequence::narrow_from_decl (d);
  if (!seq)
    return -1;

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
      os = cg->client_header (); // get client header stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
      os = cg->client_stubs (); // get client stubs stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
      os = cg->client_inline (); // get client inline stream
      break;
    default:
      return -1;
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
    // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // enclosing scope in which the typedef occurs
  be_decl *scope =
    be_decl::narrow_from_decl (ScopeAsDecl (bt->defined_in ()));

  // for sequences, all we do is generate the type
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
            *os << bt->nested_type_name (scope, "_var");
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
          default:
            *os << bt->name () << "_var";
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
            *os << bt->nested_type_name (scope, "");
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
          default:
            *os << bt->name ();
          }
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
            {
              // generate the base type sequence
              if (bt->gen_client_header () == -1)
                {
                  return -1;
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
            {
              // generate the base type sequence
              if (bt->gen_client_stubs () == -1)
                {
                  return -1;
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            {
              // generate the base type sequence
              if (bt->gen_client_inline () == -1)
                {
                  return -1;
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
            *os << bt->nested_type_name (scope, "");
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
          default:
            *os << bt->name ();
          }
      }
      break;
    case AST_Decl::NT_except: // type is an exception
      {
        // XXXASG: Is this case valid ???
      }
      break;
    case AST_Decl::NT_typedef: // type is a typedef
      {
        be_type *temp; // most primitive base type
        be_typedef *t = be_typedef::narrow_from_decl (bt);

        if (!t)
          return -1;

        temp = t->primitive_base_type ();
        // make a recursive call
        return this->gen_code (t, d, temp);
      }
      //break;  unreachable statement!
    } // end of switch
  return 0;
}
