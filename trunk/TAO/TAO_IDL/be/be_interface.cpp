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
#include	"be.h"

/*
 * BE_Interface
 */

// default constructor
be_interface::be_interface (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

// constructor used to build the AST
be_interface::be_interface (UTL_ScopedName *n, AST_Interface **ih, long nih,
                            UTL_StrList *p)
  : AST_Interface (n, ih, nih, p),
    AST_Decl (AST_Decl::NT_interface, n, p),
    UTL_Scope (AST_Decl::NT_interface)
{
  this->size_type (be_decl::VARIABLE); // always the case

  // computes the repoID
  compute_repoID ();

  // computes the fully scoped name
  compute_fullname ();

  // computes the fully scoped typecode name
  compute_tc_name ();

  // compute the flattened fully scoped name 
  compute_flatname ();

  // compute the full skel name
  compute_fullskelname ();
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
      first = I_TRUE;
      second - I_FALSE;
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

const char*
be_interface::full_skel_name (void)
{
  return this->full_skel_name_;
}

// ----------------------------------------
//            CODE GENERATION METHODS
// ----------------------------------------

// generate the client header 
int be_interface::gen_client_header (void)
{
  TAO_OutStream *ch; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_CH); // set the current code generation
                                            // state 
  ch = cg->client_header ();
  // pass info
  cg->outstream (ch);
  cg->node (this);

  // == STEP 1:  generate the class name and class names we inherit ==
  ch->indent (); // start with whatever indentation level we are at

  // generate the forward declaration
  *ch << "class " << local_name () << ";" << nl;

  // generate the _ptr declaration
  *ch << "typedef " << local_name () << " *" << local_name () << "_ptr;" <<
    nl;

  // now generate the class definition
  *ch << "class " << local_name ();

  if (n_inherits () > 0)  // this interface inherits from other interfaces
    {
      *ch << ": ";
      for (i = 0; i < n_inherits (); i++) 
        {
          *ch << "public virtual ";
          *ch << inherits ()[i]->name ();  // dump the scoped name
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
  *ch << "static " << local_name () << "_ptr " << "_duplicate (" <<
    local_name () << "_ptr obj);" << nl;
  *ch << "static " << local_name () << "_ptr " << "_narrow (" <<
    "CORBA::Object_ptr obj);" << nl;
  *ch << "static " << local_name () << "_ptr " << "_nil (" <<
    "void);\n\n"; 

  // generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (be_scope::gen_client_header () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_client_header\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // generate the "protected" constructor so that users cannot instantiate
  // us 
  ch->decr_indent ();
  *ch << "protected:\n";
  ch->incr_indent ();
  *ch << local_name () << " (void); // default constructor" << nl;
  *ch << local_name () << " (STUB_Object *objref);" << nl;
  *ch << "virtual ~" << local_name () << " (void);\n";
  ch->decr_indent ();
  // private copy constructor and assignment operator. These are not
  // allowed, hence they are private.
  *ch << "private:\n";
  ch->incr_indent ();
  *ch << local_name () << " (const " << local_name () << "&);" << nl;
  *ch << "void operator= (const " << local_name () << "&);\n";
  ch->decr_indent ();
  *ch << "};\n\n";

  // generate the _var declaration
  if (this->gen_var_defn () == -1)
    {
      ACE_ERROR ((LM_ERROR, 
                  "be_interface - error generating _var definition\n"));
      return -1;
    }

  // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec 
  if (this->gen_out_defn () == -1)
    {
      ACE_ERROR ((LM_ERROR, 
                  "be_interface - error generating _var definition\n"));
      return -1;
    }

  // generate the typecode decl. If we are in the outermost scope, our typecode
  // decl is extern
  if (this->name ()->length () > 2)
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
  return 0;
}

int be_interface::gen_client_stubs (void)
{
  TAO_OutStream *cs; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line


  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_CS); // set the current code generation
                                            // state 

  cs = cg->client_stubs ();
  //pass info
  cg->node (this);
  cg->outstream (cs);

  cs->indent (); // start with whatever indentation level we are at

  // first generate the code for the static methods
  // The _duplicate method
  *cs << name () << "_ptr " << name () << "::_duplicate (" <<
    name () << "_ptr obj)" << nl;
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
  *cs << name () << "_ptr " << name () << 
    "::_narrow (CORBA::Object_ptr obj)" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "CORBA::Environment env;" << nl;
  *cs << "if (obj->_is_a (\"" << this->repoID () << "\", env))" << nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "STUB_Object *istub;" << nl;
  *cs << name () << "_ptr new_obj; // to be returned " << nl;
#if 0 // XXXASG - I was told that emitting this line of code is the root cause
      // of all evil 
  *cs << "obj->Release ();" << 
    " // need this since _is_a grabbed an obj reference " << nl;
#endif
  *cs << "if (obj->QueryInterface (IID_STUB_Object, (void **)&istub) " <<
    "!= NOERROR)\n";
  cs->incr_indent ();
  *cs << "return " << name () << "::_nil ();\n";
  cs->decr_indent ();
  *cs << nl;
  *cs << "obj->Release (); " << 
    "// need this since QueryIntf bumped our refcount" << nl;
  *cs << "new_obj = new " << name () << " (istub); " <<
    "// construct obj ref using the stub object" << nl;
  *cs << "return new_obj;\n";
  cs->decr_indent ();
  *cs << "} // end of if" << nl;
  *cs << "return " << name () << "::_nil (); // _narrow failed\n";
  cs->decr_indent ();
  *cs << "} // end of _narrow" << nl << nl;

  // _nil method
  *cs << name () << "_ptr " << name () << "::_nil (void)" <<
    nl;
  *cs << "{\n";
  cs->incr_indent ();
  *cs << "return (" << name () << "_ptr)NULL;\n";
  cs->decr_indent ();
  *cs << "} // end of _nil" << nl << nl;

  // generate code for the elements of the interface
  if (be_scope::gen_client_stubs () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface::gen_client_stubs\n")); 
      ACE_ERROR ((LM_ERROR, "Scope code generation failure\n"));
      return -1;
    }

  // generate the typecode information here
  cs->indent (); // start from current indentation level
  *cs << "static const CORBA::Long _oc_" << this->flatname () << "[] =" <<
    nl; 
  *cs << "{\n";
  cs->incr_indent (0);
  if (this->gen_typecode () == -1)
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
  static char namebuf [MAXNAMELEN]; // holds the class name
  AST_Decl *d;       // enclosing scope

  ACE_OS::memset (namebuf, '\0', MAXNAMELEN);

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_SH); // set the current code generation
                                          // state 

  sh = cg->server_header ();
  // pass info
  cg->node (this);
  cg->outstream (sh);

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
  *sh << "class " << namebuf << " : public virtual " << name ();
  if (n_inherits () > 0)  // this interface inherits from other interfaces
    {
      be_interface *intf;

      for (i = 0; i < n_inherits (); i++) 
        {
          *sh << ", public virtual ";
          intf = be_interface::narrow_from_decl (inherits ()[i]);
          *sh << intf->full_skel_name ();  // dump the scoped name
        }  // end of for loop 
    }
  *sh << nl;
  *sh << "{" << nl;
  *sh << "protected:\n";
  sh->incr_indent ();
  *sh << namebuf << " (const char *obj_name = 0);" << nl;
  *sh << "virtual ~" << namebuf << " (void);\n";
  sh->decr_indent ();
  *sh << "public:\n";
  sh->incr_indent (0);
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
    "CORBA::Object_ptr obj, CORBA::Environment &env);\n\n";

  sh->decr_indent ();
  *sh << "};\n\n";

  cg->pop ();
  return 0;
}

int be_interface::gen_server_skeletons (void)
{
  TAO_OutStream *ss; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  AST_Decl *d;        // temporary

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();
  cg->push (TAO_CodeGen::TAO_INTERFACE_SS); // set the current code generation
                                          // state 

  ss = cg->server_skeletons ();
  // pass info
  cg->node (this);
  cg->outstream (ss);

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

  if (d && d->node_type () == AST_Decl::NT_root)
    {
      // we are outermost. So the POA_ prefix is prepended to our name
      *ss << this->full_skel_name () << "::POA_" << this->local_name () << 
        " (const char *obj_name)" << nl;
    }
  else
    {
      // the POA_ prefix is prepended to our outermost module name
      *ss << this->full_skel_name () << "::" << this->local_name () << 
        " (const char *obj_name)" << nl;
    }
  
  *ss << "{\n";
  ss->incr_indent ();
  // code for the skeleton constructor
  *ss << "const CORBA::String repoID = \"" << this->repoID () << "\"; // repository ID" << nl;
  *ss << "IIOP_Object *data; // Actual object reference" << nl;
  *ss << "CORBA::ORB_ptr orb = TAO_ORB_Core_instance ()->orb (); " <<
    "// underlying ORB" << nl;
  *ss << "CORBA::POA_ptr oa = TAO_ORB_Core_instance ()->root_poa (); " <<
    "// underlying OA" << nl;
  *ss << "this->optable_ = &tao_" << local_name () << "_optable;" << nl <<
    nl; 
  *ss << "// set up an IIOP object" << nl;
  *ss << "data = new IIOP_Object (CORBA::string_dup (repoID));" << nl;
  *ss << "data->profile.iiop_version.major = IIOP::MY_MAJOR;" << nl;
  *ss << "data->profile.iiop_version.minor = IIOP::MY_MINOR;" << nl;
  *ss << "const ACE_INET_Addr &addr = orb->params ()->addr ();" << nl;
  *ss << "data->profile.host = ACE_OS::strdup (" << 
    "addr.get_host_name ());" << nl;
  *ss << "data->profile.port = addr.get_port_number ();" << nl;
  *ss << "data->profile.object_key.length = " << 
    "ACE_OS::strlen (obj_name);" << nl;
  *ss << "data->profile.object_key.maximum = " << 
    "data->profile.object_key.length;" << nl;
  *ss << "data->profile.object_key.buffer = " << 
    "new CORBA::Octet [(size_t)data->profile.object_key.length+1];" << nl;
  *ss << "ACE_OS::memcpy (data->profile.object_key.buffer, obj_name, " << 
    "data->profile.object_key.length); // set the obj key" << nl;
  *ss << "this->set_parent (data); // store the IIOP obj ref with us" <<
    nl;
  *ss << "this->sub_ = this; // set the most derived type to be us" << nl;
  *ss << "if (oa) oa->bind (data->profile.object_key, this); " <<
    "// register ourselves\n";
  ss->decr_indent ();
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
  *ss << "void " << this->full_skel_name () << 
    "::_is_a_skel (CORBA::ServerRequest &req, " << 
    "CORBA::Object_ptr obj, CORBA::Environment &env)" << nl;
  *ss << "{\n";
  ss->incr_indent ();
  *ss << "const CORBA::String type_id = \"" << this->repoID () << 
    "\"; // repository ID" << nl;
  *ss << "CORBA::NVList_ptr nvlist;" << nl;
  *ss << "CORBA::NamedValue_ptr nv;" << nl;
  *ss << "CORBA::Any temp_value (CORBA::_tc_string);" << nl;
  *ss << "CORBA::Any *any;" << nl;
  *ss << "CORBA::Boolean *retval;" << nl;
  *ss << "CORBA::String value;" << nl;
  *ss << nl;
  *ss << "req.orb()->create_list (0, nvlist);" << nl;
  *ss << "nv = nvlist->add_value (0, temp_value, CORBA::ARG_IN, env);" << nl;
  *ss << "req.params (nvlist, env); // parse the args" << nl;
  *ss << "if (env.exception () != 0) return;" << nl;
  *ss << "value = *(CORBA::String *)nv->value ()->value ();" << nl;
  *ss << "if (ACE_OS::strcmp ((char *)value, (char *)type_id) == 0" << nl;
  *ss << "  || ACE_OS::strcmp ((char *)value, CORBA::_tc_Object->id (env)) == 0)\n"
      << nl;
  *ss << "\tretval = new CORBA::Boolean (CORBA::B_TRUE);" << nl;
  * ss << "else" << nl;
  *ss << "\tretval = new CORBA::Boolean (CORBA::B_FALSE);" << nl;
  *ss << "any = new CORBA::Any (CORBA::_tc_boolean, retval, CORBA::B_TRUE);" <<
    nl; 
  *ss << "req.result (any, env);\n";
  ss->decr_indent ();
  *ss << "}\n\n";

  cg->pop ();
  return 0;
}

// helper
int
be_interface::gen_operation_table (void)
{
  int count = 0;
  UTL_ScopeActiveIterator *si;
  AST_Decl *d;
  TAO_OutStream *ss; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ss = cg->server_skeletons ();

  ss->indent (); // start from current indentation level
  *ss << "static const TAO_operation_db_entry " << local_name () <<
    "_operations [] = {\n";
  ss->incr_indent ();
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
	      // we are not imported.

              if (d->node_type () == AST_Decl::NT_op)
                {
                  // we are an operation node
                  *ss << "{\"" << d->local_name () << "\", &" << this->full_skel_name
                    () << "::" << d->local_name () << "_skel},"
                      << nl; 
                  count++;
                }
            }
          si->next ();
        } // end of while
      delete si; // free the iterator object
    }
  *ss << "{\"_is_a\", &" << this->full_skel_name () << "::_is_a_skel}\n";
  count++;
  ss->decr_indent ();
  *ss << "};" << nl << nl;

  // XXXASG - this code should be based on using different strategies for
  // demux - for next release
  *ss << "TAO_Dynamic_Hash_OpTable tao_" << local_name () << "_optable " <<
    "(" << local_name () << "_operations, " << count << ", " << 2*count << ");"
      << nl; 
  return 0;
}

