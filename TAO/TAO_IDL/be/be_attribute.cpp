// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_attribute.cpp
//
// = DESCRIPTION
//    Extension of class AST_Attribute that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Attribute
 */
be_attribute::be_attribute (void)
{
}

be_attribute::be_attribute (idl_bool ro, AST_Type *ft, UTL_ScopedName *n,
                            UTL_StrList *p)
  : AST_Attribute (ro, ft, n, p),
    AST_Field (AST_Decl::NT_attr, ft, n, p),
    AST_Decl (AST_Decl::NT_attr, n, p)
{
}

// Handling attributes is very similar to operations. Attributes are mapped to
// two methods - one to set the value and one to get the value. For a readonly
// attribute we only have the method that retrieves the value. For the "set"
// method we pass an in parameter which is of the type of the attribute. So the
// handling here is exactly like the "in" parameters for arguments. The
// handling of the "get" method is exactly like the return values of
// operations.

int
be_attribute::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  ch = cg->client_header ();

  ch->indent (); // start with the current indentation level

  bt = be_type::narrow_from_decl (this->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_header - "
                         "bad type\n"),
                        -1);
    }


  // first the "get" method

  // the retrieve method is defined virtual
  *ch << "virtual ";

  // first generate the return type
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CH); // we are now generating
                                                       // an attribute "get"
                                                       // definition with the
                                                       // return type first

  s = cg->make_state (); // retrieve state based object

  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_header - "
                         "return type generation failure\n"),
                        -1);
    }
  cg->pop (); // restore previous state

  // generate the operation name
  // additional argument in the form of CORBA::Environment since TAO does not
  // yet support C++ style Exceptions. However, we can have an option to the
  // "tao" IDL compiler to ask it to generate code for exceptions. This will be
  // handled in a later release.
  *ch << " " << this->local_name () << " (CORBA::Environment &env);\n";

  // now the set method. However, this is not defined if we are readonly
  if (!this->readonly ())
    {
      ch->indent ();
      *ch << "virtual void " << this->local_name () << "(";

      // we pass the type as an "in" parameter
      cg->push (TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CH);

      s = cg->make_state (); // retrieve state based object

      if (!s || !bt || (s->gen_code (bt, this) == -1))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_attribute::gen_client_header - "
                             "in param type generation failure\n"),
                            -1);
        }
      cg->pop (); // restore previous state
      *ch << ", CORBA::Environment &env);\n";
    }

  return 0;
}

