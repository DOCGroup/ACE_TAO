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

// generate code for the mapping of the argument.
// The different cases below have the following meaning:
// TAO_ARGUMENT_CH
// TAO_ARGUMENT_CS
// TAO_ARGUMENT_SH -- all three generate the arguments in the signature of the
//                    method . The CH case uses the ACE_NESTED_CLASS macro to
//                    keep the MSVC++ compiler happy
// The rest of the enumerated constants are self-explanatory
//
// There are 3 levels of nested switch statements
// Level (1): The "type" of each argument, e.g., predefined, structure, ...
// Level (2): The direction - in, inout, out
// Level (3): The current state we are in

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
    case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
    case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
    case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SH:
      os = cg->server_header ();
      break;
    case TAO_CodeGen::TAO_ARGUMENT_SS:
    case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
    case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
      os = cg->server_skeletons ();
      break;
    }

  // retrieve the argument node
  arg = be_argument::narrow_from_decl (d);
  if (!arg)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_argument.cpp - "
                         "Bad argument\n"),
                        -1);
    }

  // get the scope of the arg which is the operation. Its scope is the
  // interface node. We need this interface node for generating the
  // ACE_NESTED_CLASS macro
  bif = be_interface::narrow_from_scope (ScopeAsDecl (arg->defined_in
                                                      ())->defined_in ());
  if (!bif)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_argument.cpp - "
                         "Bad interface\n"),
                        -1);
    }

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most primitive
    // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // find the direction of the argument. Depending on the direction and the
  // type, generate the mapping
  switch (type->node_type ())  // LEVEL (1) switch on node type of the "type"
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
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // assign to a CORBA::Object_ptr
                  *os << "CORBA::Object_ptr _tao_base_" << arg->local_name ()
                      << " = " << arg->local_name () << "; // cast it" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  // pass the cast value
                  *os << ", &_tao_base_" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing for in parameters
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << "_ptr ";
                  *os << arg->local_name () << ";" << nl;
                  // we also declare a corresponding CORBA::Object_ptr to be
                  // passed to the decoder
                  *os << "CORBA::Object_ptr _tao_base_" << arg->local_name ()
                      << ";" << nl;
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &_tao_base_" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_IN, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", &_tao_base_" << arg->local_name () << ", 0, " <<
                    "_tao_environment); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // convert from the CORBA::Object_ptr to the interface type
                  // using a _narrow
                  *os << arg->local_name () << " = " << bt->name () <<
                    "::_narrow (_tao_base_" << arg->local_name () << ", " <<
                    "_tao_environment);" << nl;
                  *os << "if (_tao_environment.exception ()) return;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // we must release the in parameter
                  *os << "CORBA::release (" << arg->local_name () << ");" << nl;
                  *os << "CORBA::release (_tao_base_" << arg->local_name () <<
                    ");" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_ptr") << " " <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << "_ptr " << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
                }
              } // switch state
            break;
          case AST_Argument::dir_INOUT: // inout
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // assign to a CORBA::Object_ptr
                  *os << "CORBA::Object_ptr _tao_base_" << arg->local_name ()
                      << " = " << arg->local_name () << ";" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &_tao_base_" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // reassign to the inout parameter doing a _narrow
                  *os << arg->local_name () << " = " << bt->name () <<
                    "::_narrow (_tao_base_" << arg->local_name () << ", env);"
                      << nl;
                  *os << "CORBA::release (_tao_base_" << arg->local_name () <<
                    ");" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << "_ptr ";
                  *os << arg->local_name () << ";" << nl;
                  // we also declare a corresponding CORBA::Object_ptr to be
                  // passed to the decoder
                  *os << "CORBA::Object_ptr *_tao_base_" << arg->local_name ()
                      << " = new CORBA::Object_ptr;" << nl;
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", _tao_base_" << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_INOUT, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", _tao_base_" << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // convert from the CORBA::Object_ptr to the interface type
                  // using a _narrow
                  *os << arg->local_name () << " = " << bt->name () <<
                    "::_narrow (*_tao_base_" << arg->local_name () << ", " <<
                    "_tao_environment);" << nl;
                  *os << "if (_tao_environment.exception ()) return;" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // assign modified interface obj ref to object_ptr
                  *os << "CORBA::release (*_tao_base_" << arg->local_name () <<
                    "); // first release old obj" << nl;
                  *os << "*_tao_base_" << arg->local_name () << " = " <<
                    arg->local_name () << ";" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_ptr") << " &" <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << "_ptr &" << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
                }
              } // end switch state
            break;
          case AST_Argument::dir_OUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // assign to a CORBA::Object_ptr
                  *os << "CORBA::Object_ptr _tao_base_" << arg->local_name ()
                      << ";" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &_tao_base_" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // reassign to the inout parameter doing a _narrow
                  *os << arg->local_name () << " = " << bt->name () <<
                    "::_narrow (_tao_base_" << arg->local_name () << ", env);"
                      << nl;
                  *os << "// free the Object_ptr" << nl;
                  *os << "CORBA::release (_tao_base_" << arg->local_name () <<
                    ");" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
                  // declare a variable
                  *os << bt->name () << "_ptr " << arg->local_name () << ";" <<
                    nl;
                  *os << bt->name () << "_out " << arg->local_name () <<
                    "_out (" << arg->local_name () << ");" << nl;
                  // we also declare a corresponding CORBA::Object_ptr to be
                  // passed to the decoder
                  *os << "CORBA::Object_ptr *_tao_base_" << arg->local_name ()
                      << " = new CORBA::Object_ptr;" << nl;
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", _tao_base_" << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
#endif
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    " = nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_OUT, _tao_environment);" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // out parameter is cast to Object_ptr
                  *os << "*_tao_base_" << arg->local_name () << " = " <<
                    arg->local_name () << "_out.ptr ();" << nl;
                  *os << "nv_" << arg->local_name () << "->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", _tao_base_" << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << "_out, ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " " <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << "_out " << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_IN, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", &" << arg->local_name () << ", 0, " <<
                    "_tao_environment); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // XXXASG - TODO
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      // XXXASG - TODO
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // XXXASG - TODO
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // XXXASG - TODO
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      // nothing since the Any will call  its destructor
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      // keep MSVC++ happy
                      *os << "const " << bt->nested_type_name (bif) << " &" <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << "const " << bt->name () << " &" << arg->local_name
                        () << ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_INOUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " *";
                      *os << arg->local_name () << " = new " << bt->name () <<
                        ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                        "\", CORBA::ARG_INOUT, _tao_environment)->value ()->" <<
                        "replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // XXXASG - TODO very similar to ObjRef
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      // XXXASG - TODO very similar to ObjRef
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // XXXASG - TODO very similar to ObjRef
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << "*" << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << "const " << bt->nested_type_name (bif) << " &" <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << " &" << arg->local_name () << ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_OUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = 0;" << nl;
                      *os << bt->name () << "_out " << arg->local_name () <<
                        "_out (" << arg->local_name () << ");" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