// Generates the client-side inline information
int 
be_interface::gen_client_inline (void)
{
  TAO_OutStream *ci; // output stream
  TAO_NL  nl;        // end line

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();
  ci->indent (); // start from the current indentation level

  // generate the constructors and destructor
  *ci << "ACE_INLINE" << nl;
  *ci << this->name () << "::" << this->local_name () << 
    " (void) // default constructor" << nl;
  *ci << "{}" << nl << nl;

  *ci << "ACE_INLINE" << nl;
  *ci << this->name () << "::" << this->local_name () << 
    " (STUB_Object *objref) // constructor" << nl;
  *ci << "\t: CORBA::Object (objref)" << nl;
  *ci << "{}" << nl << nl;
  
  *ci << "ACE_INLINE" << nl;
  *ci << this->name () << "::~" << this->local_name () << 
    " (void) // destructor" << nl;
  *ci << "{}\n\n";

  if (this->gen_var_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: _var impl code gen failed\n"));
      return -1;
    }
  if (this->gen_out_impl () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: _out impl code gen failed\n"));
      return -1;
    }
  if (be_scope::gen_client_inline () == -1)
    {
      ACE_ERROR ((LM_ERROR, "be_interface: code gen failed for scope\n"));
      return -1;
    }
  return 0;
}

