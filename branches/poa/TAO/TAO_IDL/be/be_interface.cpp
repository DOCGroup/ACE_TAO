// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface.cpp
//
// = DESCRIPTION
//    Extension of class AST_Interface that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include "be_visitor_interface.h"
#include	"be.h"

/*
 * BE_Interface
 */

// default constructor
be_interface::be_interface (void)
  : full_skel_name_ (0),
    skel_count_ (0),
    full_coll_name_ (0),
    local_coll_name_ (0)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// constructor used to build the AST
be_interface::be_interface (UTL_ScopedName *n, AST_Interface **ih, long nih,
                            UTL_StrList *p)
  : AST_Interface (n, ih, nih, p),
    AST_Decl (AST_Decl::NT_interface, n, p),
    UTL_Scope (AST_Decl::NT_interface),
    full_skel_name_ (0),
    skel_count_ (0),
    full_coll_name_ (0),
    local_coll_name_ (0)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_interface::~be_interface (void)
{
  if (this->full_skel_name_ != 0)
    {
      delete[] this->full_skel_name_;
      this->full_skel_name_ = 0;
    }
  if (this->full_coll_name_ != 0)
    {
      delete[] this->full_coll_name_;
      this->full_coll_name_ = 0;
    }
  if (this->local_coll_name_ != 0)
    {
      delete[] this->local_coll_name_;
      this->local_coll_name_ = 0;
    }
}

// compute stringified fully qualified collocated class name.
void
be_interface::compute_coll_name (void)
{
  if (this->full_coll_name_ != 0)
    return;

  const char collocated[] = "_tao_collocated_";
  const char poa[] = "POA_";
  // Reserve enough room for the "POA_" prefix, the "_tao_collocated_"
  // prefix and the local name and the (optional) "::"
  int namelen = sizeof (collocated) + sizeof (poa);

  UTL_IdListActiveIterator *i;
  ACE_NEW (i, UTL_IdListActiveIterator (this->name ()));
  while (!i->is_done ())
    {
      // reserve 2 characters for "::".
      namelen += ACE_OS::strlen (i->item ()->get_string ()) + 2;
      i->next ();
    }
  delete i;

  ACE_NEW (this->full_coll_name_, char[namelen+1]);
  this->full_coll_name_[0] = 0; // null terminate the string...

  // Iterate again....
  ACE_NEW (i, UTL_IdListActiveIterator (this->name ()));

  // Only the first component get the "POA_" preffix.
  int poa_added = 0;
  while (!i->is_done ())
    {
      const char* item = i->item ()->get_string ();

      // Increase right away, so we can test for the final component
      // in the loop.
      i->next ();

      // We add the POA_ preffix only if the first component is not
      // the global scope...
      if (ACE_OS::strcmp (item, "") != 0)
	{
	  if (!i->is_done ())
	    {
	      // We only add the POA_ preffix if there are more than
	      // two components in the name, in other words, if the
	      // class is inside some scope.
	      if (!poa_added)
		{
		  ACE_OS::strcat (this->full_coll_name_, poa);
		  poa_added = 1;
		}
	      ACE_OS::strcat (this->full_coll_name_, item);
	      ACE_OS::strcat (this->full_coll_name_, "::");
	    }
	  else
	    {
	      ACE_OS::strcat (this->full_coll_name_, collocated);
	      ACE_OS::strcat (this->full_coll_name_, item);
	    }
	}
    }
  delete i;

  // Compute the local name for the collocated class.
  int localen = sizeof (collocated);
  localen += ACE_OS::strlen (this->local_name ()->get_string ());
  ACE_NEW (this->local_coll_name_, char[localen]);
  ACE_OS::strcpy(this->local_coll_name_, collocated);
  ACE_OS::strcat(this->local_coll_name_,
		 this->local_name ()->get_string ());
}

const char*
be_interface::full_coll_name (void) const
{
  if (this->full_coll_name_ == 0)
    ACE_const_cast(be_interface*, this)->compute_coll_name ();

  return this->full_coll_name_;
}

const char*
be_interface::local_coll_name (void) const
{
  if (this->local_coll_name_ == 0)
    ACE_const_cast(be_interface*, this)->compute_coll_name ();

  return this->local_coll_name_;
}

// compute stringified fully scoped skel name
void
be_interface::compute_fullskelname (void)
{
  if (full_skel_name_)
    return;
  else
    {
      long namelen;
      UTL_IdListActiveIterator	*i;
      long				first = I_TRUE;
      long				second = I_FALSE;

      // in the first loop compute the total length
      namelen = 4;
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ()))
        {
          if (!first)
            namelen += 2; // for "::"
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          namelen += ACE_OS::strlen (i->item ()->get_string ()); //
          // additional 4 for the POA_ characters
          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                // does not start with a ""
                first = I_FALSE;
              else
                second = I_TRUE;
            }
          i->next ();
        }
      delete i;

      this->full_skel_name_ = new char [namelen+1];
      this->full_skel_name_[0] = '\0';
      first = I_TRUE;
      second = I_FALSE;
      ACE_OS::strcat (this->full_skel_name_, "POA_");
      i = new UTL_IdListActiveIterator (this->name ());
      while (!(i->is_done ()))
        {
          if (!first)
            ACE_OS::strcat (this->full_skel_name_, "::");
          else if (second)
            first = second = I_FALSE;
          // print the identifier
          ACE_OS::strcat (this->full_skel_name_, i->item ()->get_string ());
          if (first)
            {
              if (ACE_OS::strcmp (i->item ()->get_string (), "") != 0)
                // does not start with a ""
                first = I_FALSE;
              else
                second = I_TRUE;
            }
          i->next ();
        }
      delete i;
    }
  return;
}

