
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_attribute.cpp
//
// = DESCRIPTION
//    state based code generation for attributes.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// constructor
be_state_attribute::be_state_attribute (void)
{
}

int
be_state_attribute::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_attribute *attr; // attribute node
  be_interface *bif;  // enclosing scope which is an interface

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
    case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
    case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
      os = cg->server_skeletons ();
      break;
    }

  // retrieve the ATTRIBUTE node
  attr = be_attribute::narrow_from_decl (d);
  if (!attr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_attribute.cpp - "
                         "Bad attribute node\n"),
                        -1);
    }

  // get the enclosing interface scope to be used in the NESTED_CLASS macro
  bif = be_interface::narrow_from_decl (ScopeAsDecl (attr->defined_in ()));
  if (!bif)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_attribute.cpp - "
                         "Bad interface\n"),
                        -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
    // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // Two level switching
  // (1) switch on node type
  // (2) switch on current code generation state

  switch (type->node_type ())  // LEVEL (1) switch based on node type
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        // what state are we in
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif, "_ptr") << " ";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
            {
              // to keep the MSVC++ compiler happy
              *os << bt->nested_type_name (bif, "_ptr") << " " <<
                attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
            {
              *os << bt->name () << "_ptr ";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
            {
              *os << bt->name () << "_ptr " << attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
            {
              *os << "CORBA::Object_ptr retval = CORBA::Object::_nil ();" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
            {
              *os << "return " << bt->name () << "::_nil ();\n";
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
            {
              *os << "return " << bt->name () << "::_narrow (retval, env);" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
            {
              *os << "CORBA::Object_ptr *retval ="
                " new CORBA::Object_ptr;" << nl;
              *os << "*retval = CORBA::Object::_nil ();" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
            {
              *os << "*retval"; // assign to retval
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
            {
              // assign to a CORBA::Object_ptr
              *os << "CORBA::Object_ptr _tao_base_" << attr->local_name ()
                  << " = " << attr->local_name () << "; // cast it" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
            {
              // pass the cast value
              *os << ", &_tao_base_" << attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
            {
              // nothing for in parameters
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
            {
              // declare a variable
              *os << bt->name () << "_ptr ";
              *os << attr->local_name () << ";" << nl;
              // we also declare a corresponding CORBA::Object_ptr to be
              // passed to the decoder
              *os << "CORBA::Object_ptr _tao_base_" << attr->local_name ()
                  << ";" << nl;
              // now define a NamedValue_ptr
              *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                ";" << nl;
              // declare an Any
              *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                bt->tc_name () << ", &_tao_base_" << attr->local_name () <<
                "); // ORB does not own" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
            {
              // convert from the CORBA::Object_ptr to the interface type
              // using a _narrow
              *os << attr->local_name () << " = " << bt->name () <<
                "::_narrow (_tao_base_" << attr->local_name () << ", " <<
                "_tao_environment);" << nl;
              *os << "if (_tao_environment.exception ()) return;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
            {
              // nothing for an in parameter
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
            {
              *os << attr->local_name () << ", ";
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_attribute - unknown state\n"), -1);
            }
          } // switch state
      } // end of case interface/interface_fwd
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);

        // check if the type is an any
        if (bpd->pt () == AST_PredefinedType::PT_any)
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
                {
                  // if it is an any, return a pointer to it
                  *os << "return 0;\n";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
                {
                  *os << "return retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
                {
                  // if it is an any, return a pointer to it
                  *os << bt->name () << " *retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
                {
                  // if it is an any, return a pointer to it
                  *os << "retval";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
                {
                  // if it is an any, return a pointer to it
                  *os << "result = new CORBA::Any (" << bt->tc_name () <<
                    ", retval, 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
                {
                  *os << bt->name () << " ";
                    // declare a variable
                  *os << attr->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                    bt->tc_name () << ", &" << attr->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
                {
                  // XXXASG - TODO
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
                {
                  // XXXASG - TODO
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
                {
                  // XXXASG - TODO
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
                {
                  *os << attr->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
                {
                  // XXXASG - TODO
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
                {
                  // XXXASG - TODO
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
                {
                  // keep MSVC++ happy
                  *os << bt->nested_type_name (bif);
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
                {
                  *os << bt->name () << "* ";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
                {
                  // keep MSVC++ happy
                  *os << "const " << bt->nested_type_name (bif) << " &" <<
                    attr->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
                {
                  *os << "const " << bt->name () << " &" << attr->local_name
                    ();
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_attribute - unknown state\n"), -1);
                }
              } // end switch state
          } // end of if
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
                {
                  // pseudo object, return a pointer
                  *os << "return 0;\n";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
                {
                  *os << "return retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
                {
                  // pseudo object, return a pointer
                  *os << bt->name () << "_ptr *retval = new " << bt->name () <<
                    "_ptr;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
                {
                  // pseudo object, return a pointer
                  *os << "*retval";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
                {
                  // pseudo object, return a pointer
                  *os << "result = new CORBA::Any (" << bt->tc_name () <<
                    ", retval, 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
                {
                    // declare a variable
                  *os << bt->name () << "_ptr ";
                  *os << attr->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                    bt->tc_name () << ", &" << attr->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
                {
                  // no casting necessary as we already are object_ptr
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
                {
                  *os << ", &" << attr->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
                {
                  *os << attr->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
                {
                  // nothing to be done as we are Object_ptr
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
                {
                  // keep MSVC++ happy
                  *os << bt->nested_type_name (bif);
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
                {
                  *os << bt->name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
                {
                  // keep MSVC++ happy
                  *os << "const " << bt->nested_type_name (bif) << " &" <<
                    attr->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
                {
                  *os << "const " << bt->name () << " &" << attr->local_name
                    ();
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_attribute - unknown state\n"), -1);
                }
              } // end of switch state
          } // end else if
        else // simple predefined types
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
                {
                  *os << bt->name () << " retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
                {
                  *os << "return retval;\n";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
                {
                  *os << "return retval;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
                {
                  *os << bt->name () << " *retval = new " << bt->name () << ";"
                      << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
                {
                  *os << " *retval";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
                {
                  *os << "result = new CORBA::Any (" << bt->tc_name () <<
                    ", retval, 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
                {
                    // declare a variable
                  *os << bt->name () << " " << attr->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                    bt->tc_name () << ", &" << attr->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
                {
                  *os << ", &" << attr->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
                {
                  *os << attr->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
                {
                  // keep MSVC++ happy
                  *os << bt->nested_type_name (bif);
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
              case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
                {
                  *os << bt->name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
                {
                  // keep MSVC++ happy
                  *os << "const " << bt->nested_type_name (bif) << " &" <<
                    attr->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
              case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
                {
                  *os << "const " << bt->name () << " &" << attr->local_name
                    ();
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                     "(%N:%l) be_state_attribute - unknown state\n"), -1);
                }
              } // end switch state
          } // end of else
      } // end of case predefined
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name () << " retval = 0;" << nl;
                }
              else
                {
                  *os << "char *retval = 0;" << nl;
                }
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;\n";
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name () << " *retval = new " << bt->name () << "*;" << nl;
                }
              else
                {
                  *os << "char **retval = new char*;" << nl;
                }
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
            {
              *os << "*retval";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
            {
              *os << ", &" << attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
            {
              *os << attr->local_name () << ", ";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  // to keep MSVC++ happy
                  *os << bt->nested_type_name (bif);
                }
              else
                {
                  *os << "char *";
                }
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
            {
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << bt->name ();
                }
              else
                {
                  *os << "char *";
                }
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
            {
                    // declare a variable
              *os << "char *" << attr->local_name () << ";" <<
                nl;
                    // now define a NamedValue_ptr
              *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                ";" << nl;
                    // declare an Any
              *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                bt->tc_name () << ", &" << attr->local_name () <<
                "); // ORB does not own" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
            {
              *os << "const char *" << attr->local_name ();
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_attribute - unknown state\n"), -1);
            }
          } // end switch state
      } // end case string
      break;
    case AST_Decl::NT_array: // type is an array
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
            {
              *os << bt->name () << "_slice *retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;\n";
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
            {
              *os << bt->name () << "_slice *retval;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
            {
              // declare a variable
              *os << bt->name () << " " << attr->local_name ()
                  << ";" << nl;
              // now define a NamedValue_ptr
              *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                ";" << nl;
              // declare an Any
              *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                bt->tc_name () << ", " << attr->local_name () <<
                "); // ORB does not own" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
            {
              // XXXASG TODO
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
            {
              // XXXASG TODO
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
            {
              // XXXASG TODO
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
            {
              *os << attr->local_name () << ", ";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif, "_slice") << " *";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
            {
              *os << bt->name () << "_slice *";
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
            {
              // to keep the MSVC++ compiler happy
              *os << "const " << bt->nested_type_name (bif) << " " <<
                attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
            {
              *os << "const " << bt->name () << " " << attr->local_name ();
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_attribute - unknown state\n"), -1);
            }
          } // end switch state
      } // end of case array
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
            {
              // declare a variable
              *os << bt->name () << " " << attr->local_name () <<
                ";" << nl;
              // now define a NamedValue_ptr
              *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                ";" << nl;
              // declare an Any
              *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                bt->tc_name () << ", &" << attr->local_name () <<
                "); // ORB does not own" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
            {
              *os << ", &" << attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
            {
              *os << attr->local_name () << ", ";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
            {
              // nothing
            }
            break;
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
            {
              *os << bt->name () << " *retval = 0;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
            {
              *os << "return 0;\n";
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
            {
              *os << bt->name () << " *retval;" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
            {
              *os << "retval";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
            {
              if (bt->size_type () == be_decl::VARIABLE)
                // to keep MSVC++ happy
                *os << bt->nested_type_name (bif) << " *";
              else
                *os << bt->nested_type_name (bif);
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
            {
              if (bt->size_type () == be_decl::VARIABLE)
                *os << bt->name () << " *";
              else
                *os << bt->name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
            {
              // to keep the MSVC++ compiler happy
              *os << "const " << bt->nested_type_name (bif) << " &" <<
                attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
            {
              *os << "const " << bt->name () << " &" << attr->local_name ();
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_attribute - unknown state\n"), -1);
            }
          } // end switch state
      } // end case
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS:
            {
              *os << bt->name () << " retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS:
            {
              *os << "return retval;\n";
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS:
            {
              *os << "return retval;" << nl;
            }
          break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS:
            {
              *os << bt->name () << " *retval = new " << bt->name () << ";" <<
                nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS:
            {
              *os << "*retval";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS:
            {
              *os << "result = new CORBA::Any (" << bt->tc_name () <<
                ", retval, 1); // ORB owns" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS:
            {
                    // declare a variable
              *os << bt->name () << " " << attr->local_name () << ";" <<
                nl;
                    // now define a NamedValue_ptr
              *os << "CORBA::NamedValue_ptr nv_" << attr->local_name () <<
                ";" << nl;
                    // declare an Any
              *os << "CORBA::Any \t any_" << attr->local_name () << " (" <<
                bt->tc_name () << ", &" << attr->local_name () <<
                "); // ORB does not own" << nl;
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS:
            {
              *os << attr->local_name () << ", ";
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS:
            {
              // nothing
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH:
            {
              // to keep the MSVC++ compiler happy
              *os << bt->nested_type_name (bif) << " " << attr->local_name
                ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH:
            {
              *os << bt->name () << " " << attr->local_name ();
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH:
            {
              // to keep MSVC++ happy
              *os << bt->nested_type_name (bif);
            }
            break;
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS:
          case TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH:
            {
              *os << bt->name ();
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_attribute - unknown state\n"), -1);
            }
          } // end switch state
      } //   end case
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
      } // end of case
      break;
    } //end switch node type
  return 0;
}
