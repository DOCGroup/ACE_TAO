// Parse_Node.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Task.h"
#include "ace/Parse_Node.h"

// Provide the class hierarchy that defines the parse tree of Service
// Nodes.

#if !defined (__ACE_INLINE__)
#include "ace/Parse_Node.i"
#endif /* ____ */

// Keeps track of the number of errors encountered so far.
extern int ace_yyerrno;

// Global variable used to communicate between the parser and the main
// program.
extern ACE_Service_Config *ace_this_svc;

ACE_ALLOC_HOOK_DEFINE(ACE_Stream_Node)

void
ACE_Stream_Node::dump (void) const
{
  ACE_TRACE ("ACE_Stream_Node::dump");
}

void
ACE_Stream_Node::apply (void)
{
  ACE_TRACE ("ACE_Stream_Node::apply");

  if (ACE_Service_Config::initialize (this->node_->record (), 
				      this->node_->parameters ()) == -1)
    ace_yyerrno++;

  ACE_DEBUG ((LM_DEBUG, "did stream on %s, error = %d\n", 
	      this->node_->name (), 
	      ace_yyerrno));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Parse_Node)

void
ACE_Parse_Node::dump (void) const
{
  ACE_TRACE ("ACE_Parse_Node::dump");
}

const char *
ACE_Parse_Node::name (void) const
{
  ACE_TRACE ("ACE_Parse_Node::name");
  return this->name_;
}

ACE_Parse_Node *
ACE_Parse_Node::link (void) const
{
  ACE_TRACE ("ACE_Parse_Node::link");
  return this->next_;
}

void 
ACE_Parse_Node::link (ACE_Parse_Node *n)
{
  ACE_TRACE ("ACE_Parse_Node::link");
  this->next_ = n;
}

ACE_Stream_Node::ACE_Stream_Node (const ACE_Static_Node *str_ops, 
				  const ACE_Parse_Node *str_mods)
  : node_ (str_ops), 
    mods_ (str_mods), 
    ACE_Parse_Node (str_ops->name ())
{
  ACE_TRACE ("ACE_Stream_Node::ACE_Stream_Node");
}


ACE_Stream_Node::~ACE_Stream_Node (void)
{
  ACE_TRACE ("ACE_Stream_Node::~ACE_Stream_Node");
  delete (ACE_Static_Node *) this->node_;
  delete (ACE_Parse_Node *) this->mods_;
}

ACE_Parse_Node::ACE_Parse_Node (void)
  : next_ (0)
{
  ACE_TRACE ("ACE_Parse_Node::ACE_Parse_Node");
}


ACE_Parse_Node::ACE_Parse_Node (const char *nm)
  : name_ (nm), 
    next_ (0)
{
  ACE_TRACE ("ACE_Parse_Node::ACE_Parse_Node");
}

void
ACE_Parse_Node::print (void) const
{
  ACE_TRACE ("ACE_Parse_Node::print");
  ACE_DEBUG ((LM_DEBUG, "svc = %s\n", this->name ()));
  if (this->next_)
    this->next_->print ();
}


ACE_Parse_Node::~ACE_Parse_Node (void)
{
  ACE_TRACE ("ACE_Parse_Node::~ACE_Parse_Node");
  delete this->next_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Suspend_Node)

void
ACE_Suspend_Node::dump (void) const
{
  ACE_TRACE ("ACE_Suspend_Node::dump");
}