// retrieve the fully scoped skeleton name
const char*
be_interface::full_skel_name (void)
{
  if (!this->full_skel_name_)
    compute_fullskelname ();

  return this->full_skel_name_;
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// generate the client header
int
be_interface::gen_client_header (void)
{
  if (!this->cli_hdr_gen_) // not already generated
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
      ch->gen_ifdef_macro (this->flatname (), "_ptr");

      // the following two are required to be under the ifdef macro to avoid
      // multiple declarations

      // forward declaration
      *ch << "class " << this->local_name () << ";" << nl;
      // generate the _ptr declaration
      *ch << "typedef " << this->local_name () << " *" << this->local_name ()
	  << "_ptr;" << nl;

      ch->gen_endif ();

      // generate the ifdefined macro for the var type
      ch->gen_ifdef_macro (this->flatname (), "_var");

      // generate the _var declaration
      if (this->gen_var_defn () == -1)
	{
	  ACE_ERROR ((LM_ERROR,
		      "be_interface - error generating _var definition\n"));
	  return -1;
	}
      ch->gen_endif ();

      // generate the ifdef macro for the _out class
      ch->gen_ifdef_macro (this->flatname (), "_out");

      // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
      if (this->gen_out_defn () == -1)
	{
	  ACE_ERROR ((LM_ERROR,
		      "be_interface - error generating _var definition\n"));
	  return -1;
	}
      // generate the endif macro
      ch->gen_endif ();

      ch->gen_ifdef_macro (this->flatname ());

      // now generate the class definition
      ch->indent ();
      *ch << "class " << this->local_name ();

      if (n_inherits () > 0)  // this interface inherits from other interfaces
        {
          *ch << ": ";
          for (i = 0; i < n_inherits (); i++)
            {
              be_interface *inherited = be_interface::narrow_from_decl
                (this->inherits ()[i]);
              be_decl *scope = 0;
              if (inherited->is_nested ())
                {
                  // inherited node is used in the scope of "this" node
                  scope = be_scope::narrow_from_scope (this->defined_in
                                                       ())->decl ();
                }

              *ch << "public virtual ";
              *ch << inherited->nested_type_name (scope);  // dump the scoped name
              if (i < n_inherits () - 1) // this is the case of multiple
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
          *ch << ": public virtual CORBA::Object" << nl;
        }

      // generate the body

      *ch << "{" << nl;
      *ch << "public:\n";
      ch->incr_indent ();
      // generate the static _duplicate, _narrow, and _nil operations
      *ch << "// the static operations" << nl;
      *ch << "static " << this->local_name () << "_ptr " << "_duplicate ("
	  << this->local_name () << "_ptr obj);" << nl;
      *ch << "static " << this->local_name () << "_ptr " << "_narrow ("
	  << "CORBA::Object_ptr obj, CORBA::Environment &env);" << nl;
      *ch << "static " << this->local_name () << "_ptr "
	  << "_nil (void);" << nl;

      // generate a TAO-specific _bind method similar to what Orbix and VisiBroker
      // have
      *ch << "static " << this->local_name () << "_ptr _bind (const char *host, "
	  << "CORBA::UShort port, const char *key, CORBA::Environment &env);\n\n";

      // generate code for the interface definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.
      if (this->be_scope::gen_client_header () == -1)
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
      *ch << this->local_name () << " (" << be_idt << be_idt_nl
	  << "STUB_Object *objref = 0," << be_nl
	  << "TAO_ServantBase *servant = 0," << be_nl
	  << "CORBA::Boolean collocated = CORBA::B_FALSE" << be_uidt_nl
	  << ");\n" << be_uidt;
      ch->decr_indent ();

      // dtor is public...
      *ch << "public:" << be_idt_nl
	  << "virtual ~" << this->local_name () << " (void);" << be_uidt_nl;

      // private copy constructor and assignment operator. These are not
      // allowed, hence they are private.
      *ch << "private:\n";
      ch->incr_indent ();
      *ch << this->local_name () << " (const " << this->local_name () << "&);" << nl;
      *ch << "void operator= (const " << this->local_name () << "&);\n";
      ch->decr_indent ();
      *ch << "};\n\n";
      ch->gen_endif ();


      // generate the typecode decl. If we are in the outermost scope, our typecode
      // decl is extern
      if (this->is_nested ())
	{
	  // we have a scoped name
	  ch->indent ();
	  *ch << "static CORBA::TypeCode_ptr " << this->tc_name
	    ()->last_component () << ";\n\n";
	}
      else
	{
	  // we are in the ROOT scope
	  ch->indent ();
	  *ch << "extern CORBA::TypeCode_ptr " << this->tc_name
	    ()->last_component () << ";\n\n";
	}

      cg->pop ();
      this->cli_hdr_gen_ = I_TRUE;
    }
  return 0;
}

// Generates the client-side inline functions
int
be_interface::gen_client_inline (void)
{
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  TAO_OutStream *ci = cg->client_inline ();
  ci->indent (); // start from the current indentation level

  *ci << "ACE_INLINE" << nl;
  *ci << this->name () << "::"
      << this->local_name () << "(" << be_idt << be_idt_nl
      << "STUB_Object *objref," << be_nl
      << "TAO_ServantBase *servant," << be_nl
      << "CORBA::Boolean collocated" << be_uidt_nl
      << ")" << be_uidt_nl
      << " : ACE_CORBA_1 (Object) (objref, servant, collocated)" << be_nl
      << "{}" << be_nl << be_nl;

  *ci << "ACE_INLINE" << nl;
  *ci << this->name () << "::~" << this->local_name () <<
    " (void) // destructor" << nl;
  *ci << "{}\n\n";

  // _nil method
  *ci << "ACE_INLINE " << this->name () << "_ptr" << be_nl
      << this->name () << "::_nil (void)" << be_nl
      << "{" << be_idt_nl
      << "return (" << this->name () << "_ptr)0;" << be_uidt_nl
      << "}\n" << be_nl;

  // generate the ifdefined macro for  the _var type
  ci->gen_ifdef_macro (this->flatname (), "_var");

  if (this->gen_var_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: _var impl code gen failed\n"));
      return -1;
    }
  ci->gen_endif ();

  // generate the ifdefined macro for  the _out type
  ci->gen_ifdef_macro (this->flatname (), "_out");

  if (this->gen_out_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: _out impl code gen failed\n"));
      return -1;
    }
  ci->gen_endif ();

  if (this->be_scope::gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: code gen failed for scope\n"));
      return -1;
    }

  return 0;
}


// Generate the client-side stubs
int
be_interface::gen_client_stubs (void)
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
  *cs << this->name () << "_ptr " << this->name () << "::_duplicate ("
      << this->name () << "_ptr obj)" << nl;
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
  *cs << this->name () << "_ptr " << this->name ()
      << "::_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object_ptr obj," << be_nl
      << "CORBA::Environment &env" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (CORBA::is_nil (obj))" << be_idt_nl
      << "return " << this->name () << "::_nil ();" << be_uidt_nl
      << "if (!obj->_is_a (\"" << this->repoID () << "\", env))"
      << be_idt_nl
      << "return " << this->name () << "::_nil ();" << be_uidt_nl;

  *cs << "if (!obj->_is_collocated () || !obj->_servant())" << be_nl
      << "{" << be_idt_nl;
  *cs << "STUB_Object *istub;" << nl;
  *cs << this->name () << "_ptr new_obj; // to be returned " << nl;
#if 0 // XXXASG - I was told that emitting this line of code is the root cause
      // of all evil
  *cs << "obj->Release ();" <<
    " // need this since _is_a grabbed an obj reference " << nl;
#endif
  *cs << "if (obj->QueryInterface (IID_STUB_Object, (void **)&istub) " <<
    "!= TAO_NOERROR)\n";
  cs->incr_indent ();
  *cs << "return " << this->name () << "::_nil ();\n";
  cs->decr_indent ();
  *cs << nl;
#if 0 // also the cause of all evil
  *cs << "obj->Release (); " <<
    "// need this since QueryIntf bumped our refcount" << nl;
#endif
  *cs << "new_obj = new " << this->name () << " (istub); "
      << "// construct obj ref using the stub object" << nl;
  *cs << "return new_obj;\n";
  cs->decr_indent ();
  *cs << "} // end of if\n" << nl;

  *cs << "STUB_Object *stub = obj->_servant ()->_create_stub (env);" << be_nl
      << "if (env.exception () != 0)" << be_idt_nl
      << "return " << this->name () << "::_nil ();" << be_uidt_nl
      << "void* servant = obj->_servant ()->_downcast (\""
      << this->repoID () << "\");" << be_nl
      << "return new "
      << this->full_coll_name () << "(" << be_idt << be_idt_nl
      << "ACE_reinterpret_cast(" << this->full_skel_name ()
      << "_ptr, servant)," << be_nl
      << "stub" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // the _bind method
  *cs << this->name () << "_ptr " << this->name () << "::_bind (" <<
    "const char *host, CORBA::UShort port, const char *key, " <<
    "CORBA::Environment &env)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::Object_ptr objref = CORBA::Object::_nil ();" << nl;
  *cs << "IIOP_Object *data = new IIOP_Object (host, port, key);" << nl;
  *cs << "if (!data) return " << this->name () << "::_nil ();" << nl;
  *cs << "// get the object_ptr using Query Interface" << nl;
  *cs <<
    "if (data->QueryInterface (IID_CORBA_Object, (void **)&objref) != TAO_NOERROR)"
      << nl;
  *cs << "{" << nl;
  *cs << "\tenv.exception (new CORBA::DATA_CONVERSION (CORBA::COMPLETED_NO));"
      << nl;
  *cs << "\treturn " << this->name () << "::_nil ();" << nl;
  *cs << "}" << nl;
  *cs << "data->Release (); // QueryInterface had bumped up our count" << nl;
  *cs << "if (CORBA::is_nil (objref))" << nl;
  *cs << "\treturn " << this->name () << "::_nil ();" << nl;
  *cs << "else // narrow it" << nl;
  *cs << "\treturn " << this->name () << "::_narrow (objref, env);\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // generate code for the elements of the interface
  if (this->be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_client_stubs\n"));
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // generate the is_a method
  cs->indent ();
  *cs << "CORBA::Boolean " << this->name () << "::_is_a (" <<
    "const CORBA::Char *value, CORBA::Environment &_tao_environment)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "if (\n";
  cs->incr_indent (0);
  if (this->traverse_inheritance_graph (be_interface::is_a_helper, cs) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_client_stubs - "
                         "inheritance graph failed\n"), -1);
    }
  cs->indent ();
  *cs << "(!ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (_tao_environment))))\n";
  *cs << "\treturn 1; // success using local knowledge\n";
  cs->decr_indent ();
  *cs << "else" << nl;
  *cs << "\treturn this->CORBA_Object::_is_a (value, _tao_environment); // remote call\n";
  cs->decr_indent ();
  *cs << "}\n\n";

  // generate the typecode information here
  cs->indent (); // start from current indentation level
  *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
    nl;
  *cs << "{\n";
  cs->incr_indent (0);
  if (this->gen_encapsulation () == -1)
    {
      ACE_ERROR ((LM_ERROR, "Error generating typecode\n\n"));
      return -1;
    }
  cs->decr_indent ();
  *cs << "};" << nl;

  *cs << "static CORBA::TypeCode _tc__tc_" << this->flatname () <<
    " (CORBA::tk_objref, sizeof (_oc_" <<  this->flatname () <<
    "), (unsigned char *) &_oc_" << this->flatname () <<
    ", CORBA::B_FALSE);" << nl;
  *cs << "CORBA::TypeCode_ptr " << this->tc_name () << " = &_tc__tc_" <<
    this->flatname () << ";\n\n";

  cg->pop ();
  return 0;
}

