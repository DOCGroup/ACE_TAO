// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_exception.cpp
//
// = DESCRIPTION
//    state based code generation for exceptions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

be_state_exception::be_state_exception (void)
{
}

// generate code for exception member
int
be_state_exception ::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL nl; // end of line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_field *f;       // field node
  be_exception *bexcp;  // enclosing exception node

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_EXCEPTION_CH:
      os = cg->client_header ();
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CS:
      os = cg->client_stubs ();
      break;
    case TAO_CodeGen::TAO_EXCEPTION_CI:
      os = cg->client_inline ();
      break;
    }

  f = be_field::narrow_from_decl (d); // downcast to field node
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_exception.cpp - "
                         "Bad field\n"),
                        -1);
    }

  bexcp = be_exception::narrow_from_scope (f->defined_in ());
  if (!bexcp)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_state_exception.cpp - "
                         "Bad exception\n"),
                        -1);
    }

  // pass the field node just incase it is needed to generate definition for
  // sequence
  cg->node (f);

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
          case TAO_CodeGen::TAO_EXCEPTION_CH:
            {
              os->indent (); // start from current indentation
              // constructor taking this type
              *os << bexcp->local_name () << "(const " << bt->nested_type_name
                (bexcp, "_ptr") << "&); // constructor" << nl;
              *os << bt->nested_type_name (bexcp, "_var") << " " << f->local_name
                () << ";\n";
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CI:
            {
              os->indent (); // start from current indentation
              // constructor per member
              *os << "ACE_INLINE" << nl;
              *os << bexcp->name () << "::" << bexcp->local_name () <<
                "(const " << bt->name () << "_ptr &_tao_param)" << nl;
              *os << "\t:ACE_CORBA_1 (UserException) (" <<
                "ACE_CORBA_3 (TypeCode, _duplicate) (" << bexcp->tc_name () <<
                "))" << nl;
              *os << "{\n";
              os->incr_indent ();
              *os << "this->" << f->local_name () << " = _tao_param;\n";
              os->decr_indent ();
              *os << "}\n\n";
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CS:
            {
              // assign
              os->indent (); // start with current indentation
              *os << "this->" << f->local_name () << " = _tao_excp." <<
                f->local_name () << ";\n";
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_exception - unknown state\n"), -1);
            }
          } // end switch state
      } // end case interface
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        be_predefined_type *bpd = be_predefined_type::narrow_from_decl (type);
        if (!bpd)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_state_exception - "
                               "bad predefined type\n"), -1);
          }
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_EXCEPTION_CH:
            {
              switch (bpd->pt ())
                {
                case AST_PredefinedType::PT_any:
                case AST_PredefinedType::PT_pseudo:
                  {
                    os->indent (); // start from current indentation
                    // constructor taking this type
                    *os << bexcp->local_name () << "(const " <<
                      bt->nested_type_name (bexcp, "_ptr") <<
                      "&); // constructor" << nl;
                    *os << bt->nested_type_name (bexcp) << "_var " <<
                      f->local_name () << ";\n";
                  }
                  break;
                default:
                  {
                    os->indent (); // start from current indentation
                    // constructor taking this type
                    *os << bexcp->local_name () << "(const " <<
                      bt->nested_type_name (bexcp) <<
                      "); // constructor" << nl;
                    *os << bt->nested_type_name (bexcp) << " " << f->local_name
                      () << ";\n";
                  }
                }
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CI:
            {
              switch (bpd->pt ())
                {
                case AST_PredefinedType::PT_any:
                case AST_PredefinedType::PT_pseudo:
                  {
                    os->indent (); // start from current indentation
                    // constructor per member
                    *os << "ACE_INLINE" << nl;
                    *os << bexcp->name () << "::" << bexcp->local_name () <<
                      "(const " << bt->name () << "_ptr &_tao_param)" << nl;
                    *os << "\t:ACE_CORBA_1 (UserException) (" <<
                      "ACE_CORBA_3 (TypeCode, _duplicate) (" << bexcp->tc_name
                      () << "))" << nl;
                    *os << "{\n";
                    os->incr_indent ();
                    *os << "this->" << f->local_name () << " = _tao_param;\n";
                    os->decr_indent ();
                    *os << "}\n\n";
                  }
                  break;
                default:
                  {
                    os->indent (); // start from current indentation
                    // constructor per member
                    *os << "ACE_INLINE" << nl;
                    *os << bexcp->name () << "::" << bexcp->local_name () <<
                      "(const " << bt->name () << " _tao_param)" << nl;
                    *os << "\t:ACE_CORBA_1 (UserException) (" <<
                      "ACE_CORBA_3 (TypeCode, _duplicate) (" << bexcp->tc_name
                      () << "))" << nl;
                    *os << "{\n";
                    os->incr_indent ();
                    *os << "this->" << f->local_name () << " = _tao_param;\n";
                    os->decr_indent ();
                    *os << "}\n\n";
                  }
                }
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CS:
            {
              // assign
              os->indent (); // start with current indentation
              *os << "this->" << f->local_name () << " = _tao_excp." <<
                f->local_name () << ";\n";
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_exception - unknown state\n"), -1);
            }
          } // end switch state
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_EXCEPTION_CH:
            {
              os->indent (); // start from current indentation
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  // constructor taking this type
                  *os << bexcp->local_name () << "(const " <<
                    bt->nested_type_name (bexcp) <<
                    "); // constructor" << nl;
                  *os << bt->nested_type_name (bexcp, "_var") << " " <<
                    f->local_name () << ";\n";
                }
              else
                {
                  // constructor taking this type
                  *os << bexcp->local_name () <<
                    "(const char *); //constructor" << nl;
                  *os << "CORBA::String_var " << f->local_name () << ";\n";
                }
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CI:
            {
              os->indent (); // start from current indentation
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  os->indent (); // start from current indentation
                    // constructor per member
                  *os << "ACE_INLINE" << nl;
                  *os << bexcp->name () << "::" << bexcp->local_name () <<
                    "(const " << bt->name () << " _tao_param)" << nl;
                  *os << "\t:ACE_CORBA_1 (UserException) (" <<
                    "ACE_CORBA_3 (TypeCode, _duplicate) (" << bexcp->tc_name
                    () << "))" << nl;
                  *os << "{\n";
                  os->incr_indent ();
                  *os << "this->" << f->local_name () << " = _tao_param;\n";
                  os->decr_indent ();
                  *os << "}\n\n";
                }
              else
                {
                  os->indent (); // start from current indentation
                    // constructor per member
                  *os << "ACE_INLINE" << nl;
                  *os << bexcp->name () << "::" << bexcp->local_name () <<
                    "(const char *_tao_param)" << nl;
                  *os << "\t:ACE_CORBA_1 (UserException) (" <<
                    "ACE_CORBA_3 (TypeCode, _duplicate) (" << bexcp->tc_name
                    () << "))" << nl;
                  *os << "{\n";
                  os->incr_indent ();
                  *os << "this->" << f->local_name () << " = _tao_param;\n";
                  os->decr_indent ();
                  *os << "}\n\n";
                }
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CS:
            {
              // assign
              os->indent (); // start with current indentation
              *os << "this->" << f->local_name () << " = _tao_excp." <<
                f->local_name () << ";\n";
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_exception - unknown state\n"), -1);
            }
          } // end switch state
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_enum: // type is an enum
    case AST_Decl::NT_union: // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_EXCEPTION_CH:
            {
              // We first need to generate code for this aggregate type. Check
              // if we are not called recursively thru a typedef
              if (bt->node_type () != AST_Decl::NT_typedef
                  && !bt->imported ())
                if (bt->gen_client_header () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_exception - "
                                       "error generating code for type\n"),
                                      -1);
                  }
              os->indent ();
              // constructor taking this type
              *os << bexcp->local_name () << "(const " << bt->nested_type_name
                (bexcp) << "&); // constructor" << nl;
              *os << bt->nested_type_name (bexcp) << " " << f->local_name () <<
                ";\n";
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CI:
            {
              // We first need to generate code for this aggregate type. Check
              // if we are not called recursively thru a typedef
              if (bt->node_type () != AST_Decl::NT_typedef
                  && !bt->imported ())
                if (bt->gen_client_inline () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_exception - "
                                       "error generating code for type\n"),
                                      -1);
                  }
              os->indent (); // start from current indentation
              // constructor per member
              *os << "ACE_INLINE" << nl;
              *os << bexcp->name () << "::" << bexcp->local_name () <<
                "(const " << bt->name () << " &_tao_param)" << nl;
              *os << "\t:ACE_CORBA_1 (UserException) (" <<
                "ACE_CORBA_3 (TypeCode, _duplicate) (" << bexcp->tc_name () <<
                "))" << nl;
              *os << "{\n";
              os->incr_indent ();
              *os << "this->" << f->local_name () << " = _tao_param;\n";
              os->decr_indent ();
              *os << "}\n\n";
            }
            break;
          case TAO_CodeGen::TAO_EXCEPTION_CS:
            {
              // We first need to generate code for this aggregate type. Check
              // if we are not called recursively thru a typedef
              if (bt->node_type () != AST_Decl::NT_typedef
                  && !bt->imported ())
                if (bt->gen_client_stubs () == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       "(%N:%l) be_state_exception - "
                                       "error generating code for type\n"),
                                      -1);
                  }
              // assign
              os->indent (); // start with current indentation
              *os << "this->" << f->local_name () << " = _tao_excp." <<
                f->local_name () << ";\n";
            }
            break;
          default:
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_state_exception - unknown state\n"), -1);
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
  // the enclosing exception will be variable length if the field's type is
  // variable length
  bexcp->size_type (type->size_type ());
  return 0;
}
