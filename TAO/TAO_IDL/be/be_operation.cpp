// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_operation.cpp
//
// = DESCRIPTION
//    Extension of class AST_Operation that provides additional means for C++
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
 * BE_Operation
 */
be_operation::be_operation (void)
  : argument_count_ (-1),
    has_native_ (0)
{
}

be_operation::be_operation (AST_Type *rt, AST_Operation::Flags fl,
                            UTL_ScopedName *n, UTL_StrList *p)
  : AST_Operation (rt, fl, n, p),
    AST_Decl (AST_Decl::NT_op, n, p),
    UTL_Scope (AST_Decl::NT_op),
    argument_count_ (-1),
    has_native_ (0)
{
}

// compute total number of members
int
be_operation::compute_argument_attr (void)
{
  if (this->argument_count_ != -1)
    return 0;

  this->argument_count_ = 0;

  // if there are elements in this scope
  if (this->nmembers () > 0)
    {
      // instantiate a scope iterator.
      UTL_ScopeActiveIterator *si =
	new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);

      while (!(si->is_done ()))
        {
          // get the next AST decl node
	  AST_Decl *d = si->item ();

          if (!d->imported ()
	      && d->node_type () == AST_Decl::NT_argument)
	    {
	      this->argument_count_++;
	      be_argument *arg = be_argument::narrow_from_decl (d);
	      be_type* type =
		be_type::narrow_from_decl (arg->field_type ());

	      if (type->base_node_type () == AST_Decl::NT_native)
		this->has_native_ = 1;
	    }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }

  be_type* type = be_type::narrow_from_decl (this->return_type ());
  if (type->base_node_type () == AST_Decl::NT_native)
    this->has_native_ = 1;

  return 0;
}

// return the member count
int
be_operation::argument_count (void)
{
  this->compute_argument_attr ();

  return this->argument_count_;
}

// return if any argument or the return type is a <native> type.
int
be_operation::has_native (void)
{
  this->compute_argument_attr ();

  return this->has_native_;
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

int
be_operation::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  ch = cg->client_header ();

  ch->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *ch << "virtual ";

  // first generate the return type
  bt = be_type::narrow_from_decl (this->return_type ());
  cg->push (TAO_CodeGen::TAO_OPERATION_CH); // we are now generating an
                                            // operation definition with the
                                            // return type first

  s = cg->make_state (); // retrieve state based object

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_operation::gen_client_header - "
                         "return type generation failure\n"),
                        -1);
    }
  cg->pop (); // restore previous state

  // generate the operation name
  *ch << " " << this->local_name () << " (";

  // generate the arguments with the appropriate mapping
  cg->push (TAO_CodeGen::TAO_ARGUMENT_CH); // generating code for arguments in
                                           // the signature
  if (be_scope::gen_client_header () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_operation::gen_client_header - "
                         "failed in argument generation in signature\n"),
                        -1);
    }
  cg->pop ();

  // XXXASG
  // additional argument in the form of CORBA::Environment since TAO does not
  // yet support C++ style Exceptions. However, we can have an option to the
  // "tao" IDL compiler to ask it to generate code for exceptions. This will be
  // handled in a later release.
  *ch << "CORBA::Environment &env);\n";

  return 0;
}