int
be_attribute::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_state *s;       // state based code gen object


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();

  // retrieve the type
  bt = be_type::narrow_from_decl (this->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_stubs - "
                         "bad type\n"),
                        -1);
    }

  // first the retrieve method
  cs->indent (); // start with current indentation level

  // generate the TAO_Param_Data table for the "get" method
  *cs << "static const TAO_Param_Data _get_" << this->flatname () <<
    "_paramdata [] = " << nl;
  *cs << "{\n";
  cs->incr_indent ();

  // entry for the return type
  *cs << "{" << bt->tc_name () << ", PARAM_RETURN, ";
  // Are we returning a pointer to value? i.e., is the type variable? If it is,
  // we must tell the stub what is the size of the top level structure
  if (bt->size_type () == be_decl::VARIABLE)
    {
      switch (bt->node_type ())
        {
        case AST_Decl::NT_interface:
        case AST_Decl::NT_interface_fwd:
          // no need of size here
          *cs << "0}";
          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *bpd = be_predefined_type::narrow_from_decl
              (bt);
            if (bpd->pt () == AST_PredefinedType::PT_pseudo)
              // no need of size here
              *cs << "0}";
            else
              *cs << "sizeof (" << bt->name () << ")}";
          }
          break;
        default:
          *cs << "sizeof (" << bt->name () << ")}";
        }
    }
  else
    *cs << "0}";
  cs->decr_indent ();
  *cs << "};\n\n";

  // now generate the calldata table

  cs->indent ();
  *cs << "static const TAO_Call_Data _get_" << this->flatname () << "_calldata = "
      << nl;
  *cs << "{";
  // prepend a "_get_"
  *cs << "\"_get_" << this->local_name () << "\", ";
  *cs << "1, "; // always a twoway call
  *cs << "1, "; // size is 1
  // insert the address of the paramdata table
  *cs << "_get_" << this->flatname () << "_paramdata, ";
  // XXXASG - Exception list goes here (if it exists) - TODO
  *cs << "0, 0};\n\n";

  // now generate the actual stub

  // first generate the return type
  cs->indent ();
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_CS); // declare a return
                                                        // type of the stub
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_stubs - "
                         "return type generation failure\n"),
                        -1);
    }
  cg->pop ();

  // generate the operation name
  *cs << " " << this->name () << "(CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();

  // declare a return type
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_CS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_stubs - "
                         "retval declaration failure\n"),
                        -1);
    }
  cg->pop ();

  // generate code that calls QueryInterface
  *cs << "STUB_Object *istub;\n\n";
  cs->indent ();
  *cs << "if (this->QueryInterface (IID_STUB_Object, " <<
    "(void **)&istub) != TAO_NOERROR)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));" <<
    nl;

  // return the appropriate error value on exception
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_stubs - "
                         "failure returning from exception\n"),
                        -1);
    }
  cg->pop ();

  cs->decr_indent ();
  *cs << "}" << nl;
  *cs << "this->Release (); // QueryInterface has bumped up our refcount" << nl;

  // call do_call with appropriate number of arguments
  *cs << "istub->do_call (env, &_get_" << this->flatname () <<
    "_calldata, &retval);" << nl;

  // return the retval
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_RETURN_CS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_operation::gen_client_stubs\n"
                         "return val return generation failure\n"),
                        -1);
      return -1;
    }
  cg->pop ();

  cs->decr_indent (0);
  *cs << "\n}\n\n";

  if (this->readonly ())
    return 0;  // we were readonly, nothing else to do.

  // Now generate the stub for the "set" method
  cs->indent (); // start with current indentation level

  // generate the TAO_Param_Data table for the "set" method
  *cs << "static const TAO_Param_Data _set_" << this->flatname () <<
    "_paramdata [] = " << nl;
  *cs << "{\n";
  cs->incr_indent ();

  // entry for the return type
  *cs << "{CORBA::_tc_void, PARAM_RETURN, 0}," << nl;
  *cs << "{" << bt->tc_name () << ", PARAM_IN, 0}\n";
  cs->decr_indent ();
  *cs << "};\n\n";

  // now generate the calldata table

  cs->indent ();
  *cs << "static const TAO_Call_Data _set_" << this->flatname () << "_calldata = "
      << nl;
  *cs << "{";
  // prepend a "_set_"
  *cs << "\"_set_" << this->local_name () << "\", ";
  *cs << "1, "; // always a twoway call
  *cs << "2, "; // size is 2
  // insert the address of the paramdata table
  *cs << "_get_" << this->flatname () << "_paramdata, ";
  // XXXASG - Exception list goes here (if it exists) - TODO
  *cs << "0, 0};\n\n";

  // now generate the actual stub

  cs->indent ();
  *cs << "void " << this->name () << "(";
  // generate the in parameter
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_CS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_operation::gen_client_stubs - "
                         "failure in argument generation in signature\n"),
                        -1);
    }
  cg->pop ();

  // last argument - is always CORBA::Environment
  *cs << ", CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();

  // generate code that calls QueryInterface
  *cs << "STUB_Object *istub;\n\n";
  cs->indent ();
  *cs << "if (this->QueryInterface (IID_STUB_Object, " <<
    "(void **)&istub) != TAO_NOERROR)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));" <<
    nl;
  *cs << "return;\n";
  cs->decr_indent ();
  *cs << "}" << nl;
  *cs << "this->Release (); // QueryInterface has bumped up our refcount" << nl;

  // do any pre do_call stuff with the lone IN parameter
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_PRE_DOCALL_CS);
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_atribute::gen_client_stubs - "
                         "failure generating pre docall stuff\n"),
                        -1);
    }
  cg->pop ();

  // call do_call with appropriate number of arguments
  *cs << "istub->do_call (env, &_set_" << this->flatname () <<
  "_calldata, 0"; // no return value

  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_DOCALL_CS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_stubs - "
                         "in param in docall generation failure\n"),
                        -1);
      return -1;
    }
  cg->pop ();
  *cs << ");" << nl;

  // do any post do_call processing
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_POST_DOCALL_CS);
  s = cg->make_state ();
  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_client_stubs\n"
                         "post docall generation failure\n"),
                        -1);
      return -1;
    }
  cg->pop ();

  cs->decr_indent (0);
  *cs << "\n}\n\n";

  return 0;
}

int
be_attribute::gen_server_header (void)
{
  TAO_OutStream *sh; // output stream
  be_type *bt;       // type node
  be_state *s;       // state based code generator object
  TAO_NL nl;         // newline generator

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  sh = cg->server_header ();

  // first retrieve the type
  bt = be_type::narrow_from_decl (this->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_header - "
                         "bad type\n"),
                        -1);
    }

  // first the "get" method

  sh->indent (); // start with the current indentation level

  // the retrieve method is defined pure virtual
  *sh << "virtual ";
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETURN_TYPE_SH); // we are now generating
                                                       // an operation
                                                       // definition with the
                                                       // return type first

  s = cg->make_state (); // retrieve state based object

  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_header - "
                         "return type generation failure\n"),
                        -1);
    }
  cg->pop (); // restore previous state

  *sh << " " << this->local_name () << " (CORBA::Environment &env) = 0;" << nl;

  // generate the static method corresponding to the method
  *sh << "static void _get_" << this->local_name () <<
    "_skel (CORBA::ServerRequest &req, void *obj, " <<
    "void *context, CORBA::Environment &env);\n\n";


  // now the set method. However, this is not defined if we are readonly
  if (this->readonly ())
    return 0;

  sh->indent ();
  *sh << "virtual void " << this->local_name () << "(";

  // we pass the type as an "in" parameter
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SH);

  s = cg->make_state (); // retrieve state based object

  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_header - "
                         "in param type generation failure\n"),
                        -1);
    }
  cg->pop (); // restore previous state
  *sh << ", CORBA::Environment &env) = 0;" << nl;

  // generate the static method corresponding to the method
  *sh << "static void _set_" << this->local_name () <<
    "_skel (CORBA::ServerRequest &req, void *obj, " <<
    "void *context, CORBA::Environment &env);\n\n";


  return 0;
}

