//
// $Id$
//

#include "idl.h"
#include "be.h"
#include "be_visitor_args.h"
#include "be_visitor_interface.h"

be_visitor_interface_ch::be_visitor_interface_ch (void)
{
}

be_visitor_interface_ch::~be_visitor_interface_ch (void)
{
}

int be_visitor_interface_ch::visit_interface (be_interface *node)
{
  long i;            // loop index
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cg->push (TAO_CodeGen::TAO_INTERFACE_CH); // set the current code
  // generation  state

  TAO_OutStream *ch = cg->client_header ();

  // == STEP 1:  generate the class name and class names we inherit ==
  ch->indent (); // start with whatever indentation level we are at

  // generate the ifdefined macro for  the _ptr type
  ch->gen_ifdef_macro (node->flatname (), "_ptr");

  // the following two are required to be under the ifdef macro to avoid
  // multiple declarations

  // forward declaration
  *ch << "class " << node->local_name () << ";" << nl;
  // generate the _ptr declaration
  *ch << "typedef " << node->local_name () << " *" << node->local_name ()
      << "_ptr;" << nl;

  ch->gen_endif ();

  // generate the ifdefined macro for the var type
  ch->gen_ifdef_macro (node->flatname (), "_var");

  // generate the _var declaration
  if (node->gen_var_defn () == -1)
    {
      ACE_ERROR ((LM_ERROR,
		  "be_interface - error generating _var definition\n"));
      return -1;
    }
  ch->gen_endif ();

  // generate the ifdef macro for the _out class
  ch->gen_ifdef_macro (node->flatname (), "_out");

  // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
  if (node->gen_out_defn () == -1)
    {
      ACE_ERROR ((LM_ERROR,
		  "be_interface - error generating _var definition\n"));
      return -1;
    }
  // generate the endif macro
  ch->gen_endif ();

  ch->gen_ifdef_macro (node->flatname ());

  // now generate the class definition
  *ch << "class " << node->local_name ();

  if (node->n_inherits () > 0)  // this interface inherits from other interfaces
    {
      *ch << ": ";
      for (i = 0; i < node->n_inherits (); i++)
	{
	  *ch << "public virtual ";
	  *ch << node->inherits ()[i]->name ();  // dump the scoped name
	  if (i < node->n_inherits () - 1) // this is the case of multiple
	    // inheritance
	    {
	      *ch << ", ";
	    }
	}  // end of for loop
      *ch << nl;
    }
  else
    {
      // we do not inherit from anybody, hence we do so from the base
      // CORBA::Object class
      *ch << ": public virtual ACE_CORBA_1 (Object)" << nl;
    }

  // generate the body

  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();
  // generate the static _duplicate, _narrow, and _nil operations
  *ch << "// the static operations" << nl;
  *ch << "static " << node->local_name () << "_ptr " << "_duplicate ("
      << node->local_name () << "_ptr obj);" << nl;
  *ch << "static " << node->local_name () << "_ptr " << "_narrow ("
      << "CORBA::Object_ptr obj, CORBA::Environment &env);" << nl;
  *ch << "static " << node->local_name () << "_ptr "
      << "_nil (void);" << nl;

  // generate a TAO-specific _bind method similar to what Orbix and VisiBroker
  // have
  *ch << "static " << node->local_name () << "_ptr _bind (const char *host, "
      << "CORBA::UShort port, const char *key, CORBA::Environment &env);\n\n";

  // generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (node->be_scope::gen_client_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_client_header\n"));
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // the _is_a method
  ch->indent ();
  *ch << "virtual CORBA::Boolean _is_a (const CORBA::Char *type_id, "
      << "CORBA::Environment &env);\n";

  // generate the "protected" constructor so that users cannot instantiate
  // us
  ch->decr_indent ();
  *ch << "protected:\n";
  ch->incr_indent ();
  *ch << node->local_name () << " (void); // default constructor" << nl;
  *ch << node->local_name () << " (STUB_Object *objref);" << nl;
  *ch << "virtual ~" << node->local_name () << " (void);\n";
  ch->decr_indent ();

  // private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << node->local_name () << " (const " << node->local_name () << "&);" << nl;
  *ch << "void operator= (const " << node->local_name () << "&);\n";
  ch->decr_indent ();
  *ch << "};\n\n";
  ch->gen_endif ();


  // generate the typecode decl. If we are in the outermost scope, our typecode
  // decl is extern
  if (node->is_nested ())
    {
      // we have a scoped name
      ch->indent ();
      *ch << "static CORBA::TypeCode_ptr " << node->tc_name
	()->last_component () << ";\n\n";
    }
  else
    {
      // we are in the ROOT scope
      ch->indent ();
      *ch << "extern CORBA::TypeCode_ptr " << node->tc_name
	()->last_component () << ";\n\n";
    }

  cg->pop ();

  return 0;
}