int
be_operation::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  UTL_ScopeActiveIterator *si; // scope iterator
  AST_Decl *d;       // temp node
  be_argument *bd;   // argument node
  be_predefined_type *bpd=0; // predefined type used for return VOID type
  be_state *s;       // state based code gen object
  long paramtblsize = 0;  // size of the paradata table


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();

  // for each operation, generate the paramdata and calldata tables followed by
  // the actual stub

  cs->indent (); // start with current indentation level

  // retrieve the return type
  bt = be_type::narrow_from_decl (this->return_type ());

  // bt holds the return type. Declare a variable that will hold the return
  // type. However, we must be careful if the return type is a void
  if (bt->node_type () == AST_Decl::NT_pre_defined)
    {
      bpd = be_predefined_type::narrow_from_decl (bt);
    }

  if (!this->has_native ())
    {
      // generate the TAO_Param_Data table
      *cs << "static const TAO_Param_Data " << this->flatname () <<
	"_paramdata [] = " << nl;
      *cs << "{\n";
      cs->incr_indent ();

      // entry for the return type
      *cs << "{" << bt->tc_name () << ", PARAM_RETURN, ";

      // Are we returning a pointer to value? i.e., is the type
      // variable? If it is, we must tell the stub what is the size of
      // the top level structure
      if (bt->size_type () == be_decl::VARIABLE)
	{
	  switch (bt->node_type ())
	    {
	    case AST_Decl::NT_interface:
	    case AST_Decl::NT_interface_fwd:
	    case AST_Decl::NT_string:
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
      paramtblsize++;

      // if we have any arguments, get each one of them
      if (this->nmembers () > 0)
	{
	  // if there are elements in this scope
	  
	  si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
	  // instantiate a scope iterator.
	  
	  while (!(si->is_done ()))
	    {
	      // get the next AST decl node
	      d = si->item ();
	      // only if this is an argument node
	      if (d->node_type () == AST_Decl::NT_argument)
		{
		  bd = be_argument::narrow_from_decl (d);
		  bt = be_type::narrow_from_decl (bd->field_type ());
		  *cs << "," << nl; // put a comma and newline before the
		  // previous entry
		  *cs << "{" << bt->tc_name ();
		  // based on the direction, output the appropriate constant.
		  switch (bd->direction ())
		    {
		    case AST_Argument::dir_IN:
		      *cs << ", PARAM_IN, 0}";
		      break;
		    case AST_Argument::dir_INOUT:
		      *cs << ", PARAM_INOUT, 0}";
		      break;
		    case AST_Argument::dir_OUT:
		      {
			*cs << ", PARAM_OUT, 0}";
#if 0
			// Are we returning a pointer to value? i.e.,
			// is the type variable? If it is, we must
			// tell the stub what is the size of the top
			// level structure 
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
				  be_predefined_type *bpd =
				    be_predefined_type::narrow_from_decl (bt);
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
#endif
		      }
		      break;
		    } // end switch
		  paramtblsize++;
		} // end if argument node
	      si->next ();
	    } // end of while
	  delete si; // free the iterator object
	} // end of arg list
      *cs << "\n";
      cs->decr_indent ();
      *cs << "};\n\n";

      // now generate the calldata table

      cs->indent ();
      *cs << "static const TAO_Call_Data " << this->flatname ()
	  << "_calldata = " << nl
	  << "{"
	  << "\"" << this->local_name () << "\", ";

      // are we oneway or two operation?
      if (this->flags () == AST_Operation::OP_oneway)
	{
	  *cs << "0, "; // for false
	}
      else
	{
	  *cs << "1, "; // for true
	}
      // insert the size of the paramdata table
      *cs << paramtblsize << ", ";
      
      // insert the address of the paramdata table
      *cs << this->flatname () << "_paramdata, ";

      // XXXASG - Exception list goes here (if it exists) - TODO
      *cs << "0, 0};\n\n";
    }

  // now generate the actual stub

  // retrieve the return type again because we have used bt to also retrieve
  // the argument types
  bt = be_type::narrow_from_decl (this->return_type ());

  // bt holds the return type. Declare a variable that will hold the return
  // type. However, we must be careful if the return type is a void
  if (bt->node_type () == AST_Decl::NT_pre_defined)
    {
      bpd = be_predefined_type::narrow_from_decl (bt);
    }

  // first generate the return type
  cs->indent ();
  cg->push (TAO_CodeGen::TAO_OPERATION_RETURN_TYPE_CS); // declare a return
                                                        // type of the stub
  s = cg->make_state ();
  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_operation::gen_client_stubs - "
                         "return type generation failure\n"),
                        -1);
    }
  cg->pop ();

  // generate the operation name
  *cs << " " << this->name () << " (";

  // generate the arguments with the appropriate mapping
  cg->push (TAO_CodeGen::TAO_ARGUMENT_CS); // generating code for arguments in
                                           // the signature
  if (be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_operation::gen_client_stubs - "
                         "failure in argument generation in signature\n"),
                        -1);
    }
  cg->pop ();

  // last argument - is always CORBA::Environment
  *cs << "CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();

  if (this->has_native ())
    {
      *cs << "env.exception (new CORBA::MARSHAL "
	  << "(CORBA::COMPLETED_NO));" << nl;
      // return the appropriate error value on exception
      cg->push (TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS);
      s = cg->make_state ();
      if (!s || !bt || (s->gen_code (bt, this) == -1))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs - "
			     "failure returning from exception\n"),
			    -1);
	}
    }
  else
    {
      // declare a return type
      cg->push (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_CS);
      s = cg->make_state ();
      if (!s || !bt || (s->gen_code (bt, this) == -1))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs - "
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
      *cs << "env.exception (new CORBA::DATA_CONVERSION "
	  << "(CORBA::COMPLETED_NO));" << nl;
      
      // return the appropriate error value on exception
      cg->push (TAO_CodeGen::TAO_OPERATION_RETVAL_EXCEPTION_CS);
      s = cg->make_state ();
      if (!s || !bt || (s->gen_code (bt, this) == -1))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs - "
			     "failure returning from exception\n"),
			    -1);
	}
      cg->pop ();

      cs->decr_indent ();
      *cs << "}" << nl;
      *cs << "this->Release (); "
	  << "// QueryInterface has bumped up our refcount" << nl;

      // do any pre do_call stuff with arguments
      cg->push (TAO_CodeGen::TAO_ARGUMENT_PRE_DOCALL_CS);
      if (be_scope::gen_client_stubs () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs - "
			     "failure generating pre docall stuff\n"),
			    -1);
	}
      cg->pop ();

      // call do_call with appropriate number of arguments
      *cs << "istub->do_call (env, &" << this->flatname ()
	  << "_calldata";

      // if our return type is not void, then pass the address of retval
      if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
	{
	  *cs << ", &retval";
	}
      else
	{
	  // pass a 0
	  *cs << ", 0";
	}

      cg->push (TAO_CodeGen::TAO_ARGUMENT_DOCALL_CS);
      if (be_scope::gen_client_stubs () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs - "
			     "failed to emit code for arguments in docall\n"),
			    -1);
	}
      cg->pop ();
      *cs << ");" << nl;

      // do any post do_call stuff with arguments
      cg->push (TAO_CodeGen::TAO_ARGUMENT_POST_DOCALL_CS);
      if (be_scope::gen_client_stubs () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs - "
			     "failed to emit code for post "
			     "docall processing\n"),
			    -1);
	}
      cg->pop ();

      // return the retval
      cg->push (TAO_CodeGen::TAO_OPERATION_RETVAL_RETURN_CS);
      s = cg->make_state ();
      if (!s || !bt || (s->gen_code (bt, this) == -1))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "(%N:%l) be_operation::gen_client_stubs\n"
			     "return val return generation failure\n"),
			    -1);
	}
      cg->pop ();
    }

  cs->decr_indent (0);
  *cs << "\n}\n\n";
  return 0;
}