// generate server header
int be_interface::gen_server_header (void)
{
  TAO_OutStream *sh; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  static char namebuf [NAMEBUFSIZE]; // holds the class name
  AST_Decl *d;       // enclosing scope

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_SH); // set the current code generation
                                          // state

  sh = cg->server_header ();

  // generate the skeleton class name

  sh->indent (); // start with whatever indentation level we are at

  // we shall have a POA_ prefix only if we are at the topmost level
  // generate the forward declaration
  d = ScopeAsDecl (this->defined_in ());
  if (d->node_type () == AST_Decl::NT_root)
    {
      // we are outermost
      ACE_OS::sprintf (namebuf, "POA_%s", this->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "%s", this->local_name ()->get_string ());
    }

  *sh << "class " << namebuf << ";" << nl;

  // generate the _ptr declaration
  *sh << "typedef " << namebuf << " *" << namebuf
      << "_ptr;" << nl;

  // now generate the class definition
  *sh << "class " << namebuf << " : ";
  if (n_inherits () > 0)  // this interface inherits from other interfaces
    {
      be_interface *intf;

      *sh << "public virtual ";
      intf = be_interface::narrow_from_decl (inherits ()[0]);
      *sh << intf->full_skel_name ();
      for (i = 1; i < n_inherits (); i++)
        {
          *sh << ", public virtual ";
          intf = be_interface::narrow_from_decl (inherits ()[i]);
          *sh << intf->relative_skel_name (this->full_skel_name ());
        }  // end of for loop
    }
  else
    {
      // We don't inherit from another user defined object, hence our
      // base class is the ServantBase class.
      *sh << " public virtual PortableServer::ServantBase";
    }
  *sh << nl;
  *sh << "{" << nl;
  *sh << "protected:\n";
  sh->incr_indent ();
  *sh << namebuf << " (void);" << nl;
  sh->decr_indent ();
  *sh << "public:\n";
  sh->incr_indent ();
  *sh << "virtual ~" << namebuf << " (void);\n";

  sh->indent ();
  *sh << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt
      << ");\n" << be_uidt;

  sh->indent ();
  *sh << "virtual void* _downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ");\n" << be_uidt;

  // generate code for elements in the scope (e.g., operations)
  if (be_scope::gen_server_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_server_header\n"));
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // add our _is_a method
  sh->indent ();
  *sh << "static void _is_a_skel (CORBA::ServerRequest &req, " <<
    "void *obj, void *context, CORBA::Environment &_tao_enviroment);\n\n";

  // generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the call
  if (this->traverse_inheritance_graph (be_interface::gen_skel_helper, sh) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_server_header - "
                         "inheritance graph traversal failed\n"), -1);
    }

  // add the dispatch method
  sh->indent ();
  *sh << "virtual void dispatch (CORBA::ServerRequest &req, " <<
    "void *context, CORBA::Environment &env);\n\n";

  // Print out the _this() method.
  sh->indent ();
  *sh << this->name () << " *_this (CORBA::Environment &_tao_environment);\n";

  sh->indent ();
  *sh << "virtual const char* _interface_repository_id"
      << " (void) const;\n";

  sh->decr_indent ();

  *sh << "};\n\n";

  be_visitor_collocated_sh visitor;
  this->accept (&visitor);
  *sh << "\n";

  cg->pop ();
  return 0;
}