be_visitor_interface_ci::be_visitor_interface_ci (void)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int be_visitor_interface_ci::visit_interface (be_interface *node)
{
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  TAO_OutStream *ci = cg->client_inline ();
  ci->indent (); // start from the current indentation level

  // generate the constructors and destructor
  *ci << "ACE_INLINE" << nl;
  *ci << node->name () << "::" << node->local_name () <<
    " (void) // default constructor" << nl;
  *ci << "{}" << nl << nl;

  *ci << "ACE_INLINE" << nl;
  *ci << node->name () << "::" << node->local_name () <<
    " (STUB_Object *objref) // constructor" << nl;
  *ci << "\t: ACE_CORBA_1 (Object) (objref)" << nl;
  *ci << "{}" << nl << nl;

  *ci << "ACE_INLINE" << nl;
  *ci << node->name () << "::~" << node->local_name () <<
    " (void) // destructor" << nl;
  *ci << "{}\n\n";

  // generate the ifdefined macro for  the _var type
  ci->gen_ifdef_macro (node->flatname (), "_var");

  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: _var impl code gen failed\n"));
      return -1;
    }
  ci->gen_endif ();

  // generate the ifdefined macro for  the _out type
  ci->gen_ifdef_macro (node->flatname (), "_out");

  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: _out impl code gen failed\n"));
      return -1;
    }
  ci->gen_endif ();

  if (node->be_scope::gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: code gen failed for scope\n"));
      return -1;
    }

  return 0;
}



be_visitor_interface_cs::be_visitor_interface_cs (void)
{
}

be_visitor_interface_cs::~be_visitor_interface_cs (void)
{
}

int be_visitor_interface_cs::visit_interface (be_interface *node)
{
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_CS); // set the current code generation
                                            // state

  TAO_OutStream *cs = cg->client_stubs ();

  cs->indent (); // start with whatever indentation level we are at

  // first generate the code for the static methods
  // The _duplicate method
  *cs << node->name () << "_ptr " << node->name () << "::_duplicate ("
      << node->name () << "_ptr obj)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "if (!CORBA::is_nil (obj))\n";
  cs->incr_indent ();
  *cs << "obj->AddRef ();\n";
  cs->decr_indent ();
  *cs << nl;
  *cs << "return obj;\n";
  cs->decr_indent ();
  *cs << "} // end of _duplicate" << nl << nl;

  // The _narrow method
  *cs << node->name () << "_ptr " << node->name ()
      << "::_narrow (CORBA::Object_ptr obj, CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "if (CORBA::is_nil (obj)) return " << node->name () << "::_nil ();" <<
    nl;
  *cs << "if (obj->_is_a (\"" << node->repoID () << "\", env))" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "STUB_Object *istub;" << nl;
  *cs << node->name () << "_ptr new_obj; // to be returned " << nl;
#if 0 // XXXASG - I was told that emitting this line of code is the root cause
      // of all evil
  *cs << "obj->Release ();" <<
    " // need this since _is_a grabbed an obj reference " << nl;