int
be_operation::gen_server_header (void)
{
  TAO_OutStream *sh; // output stream
  be_type *bt;       // type node
  be_state *s;       // state based code generator object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_OPERATION_SH); // we are now generating an
                                            // operation definition

  sh = cg->server_header ();
  sh->indent (); // start with the current indentation level

  // every operation is declared virtual
  *sh << "virtual ";

  // first generate the return type
  bt = be_type::narrow_from_decl (this->return_type ());
  s = cg->make_state (); // retrieve code gen object for this state
  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_server_header\n"));
      ACE_ERROR ((LM_ERROR, "return type generation failure\n"));
      return -1;
    }
  // generate the operation name
  *sh << " " << this->local_name () << " (";

  // generate the arguments with the appropriate mapping
  cg->push (TAO_CodeGen::TAO_ARGUMENT_SH); // generating code for arguments in
                                           // the signature
  if (be_scope::gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_server_header\n"));
      ACE_ERROR ((LM_ERROR, "Argument generation failure\n"));
      return -1;
    }
  cg->pop ();

  // XXXASG
  // additional argument in the form of CORBA::Environment since TAO does not
  // yet support Exceptions. However, we can have an option to the "tao" IDL
  // compiler to ask it to generate code for exceptions. This will be handled
  // in a later release.
  *sh << " CORBA::Environment &env) = 0; // pure virtual\n";

  sh->indent ();

  // generate the static method corresponding to this method
  *sh << "static void " << this->local_name () <<
    "_skel (CORBA::ServerRequest &req, void *obj,"
      << " void *context, CORBA::Environment &env);\n\n";
  cg->pop (); // restore previous state
  return 0;
}