#endif
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name ()
                          << " = nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_OUT, _tao_environment);" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // TODO
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
#if 0 // causing ambiguity on NT compiler
                      *os << arg->local_name () << " = " << arg->local_name ()
                          << "_out;" << nl;
#endif
                      *os << "nv_" << arg->local_name () << "->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_out") << " " <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << "_out " << arg->local_name () <<
                        ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              } // end switch direction
          } // end of if
        else if (bpd->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
          {
            switch (arg->direction ())
              {
              case AST_Argument::dir_IN:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << "_ptr " << arg->local_name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        "); // ORB does not own" << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_IN, _tao_environment)->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", &" << arg->local_name () << ", 0, " <<
                        "_tao_environment); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // no casting necessary as we already are object_ptr
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      *os << ", &" << arg->local_name ();
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing to be done as we are Object_ptr
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      //release the in parameter
                      *os << "CORBA::release (" << arg->local_name () << ");"
                          << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_ptr") << " " <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << "_ptr " << arg->local_name () <<
                        ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end of switch state
                break;
              case AST_Argument::dir_INOUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << "_ptr *" << arg->local_name () <<
                        " = new " << bt->name () << "_ptr;" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_INOUT, _tao_environment)->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      *os << ", &" << arg->local_name ();
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << "*" << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_ptr") << " &" <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << "_ptr &" << arg->local_name () <<
                        ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_OUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << "_ptr *" << arg->local_name () <<
                        " = new " << bt->name ()  << "_ptr;" << nl;
                      *os << bt->name () << "_out ";
                      *os << arg->local_name () << "_out (*" << arg->local_name
                        () << ");" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        ", 0); // ORB doesn't own" << nl;
#endif
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name ()
                          << " = nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_OUT, _tao_environment);" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // declare a variable
                      *os << bt->name () << "_ptr _tao_base_" <<
                        arg->local_name () << ";" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      *os << ", &_tao_base_" << arg->local_name ();
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // this was causing an ambiguity on NT compiler
                      // assign to the _out parameter
                      *os << arg->local_name () << " = _tao_base_" <<
                        arg->local_name () << ";" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << "_out, ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
#if 0 // causing ambiguity on NT compiler
                      *os << "*" << arg->local_name () << " = " <<
                        arg->local_name () << "_out;" << nl;
#endif
                      *os << "nv_" << arg->local_name () << "->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_out") << " " <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << "_out " << arg->local_name () <<
                        ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              } // end switch direction
          } // end else if
        else // simple predefined types
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
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        "); // ORB does not own" << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_IN, _tao_environment)->value ()"
                          << "->replace (" << bt->tc_name ()
                          << ", &" << arg->local_name () << ", 0, " <<
                        "_tao_environment); // ORB does not own" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      *os << ", &" << arg->local_name ();
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif) << " " <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << " " << arg->local_name () << ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_INOUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = new " << bt->name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        "); // ORB owns " << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_INOUT, _tao_environment)->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      *os << ", &" << arg->local_name ();
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << "*" << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif) << " &" <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << " &" << arg->local_name () << ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_state_argument - unknown state\n"), -1);
                    }
                  } // end switch state
                break;
              case AST_Argument::dir_OUT:
                switch (cg->state ())
                  {
                  case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                    {
                    // declare a variable
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = new " << bt->name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 1); // ORB owns" << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_OUT, _tao_environment)->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                    {
                      *os << ", &" << arg->local_name ();
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                    {
                      *os << "*" << arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                    {
                      // nothing
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CH:
                    {
                      *os << bt->nested_type_name (bif, "_out") << " " <<
                        arg->local_name () << ", ";
                    }
                    break;
                  case TAO_CodeGen::TAO_ARGUMENT_CS:
                  case TAO_CodeGen::TAO_ARGUMENT_SH:
                    {
                      *os << bt->name () << "_out " << arg->local_name () <<
                        ", ";
                    }
                    break;
                  default:
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) be_state_argument - unknown state\n"), -1);
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
#if 0
                  // XXXASG- check for memory leak here - TODO
                    // declare a variable
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->name ();
                  else
                    *os << "char *";
#endif
                  *os << "char *" << arg->local_name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_IN, _tao_environment)->value ()->replace ("
                      << bt->tc_name () << ", &" << arg->local_name () <<
                    ", 0, _tao_environment); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // we need to free the "in" string that got decoded
                  *os << "CORBA::string_free (" << arg->local_name () << ");"
                      << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
#if 0
                  *os << "const ";
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->nested_type_name (bif);
                  else
                    *os << "char *";
#endif
                  *os << "const char *" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              case TAO_CodeGen::TAO_ARGUMENT_CS:
                {
#if 0
                  *os << "const ";
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->name ();
                  else
                    *os << "char *";
#endif
                  *os << "const char *" << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
                }
              } // end switch state
            break;
          case AST_Argument::dir_INOUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
#if 0
                    // declare a variable
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    {
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = new " << bt->name () << ";" << nl;
                    }
                  else
                    {
                      *os << "char **" << arg->local_name () <<
                        " = new char *;" << nl;
                    }
#endif
                  *os << "char **" << arg->local_name () << " = new char *;" <<
                    nl;
#if 0
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_INOUT, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", " << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << "*" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
#if 0
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->nested_type_name (bif, " &");
                  else
                    *os << "char *&";
#endif
                  *os << "char *&" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_SH:
              case TAO_CodeGen::TAO_ARGUMENT_CS:
                {
#if 0
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->name ();
                  else
                    *os << "char *&";
#endif
                  *os << "char *&" << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
                }
              } // end switch state
            break;
          case AST_Argument::dir_OUT:
            switch (cg->state ())
              {
              case TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS:
                {
#if 0
                    // declare a variable
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    {
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = new " << bt->name () << ";" << nl;
                    }
                  else
                    {
                      *os << "char **" << arg->local_name () <<
                        " = new char *;" << nl;
                    }
                  // now declare a _out variable to be passed as a parameter
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->name () << "_out";
                  else
                    *os << "CORBA::String_out";
#endif
                  *os << "char **" << arg->local_name () <<
                    " = new char *;" << nl;
                  *os << "CORBA::String_out " << arg->local_name () <<
                    "_out (*" << arg->local_name () << ");" << nl;
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", _tao_base_" << arg->local_name () <<
                    ", 1); // ORB owns" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_OUT, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", " << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
#if 0
                  // declare a string variable
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->name ();
                  else
                    *os << "char *";
#endif
                  *os << "char *_tao_base_" << arg->local_name () << ";" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &_tao_base_" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // assign the _tao_base_<string> variable to the out
                  // parameter
                  *os << arg->local_name () << " = _tao_base_" <<
                    arg->local_name () << ";" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << "_out, ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
#if 0 // causing ambiguity on NT compiler
                  // out parameter is cast back to the real parameter
                  *os << "*" << arg->local_name () << " = " <<
                    arg->local_name () << "_out;" << nl;
#endif
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
#if 0
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->nested_type_name (bif, "_out");
                  else
                    *os << "CORBA::String_out";
#endif
                  *os << "CORBA::String_out " << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
#if 0
                  if (bt->node_type () == AST_Decl::NT_typedef)
                    *os << bt->name () << "_out";
                  else
                    *os << "CORBA::String_out";
#endif
                  *os << "CORBA::String_out " << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", " << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_INOUT, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", &" << arg->local_name () << ", 0, " <<
                    "_tao_environment); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // XXXASG TODO
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  // XXXASG TODO
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // XXXASG TODO
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << "const " << bt->nested_type_name (bif) << " " <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << "const " << bt->name () << " " << arg->local_name ()
                      << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // TODO XXXASG
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  // TODO XXXASG
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // TODO XXXASG
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << bt->nested_type_name (bif, "_slice") << " *" <<
                        arg->local_name () << ", ";
                    }
                  else
                    {
                      *os << bt->nested_type_name (bif) << " " <<
                        arg->local_name () << ", ";
                    }
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name ();
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << "_slice *";
                    }
                  *os << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // TODO XXXASG
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  // TODO XXXASG
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // TODO XXXASG
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " " <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << "_out " << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_IN, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", &" << arg->local_name () << ", 0, " <<
                    "_tao_environment); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing - the destructor will ensure proper memory release
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << "const " << bt->nested_type_name (bif) << " &" <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << "const " << bt->name () << " &" << arg->local_name ()
                      << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
                  *os << bt->name () << " *" << arg->local_name () <<
                    " = new " << bt->name () << ";" << nl;
#if 0
                  // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                  // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                    "\", CORBA::ARG_INOUT, _tao_environment)->value ()->" <<
                    "replace (" << bt->tc_name ()
                      << ", " << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
#if 0
                  // if we are sequence, call init manager
                  if (type->node_type () == AST_Decl::NT_sequence)
                    {
                      be_sequence *seq = be_sequence::narrow_from_decl (type);
                      // init_mgr method for managed types
                      switch (seq->managed_type ())
                        {
                        case be_sequence::MNG_OBJREF:
                        case be_sequence::MNG_STRING:
                          *os << arg->local_name () << ".init_mgr ();" << nl;
                          break;
                        default:
                          break;
                        }
                    }
#endif
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << "*" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif) << " &" << arg->local_name
                    () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << " &" << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
                  // for a variable sized type, the caller allocates a pointer
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                    // declare a variable
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = 0;" << nl;
                      *os << bt->name () << "_out " << arg->local_name () <<
                        "_out (" << arg->local_name () << ");" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", " << arg->local_name () <<
                        ", 0); // ORB does not own" << nl;
