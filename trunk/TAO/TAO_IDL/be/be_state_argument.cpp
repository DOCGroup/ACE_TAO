// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_argument.cpp
//
// = DESCRIPTION
//    state based code generation for arguments.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// return type for argument
be_state_argument::be_state_argument (void)
{
}

// generate code for return type of argument
int
be_state_argument::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os = 0; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_argument *arg;  // argument node
  be_interface *bif; // interface inside which the operation that uses this
  // argument was defined

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_ARGUMENT_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SS:
    case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
      os = cg->server_skeletons ();
      break;
    }

  arg = be_argument::narrow_from_decl (d);
  if (!arg)
    return -1;

  // get the scope of the arg which is the operation. Its scope is the
  // interface node
  bif = be_interface::narrow_from_scope (ScopeAsDecl (arg->defined_in
                                                      ())->defined_in ());
  if (!bif)
    return -1;

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
    // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // find the direction of the argument. Depending on the direction and the
  // type, generate the mapping
  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            // what state are we in
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  *os << bt->name () << "_ptr ";
                  // declare a variable
                  *os << arg->local_name () << ";" << nl;
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_ptr") << " ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_ptr ";
                }
              } // switch state
            break;
          case AST_Argument::dir_INOUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  *os << bt->name () << "_ptr ";
                  // declare a variable
                  *os << arg->local_name () << ";" << nl;
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_ptr") << " &";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_ptr &";
                }
              } // end switch state
            break;
          case AST_Argument::dir_OUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << "_ptr ";
                  *os << arg->local_name () << ";" << nl;
                  *os << bt->name () << "_out " << arg->local_name () <<
                    "_out (" << arg->local_name () << ");" << nl;
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << "_out, ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_out ";
                }
              } // end switch state
            break;
          } // end switch direction
      } // end of case interface/interface_fwd
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);

        // check if the type is an any
        if (bpd->pt () == AST_PredefinedType::PT_any)
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                      *os << bt->name () << " ";
                    // declare a variable
                      *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        "); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << "const " << bt->nested_type_name (bif) << " &";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
		  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << "const " << bt->name () << " &";
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_INOUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " ";
                      *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << "const " << bt->nested_type_name (bif) << " &";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
		  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << " &";
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_OUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " *";
                      *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_out") << " ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
		  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << "_out ";
                    }
                  } // end switch state
                break;
              } // end switch direction
          } // end of if
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo)
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << "_ptr ";
                      *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        "); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_ptr") << " ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
		  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << "_ptr ";
                    }
                  } // end of switch state
                break;
              case AST_Argument::dir_INOUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << "_ptr ";
                      *os << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_ptr") << " &";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
		  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << "_ptr &";
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_OUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << "_ptr ";
                      *os << arg->local_name () << ";" << nl;
                      *os << bt->name () << "_out ";
                      *os << arg->local_name () << "_out (" << arg->local_name
                        () << ");" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_out") << " ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << "_out, ";
                    }
                    break;
		  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << "_out ";
                    }
                  } // end switch state
                break;
              } // end switch direction
          } // end else if
        else
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " " << arg->local_name () << ";" <<
                        nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        "); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif);
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name ();
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_INOUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " " << arg->local_name () << ";" <<
                        nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        "); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif) << " &";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << " &";
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_OUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " " << arg->local_name () << ";" <<
                        nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_out");
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                  default:
                    {
                      *os << bt->name () << "_out";
                    }
                  } // end of switch
                break;
              } // end switch direction
          } // end of else
      } // end of case predefined
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (arg->direction ())
          {
          case AST_Argument::dir_IN:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << "char *" << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  *os << "const char *";
                }
              } // end switch state
            break;
          case AST_Argument::dir_INOUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << "char *" << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  *os << "char *&";
                }
              } // end switch state
            break;
          case AST_Argument::dir_OUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << "char *" << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  *os << bt->nested_type_name (bif, "_out");
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_out";
                }
              } // end switch state
            break;
          } // end switch direction
      } // end case string
      break;
    case AST_Decl::NT_array: // type is an array
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << " " << arg->local_name ()
                      << ";" << nl;
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << "const " << bt->nested_type_name (bif);
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << "const " << bt->name ();
                }
              } // end switch state
          } // end case
          break;
        case AST_Argument::dir_INOUT:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << bt->name ();
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << "_slice *";
                    }
                  *os << " " << arg->local_name () << ";" << nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << bt->nested_type_name (bif, "_slice") << " *";
                    }
                  else
                    {
                      *os << bt->nested_type_name (bif);
                    }
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name ();
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << "_slice *";
                    }
                }
              } // end switch state
          } // end case
          break;
        case AST_Argument::dir_OUT:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << bt->name () << "_slice *" << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_out";
                }
              } // end switch state
          }
          break;
        } // end of switch direction
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct: // type is a struct
    case AST_Decl::NT_union: // type is a union
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << " " << arg->local_name () <<
                    ";" << nl;
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << "const " << bt->nested_type_name (bif) << " &";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << "const " << bt->name () << " &";
                }
              } // end switch state
          }
          break;
        case AST_Argument::dir_INOUT:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << " " << arg->local_name () <<
                    ";" << nl;
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif) << " &";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << " &";
                }
              } // end switch state
          } // end case
          break;
        case AST_Argument::dir_OUT:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                    // declare a variable
                      *os << bt->name () << " *" << arg->local_name () << ";" <<
                        nl;
                      *os << bt->name () << "_out " << arg->local_name () <<
                        "_out (" << arg->local_name () << ");" << nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    }
                  else
                    {
                    // declare a variable
                      *os << bt->name () << " " << arg->local_name () << ";" <<
                        nl;
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
                    } // end else
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  if (bt->size_type () == be_decl::VARIABLE)
                    *os << arg->local_name () << "_out, ";
                  else
                    *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_out";
                  break;
                }
              } // end switch state
          } // end case
          break;
        } // end switch direction
      break;
    case AST_Decl::NT_enum: // type is an enum
      switch (arg->direction ())
        {
        case AST_Argument::dir_IN:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << bt->name () << " " << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif);
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name ();
                }
              } // end of switch state
          } // end case
          break;
        case AST_Argument::dir_INOUT:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << bt->name () << " " << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif) << " &";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << " &";
                }
              } // end switch state
          } // end case
          break;
        case AST_Argument::dir_OUT:
          {
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                    // declare a variable
                  *os << bt->name () << " " << arg->local_name () << ";" <<
                    nl;
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              default:
                {
                  *os << bt->name () << "_out";
                }
              } // end switch state
          } // end case
          break;
        } // end switch direction
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
