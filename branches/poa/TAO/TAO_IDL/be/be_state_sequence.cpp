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
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_sequence.cpp - "
                         "Bad sequence type\n"),
                        -1);
    }

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH:
      os = cg->client_header (); // get client header stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
      os = cg->client_stubs (); // get client stubs stream
      break;
    case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
    case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
    case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
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

  // enclosing scope in which the sequence occurs
  be_decl *scope;
  if (bt->node_type () == AST_Decl::NT_typedef)
    scope =  be_scope::narrow_from_scope (bt->defined_in ())->decl ();
  else // no anonymous data types are allowed here
    scope = 0;

  // for sequences, all we do is generate the type
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
            // if it is the case of sequence base, we generate the code for
            // managed types
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
            if (seq->gen_managed_type_ch () == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_state_sequence.cpp - "
                                   "codegen for managed type\n"),
                                  -1);
              }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
            if (seq->gen_managed_type_cs () == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_state_sequence.cpp - "
                                   "codegen for managed type\n"),
                                  -1);
              }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            if (seq->gen_managed_type_ci () == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_state_sequence.cpp - "
                                   "codegen for managed type\n"),
                                  -1);
              }
            break;
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH: // managed type returned by
                                                    // operator []
            {
              if ((type->node_type () == AST_Decl::NT_interface)
                  || (type->node_type () == AST_Decl::NT_interface_fwd))
                {
                  *os << "ACE_NESTED_CLASS (" << seq->local_name () <<
                    ", TAO_ObjRefMngType)";
                }
              else // must be a string
                {
                  *os << "ACE_NESTED_CLASS (" << seq->local_name () <<
                    ", TAO_StrMngType)";
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
            {
              if ((type->node_type () == AST_Decl::NT_interface)
                  || (type->node_type () == AST_Decl::NT_interface_fwd))
                {
                  *os << seq->name () << "::TAO_ObjRefMngType";
                }
              else // must be a string
                {
                  *os << seq->name () << "::TAO_StrMngType";
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
            {
              if ((type->node_type () == AST_Decl::NT_interface)
                  || (type->node_type () == AST_Decl::NT_interface_fwd))
                {
                  *os << bt->nested_type_name (scope, "_ptr");
                }
              else // must be a string
                {
                  *os << "char *";
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
            {
              if ((type->node_type () == AST_Decl::NT_interface)
                  || (type->node_type () == AST_Decl::NT_interface_fwd))
                {
                  *os << bt->name () << "_ptr";
                }
              else // must be a string
                {
                  *os << "char *";
                }
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_sequence.cpp - "
                                 "bad state\n"),
                                -1);
            }
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);

        if (!bpd)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_state_sequence.cpp - "
                               "bad type\n"),
                              -1);
          }
        if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            // managed types for pseudo objs
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
                if (seq->gen_managed_type_ch () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_sequence.cpp - "
                                       "codegen for managed type\n"),
                                      -1);
                  }
                break;
              case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
                if (seq->gen_managed_type_cs () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_sequence.cpp - "
                                       "codegen for managed type\n"),
                                      -1);
                  }
                break;
              case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
                if (seq->gen_managed_type_ci () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_sequence.cpp - "
                                       "codegen for managed type\n"),
                                      -1);
                  }
                break;
              case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
                *os << bt->nested_type_name (scope, "_ptr");
                break;
              case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
              case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
                *os << bt->name () << "_ptr";
                break;
              case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH: // managed type returned by
                // operator []
                {
                  *os << "ACE_NESTED_CLASS (" << seq->local_name () <<
                    ", TAO_ObjRefMngType)";
                }
                break;
              case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
              case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
                {
                  *os << seq->name () << "::TAO_ObjRefMngType";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_sequence.cpp - "
                                     "bad state\n"),
                                    -1);
                }
              } // end switch state
          } // if pseudo
        else
          {
            // rest of the predefined types
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_SEQUENCE_BASE_CH:
              case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
              case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
                // nothing to be done
                break;
              case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH:
              case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
                *os << bt->nested_type_name (scope, "");
                break;
              case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
              case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
              case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
              case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
                *os << bt->name ();
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_sequence.cpp - "
                                     "bad state\n"),
                                    -1);
                }
              } // end of switch
          } // end else
      } // end case predefined type
      break;
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
            // nothing to do because we cannot have anonymous data types here
            // that are enums or structures or unions.
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH:
            *os << bt->nested_type_name (scope, "");
            break;
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
            *os << bt->name ();
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_sequence.cpp - "
                                 "bad state\n"),
                                -1);
            }
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
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_sequence.cpp - "
                                     "Bad sequence type\n"),
                                    -1);
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CS:
            {
              // generate the base type sequence
              if (bt->gen_client_stubs () == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_sequence.cpp - "
                                     "Bad sequence type\n"),
                                    -1);
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BASE_CI:
            {
              // generate the base type sequence
              if (bt->gen_client_inline () == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_sequence.cpp - "
                                     "Bad sequence type\n"),
                                    -1);
                }
            }
            break;
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CH:
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CH:
            *os << bt->nested_type_name (scope, "");
            break;
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CS:
          case TAO_CodeGen::TAO_SEQELEM_RETTYPE_CI:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CS:
          case TAO_CodeGen::TAO_SEQUENCE_BODY_CI:
            *os << bt->name ();
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_sequence.cpp - "
                                 "bad state\n"),
                                -1);
            }
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