#endif
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name ()
                          << " = nvlist->add_item (\"" << arg->local_name
                        () << "\", CORBA::ARG_OUT, _tao_environment);" << nl;
                    }
                  else
                    {
                    // declare a variable
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = new " << bt->name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                      *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                        ";" << nl;
                    // declare an Any
                      *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                        bt->tc_name () << ", &" << arg->local_name () <<
                        ", 0); // ORB does not own" << nl;
#endif
                      // insert into the Any
                      *os << "(void) nvlist->add_item (\"" << arg->local_name () <<
                        "\", CORBA::ARG_OUT, _tao_environment)->value ()->" <<
                        "replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    } // end else
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // declare a variable. We need this only if we are variable
                  // sized. If we are fixed sized, we already know our size and
                  // hence we have already been allocated
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << bt->name () << " *_tao_base_" << arg->local_name
                        () << " = new " << bt->name () << ";" << nl;
                    }
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << ", _tao_base_" << arg->local_name ();
                    }
                  else
                    {
                      // simply pass our address
                      *os << ", &" << arg->local_name ();
                    }
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
#if 0
                  // if we are sequence, call init manager
                  if (type->node_type () == AST_Decl::NT_sequence)
                    {
                      be_sequence *seq = be_sequence::narrow_from_decl (type);
                      // init_mgr method for managed types
                      switch (seq->managed_type ())
                        {
                        case be_sequence::MNG_OBJREF:
                        case be_sequence::MNG_STRING:
                          *os << "_tao_base_" << arg->local_name () <<
                            "->init_mgr ();" << nl;
                          break;
                        default:
                          break;
                        }
                    }
