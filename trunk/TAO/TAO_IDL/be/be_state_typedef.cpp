// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_state_typedef.cpp
//
// = DESCRIPTION
//    state based code generation for typedefs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

// return type for typedef
be_state_typedef::be_state_typedef (void)
{
}

// generate code for structure member
int
be_state_typedef::gen_code (be_type *bt, be_decl *d, be_type *type)
{
  TAO_OutStream *os; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  be_typedef *tdef;  // typedef node
  be_decl *scope; // enclosing scope in which the typedef occurs

  switch (cg->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      os = cg->client_header (); // set the stream to be the client header
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      os = cg->client_inline ();
      break;
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      os = cg->client_stubs ();
      break;
    default:
      return -1;
    } // end of outermost switch


  tdef = be_typedef::narrow_from_decl (d); // downcast to typedef node
  if (!tdef)
    return -1;

  scope = be_scope::narrow_from_scope (tdef->defined_in ())->decl ();

  if (!type) // not a recursive call
    type = bt;
  else // recursively called thru a typedef. "type" will have the most
       // primitive
    // base class of the typedef
    ACE_ASSERT (bt->node_type () == AST_Decl::NT_typedef);

  // the typedef node has the same size type as its base type
  tdef->size_type (bt->size_type ());

  switch (type->node_type ())
    {
    case AST_Decl::NT_interface: // type is an obj reference
    case AST_Decl::NT_interface_fwd: // type is an obj reference
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              os->indent (); // start from current indentation

              // typedef of an interface results in 3 typedefs as shown below
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_ptr") << " " <<
                d->local_name () << "_ptr;" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_var") << " " <<
                d->local_name () << "_var;" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " " <<
                d->local_name () << "_out;\n\n";
            }
            break;
          default:  // nothing to do for other cases
            break;
          } // end of switch state
      }
      break;
    case AST_Decl::NT_pre_defined: // type is predefined type
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              be_predefined_type *pd = be_predefined_type::narrow_from_decl (type);

              if (!pd)
                return -1;
              os->indent (); // start from current indentation
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";";
              // if the predefined type is an ANY, we also define a typedef to _var
              if (pd->pt () == AST_PredefinedType::PT_any)
                {
                  *os << nl;
                  *os << "typedef " << bt->name () << "_var " << d->local_name
                    () << "_var;";
                }
              else if (pd->pt () == AST_PredefinedType::PT_pseudo)
                {
                  // pseudo object
                  *os << "typedef " << bt->nested_type_name (scope, "_ptr") <<
                    " " << d->local_name () << "_ptr;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_var") <<
                    " " << d->local_name () << "_var;" << nl;
                }
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                  << d->local_name () << "_out;\n\n";
            } // end of case
            break;
          default: // nothing to do for other cases
            break;
          } // end switch
      }
      break;
    case AST_Decl::NT_string: // type is a string
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              os->indent (); // start from current indentation
              if (bt->node_type () == AST_Decl::NT_typedef)
                {
                  *os << "typedef " << bt->nested_type_name (scope) << " " <<
                    d->local_name () << ";" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_var") <<
                    " " << d->local_name () << "_var;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_out") <<
                    " " << d->local_name () << "_out;\n\n";
                }
              else
                {
                  *os << "typedef CORBA::String " << d->local_name () << ";" <<
                    nl;
                  *os << "typedef CORBA::String_var " << d->local_name
                    () << "_var;" << nl;
                  *os << "typedef CORBA::String_out " << d->local_name
                    () << "_out;\n\n";
                }
            }
            break;
          default: // nothing to do for the rest of the cases
            break;
          } // end switch state
      }
      break;
    case AST_Decl::NT_enum: // type is an enum
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              os->indent (); // start from current indentation
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_state_typedef - enum gen failed\n"), -1);
                    }
                }
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                  << d->local_name () << "_out;\n\n";
            }
            break;
          default:
            break;
          } // end of switch
      }
      break;
      // these are all anonymous types
    case AST_Decl::NT_array: // type is an array
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              // if we are not here recursively, then we need to generate the
              // definition first i.e., this typedef had its immediate base
              // class that was an array declaration. The code for arrays will
              // handle all the code generation. We just prepend the word
              // "typedef".
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  os->indent ();
                  *os << "typedef ";
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                           "be_state_typedef - array gen failed\n"), -1);
                    }
                }
              else
                {
                  // we are a typedef node whose immediate base class is also a
                  // typedefed node to some array node. We simply output a
                  // number of typedef statements
                  os->indent ();
                  *os << "typedef " << bt->nested_type_name (scope) << " " <<
                    d->local_name () << ";" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_slice") <<
                    " " << d->local_name () << "_slice;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_var") << " "
                      << d->local_name () << "_var;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                      << d->local_name () << "_out;" << nl;
                  *os << "typedef " << bt->nested_type_name (scope, "_forany") << " "
                      << d->local_name () << "_forany;" << nl;

                  // typedefs for the auxiliary methods. If we are nested inside
                  // some scope, these methods become static to the enclosing scope
                  if (d->is_nested ())
                    *os << "static ";
                  *os << bt->nested_type_name (scope) << "_slice* " << d->local_name () <<
                    "_alloc (void);" << nl;
                  if (d->is_nested ())
                    *os << "static ";
                  *os << bt->nested_type_name (scope, "_slice") << " *" <<
                    d->local_name () << "_dup ("
                      << bt->nested_type_name (scope) << "_slice* " << ");" << nl;
                  if (d->is_nested ())
                    *os << "static ";
                  *os << "void " << bt->nested_type_name (scope) << "_free (" <<
                    d->name () << "_slice *);\n\n";
                }
            }
            break;
          case TAO_CodeGen::TAO_TYPEDEF_CI:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_inline () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
            }
            break;
          case TAO_CodeGen::TAO_TYPEDEF_CS:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_stubs () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
            }
            break;
          default:
            break;
          } // end of switch
      }
      break;
    case AST_Decl::NT_sequence: // type is a sequence
    case AST_Decl::NT_struct:   // type is a struct
    case AST_Decl::NT_union:    // type is a union
      {
        switch (cg->state ())
          {
          case TAO_CodeGen::TAO_TYPEDEF_CH:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_header () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
              os->indent (); // start from current indentation
              *os << "typedef " << bt->nested_type_name (scope) << " " <<
                d->local_name () << ";" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_var") << " "
                  << d->local_name () << "_var;" << nl;
              *os << "typedef " << bt->nested_type_name (scope, "_out") << " "
                  << d->local_name () << "_out;\n\n";
            }
            break;
          case TAO_CodeGen::TAO_TYPEDEF_CI:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_inline () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
            }
            break;
          case TAO_CodeGen::TAO_TYPEDEF_CS:
            {
              // if we are not here recursively, then we need to generate the
              // definition first
              if (bt->node_type () != AST_Decl::NT_typedef)
                {
                  if (bt->gen_client_stubs () == -1)
                    {
                      ACE_ERROR_RETURN ((LM_ERROR,
                          "be_state_typedef - struct/union/seq gen failed\n"),
                                        -1);
                    }
                }
            }
            break;
          default:
            break;
          } // end of switch state
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
        return this->gen_code (t, tdef, temp);
      } // end of case
      //break;  unreachable statement!
    } // end of switch

  return 0;
}