// Generate code for the operation skeleton that makes the upcall.
// Special Note: We deviate a bit from our policy of handing over code
// generation for elements in our scope to the be_scope class. For this method,
// it is best to simulate that behavior here as it involves a lot of
// complexity.
int
be_operation::gen_server_skeletons (void)
{
  TAO_OutStream *ss; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_type *rt;       // type node for the return type
  UTL_ScopeActiveIterator *si; // scope iterator
  AST_Decl *d;       // temp node
  be_argument *bd = 0;   // argument node
  be_state *s;       // state based code gen object
  be_predefined_type *bpd=0; // predefined return type
  be_interface *intf; // enclosing interface node

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  ss = cg->server_skeletons ();

  ss->indent (); // start with the current indentation level

  // retrieve our enclosing interface decl
  intf = be_interface::narrow_from_decl (ScopeAsDecl (this->defined_in ()));

  *ss << "void " << intf->full_skel_name () << "::"
      << this->local_name () << "_skel ("
      << "CORBA::ServerRequest &_tao_server_request, "
    //@@XXASG      << "CORBA::Object_ptr _tao_object_reference, "
      << "void *_tao_object_reference, "
      << "void *context, "
      << "CORBA::Environment &_tao_environment)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  *ss << "ACE_UNUSED_ARG (context);" << nl;

  if (this->has_native ())
    {
      // Native types cannot be exported...
      *ss << "_tao_environment.exception (new CORBA::MARSHAL"
	  << " (CORBA::COMPLETED_NO));\n";
    }
  else
    {
      // define an NVList to hold arguments
      *ss << "CORBA::NVList_ptr \t nvlist;" << nl;
      // define a variable that will eventually point to our
      // implementation object 
      *ss << intf->full_skel_name () << "_ptr \t impl = ("
	  << intf->full_skel_name () << "_ptr) _tao_object_reference;"
	  << nl;

      // verify if we need to define a variable intended to hold the
      // operation return type. We do not need one if the return type
      // is void

      rt = be_type::narrow_from_decl (this->return_type ());
      if (!rt)
	{
	  ACE_ERROR ((LM_ERROR,
		      "be_operation::gen_server_skeletons - bad "
		      "return type\n"));
	  return -1;
	}
      if (rt->node_type () == AST_Decl::NT_pre_defined)
	{
	  bpd = be_predefined_type::narrow_from_decl (rt);
	}
      if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
	{
	  // not a void type
	  *ss << "CORBA::Any *result;" << nl;

	  // emit the return type

	  // emit type for return value
	  cg->push (TAO_CodeGen::TAO_OPERATION_RETVAL_DECL_SS);

	  // get a state based code gen object
	  s = cg->make_state ();

	  if (s->gen_code (rt, this) == -1)
	    {
	      ACE_ERROR ((LM_ERROR,
			  "be_operation::gen_server_skeletons - "
			  "codegen failed for return type\n"));
	      return -1;
	    }
	  cg->pop ();
	}

#if 0
      // if we have any arguments, get each one of them and allocate
      // an Any and NamedValue for each. In addition, define a
      // variable of that type
      cg->push (TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS);
      if (be_scope::gen_server_skeletons () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "be_operation::gen_server_skeletons - "
			     "argument gen code failed\n"),
			    -1);
	}
      *ss << "\n";
      cg->pop ();