#endif
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
                      *os << arg->local_name () << " = _tao_base_" <<
                        arg->local_name () << ";" << nl;
                    }
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  if (bt->size_type () == be_decl::VARIABLE)
                    *os << arg->local_name () << "_out, ";
                  else
                    *os << "*" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  if (bt->size_type () == be_decl::VARIABLE)
                    {
#if 0 // causing ambiguity on NT compiler
                      *os << arg->local_name () << " = " << arg->local_name ()
                          << "_out;" << nl;
#endif
                      *os << "nv_" << arg->local_name () << "->" <<
                        "value ()->replace (" << bt->tc_name ()
                          << ", " << arg->local_name () << ", 1, " <<
                        "_tao_environment); // ORB owns" << nl;
                    }
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " " <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << "_out " << arg->local_name () << ", ";
                  break;
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
#if 0
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name
                    () << "\", CORBA::ARG_IN, _tao_environment)->value ()"
                      << "->replace (" << bt->tc_name ()
                      << ", &" << arg->local_name () << ", 0, " <<
                    "_tao_environment); // ORB does not own" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif) << " " << arg->local_name
                    () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << " " << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
                      *os << bt->name () << " *" << arg->local_name () <<
                        " = new " << bt->name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name
                    () << "\", CORBA::ARG_INOUT, _tao_environment)->" <<
                    "value ()->replace (" << bt->tc_name ()
                      << ", " << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << "*" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif) << " &" << arg->local_name
                    () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << " &" << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
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
                  *os << bt->name () << " *" << arg->local_name () <<
                    " = new " << bt->name () << ";" << nl;