int be_interface::gen_server_skeletons (void)
{
  TAO_OutStream *ss; // output stream
  TAO_NL  nl;        // end line
  AST_Decl *d;        // temporary

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_SS); // set the current code generation
                                          // state

  ss = cg->server_skeletons ();

  // generate the skeleton class name

  ss->indent (); // start with whatever indentation level we are at

  if (this->gen_operation_table () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "be_interface::gen_operation_table failure\n"));
      return -1;
    }

  *ss << "// skeleton constructor" << nl;
  // find if we are at the top scope or inside some module
  d = ScopeAsDecl (this->defined_in ());

  //  if (d && d->node_type () == AST_Decl::NT_root)
  if (!this->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *ss << this->full_skel_name () << "::POA_" << this->local_name () <<
        " (void)" << nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *ss << this->full_skel_name () << "::" << this->local_name () <<
        " (void)" << nl;
    }

  *ss << "{\n";
  *ss << "}\n\n";

  // generate code for elements in the scope (e.g., operations)
  if (be_scope::gen_server_skeletons () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_server_skeletons\n"));
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // generate code for the _is_a skeleton
  ss->indent ();
  *ss << "void " << this->full_skel_name ()
      << "::_is_a_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &req, " << be_nl
      << "void * _tao_object_reference," << be_nl
      << "void * /*context*/," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl;

  *ss << "{\n";
  ss->incr_indent ();
  *ss << "CORBA::NVList_ptr nvlist;" << nl;
  *ss << "CORBA::NamedValue_ptr nv;" << nl;
  *ss << "CORBA::Any temp_value (CORBA::_tc_string);" << nl;
  *ss << "CORBA::Any *any;" << nl;
  *ss << "CORBA::Boolean *retval = new CORBA::Boolean;" << nl;
  *ss << "CORBA::String value;" << nl;
  *ss << nl;
  *ss << "req.orb()->create_list (0, nvlist);" << nl;
  *ss << "nv = nvlist->add_value (0, temp_value, "
      << "CORBA::ARG_IN, _tao_environment);" << nl;
  *ss << "req.params (nvlist, _tao_environment); // parse the args" << nl;
  *ss << "if (_tao_environment.exception () != 0) return;" << nl;
  *ss << "value = *(CORBA::String *)nv->value ()->value ();" << nl;

  *ss << this->full_skel_name () << "_ptr \t impl = ("
      << this->full_skel_name () << "_ptr) _tao_object_reference;"
      << nl;

  *ss << "*retval = impl->_is_a (value, _tao_environment);" << be_nl
      << "if (_tao_environment.exception () != 0) return;" << be_nl;
  *ss << "any = new CORBA::Any (CORBA::_tc_boolean, "
      << "retval, CORBA::B_TRUE);" << nl;
  *ss << "req.result (any, _tao_environment);\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  ss->indent ();
  *ss << "CORBA::Boolean " << this->full_skel_name ()
      << "::_is_a (" << be_idt << be_idt_nl
      << "const char* value," << be_nl
      << "CORBA::Environment &_tao_environment" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "if (\n" << be_idt;
  if (this->traverse_inheritance_graph (be_interface::is_a_helper, ss) == -1)
    {
      return -1;
    }

  ss->indent ();
  *ss << "(!ACE_OS::strcmp ((char *)value, "
      << "CORBA::_tc_Object->id (_tao_environment))))"
      << be_idt_nl << "return CORBA::B_TRUE;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return CORBA::B_FALSE;" << be_uidt << be_uidt << be_uidt_nl
      << "}\n\n";

  ss->indent ();
  *ss << "void* " << this->full_skel_name ()
      << "::_downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (this->traverse_inheritance_graph (be_interface::downcast_helper, ss) == -1)
    {
      return -1;
    }
  
  *ss << "if (ACE_OS::strcmp (logical_type_id, "
      << "\"IDL:omg.org/CORBA/Object:1.0\") == 0)" << be_idt_nl
      << "return ACE_static_cast(PortableServer::Servant, this);"
      << be_uidt_nl;

  *ss << "return 0;" << be_uidt_nl
      << "}\n\n";


  // now the dispatch method
  ss->indent ();
  *ss << "void " << this->full_skel_name () <<
    "::dispatch (CORBA::ServerRequest &req, " <<
    "void *context, CORBA::Environment &env)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  *ss << "TAO_Skeleton skel; // pointer to skeleton for operation" << nl;
  *ss << "CORBA::String opname = req.op_name (); // retrieve operation name" <<
                                                 nl;
  *ss << "// find the skeleton corresponding to this opname" << nl;
  *ss << "if (this->find (opname, skel) == -1)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  *ss << "env.exception (new CORBA_BAD_OPERATION (CORBA::COMPLETED_NO));" <<
    nl;
  *ss << "ACE_ERROR ((LM_ERROR, \"Bad operation <%s>\\n\", opname));\n";
  ss->decr_indent ();
  *ss << "}\n";
  *ss << "else" << nl;
  *ss << "\tskel (req, this, context, env);\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  ss->indent ();
  *ss << "const char* " << this->full_skel_name ()
      << "::_interface_repository_id (void) const"
      << nl;
  *ss << "{\n";
  ss->incr_indent ();
  *ss << "return \"" << this->repoID () << "\";\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  cg->pop ();

  be_visitor_collocated_ss visitor;
  this->accept (&visitor);
  *ss << "\n";

  *ss << this->name () << "*" << be_nl
      << this->full_skel_name ()
      << "::_this (CORBA_Environment &_env)" << be_nl
      << "{" << be_idt_nl
      << "STUB_Object *stub = this->_create_stub (_env);" << be_nl
      << "if (_env.exception () != 0)" << be_idt_nl
      << "return 0;" << be_uidt_nl

#if 0
      << "TAO_ORB_Core *orb_core = TAO_ORB_Core_instance ();" << be_nl
      << "if (orb_core->get_current ()->in_servant_upcall ())" << be_nl
      << "{" << be_idt << be_nl
      << "stub = new IIOP_Object (" << be_idt << be_idt << be_nl
      << "CORBA::string_copy (this->_interface_repository_id ())," << be_nl
      << "IIOP::Profile (" << be_idt << be_idt << be_nl
      << "TAO_ORB_Core_instance ()->orb_params ()->addr ()," << be_nl
      << "orb_core->get_current ()->object_key ()" << be_uidt << be_nl
      << ")" << be_uidt << be_uidt << be_nl
      << ");" << be_uidt << be_uidt << be_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "POA* poa = this->default_poa (_env);" << be_nl
      << "if (_env.exception () != 0)" << be_idt << be_nl
      << "return 0;" << be_uidt << be_nl
      << "const TAO::ObjectKey& object_key = " << be_idt << be_nl
      << "poa->servant_to_id (this, _env);" << be_uidt << be_nl
      << "if (_env.exception () != 0)" << be_idt << be_nl
      << "return 0;" << be_uidt << be_nl
      << "stub = new IIOP_Object (" << be_idt << be_idt << be_nl
      << "CORBA::string_copy (this->_interface_repository_id ())," << be_nl
      << "IIOP::Profile (" << be_idt << be_idt << be_nl
      << "TAO_ORB_Core_instance ()->orb_params ()->addr ()," << be_nl
      << "object_key" << be_uidt << be_nl
      << ")" << be_uidt << be_uidt << be_nl
      << ");" << be_uidt << be_uidt << be_nl
      << "}\n" << be_nl
#endif /* 0 */

      << "return new " << this->full_coll_name ()
      << " (this, stub);" << be_uidt << be_nl;

  *ss << "}\n\n";

   return 0;
}