#endif

      // declare an NVList and create one
      ss->indent ();
      *ss << "// create an NV list and populate it with typecodes" << nl;
      *ss << "_tao_server_request.orb ()->create_list ("
	  << this->argument_count ()
	  << ", nvlist); // initialize a list" << nl;

      // add each argument according to the in, out, inout semantics
      if (this->nmembers () > 0)
	{
	  *ss << "// add each argument according to the "
	      << "in, out, inout semantics" << nl;
	  // if we have any arguments, insert its typecode and a
	  // pointer to storage for the variable
	  cg->push (TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS);
	  s = cg->make_state ();
	  if (!s)
	    {
	      ACE_ERROR_RETURN ((LM_ERROR,
				 "(%N:%l) be_operation::"
				 "gen_server_skeletons - "
				 "Bad state\n"),
				-1);
	    }

	  // if there are elements in this scope
	  si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
	  // instantiate a scope iterator.

	  while (!(si->is_done ()))
	    {
	      // get the next AST decl node
	      d = si->item ();
	      if (!d->imported ())
		{
		  // only if this is an argument node
		  if (d->node_type () == AST_Decl::NT_argument)
		    {
		      bd = be_argument::narrow_from_decl (d);
		      if (!bd)
			{
			  ACE_ERROR_RETURN ((LM_ERROR,
					     "(%N:%l) be_operation::"
					     "gen_server_skeletons - "
					     "Bad argument\n"),
					    -1);
			}
		      bt = be_type::narrow_from_decl (bd->field_type ());
		      if (!bt)
			{
			  ACE_ERROR_RETURN ((LM_ERROR,
					     "(%N:%l) be_operation::"
					     "gen_server_skeletons - "
					     "Bad type\n"),
					    -1);
			}
		      // emit code that adds this argument to the
		      // NVList
#if 0
		      *ss << "nv_" << bd->local_name ()
			  << " = nvlist->add_value (\""
			  << bd->local_name () << "\", "
			  << "any_" << bd->local_name () << ", ";
		      switch (bd->direction ())
			{
			case AST_Argument::dir_IN:
			  *ss << "CORBA::ARG_IN, _tao_environment);" << nl;
			  break;
			case AST_Argument::dir_INOUT:
			  *ss << "CORBA::ARG_INOUT, _tao_environment);" << nl;
			  break;
			case AST_Argument::dir_OUT:
			  *ss << "CORBA::ARG_OUT, _tao_environment);" << nl;
			  break;
			}
#endif
		      if (s->gen_code (bt, bd) == -1)
			{
			  ACE_ERROR_RETURN ((LM_ERROR,
					     "(%N:%l) be_operation::"
					     "gen_server_skeletons - "
					     "state based code gen failed\n"),
					    -1);
			}
		    } // end if argument node
		} // end if ! imported
	      si->next ();
	    } // end of while
	  delete si; // free the iterator object
	  cg->pop ();
	} // end of arg list

      // parse the arguments
      *ss << "// parse the arguments" << nl;
      *ss << "_tao_server_request.params (nvlist, _tao_environment);" << nl;
      *ss << "if (_tao_environment.exception ()) return;" << nl;

      cg->push (TAO_CodeGen::TAO_ARGUMENT_PRE_UPCALL_SS);
      if (be_scope::gen_server_skeletons () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "be_operation::gen_server_skeletons - "
			     "argument gen code failed\n"),
			    -1);
	}
      cg->pop ();

      // make the upcall
      //  *ss << "impl = (" << intf->full_skel_name () 
      //      << "_ptr) _tao_object_reference->get_subclass ();"
      //  << nl;
      if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
	{
	  cg->push (TAO_CodeGen::TAO_OPERATION_RETVAL_ASSIGN_SS);
	  s = cg->make_state ();
	  // emit code to assign to retval
	  if (!s || (s->gen_code (rt, this) == -1))
	    {
	      return -1;
	    }
	  *ss << " = impl->" << this->local_name () << "(";
	  cg->pop ();
	}
      else
	{
	  // void return type
	  *ss << "impl->" << this->local_name () << "(";
	}

      cg->push (TAO_CodeGen::TAO_ARGUMENT_UPCALL_SS);
      if (be_scope::gen_server_skeletons () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "be_operation::gen_server_skeletons - "
			     "argument in upcall\n"),
			    -1);
	}
      cg->pop ();
      *ss << "_tao_environment);" << nl;

      cg->push (TAO_CodeGen::TAO_ARGUMENT_POST_UPCALL_SS);
      if (be_scope::gen_server_skeletons () == -1)
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "be_operation::gen_server_skeletons - "
			     "argument gen code failed\n"),
			    -1);
	}
      cg->pop ();

      // if there is any return type, send it via the ServerRequest
      if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
	{
	  cg->push (TAO_CodeGen::TAO_OPERATION_RESULT_SS);
	  s = cg->make_state ();
	  if (!s || (s->gen_code (rt, this) == -1))
	    return -1;
	  cg->pop ();
	  *ss << "_tao_server_request.result (result, "
	      << "_tao_environment);" << nl;
	}
      *ss << "\n";
    }
  
  ss->decr_indent ();
  *ss << "}\n\n";

  return 0;
}

// Generates the client-side inline information
int
be_operation::gen_client_inline (void)
{
  // nothing to be done
  return 0;
}

// Generates the server-side inline
int
be_operation::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

// compute the size type of the node in question
int
be_operation::compute_size_type (void)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  be_decl *bd;

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          bd = be_decl::narrow_from_decl (d);
          if (bd != 0)
            {
              // our sizetype depends on the sizetype of our members. Although
              // previous value of sizetype may get overwritten, we are
              // guaranteed by the "size_type" call that once the value reached
              // be_decl::VARIABLE, nothing else can overwrite it.
              this->size_type (bd->size_type ());
            }
          else
            {
              ACE_DEBUG ((LM_DEBUG,
                          "WARNING (%N:%l) be_operation::compute_size_type - "
                          "narrow_from_decl returned 0\n"));
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

int
be_operation::accept (be_visitor *visitor)
{
  return visitor->visit_operation (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_operation, AST_Operation, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_operation)
IMPL_NARROW_FROM_SCOPE (be_operation)