#if 0
                    // now define a NamedValue_ptr
                  *os << "CORBA::NamedValue_ptr nv_" << arg->local_name () <<
                    ";" << nl;
                    // declare an Any
                  *os << "CORBA::Any \t any_" << arg->local_name () << " (" <<
                    bt->tc_name () << ", &" << arg->local_name () <<
                    "); // ORB does not own" << nl;
#endif
                  // insert into the Any
                  *os << "(void) nvlist->add_item (\"" << arg->local_name
                    () << "\", CORBA::ARG_OUT, _tao_environment)->" <<
                    "value ()->replace (" << bt->tc_name ()
                      << ", " << arg->local_name () << ", 1, " <<
                    "_tao_environment); // ORB owns" << nl;
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
                {
                  *os << ", &" << arg->local_name ();
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
                {
                  *os << "*" << arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS:
                {
                  // nothing
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CH:
                {
                  // to keep the MSVC++ compiler happy
                  *os << bt->nested_type_name (bif, "_out") << " " <<
                    arg->local_name () << ", ";
                }
                break;
              case TAO_CodeGen::TAO_ARGUMENT_CS:
              case TAO_CodeGen::TAO_ARGUMENT_SH:
                {
                  *os << bt->name () << "_out " << arg->local_name () << ", ";
                }
                break;
              default:
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                   "(%N:%l) be_state_argument - unknown state\n"), -1);
                }
              } // end switch state
          } // end case
          break;
        } // end switch direction
      break;
    case AST_Decl::NT_native:
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_ARGUMENT_CH:
            // to keep the MSVC++ compiler happy
            *os << bt->nested_type_name (bif) << " " << arg->local_name () <<
              ", ";
            break;
          case TAO_CodeGen::TAO_ARGUMENT_CS:
          case TAO_CodeGen::TAO_ARGUMENT_SH:
            *os << bt->name () << arg->local_name () << ", ";
            break;
          case TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS:
          case TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS:
            *os << "env.exception (new CORBA::MARSHAL " <<
              "(CORBA::COMPLETED_NO));" << nl;
            break;
          default:
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
      } // end of case
      break;
    } //end switch node type
  return 0;
}