// Generates the server-side inline
int
be_interface::gen_server_inline (void)
{
  // nothing to be done
  TAO_OutStream *si; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line

  // Macro to avoid "warning: unused parameter" type warning.
  ACE_UNUSED_ARG (i);

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  si = cg->server_inline ();

  // generate the skeleton class name

  si->indent (); // start with whatever indentation level we are at

  *si << "// skeleton destructor" << nl;

  *si << "ACE_INLINE" << nl;
  if (!this->is_nested ())
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *si << this->full_skel_name () << "::~POA_" << this->local_name () <<
        " (void)" << nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *si << this->full_skel_name () << "::~" << this->local_name () <<
        " (void)" << nl;
    }

  *si << "{\n";
  *si << "}\n";


  // generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the call
  if (this->traverse_inheritance_graph (be_interface::gen_skel_helper, si) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_server_inline - "
                         "inheritance graph traversal failed\n"), -1);
    }

  return 0;
}

// generate the var definition
int
be_interface::gen_var_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // names

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_var", this->local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

  // generate the var definition (always in the client header).
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ch->indent (); // start with whatever was our current indent level
  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // default constr
  *ch << namebuf << " (void); // default constructor" << nl;
  *ch << namebuf << " (" << local_name () << "_ptr);" << nl;

  // copy constructor
  *ch << namebuf << " (const " << namebuf <<
    " &); // copy constructor" << nl;

  // destructor
  *ch << "~" << namebuf << " (void); // destructor" << nl;
  *ch << nl;

  // assignment operator from a pointer
  *ch << namebuf << " &operator= (" << local_name () << "_ptr);" << nl;

  // assignment from _var
  *ch << namebuf << " &operator= (const " << namebuf <<
    " &);" << nl;

  // arrow operator
  *ch << local_name () << "_ptr operator-> (void) const;" << nl;

  *ch << nl;

  // other extra types (cast operators, [] operator, and others)
  *ch << "operator const " << local_name () << "_ptr &() const;" << nl;
  *ch << "operator " << local_name () << "_ptr &();" << nl;

  *ch << "// in, inout, out, _retn " << nl;
  // the return types of in, out, inout, and _retn are based on the parameter
  // passing rules and the base type
  *ch << local_name () << "_ptr in (void) const;" << nl;
  *ch << local_name () << "_ptr &inout (void);" << nl;
  *ch << local_name () << "_ptr &out (void);" << nl;
  *ch << local_name () << "_ptr _retn (void);" << nl;

  // generate an additional member function that returns the underlying pointer
  *ch << local_name () << "_ptr ptr (void) const;\n";

  *ch << "\n";
  ch->decr_indent ();

  // private
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_ptr ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";

  return 0;
}

// implementation of the _var class. All of these get generated in the inline
// file
int
be_interface::gen_var_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _var names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_var", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  // generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

  // default constr
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname <<
    " (void) // default constructor" << nl;
  *ci << "\t" << ": ptr_ (" << this->name () << "::_nil ())" << nl;
  *ci << "{}\n\n";

  // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{}\n\n";

  // the additional ptr () member function. This member function must be
  // defined before the remaining member functions including the copy
  // constructor because this inline function is used elsewhere. Hence to make
  // inlining of this function possible, we must define it before its use.
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr " << nl;
  *ci << fname << "::ptr (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname <<
    " &p) // copy constructor" << nl;
  *ci << "\t: ptr_ (" << name () << "::_duplicate (p.ptr ()))" << nl;
  *ci << "{}\n\n";

  // destructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::~" << lname << " (void) // destructor" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << name () <<
    "_ptr p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // assignment operator from _var
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "if (this != &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << name () << "::_duplicate (p.ptr ());\n";
  ci->decr_indent ();
  *ci << "}" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // other extra methods - cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator const " << name () <<
    "_ptr &() const // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << name () << "_ptr &() // cast " << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // operator->
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr " << nl;
  *ci << fname << "::operator-> (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // in, inout, out, and _retn
  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr" << nl;
  *ci << fname << "::in (void) const" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr &" << nl;
  *ci << fname << "::inout (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr &" << nl;
  *ci << fname << "::out (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();" << nl;
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  ci->indent ();
  *ci << "ACE_INLINE " << name () << "_ptr " << nl;
  *ci << fname << "::_retn (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "// yield ownership of managed obj reference" << nl;
  *ci << this->name () << "_ptr val = this->ptr_;" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();" << nl;
  *ci << "return val;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// generate the _out definition
