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
{
}

be_operation::be_operation (AST_Type *rt, AST_Operation::Flags fl,
                            UTL_ScopedName *n, UTL_StrList *p)
  : AST_Operation (rt, fl, n, p),
    AST_Decl (AST_Decl::NT_op, n, p),
    UTL_Scope (AST_Decl::NT_op)
{
  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // compute the flattened fully scoped name 
  compute_flatname ();
}

int
be_operation::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  be_state *s;       // state based code gen object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_OPERATION_CH); // we are now generating an operation
                                         // definition 

  ch = cg->client_header ();
  cg->outstream (ch); // important to set the current stream

  ch->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *ch << "virtual ";

  // first generate the return type
  bt = be_type::narrow_from_decl (this->return_type ());
  s = cg->make_state (); // retrieve state based object

  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_client_header\n")); 
      ACE_ERROR ((LM_ERROR, "return type generation failure\n"));
      return -1;
    }

  // generate the operation name
  *ch << " " << this->local_name () << " (";
  // generate the arguments with the appropriate mapping
  if (be_scope::gen_client_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_client_header\n")); 
      ACE_ERROR ((LM_ERROR, "Argument generation failure\n"));
      return -1;
    }

  // XXXASG
  // additional argument in the form of CORBA::Environment since TAO does not
  // yet support C++ style Exceptions. However, we can have an option to the
  // "tao" IDL compiler to ask it to generate code for exceptions. This will be
  // handled in a later release.
  *ch << "CORBA::Environment &env);\n";
  cg->pop (); // restore previous state

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
  cg->outstream (cs); // set the current stream
 
  // for each operation, generate the paramdata and calldata tables followed by
  // the actual stub

  cs->indent (); // start with current indentation level

  // generate the TAO_Param_Data table
  *cs << "static const TAO_Param_Data " << this->flatname () << 
    "_paramdata [] = " << nl;
  *cs << "{\n";
  cs->incr_indent ();

  // entry for the return type
  bt = be_type::narrow_from_decl (this->return_type ());
  *cs << "{" << bt->tc_name () << ", PARAM_RETURN, 0}";
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
	  if (!d->imported ()) 
	    {
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
                      *cs << ", PARAM_OUT, 0}";
                      break;
                    } // end switch
                  paramtblsize++;
                } // end if argument node
            } // end if ! imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    } // end of arg list
  *cs << "\n";
  cs->decr_indent ();
  *cs << "};\n\n";

  // now generate the calldata table

  cs->indent ();
  *cs << "static const TAO_Call_Data " << this->flatname () << "_calldata = "
      << nl;
  *cs << "{";
  *cs << "\"" << this->local_name () << "\", ";

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

  // now generate the actual stub

  cg->push (TAO_CodeGen::TAO_OPERATION_CS); // we are now generating an operation
                                         // definition 
  // first generate the return type
  bt = be_type::narrow_from_decl (this->return_type ());
  s = cg->make_state ();
  if (!s || !bt || (s->gen_code (bt, this) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_client_stubs\n")); 
      ACE_ERROR ((LM_ERROR, "return type generation failure\n"));
      return -1;
    }

  // generate the operation name
  *cs << " " << this->name () << " (";
  // generate the arguments with the appropriate mapping
  if (be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_client_header\n")); 
      ACE_ERROR ((LM_ERROR, "Argument generation failure\n"));
      return -1;
    }
  // last argument
  *cs << "CORBA::Environment &env)" << nl;
  *cs << "{\n"; 
  cs->incr_indent ();

  ACE_ASSERT (cg->state () == TAO_CodeGen::TAO_OPERATION_CS);

  // bt holds the return type. Declare a variable that will hold the return
  // type. However, we must be careful if the return type is a void
  if (bt->node_type () == AST_Decl::NT_pre_defined)
    {
      bpd = be_predefined_type::narrow_from_decl (bt);
    }
  
  if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
    {
      // generate return type
      if (s->gen_code (bt, this) == -1)
        return -1;
      *cs << " retval;" << nl;
    }

  // generate code that calls QueryInterface
  *cs << "STUB_Object *istub;\n\n";
  cs->indent ();
  *cs << "if (this->QueryInterface (IID_STUB_Object, " << 
    "(void **)&istub) != NOERROR)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "env.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));" <<
    nl;

  if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
    {
      *cs << "return retval;\n";
    }
  else
    {
      *cs << "return;\n"; // return type is void
    }

  cs->decr_indent ();
  *cs << "}" << nl;
  *cs << "this->Release (); // QueryInterface has bumped up our refcount" << nl;
  *cs << "istub->do_call (env, &" << this->flatname () << "_calldata";
  
  // if our return type is not void, then pass the address of retval
  if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
    {
      *cs << ", &retval";
    }

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
	  if (!d->imported ()) 
	    {
              // only if this is an argument node
              if (d->node_type () == AST_Decl::NT_argument)
                {
                  *cs << ", &" << d->local_name ();
                                    // previous entry
                } // end if argument node
            } // end if ! imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    } // end of arg list
  *cs << ");" << nl;


  if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
    {
      *cs << "return retval;\n";
    }
  else
    {
      // return type is void. So emit code that simply returns
      *cs << "return;\n";
    }

  cs->decr_indent ();
  *cs << "}\n\n";
  cg->pop ();
  return 0;
}