int
be_attribute::gen_server_skeletons (void)
{
  TAO_OutStream *ss; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_state *s;       // state based code gen object
  be_interface *intf; // enclosing interface node

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  ss = cg->server_skeletons ();

  ss->indent (); // start with the current indentation level

  // retrieve our type
  bt = be_type::narrow_from_decl (this->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "bad type\n"),
                        -1);
    }

  // retrieve our enclosing interface decl
  intf = be_interface::narrow_from_decl (ScopeAsDecl (this->defined_in ()));
  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "bad enclosing interface\n"),
                        -1);
    }


  // code generation - first the "get" method
  *ss << "void " << intf->full_skel_name () << "::_get_"
      << this->local_name () << "_skel ("
      << "CORBA::ServerRequest &_tao_server_request, "
      << "void *_tao_object_reference, void */*context*/, "
      << "CORBA::Environment &_tao_environment)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  // define a variable that will eventually point to our implementation object
  *ss << intf->full_skel_name () << "_ptr impl = (" << intf->full_skel_name ()
      << "_ptr) _tao_object_reference;" << nl;
  // store the result in this Any
  *ss << "CORBA::Any *result;" << nl;
  // emit the return type
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_DECL_SS); // emit type for return
                                                        // value
  // get a state based code gen object
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "codegen for return val decl failed\n"),
                        -1);
    }
  cg->pop ();

  ss->indent ();
  *ss << "// this method has no incoming parameters. Nothing to parse" << nl;

  // make the upcall
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RETVAL_ASSIGN_SS);
  s = cg->make_state ();
  // emit code to assign to retval
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "codegen for return val assign failed\n"),
                        -1);
    }
  *ss << " = impl->" << this->local_name () << "(_tao_environment);" << nl;
  cg->pop ();

  // insert our return val into the result
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_RESULT_SS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "codegen for returning result failed\n"),
                        -1);
    }
  cg->pop ();
  // insert the result into the server request
  *ss << "_tao_server_request.result (result, _tao_environment);\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  if (this->readonly ())
    return 0; // nothing else to do as we are readonly

  // generate skeleton for the "set" method

  ss->indent ();
  *ss << "void " << intf->full_skel_name () << "::_set_"
      << this->local_name () << "_skel ("
      << "CORBA::ServerRequest &_tao_server_request, "
      << "void *_tao_object_reference, void */*context*/, "
      << "CORBA::Environment &_tao_environment)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  // define an NVList to hold the in argument
  *ss << "CORBA::NVList_ptr \t nvlist;" << nl;
  // define a variable that will eventually point to our implementation object
  *ss << intf->full_skel_name () << "_ptr \t impl = (" << intf->full_skel_name
    () << "_ptr)_tao_object_reference;" << nl;

  // declare an NVList and create one
  ss->indent ();
  *ss << "// create an NV list and populate it with typecodes" << nl;
  *ss << "_tao_server_request.orb ()->create_list (1, nvlist); // initialize a list" << nl;

  // if we have any arguments, get each one of them and allocate an Any and
  // NamedValue for each. In addition, define a variable of that type
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_INPARAM_TYPE_SS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "codegen for returning result failed\n"),
                        -1);
    }
  cg->pop ();

  *ss << "// parse the arguments" << nl;
  *ss << "_tao_server_request.params (nvlist, _tao_environment);" << nl;
  *ss << "if (_tao_environment.exception ()) return;" << nl;

  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_PRE_UPCALL_SS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "preupcall code failed\n"),
                        -1);
    }
  cg->pop ();

  // make the upcall
  *ss << "impl->" << this->local_name () << "(";
  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_UPCALL_SS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "upcall code failed\n"),
                        -1);
    }
  cg->pop ();
  *ss << "_tao_environment);" << nl;

  cg->push (TAO_CodeGen::TAO_ATTRIBUTE_POST_UPCALL_SS);
  s = cg->make_state ();
  if (!s || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_attribute::gen_server_skeletons - "
                         "post upcall code failed\n"),
                        -1);
    }
  cg->pop ();

  *ss << "\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  return 0;
}

// Generates the client-side inline information
int
be_attribute::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int
be_attribute::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

int
be_attribute::accept (be_visitor *visitor)
{
  return visitor->visit_attribute (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_attribute, AST_Attribute, be_decl)
IMPL_NARROW_FROM_DECL (be_attribute)