int
be_interface::gen_out_defn (void)
{
  TAO_OutStream *ch; // output stream
  TAO_NL  nl;        // end line
  char namebuf [NAMEBUFSIZE];  // to hold the _out name

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (namebuf, "%s_out", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ch = cg->client_header ();

  // generate the out definition (always in the client header)
  ch->indent (); // start with whatever was our current indent level

  *ch << "class " << namebuf << nl;
  *ch << "{" << nl;
  *ch << "public:\n";
  ch->incr_indent ();

  // No default constructor

  // constructor from a pointer
  *ch << namebuf << " (" << local_name () << "_ptr &);" << nl;
  // constructor from a _var &
  *ch << namebuf << " (" << local_name () << "_var &);" << nl;
  // constructor from a _out &
  *ch << namebuf << " (" << namebuf << " &);" << nl;
  // assignment operator from a _out &
  *ch << namebuf << " &operator= (" << namebuf << " &);" << nl;
  // assignment operator from a pointer &, cast operator, ptr fn, operator
  // -> and any other extra operators
  // only interface allows assignment from var &
  *ch << namebuf << " &operator= (const " << local_name () << "_var &);" << nl;
  *ch << namebuf << " &operator= (" << local_name () << "_ptr);" << nl;
  // cast
  *ch << "operator " << local_name () << "_ptr &();" << nl;
  // ptr fn
  *ch << local_name () << "_ptr &ptr (void);" << nl;
  // operator ->
  *ch << local_name () << "_ptr operator-> (void);" << nl;

  *ch << "\n";
  ch->decr_indent ();
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << "_ptr &ptr_;\n";

  ch->decr_indent ();
  *ch << "};\n\n";
  return 0;
}

int
be_interface::gen_out_impl (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line
  char fname [NAMEBUFSIZE];  // to hold the full and
  char lname [NAMEBUFSIZE];  // local _out names

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (fname, "%s_out", this->fullname ());

  ACE_OS::memset (lname, '\0', NAMEBUFSIZE);
  ACE_OS::sprintf (lname, "%s_out", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();

  // generate the var implementation in the inline file
  // Depending upon the data type, there are some differences which we account
  // for over here.

  ci->indent (); // start with whatever was our current indent level

  *ci << "// *************************************************************"
      << nl;
  *ci << "// Inline operations for class " << fname << nl;
  *ci << "// *************************************************************\n\n";

      // constr from a _ptr
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << name () << "_ptr &p)" << nl;
  *ci << "\t: ptr_ (p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = " << this->name () << "::_nil ();\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // constructor from _var &
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << this->name () <<
    "_var &p) // constructor from _var" << nl;
  *ci << "\t: ptr_ (p.out ())" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "CORBA::release (this->ptr_);" << nl;
  *ci << "this->ptr_ = " << this->name () << "::_nil ();\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (" << fname <<
    " &p) // copy constructor" << nl;
  *ci << "\t: ptr_ (p.ptr_)" << nl;
  *ci << "{}\n\n";

      // assignment operator from _out &
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << fname <<
    " &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p.ptr_;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // assignment operator from _var
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (const " << this->name () <<
    "_var &p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = " << this->name () << "::_duplicate (p.ptr ());" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // assignment operator from _ptr
  ci->indent ();
  *ci << "ACE_INLINE " << fname << " &" << nl;
  *ci << fname << "::operator= (" << this->name () <<
    "_ptr p)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "this->ptr_ = p;" << nl;
  *ci << "return *this;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // other extra methods - cast operator ()
  ci->indent ();
  *ci << "ACE_INLINE " << nl;
  *ci << fname << "::operator " << this->name () <<
    "_ptr &() // cast" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  // ptr function
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_ptr &" << nl;
  *ci << fname << "::ptr (void) // ptr" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

      // operator->
  ci->indent ();
  *ci << "ACE_INLINE " << this->name () << "_ptr " << nl;
  *ci << fname << "::operator-> (void)" << nl;
  *ci << "{\n";
  ci->incr_indent ();
  *ci << "return this->ptr_;\n";
  ci->decr_indent ();
  *ci << "}\n\n";

  return 0;
}

// generate typecode.
// Typecode for interface comprises the enumerated value followed by the
// encapsulation of the parameters

int
be_interface::gen_typecode (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  *cs << "CORBA::tk_objref, // typecode kind" << nl;
  *cs << this->tc_size () << ", // encapsulation length\n";
  // now emit the encapsulation
  return this->gen_encapsulation ();
}

// generate encapsulation
// An encapsulation for ourselves will be necessary when we are part of some
// other IDL type and a typecode for that other type is being generated. This
// will comprise our typecode kind. IDL types with parameters will additionally
// have the encapsulation length and the entire typecode description
int
be_interface::gen_encapsulation (void)
{
  TAO_OutStream *cs; // output stream
  TAO_NL  nl;        // end line
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  long i, arrlen;
  long *arr;  // an array holding string names converted to array of longs

  cs = cg->client_stubs ();
  cs->indent (); // start from whatever indentation level we were at

  // XXXASG - byte order must be based on what m/c we are generating code -
  // TODO
  *cs << "TAO_ENCAP_BYTE_ORDER, // byte order" << nl;
  // generate repoID
  *cs << (ACE_OS::strlen (this->repoID ())+1) << ", ";
  (void)this->tc_name2long (this->repoID (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("0x%x, ", arr[i]);
    }
  *cs << " // repository ID = " << this->repoID () << nl;
  // generate name
  *cs << (ACE_OS::strlen (this->local_name ()->get_string ())+1) << ", ";
  (void)this->tc_name2long(this->local_name ()->get_string (), arr, arrlen);
  for (i=0; i < arrlen; i++)
    {
      cs->print ("0x%x, ", arr[i]);
    }
  *cs << " // name = " << this->local_name () << ",\n";

  return 0;
}

// compute size of typecode
long
be_interface::tc_size (void)
{
   return 4 + 4 + this->tc_encap_len ();
}

// compute the encapsulation length
long
be_interface::tc_encap_len (void)
{
  if (this->encap_len_ == -1) // not computed yet
    {
      long slen;

      // Macro to avoid "warning: unused parameter" type warning.
      ACE_UNUSED_ARG (slen);

      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += this->repoID_encap_len (); // for repoID

      // do the same thing for the local name
      this->encap_len_ += this->name_encap_len ();

    }
  return this->encap_len_;
}

// helper
int
be_interface::gen_operation_table (void)
{
  TAO_OutStream *ss; // output stream
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ss = cg->server_skeletons ();

  ss->indent (); // start from current indentation level
  *ss << "static const TAO_operation_db_entry " << this->flatname () <<
    "_operations [] = {\n";
  ss->incr_indent (0);

  if (this->traverse_inheritance_graph (be_interface::gen_optable_helper, ss) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_operation_table - "
                         "inheritance graph traversal failed\n"), -1);
    }

  // generate the skeleton for the is_a method
  ss->indent ();
  *ss << "{\"_is_a\", &" << this->full_skel_name () << "::_is_a_skel}\n";
  this->skel_count_++;

  ss->decr_indent ();
  *ss << "};" << nl << nl;

  // XXXASG - this code should be based on using different strategies for
  // demux - for next release
  *ss << "TAO_Dynamic_Hash_OpTable tao_" << this->flatname () << "_optable " <<
    "(" << this->flatname () << "_operations, " << this->skel_count_ << ", " <<
    2*this->skel_count_ << ");"
      << nl;
  return 0;
}

// we separate the generation of operation table entries from the
// "gen_operation_table" method. This enables us to invoke generation of
// entries for interfaces from which we inherit without any additional
// code. The parameter "derived" is the one for which the entire operation
// table is being built.
int
be_interface::gen_optable_entries (be_interface *derived)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  TAO_OutStream *ss; // output stream

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ss = cg->server_skeletons ();

  if (this->nmembers () > 0)
    {
      // if there are elements in this scope i.e., any operations and
      // attributes defined by "this" which happens to be the same as "derived"
      // or one of its ancestors.

      si = new UTL_ScopeActiveIterator (this, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (d->node_type () == AST_Decl::NT_op)
            {
              ss->indent (); // start from current indentation level
              // we are an operation node
              *ss << "{\"" << d->local_name () << "\", &" << derived->full_skel_name
                () << "::" << d->local_name () << "_skel},\n";
              derived->skel_count_++;
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr;

              ss->indent (); // start from current indentation level
              // generate only the "get" entry if we are readonly
              *ss << "{\"_get_" << d->local_name () << "\", &" <<
                derived->full_skel_name () << "::_get_" << d->local_name () <<
                "_skel},\n";
              derived->skel_count_++;

              attr = AST_Attribute::narrow_from_decl (d);
              if (!attr)
                return -1;

              if (!attr->readonly ())
                {
                  // the set method
                  ss->indent (); // start from current indentation level
                  *ss << "{\"_set_" << d->local_name () << "\", &" <<
                    derived->full_skel_name () << "::_set_" << d->local_name
                    () << "_skel},\n";
                  derived->skel_count_++;
                }
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// template method that traverses the inheritance graph in a breadth-first
// style. The actual work on each element in the inheritance graph is carried
// out by the function passed as argument
int be_interface::traverse_inheritance_graph (be_interface::tao_code_emitter gen,
                                              TAO_OutStream *os)
{
  long i;            // loop index
  ACE_Unbounded_Queue <be_interface*> queue; // Queue data structure needed for
                                            // breadth-first traversal of
                                            // inheritance tree

  // For a special case of a deeply nested inheritance graph and one specific
  // way of inheritance in which a node that was already visited, but is not present in
  // the queue, gets inserted at the tail. This sitation arises when a node
  // multiply inherits from two or more interfaces in which the first parent is
  // higher up in the tree than the second parent. In addition, if the second
  // parent turns out to be a child of the first .

  ACE_Unbounded_Queue <be_interface*> del_queue; // queue of dequeued nodes to
                                                 // be searched for the above case

  // insert ourselves in the Queue
  if (queue.enqueue_tail (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "(%N:%l) be_interface::gen_operation_table - "
                         "error generating entries\n"), -1);
    }

  // do until queue is empty
  while (!queue.is_empty ())
    {
      be_interface *bi;  // element inside the queue

      // use breadth-first strategy i.e., first generate entries for ourselves,
      // followed by nodes that we immediately inherit from, and so on. In the
      // process make sure that we do not generate code for the same node more
      // than once. Such a case may arise due to multiple inheritance forming a
      // diamond like inheritance graph.

      // dequeue the element at the head of the queue
      if (queue.dequeue_head (bi))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "dequeue_head failed\n"), -1);
        }

      // insert the dequeued element in the del_queue
      if (del_queue.enqueue_tail (bi) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "(%N:%l) be_interface::gen_operation_table - "
                             "error generating entries\n"), -1);
        }

      // use the helper method to generate code for ourself using the
      // properties of the element dequeued. For the first iteration, the
      // element dequeued and "this" will be the same i.e., ourselves
      if (gen (this, bi, os) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_interface::traverse_graph - "
                             "helper code gen failed\n"), -1);
        }

      // now check if the dequeued element has any ancestors. If yes, insert
      // them inside the queue making sure that there are no duplicates
      for (i=0; i < bi->n_inherits (); i++)
        {
          be_interface *parent;  // parent of the dequeued element

          // initialize an iterator to search the queue for duplicates
          ACE_Unbounded_Queue_Iterator<be_interface*> q_iter (queue);

          // retrieve the next parent from which the dequeued element inherits
          parent = be_interface::narrow_from_decl (bi->inherits ()[i]);
          if (!parent)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_interface::gen_server_skeletons - "
                                 "bad inherited interface\n"), -1);
            }

          // now insert this node at the tail of the queue, but make sure that
          // it doesn't already exist in the queue
          int found = 0;
          while (!q_iter.done ())
            {
              be_interface **temp;  // queue element

              (void) q_iter.next (temp);
              if (!ACE_OS::strcmp (parent->fullname (), (*temp)->fullname ()))
                {
                  // we exist in this queue and cannot be inserted
                  found = 1;
                }
              if (found)
                break;
              (void) q_iter.advance ();
            } // end of while

          // initialize an iterator to search the del_queue for duplicates
          ACE_Unbounded_Queue_Iterator<be_interface*> del_q_iter (del_queue);

          while (!found && !del_q_iter.done ())
            {
              be_interface **temp;  // queue element

              (void) del_q_iter.next (temp);
              if (!ACE_OS::strcmp (parent->fullname (), (*temp)->fullname ()))
                {
                  // we exist in this del_queue and cannot be inserted
                  found = 1;
                }
              if (found)
                break;
              (void) del_q_iter.advance ();
            } // end of while

          if (!found)
            {
              // insert the parent in the queue
              if (queue.enqueue_tail (parent) == -1)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_interface::gen_server_skeletons - "
                                 "enqueue op failed\n"), -1);
                }
            }
        } // end of for loop
    } // end of while queue not empty
  return 0;
}