int
be_operation::gen_server_header (void)
{
  TAO_OutStream *sh; // output stream
  TAO_NL  nl;        // end line
  be_type *bt;       // type node
  UTL_ScopeActiveIterator *si; // scope iterator
  AST_Decl *d;       // temp node
  be_argument *bd;   // argument node
  be_state *s;       // state based code generator object

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_OPERATION_SH); // we are now generating an operation
                                         // definition 

  sh = cg->server_header ();
  cg->outstream (sh); // set current stream
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
  if (be_scope::gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_server_header\n")); 
      ACE_ERROR ((LM_ERROR, "Argument generation failure\n"));
      return -1;
    }

  // XXXASG
  // additional argument in the form of CORBA::Environment since TAO does not
  // yet support Exceptions. However, we can have an option to the "tao" IDL
  // compiler to ask it to generate code for exceptions. This will be handled
  // in a later release.
  *sh << " CORBA::Environment &env) = 0; // pure virtual\n";

  sh->indent ();

  // generate the static method corresponding to this method
  *sh << "static void " << this->local_name () << "_skel (CORBA::ServerRequest &req,"
      << " CORBA::Object_ptr obj, CORBA::Environment &env);\n\n";
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
  be_argument *bd;   // argument node
  be_state *s;       // state based code gen object
  be_predefined_type *bpd=0; // predefined return type 
  be_interface *intf; // enclosing interface node

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_OPERATION_SS); // we are now generating an operation
                                         // definition 

  ss = cg->server_skeletons ();
  cg->outstream (ss); // set current stream

  // get a state based code gen object
  s = cg->make_state ();

  ss->indent (); // start with the current indentation level

  // retrieve our enclosing interface decl
  intf = be_interface::narrow_from_decl (ScopeAsDecl (this->defined_in ()));

  *ss << "void " << intf->full_skel_name () << "::" << this->local_name () << 
    "_skel (CORBA::ServerRequest &req, CORBA::Object_ptr obj, " << 
    "CORBA::Environment &env)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  // define an NVList to hold arguments
  *ss << "CORBA::NVList_ptr \t nvlist;" << nl;
  // define a variable that will eventually point to our implementation object
  *ss << intf->name () << "_ptr \t impl;" << nl;

  // verify if we need to define a variable intended to hold the operation
  // return type. We do not need one if the return type is void

  rt = be_type::narrow_from_decl (this->return_type ());
  if (!rt)
    {
      ACE_ERROR ((LM_ERROR, 
                  "be_operation::gen_server_skeletons - bad return type\n"));
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
      if (s->gen_code (rt, this) == -1)
        {
          ACE_ERROR ((LM_ERROR, 
           "be_operation::gen_server_skeletons - codegen failed for return type\n"));
          return -1;
        }
      *ss << " retval;" << nl;
    }

  // if we have any arguments, get each one of them and allocate an Any and
  // NamedValue for each. In addition, define a variable.
  cg->push (TAO_CodeGen::TAO_ARGUMENT_VARDECL_SS);
  s = cg->make_state ();
  if (!s)
    {
      return -1;
    }
  if (this->nmembers () > 0)
    {
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
                  bt = be_type::narrow_from_decl (bd->field_type ());
                  
                  // first define a variable (its type followed by the name)
                  if (s->gen_code (bt, bd) == -1)
                    {
                      return -1;
                    }
#if 0
                  *ss << bd->local_name () << ";" << nl;
                  // now define a NamedValue_ptr
                  *ss << "CORBA::NamedValue_ptr nv_" << bd->local_name () <<
                    ";" << nl;
                  *ss << "CORBA::Any \t any_" << bd->local_name () << " (" <<
                    bt->tc_name () << ");" << nl;
#endif
                } // end if argument node
            } // end if ! imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    } // end of arg list

  *ss << "\n";
  cg->pop ();

  // declare an NVList and create one
  ss->indent ();
  *ss << "// create an NV list and populate it with typecodes" << nl;
  *ss << "req.orb ()->create_list (0, nvlist); // initialize a list" << nl;

  // add each argument according to the in, out, inout semantics
  *ss << "// add each argument according to the in, out, inout semantics" << nl;
  if (this->nmembers () > 0)
    {
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
                  bt = be_type::narrow_from_decl (bd->field_type ());
                  // emit code that adds this argument to the    
                  *ss << "nv_" << bd->local_name () << 
                    " = nvlist->add_value (\"" << bd->local_name () << "\", "
                      << "any_" << bd->local_name () << ", ";
                  switch (bd->direction ())
                    {
                    case AST_Argument::dir_IN:
                      *ss << "CORBA::ARG_IN, env);" << nl;
                      break;
                    case AST_Argument::dir_INOUT:
                      *ss << "CORBA::ARG_INOUT, env);" << nl;
                      break;
                    case AST_Argument::dir_OUT:
                      *ss << "CORBA::ARG_OUT, env);" << nl;
                      break;
                    }
                } // end if argument node
            } // end if ! imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    } // end of arg list

  // parse the arguments
  *ss << "// parse the arguments" << nl;
  *ss << "req.params (nvlist, env);" << nl;
  *ss << "if (env.exception ()) return;" << nl;

  // make the upcall
  *ss << "impl = (" << intf->name () << "_ptr) obj->get_subclass ();" << nl;
  if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
    {
      *ss << "retval = impl->" << this->local_name () << "(";
    }
  else
    {
      // void return type
      *ss << "impl->" << this->local_name () << "(";
    }

  // emit code for passing arguments to the upcall
  if (this->nmembers () > 0)
    {
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
                  *ss << bd->local_name () << ", ";
                } // end if argument node
            } // end if ! imported
          si->next ();
        } // end of while
      delete si; // free the iterator object
    } // end of arg list
  *ss << "env);" << nl;

  // if there is any return type, send it via the ServerRequest
  if (!bpd || (bpd->pt () != AST_PredefinedType::PT_void))
    {
      // not a void type
      cg->push (TAO_CodeGen::TAO_OPERATION_RESULT_SS);
      s = cg->make_state ();
      if (!s || (s->gen_code (rt, this) == -1))
        return -1;
      cg->pop ();
    }
  *ss << "\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  cg->pop ();
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

// Narrowing
IMPL_NARROW_METHODS3 (be_operation, AST_Operation, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_operation)
IMPL_NARROW_FROM_SCOPE (be_operation)