ACE_Suspend_Node::ACE_Suspend_Node (const char *name)
  : ACE_Parse_Node (name)
{
  ACE_TRACE ("ACE_Suspend_Node::ACE_Suspend_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Resume_Node)

void
ACE_Resume_Node::dump (void) const
{
  ACE_TRACE ("ACE_Resume_Node::dump");
}

ACE_Resume_Node::ACE_Resume_Node (const char *name)
  : ACE_Parse_Node (name)
{
  ACE_TRACE ("ACE_Resume_Node::ACE_Resume_Node");
}

void
ACE_Suspend_Node::apply (void)
{
  ACE_TRACE ("ACE_Suspend_Node::apply");
  if (ACE_Service_Config::suspend (this->name ()) == -1)
    ace_yyerrno++;

  ACE_DEBUG ((LM_DEBUG, "did suspend on %s, error = %d\n", 
	      this->name (), ace_yyerrno));
}

void
ACE_Resume_Node::apply (void)
{
  ACE_TRACE ("ACE_Resume_Node::apply");
  if (ACE_Service_Config::resume (this->name ()) == -1)
    ace_yyerrno++;

  ACE_DEBUG ((LM_DEBUG, "did resume on %s, error = %d\n", 
	     this->name (), ace_yyerrno));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Remove_Node)

void
ACE_Remove_Node::dump (void) const
{
  ACE_TRACE ("ACE_Remove_Node::dump");
}

ACE_Remove_Node::ACE_Remove_Node (const char *name)
  : ACE_Parse_Node (name)
{
  ACE_TRACE ("ACE_Remove_Node::ACE_Remove_Node");
}

void
ACE_Remove_Node::apply (void)
{
  ACE_TRACE ("ACE_Remove_Node::apply");
  if (ACE_Service_Config::remove (this->name ()) == -1)
    ace_yyerrno++;

  ACE_DEBUG ((LM_DEBUG, "did remove on %s, error = %d\n", 
	     this->name (), ace_yyerrno));

}


ACE_Dynamic_Node::ACE_Dynamic_Node (const ACE_Service_Record *sr,
				    char *parms)
  : record_ (sr), 
    ACE_Static_Node (sr->name (), parms)
{
  ACE_TRACE ("ACE_Dynamic_Node::ACE_Dynamic_Node");
}

const ACE_Service_Record *
ACE_Dynamic_Node::record (void) const
{
  ACE_TRACE ("ACE_Dynamic_Node::record");
  return this->record_;
}

void
ACE_Dynamic_Node::apply (void)
{
  ACE_TRACE ("ACE_Dynamic_Node::apply");
  if (ACE_Service_Config::initialize (this->record (), 
				      this->parameters ()) == -1)
    ace_yyerrno++;
	
  ACE_DEBUG ((LM_DEBUG, "did dynamic on %s, error = %d\n", 
	     this->name (), ace_yyerrno));

}

ACE_ALLOC_HOOK_DEFINE(ACE_Dynamic_Node)

void
ACE_Dynamic_Node::dump (void) const
{
  ACE_TRACE ("ACE_Dynamic_Node::dump");
}

ACE_Dynamic_Node::~ACE_Dynamic_Node (void)
{
  ACE_TRACE ("ACE_Dynamic_Node::~ACE_Dynamic_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Static_Node)

void
ACE_Static_Node::dump (void) const
{
  ACE_TRACE ("ACE_Static_Node::dump");
}

ACE_Static_Node::ACE_Static_Node (const char *nm, 
				  char *params)
  : ACE_Parse_Node (nm), 
    parameters_ (params)
{
  ACE_TRACE ("ACE_Static_Node::ACE_Static_Node");
}

const ACE_Service_Record *
ACE_Static_Node::record (void) const
{
  ACE_TRACE ("ACE_Static_Node::record");
  ACE_Service_Record *sr;

  if (ACE_Service_Config::svc_rep ()->find (this->name (), 
					 (const ACE_Service_Record **) &sr) == -1)
    return 0;
  else
    return sr;
}

char *
ACE_Static_Node::parameters (void) const
{
  ACE_TRACE ("ACE_Static_Node::parameters");
  return this->parameters_;
}

void
ACE_Static_Node::apply (void)
{
  ACE_TRACE ("ACE_Static_Node::apply");
  if (ACE_Service_Config::initialize (this->name (), 
				      this->parameters ()) == -1)
    ace_yyerrno++;
	
  ACE_DEBUG ((LM_DEBUG, "did static on %s, error = %d\n", 
	     this->name (), ace_yyerrno));
}


ACE_Static_Node::~ACE_Static_Node (void)
{
  ACE_TRACE ("ACE_Static_Node::~ACE_Static_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Location_Node)

void
ACE_Location_Node::dump (void) const
{
  ACE_TRACE ("ACE_Location_Node::dump");
}

ACE_Location_Node::ACE_Location_Node (void)
  : handle_ (0), 
    symbol_ (0), 
    pathname_ (0)
{
  ACE_TRACE ("ACE_Location_Node::ACE_Location_Node");
}

ACE_Location_Node::~ACE_Location_Node (void)
{
  ACE_TRACE ("ACE_Location_Node::~ACE_Location_Node");
}

const char *
ACE_Location_Node::pathname (void) const
{
  ACE_TRACE ("ACE_Location_Node::pathname");
  return this->pathname_;
}

void 
ACE_Location_Node::pathname (const char *p)
{
  ACE_TRACE ("ACE_Location_Node::pathname");
  this->pathname_ = p;
}

void 
ACE_Location_Node::handle (const ACE_SHLIB_HANDLE h)
{
  ACE_TRACE ("ACE_Location_Node::handle");
  this->handle_ = h;
}

const ACE_SHLIB_HANDLE
ACE_Location_Node::handle (void) const
{
  ACE_TRACE ("ACE_Location_Node::handle");
  return this->handle_;
}

void 
ACE_Location_Node::set_symbol (const void *s)
{
  ACE_TRACE ("ACE_Location_Node::set_symbol");
  this->symbol_ = s;
}

int
ACE_Location_Node::dispose (void) const
{
  ACE_TRACE ("ACE_Location_Node::dispose");
  return this->must_delete_;
}

const ACE_SHLIB_HANDLE
ACE_Location_Node::open_handle (void)
{
  ACE_TRACE ("ACE_Location_Node::open_handle");

  char dl_pathname[MAXPATHLEN];

  // Transform the pathname into the appropriate dynamic link library
  // by searching the ACE_LD_SEARCH_PATH.
  ACE::ldfind (this->pathname (), 
	       dl_pathname, 
	       (sizeof dl_pathname / sizeof (char)));
  
  this->handle (ACE_OS::dlopen (dl_pathname));

  if (this->handle () == 0)
    {
      ace_yyerrno++;

      ACE_ERROR ((LM_ERROR, "dlopen failed for %s\n", dl_pathname));

      char *errmsg = ACE_OS::dlerror ();

      if (errmsg != 0)
	ACE_ERROR_RETURN ((LM_ERROR, ": %s\n", errmsg), 0);
      else
	ACE_ERROR_RETURN ((LM_ERROR, "\n"), 0);
    }
  else
    return this->handle ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Object_Node)

void
ACE_Object_Node::dump (void) const
{
  ACE_TRACE ("ACE_Object_Node::dump");
}

ACE_Object_Node::ACE_Object_Node (const char *path, 
				  const char *obj_name)
  : object_name_ (obj_name)
{
  ACE_TRACE ("ACE_Object_Node::ACE_Object_Node");
  this->pathname (path);
  this->must_delete_ = 0;
}

const void *
ACE_Object_Node::symbol (void)
{
  ACE_TRACE ("ACE_Object_Node::symbol");
  if (this->open_handle () != 0)
    {
      this->symbol_ = (const void *) 
	ACE_OS::dlsym ((ACE_SHLIB_HANDLE) this->handle (),
		       (char *) this->object_name_);

      if (this->symbol_ == 0)
	{
	  ace_yyerrno++;

	  ACE_ERROR ((LM_ERROR, 
		      "dlsym failed for object %s\n", 
		      this->object_name_));

	  char *errmsg = ACE_OS::dlerror ();
	  
	  if (errmsg != 0)
	    ACE_ERROR_RETURN ((LM_ERROR, ": %s\n", errmsg), 0);
	  else
	    ACE_ERROR_RETURN ((LM_ERROR, "\n"), 0);
	}
    }
  return this->symbol_;
}

ACE_Object_Node::~ACE_Object_Node (void)
{
  ACE_TRACE ("ACE_Object_Node::~ACE_Object_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Function_Node)

void
ACE_Function_Node::dump (void) const
{
  ACE_TRACE ("ACE_Function_Node::dump");
}

ACE_Function_Node::ACE_Function_Node (const char *path, 
				      const char *func_name)
  : function_name_ (func_name)
{
  ACE_TRACE ("ACE_Function_Node::ACE_Function_Node");
  this->pathname (path);
  this->must_delete_ = 1;
}

const void *
ACE_Function_Node::symbol (void)
{
  ACE_TRACE ("ACE_Function_Node::symbol");
  if (this->open_handle () != 0)
    {
      const void *(*func) (void) = 0;
      this->symbol_ = 0;

      // Locate the factory function <function_name> in the shared
      // object.

      func = (const void *(*)(void)) 
	ACE_OS::dlsym ((ACE_SHLIB_HANDLE) this->handle (), 
		       (ACE_DL_TYPE) this->function_name_);

      if (func == 0)
	{
	  ace_yyerrno++;

	  if (this->symbol_ == 0)
	    {
	      ace_yyerrno++;

	      ACE_ERROR ((LM_ERROR, "dlsym failed for function %s\n", 
			  this->function_name_));

	      char *errmsg = ACE_OS::dlerror ();
	      
	      if (errmsg != 0)
		ACE_ERROR_RETURN ((LM_ERROR, ": %s\n", errmsg), 0);
	      else
		ACE_ERROR_RETURN ((LM_ERROR, "\n"), 0);
	    }
	}
      // Invoke the factory function and record it's return value.
      this->symbol_ = (*func) ();

      if (this->symbol_ == 0)
	{
	  ace_yyerrno++;
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", this->function_name_), 0);
	}
    }
  return this->symbol_;
}

ACE_Function_Node::~ACE_Function_Node (void)
{
  ACE_TRACE ("ACE_Function_Node::~ACE_Function_Node");
}

ACE_ALLOC_HOOK_DEFINE(ACE_Dummy_Node)

void
ACE_Dummy_Node::dump (void) const
{
  ACE_TRACE ("ACE_Dummy_Node::dump");
}

ACE_Dummy_Node::ACE_Dummy_Node (const ACE_Static_Node *static_node, 
				const ACE_Parse_Node *str_mods)
  : node_ (static_node), 
    mods_ (str_mods), 
    ACE_Parse_Node (static_node->name ())
{
  ACE_TRACE ("ACE_Dummy_Node::ACE_Dummy_Node");
}

void
ACE_Dummy_Node::apply (void)
{
  ACE_TRACE ("ACE_Dummy_Node::apply");
  ACE_DEBUG ((LM_DEBUG, "did operations on stream %s, error = %d\n", 
	     this->name (), ace_yyerrno));
}

ACE_Dummy_Node::~ACE_Dummy_Node (void)
{
  ACE_TRACE ("ACE_Dummy_Node::~ACE_Dummy_Node");
  delete (ACE_Static_Node *) this->node_;
  delete (ACE_Parse_Node *) this->mods_;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
