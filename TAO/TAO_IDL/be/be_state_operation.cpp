// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_operation.cpp
//
// = DESCRIPTION
//    state based code generation for operations.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// return type for operation
be_state_operation::be_state_operation (void)
{
}

// generate code for return type of operation
int
be_state_operation::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_operation *bop;
  be_interface *bif; // interface in which the operation was defined

  bop = be_operation::narrow_from_decl (d);
  if (!bop)
    return -1;

  bif = be_interface::narrow_from_scope (bop->defined_in ());
  if (!bif)
    return -1;

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_OPERATION_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
    case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
    case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
      os = cg->server_skeletons ();
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
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << "_ptr ";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << "CORBA::Object_ptr retval = CORBA::Object::_nil ();" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return " << bt->name () << "::_nil ();\n";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return " << bt->name () << "::_narrow (retval, env);" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << "CORBA::Object_ptr *retval ="
                " new CORBA::Object_ptr;" << nl;
              *os << "*retval = CORBA::Object::_nil ();" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "*retval"; // assign to retval
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif, "_ptr") << " ";
            }
            break;
          default:
            {
              *os << bt->name () << "_ptr ";
            }
          }
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);

        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr ";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  *os << "void ";
                }
              else
                {
                  *os << bt->name () << " ";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *retval;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr retval;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  // no return variable
                }
              else
                {
                  *os << bt->name () << " retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << "return 0;\n";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "return 0;\n";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  *os << "return;\n";
                }
              else
                {
                  *os << "return retval;\n";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              if (bpd->pt () == AST_PredefinedType::PT_void)
                {
                  *os << "return; // no value" << nl;
                }
              else
                {
                  *os << "return retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *retval;" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr *retval = new " << bt->name () <<
                      "_ptr;" << nl;
                }
              else
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";"
                      << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << "retval";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "*retval";
                }
              else
                {
                  *os << " *retval";
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << "result = new CORBA::Any (" << bt->tc_name () <<
                    ", retval, 1); // ORB owns" << nl;
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "result = new CORBA::Any (" << bt->tc_name () <<
                    ", retval, 1); // ORB owns" << nl;
                }
              else
                {
                  *os << "result = new CORBA::Any (" << bt->tc_name () <<
                    ", retval, 1); // ORB owns" << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << bt->nested_type_name (bif, "*") << " ";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << bt->nested_type_name (bif, "_ptr") << " ";
                }
              else
                {
                  *os << bt->nested_type_name (bif) << " ";
                }
            }
            break;
          default:
            {
              *os << bt->name ();
              // check if the type is an any
              if (bpd->pt () == AST_PredefinedType::PT_any)
                {
                  // if it is an any, return a pointer to it
                  *os << " *";
                }
              else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object, return a pointer
                  *os << "_ptr";
                }
            }
          } // end switch (cg->state())
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
#if 0
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name ();
                }
              else
                {
                  *os << "char *";
                }
#endif
              *os << "char *";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
#if 0
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name () << " retval = 0;" << nl;
                }
              else
                {
                  *os << "char *retval = 0;" << nl;
                }
#endif
              *os << "char *retval = 0;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;\n";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
#if 0
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";" << nl;
                }
              else
                {
                  *os << "char **retval = new char*;" << nl;
                }
#endif
              *os << "char **retval = new char*;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "*retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
#if 0
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  // to keep MSVC++ happy
                  *os << bt->nested_type_name (bif);
                }
              else
                {
                  *os << "char *";
                }
#endif
              *os << "char *";
            }
            break;
          default:
            {
              *os << "char *";
            }
          } // end of switch cg->state
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << "_slice *";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << bt->name () << "_slice *retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;\n";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << bt->name () << "_slice *retval;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif, "_slice") << " *";
            }
            break;
          default:
            {
              // return a pointer to slice
              *os << bt->name () << "_slice *";
            }
          } // end of switch cg->state
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
      // return type is a pointer to sequence
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << " *";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << bt->name () << " *retval = new " << bt->name () << ";" <<
                nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;\n";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
#if 0
              be_sequence *seq = be_sequence::narrow_from_decl (type);
              // init_mgr method for managed types
              switch (seq->managed_type ())
                {
                case be_sequence::MNG_OBJREF:
                case be_sequence::MNG_STRING:
                  *os << "retval->init_mgr ();" << nl;
                  break;
                default:
                  break;
                }
#endif
              // if we are sequence, call init manager
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << bt->name () << " *retval;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif) << " *";
            }
            break;
          default:
            {
              *os << bt->name () << " *";
            }
          } // end of swithc cg->state
      }
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << " ";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              *os << bt->name () << " retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              *os << "return retval;\n";
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              *os << bt->name () << " *retval = new " << bt->name () << ";" <<
                nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              *os << "*retval";
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif);
            }
            break;
          default:
            {
              *os << bt->name ();
            }
          } // end of switch cg->state
      }
      break;
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name () << " ";
              if (bt->size_type () == be_decl::VARIABLE)
                {
                  *os << "*";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
            {
              if (bt->size_type () == be_decl::VARIABLE)
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";"
                      << nl;
                }
              else
                {
                  *os << bt->name () << " retval;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
            {
              if (bt->size_type () == be_decl::VARIABLE)
                {
                  *os << "return 0;\n";
                }
              else
                {
                  *os << "return retval;\n";
                }
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
            {
              if (type->size_type () == be_decl::VARIABLE)
                {
                  *os << bt->name () << " *retval;" << nl;
                }
              else
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";"
                      << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
            {
              if (type->size_type () == be_decl::VARIABLE)
                {
                  *os << "retval";
                }
              else
                {
                  *os << "*retval";
                }
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif);
              // check if we are fixed size or variable sized. Depending on that we
              // return a pointer or the aggregate itself
              if (type->size_type () == be_decl::VARIABLE)
                *os << " *";
            }
            break;
          default:
            {
              *os << bt->name ();
              // check if we are fixed size or variable sized. Depending on that we
              // return a pointer or the aggregate itself
              if (type->size_type () == be_decl::VARIABLE)
                *os << " *";
            }
          } // end of switch cg->state
      }
      break;
    case AST_Decl::NT_native:
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS:
            {
              *os << bt->name ();
            }
	    break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS:
          case TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS:
          case TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS:
          case TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS:
          case TAO_CodeGen::TAO_OPERATION_RESULT_SS:
            break;
          case TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS:
	    *os << "return 0;\n";
	    break;
          case TAO_CodeGen::TAO_OPERATION_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif) << " ";
            }
            break;
          default:
            {
              *os << bt->name ();
            }
	    break;
          }
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
    } // switch of main switch
  return 0;
}