#endif
  *cs << "if (obj->QueryInterface (IID_STUB_Object, (void **)&istub) " <<
    "!= TAO_NOERROR)\n";
  cs->incr_indent ();
  *cs << "return " << node->name () << "::_nil ();\n";
  cs->decr_indent ();
  *cs << nl;
  *cs << "obj->Release (); " <<
    "// need this since QueryIntf bumped our refcount" << nl;
  *cs << "new_obj = new " << node->name () << " (istub); " <<
    "// construct obj ref using the stub object" << nl;
  *cs << "return new_obj;\n";
  cs->decr_indent ();
  *cs << "} // end of if" << nl;
  *cs << "return " << node->name () << "::_nil (); // _narrow failed\n";
  cs->decr_indent ();
  *cs << "} // end of _narrow" << nl << nl;

  // _nil method
  *cs << node->name () << "_ptr " << node->name () << "::_nil (void)" <<
    nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return (" << node->name () << "_ptr)NULL;\n";
  cs->decr_indent ();
  *cs << "} // end of _nil" << nl << nl;

  // the _bind method
  *cs << node->name () << "_ptr " << node->name () << "::_bind (" <<
    "const char *host, CORBA::UShort port, const char *key, " <<
    "CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::Object_ptr objref = CORBA::Object::_nil ();" << nl;
  *cs << "IIOP_Object *data = new IIOP_Object (host, port, key);" << nl;
  *cs << "if (!data) return " << node->name () << "::_nil ();" << nl;
  *cs << "// get the object_ptr using Query Interface" << nl;
  *cs <<
    "if (data->QueryInterface (IID_CORBA_Object, (void **)&objref) != TAO_NOERROR)"
      << nl;
  *cs << "{" << nl;
  *cs << "\tenv.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));"
      << nl;
  *cs << "\treturn " << node->name () << "::_nil ();" << nl;
  *cs << "}" << nl;
  *cs << "data->Release (); // QueryInterface had bumped up our count" << nl;
  *cs << "if (CORBA::is_nil (objref))" << nl;
  *cs << "\treturn " << node->name () << "::_nil ();" << nl;
  *cs << "else // narrow it" << nl;
  *cs << "\treturn " << node->name () << "::_narrow (objref, env);\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // generate code for the elements of the interface
  if (node->be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_client_stubs\n"));
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // generate the is_a method
  cs->indent ();
  *cs << "CORBA::Boolean " << node->name () << "::_is_a (" <<
    "const CORBA::Char *value, CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "if (\n";
  cs->incr_indent (0);
  if (node->traverse_inheritance_graph (be_interface::is_a_helper, cs) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_client_stubs - "
                         "inheritance graph failed\n"), -1);
    }
  cs->indent ();
  *cs << "(!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (env))))\n";
  *cs << "\treturn 1; // success using local knowledge\n";
  cs->decr_indent ();
  *cs << "else" << nl;
  *cs << "\treturn ACE_CORBA_3 (Object, _is_a) (value, env); // remote call\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // generate the typecode information here
  cs->indent (); // start from current indentation level
  *cs << "static const CORBA::Long _oc_" << node->flatname () << "[] =" <<
    nl;
  *cs << "{\n";
  cs->incr_indent (0);
  if (node->gen_encapsulation () == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error generating typecode\n\n"));
      return -1;
    }
  cs->decr_indent ();
  *cs << "};" << nl;

  *cs << "static CORBA::TypeCode _tc__tc_" << node->flatname () <<
    " (CORBA::tk_objref, sizeof (_oc_" <<  node->flatname () <<
    "), (unsigned char *) &_oc_" << node->flatname () <<
    ", CORBA::B_FALSE);" << nl;
  *cs << "CORBA::TypeCode_ptr " << node->tc_name () << " = &_tc__tc_" <<
    node->flatname () << ";\n\n";

  cg->pop ();
  return 0;
}



be_visitor_collocated_sh::be_visitor_collocated_sh (void)
{
}

be_visitor_collocated_sh::~be_visitor_collocated_sh (void)
{
}