// Generates the server-side inline
int 
be_interface::gen_server_inline (void)
{
  // nothing to be done
  return 0;
}

#if 0 // XXXASG - TODO
// generate the var definition
int
be_interface::gen_var_defn (void)
{
  TAO_OutStream *ch; // output stream
  long i;            // loop index
  TAO_NL  nl;        // end line
  char namebuf [MAXNAMELEN];  // names

  ACE_OS::memset (namebuf, '\0', MAXNAMELEN);
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
  long i;            // loop index
  TAO_NL  nl;        // end line
  char fname [MAXNAMELEN];  // to hold the full and
  char lname [MAXNAMELEN];  // local _var names

  ACE_OS::memset (fname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (fname, "%s_var", this->fullname ());

  ACE_OS::memset (lname, '\0', MAXNAMELEN);
  ACE_OS::sprintf (lname, "%s_var", local_name ()->get_string ());

  // retrieve a singleton instance of the code generator
  TAO_CodeGen *cg = TAO_CODEGEN::instance ();

  ci = cg->client_inline ();
  cg->outstream (ci);

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

  // copy constructor
  ci->indent ();
  *ci << "ACE_INLINE" << nl;
  *ci << fname << "::" << lname << " (const " << fname << 
    " &p) // copy constructor" << nl;
  *ci << "\t: ptr_ (" << name () << "::_duplicate (p))" << nl; 
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
  *ci << "this->ptr_ = " << name () << "::_duplicate (p);\n";
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
  long i;            // loop index
  TAO_NL  nl;        // end line
  char namebuf [MAXNAMELEN];  // to hold the _out name

  ACE_OS::memset (namebuf, '\0', MAXNAMELEN);
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

  return 0;
}
#endif

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
  *cs << "0, // byte order" << nl; 
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

      this->encap_len_ = 4;  // holds the byte order flag

      this->encap_len_ += this->repoID_encap_len (); // for repoID

      // do the same thing for the local name
      this->encap_len_ += this->name_encap_len ();

    }
  return this->encap_len_;
}

// Narrowing
IMPL_NARROW_METHODS3 (be_interface, AST_Interface, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_interface)
IMPL_NARROW_FROM_SCOPE (be_interface)
