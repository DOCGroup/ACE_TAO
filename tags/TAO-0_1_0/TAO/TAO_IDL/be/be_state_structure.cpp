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

be_state_struct::be_state_struct (void)
{
}

// generate code for structure member
int
be_state_struct ::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_field *f;       // field node
  be_structure *bs;  // enclosing structure node

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_STRUCT_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_STRUCT_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_STRUCT_CI:
      os = cg->client_inline ();
      break;
    }

  f = be_field::narrow_from_decl (d); // downcast to field node
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_structure.cpp - "
                         "Bad field\n"),
                        -1);
    }

  // retrieve the enclosing "struct" node
  bs = be_structure::narrow_from_scope (f->defined_in ());
  if (!bs)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_struct.cpp - "
                         "Bad structure\n"),
                        -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
       // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // generate code based on type.
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_STRUCT_CH:
            {
              os->indent (); // start from current indentation
              *os << bt->nested_type_name (bs, "_var") << " " << f->local_name
                () << ";\n";
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CI:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CS:
            {
              // nothing
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_struct - unknown state\n"), -1);
            }
          } // end switch state
      } // end case interface
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_STRUCT_CH:
            {
              be_predefined_type *bpd = be_predefined_type::narrow_from_decl
                (type);
              if (!bpd)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_struct - "
                                     "bad predefined type\n"), -1);
                }
              os->indent (); // start from current indentation
              switch (bpd->pt ())
                {
                case AST_PredefinedType::PT_any:
                case AST_PredefinedType::PT_pseudo:
                  *os << bt->nested_type_name (bs) << "_var " << f->local_name
                    () << ";\n";
                  break;
                default:
                  *os << bt->nested_type_name (bs) << " " << f->local_name ()
                      << ";\n";
                }
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CI:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CS:
            {
              // nothing
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_struct - unknown state\n"), -1);
            }
          } // end switch state
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_STRUCT_CH:
            {
              os->indent (); // start from current indentation
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->nested_type_name (bs, "_var") << " " <<
                    f->local_name () << ";\n";
                }
              else
                {
                  *os << "CORBA::String_var " << f->local_name () << ";\n";
                }
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CI:
            {
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CS:
            {
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_struct - unknown state\n"), -1);
            }
          } // end switch state
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_STRUCT_CH:
            {
              // We first need to generate code for this aggregate type. Check
              // if we are not called recursively thru a typedef
              if (bt->node_type () != AST_Decl::NT_typedef
                  && !bt->imported ())
                if (bt->gen_client_header () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_struct - "
                                       "error generating code for type\n"),
                                      -1);
                  }
              // this is not to be generated for arrays
              if (bt->node_type () != AST_Decl::NT_array)
                {
                  os->indent ();
                  *os << bt->nested_type_name (bs) << " " << f->local_name () <<
                    ";\n";
                }
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CI:
            {
              // We first need to generate code for this aggregate type. Check
              // if we are not called recursively thru a typedef
              if (bt->node_type () != AST_Decl::NT_typedef
                  && !bt->imported ())
                if (bt->gen_client_inline () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_struct - "
                                       "error generating code for type\n"),
                                      -1);
                  }
            }
            break;
          case TAO_CodeGen::TAO_STRUCT_CS:
            {
              // We first need to generate code for this aggregate type. Check
              // if we are not called recursively thru a typedef
              if (bt->node_type () != AST_Decl::NT_typedef
                  && !bt->imported ())
                if (bt->gen_client_stubs () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_struct - "
                                       "error generating code for type\n"),
                                      -1);
                  }
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_struct - unknown state\n"), -1);
            }
          } // end switch state
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
  // the enclosing structure will be variable length if the field's type is
  // variable length
  bs->size_type (type->size_type ());
  return 0;
}