int be_visitor_collocated_sh::visit_interface (be_interface *node)
{
  TAO_NL  nl;        // end line

  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *os = cg->server_header ();

  os->gen_ifdef_macro (node->flatname (), "_collocated");

  os->indent ();
  *os << "class _tao_collocated_" << node->local_name ()
      << " : public virtual " << node->name () << nl;
  *os << "{" << nl;
  *os << "public:\n";
  os->incr_indent (0);

  if (node->nmembers () > 0)
    {
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      -1);
      while (!si->is_done ())
	{
	  AST_Decl *d = si->item ();
	  si->next ();
	  be_decl *bd = be_decl::narrow_from_decl (d);
	  if (d->imported ()
	      || bd == 0
	      || bd->node_type () != AST_Decl::NT_op)
	    {
	      continue;
	    }
	  if (bd->accept (this) == -1)
	    {
	      delete si;
	      return -1;
	    }
	}
      delete si;
    }

  os->decr_indent ();
  *os << "};\n\n";

  return 0;
}

int be_visitor_collocated_sh::visit_operation (be_operation *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_OPERATION_SH);

  TAO_OutStream *sh = cg->server_header ();
  sh->indent (); // start with the current indentation level

  // every operation is declared virtual
  *sh << "virtual ";

  // first generate the return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  be_state *s = cg->make_state (); // retrieve code gen object for this state
  if (!s || !bt || (s->gen_code (bt, node) == -1))
    {
      ACE_ERROR ((LM_ERROR, "be_operation::gen_server_header\n"));
      ACE_ERROR ((LM_ERROR, "return type generation failure\n"));
      return -1;
    }
  // generate the operation name
  *sh << " " << node->local_name () << " ";

  be_visitor_args_decl visitor (sh);
  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) args decl failed\n"), -1);
    }

  return 0;
}



be_visitor_collocated_ss::be_visitor_collocated_ss (void)
  :  current_interface_ (0)
{
}

be_visitor_collocated_ss::~be_visitor_collocated_ss (void)
{
}

int be_visitor_collocated_ss::visit_interface (be_interface *node)
{
  if (node->nmembers () > 0)
    {
      current_interface_ = node;
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      -1);
      while (!si->is_done ())
	{
	  AST_Decl *d = si->item ();
	  si->next ();
	  be_decl *bd = be_decl::narrow_from_decl (d);
	  // Only printout the operations, nested interfaces and
	  // structures only go in the main declaration.
	  if (d->imported () || bd == 0
	      || bd->node_type () != AST_Decl::NT_op)
	    {
	      continue;
	    }
	  if (bd->accept (this) == -1)
	    {
	      delete si;
	      return -1;
	    }
	}
      delete si;
    }

  return 0;
}

int be_visitor_collocated_ss::visit_operation (be_operation *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *ss = cg->server_skeletons ();
  TAO_NL  nl;

  // retrieve the return type again because we have used bt to also retrieve
  // the argument types
  be_type *bt = be_type::narrow_from_decl (node->return_type ());

  *ss << bt->name () << " "
      << current_interface_->full_coll_name () << "::"
      << node->local_name () << " ";

  be_visitor_args_decl visitor (ss);
  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) args decl failed\n"), -1);
    }

  // last argument - is always CORBA::Environment
  ss->indent ();
  *ss << "{\n";
  ss->incr_indent ();
  
  if (bt->node_type () != AST_Decl::NT_pre_defined
      || be_predefined_type::narrow_from_decl (bt)->pt () != AST_PredefinedType::PT_void)
    {
      *ss << "return ";
    }
  
  *ss << "this->servant_->" << node->local_name () << " (\n";
  ss->incr_indent (0);
  ss->incr_indent (0);

  this->visit_scope (node);

  ss->indent ();
  *ss << " _tao_environment\n";
  ss->decr_indent ();
  *ss << ");\n";
  ss->decr_indent (0);
  ss->decr_indent (0);
  *ss << "}\n\n";
  
  return 0;
}

int be_visitor_collocated_ss::visit_argument (be_argument *node)
{
  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  TAO_OutStream *ss = cg->server_skeletons ();

  ss->indent ();
  *ss << node->local_name () << ",\n";
  return 0;
}