// helpers passed to the template method

int
be_interface::gen_optable_helper (be_interface *derived,
                                  be_interface *ancestor,
                                  TAO_OutStream * /*os*/)
{
  // generate entries for the derived class using the properties of its
  // ancestors
  if (ancestor->gen_optable_entries (derived) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_interface::gen_operation_table - "
                         "error generating entries for inherited"
                         "interfaces\n"), -1);
    }
  return 0;
}

int
be_interface::is_a_helper (be_interface * /*derived*/,
			   be_interface *bi,
			   TAO_OutStream *os)
{
  // emit the comparison code
  os->indent ();
  *os << "(!ACE_OS::strcmp ((char *)value, \"" << bi->repoID () <<
    "\")) ||\n";

  return 0;
}

int
be_interface::downcast_helper (be_interface * /* derived */,
			       be_interface *base,
			       TAO_OutStream *os)
{
  *os << "if (ACE_OS::strcmp (logical_type_id, \""
      << base->repoID () << "\") == 0)" << be_idt_nl
      << "return ACE_static_cast ("
      << base->full_skel_name () << "_ptr, this);" << be_uidt_nl;
  return 0;
}

int
be_interface::gen_skel_helper (be_interface *derived,
                               be_interface *ancestor,
                               TAO_OutStream *os)
{
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  TAO_NL  nl;        // end line

  // if derived and ancestor are same, skip it
  if (derived == ancestor)
    return 0;

  // else generate code that does the cast to the appropriate type

  if (ancestor->nmembers () > 0)
    {
      // if there are elements in ancestor scope i.e., any operations and
      // attributes defined by "ancestor", become methods on the derived class
      // which call the corresponding method of the base class by doing the
      // proper casting

      si = new UTL_ScopeActiveIterator (ancestor, UTL_Scope::IK_decls);
      // instantiate a scope iterator.

      while (!(si->is_done ()))
        {
          // get the next AST decl node
          d = si->item ();
          if (d->node_type () == AST_Decl::NT_op)
            {
              os->indent (); // start from current indentation level
              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // generate the static method corresponding to this method
                  *os << "static void " << d->local_name () <<
                    "_skel (CORBA::ServerRequest &req, void *obj,"
                      << " void *context, CORBA::Environment &env);\n\n";
                }
              else
                { // generate code in the inline file
                  // generate the static method corresponding to this method
                  *os << "ACE_INLINE void " << derived->full_skel_name () <<
                    "::" << d->local_name () <<
                    "_skel (CORBA::ServerRequest &req, " <<
                    "void *obj, void *context, CORBA::Environment &env)" << nl;
                  *os << "{\n";
                  os->incr_indent ();
                  *os << ancestor->full_skel_name () << "_ptr impl = (" <<
                    derived->full_skel_name () << "_ptr) obj;" << nl;
                  *os << ancestor->full_skel_name () << "::" << d->local_name
                    () << "_skel (req, (" << ancestor->full_skel_name () <<
                    "_ptr) impl, context, env);\n";
                  os->decr_indent ();
                  *os << "}\n";
                }
            }
          else if (d->node_type () == AST_Decl::NT_attr)
            {
              AST_Attribute *attr;

              attr = AST_Attribute::narrow_from_decl (d);
              if (!attr)
                return -1;

              os->indent (); // start from current indentation level
              if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                {
                  // generate the static method corresponding to this method
                  *os << "static void _get_" << d->local_name () <<
                    "_skel (CORBA::ServerRequest &req, void *obj,"
                      << " void *context, CORBA::Environment &env);\n\n";
                }
              else
                { // generate code in the inline file
                  // generate the static method corresponding to this method
                  *os << "ACE_INLINE void " << derived->full_skel_name () <<
                    "::_get_" << d->local_name () <<
                    "_skel (CORBA::ServerRequest &req, " <<
                    "void *obj, void *context, CORBA::Environment &env)" << nl;
                  *os << "{\n";
                  os->incr_indent ();
                  *os << ancestor->full_skel_name () << "_ptr impl = (" <<
                    derived->full_skel_name () << "_ptr) obj;" << nl;
                  *os << ancestor->full_skel_name () << "::_get_" << d->local_name
                    () << "_skel (req, (" << ancestor->full_skel_name () <<
                    "_ptr) impl, context, env);\n";
                  os->decr_indent ();
                  *os << "}\n";
                }

              if (!attr->readonly ())
                {
                  // the set method
                  os->indent (); // start from current indentation level
                  if (os->stream_type () == TAO_OutStream::TAO_SVR_HDR)
                    {
                      // generate the static method corresponding to this method
                      *os << "static void _set_" << d->local_name () <<
                        "_skel (CORBA::ServerRequest &req, void *obj,"
                          << " void *context, CORBA::Environment &env);\n\n";
                    }
                  else
                    { // generate code in the inline file
                      // generate the static method corresponding to this method
                      *os << "ACE_INLINE void " << derived->full_skel_name ()
                          << "::_set_" << d->local_name () <<
                        "_skel (CORBA::ServerRequest &req, " <<
                        "void *obj, void *context, CORBA::Environment &env)" <<
                        nl;
                      *os << "{\n";
                      os->incr_indent ();
                      *os << ancestor->full_skel_name () << "_ptr impl = (" <<
                        derived->full_skel_name () << "_ptr) obj;" << nl;
                      *os << ancestor->full_skel_name () << "::_get_" <<
                        d->local_name () << "_skel (req, (" <<
                        ancestor->full_skel_name () <<
                        "_ptr) impl, context, env);\n";
                      os->decr_indent ();
                      *os << "}\n";
                    }

                }
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  return 0;
}

// return the relative skeleton name (needed due to NT compiler insanity)
char *
be_interface::relative_skel_name (const char *skelname)
{
  // some compilers do not like generating a fully scoped name for a type that
  // was defined in the same enclosing scope in which it was defined. For such,
  // we emit a macro defined in the ACE library.
  //

  // The tricky part here is that it is not enough to check if the
  // typename we are using was defined in the current scope. But we
  // need to ensure that it was not defined in any of our ancestor
  // scopes as well. If that is the case, then we can generate a fully
  // scoped name for that type, else we use the ACE_NESTED_CLASS macro

  // thus we need some sort of relative name to be generated

  static char macro [NAMEBUFSIZE];
  be_decl *def_scope = 0;  // our defining scope
  char // hold the fully scoped name
    def_name [NAMEBUFSIZE],
    use_name [NAMEBUFSIZE];
  char // these point to the curr and next component in the scope
    *def_curr = def_name,
    *def_next,
    *use_curr = use_name,
    *use_next;

  ACE_OS::memset (macro, '\0', NAMEBUFSIZE);
  ACE_OS::memset (def_name, '\0', NAMEBUFSIZE);
  ACE_OS::memset (use_name, '\0', NAMEBUFSIZE);

  // traverse every component of the def_scope and use_scope beginning at the
  // root and proceeding towards the leaf trying to see if the components
  // match. Continue until there is a match and keep accumulating the path
  // traversed. This forms the first argument to the ACE_NESTED_CLASS
  // macro. Whenever there is no match, the remaining components of the
  // def_scope form the second argument

  ACE_OS::strcpy (def_name, this->full_skel_name ());
  ACE_OS::strcpy (use_name, skelname);

  while (def_curr && use_curr)
    {
      // find the first occurrence of a :: and advance the next pointers accordingly
      def_next = ACE_OS::strstr (def_curr, "::");
      use_next = ACE_OS::strstr (use_curr, "::");

      if (def_next)
        *def_next = 0;

      if (use_next)
        *use_next = 0;

      if (!ACE_OS::strcmp (def_curr, use_curr))
        {
          // they have same prefix, append to arg1
          def_curr = (def_next ? (def_next+2) : 0); // skip the ::
          use_curr = (use_next ? (use_next+2) : 0); // skip the ::
        }
      else
        {
          // we had overwritten a ':' by a '\0' for string comparison. We
          // revert back because we want the rest of the relative name to be
          // used
          if (def_next)
            *def_next = ':';

          if (use_next)
            *use_next = ':';

          // no match. This is the end of the first argument. Get out
          // of the loop as no more comparisons are necessary
          break;
        }
    }

  // start the 2nd argument of the macro

  // copy the remaining def_name (if any left)
  if (def_curr)
    ACE_OS::strcat (macro, def_curr);

  return macro;
}

int
be_interface::accept (be_visitor *visitor)
{
  return visitor->visit_interface (this);
}

int be_interface::write_as_return (TAO_OutStream *stream,
				   be_type *type)
{
  *stream << type->name () << "_ptr";
  return 0;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_interface)
IMPL_NARROW_FROM_SCOPE (be_interface)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node <be_interface*>;
template class ACE_Unbounded_Queue <be_interface*>;
template class ACE_Unbounded_Queue_Iterator <be_interface*>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<be_interface*>
#pragma instantiate ACE_Unbounded_Queue<be_interface*>
#pragma instantiate ACE_Unbounded_Queue_Iterator<be_interface*>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
