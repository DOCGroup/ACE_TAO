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

ACE_RCSID(ace, Parse_Node, "$Id$")

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

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did stream on %s, error = %d\n"),
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
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
  : ACE_Parse_Node (str_ops == 0 ? ACE_static_cast (char *,
                                                    ASYS_TEXT ("<unknown>"))
                                 : ACE_static_cast (char *,
                                                    str_ops->name ())),
#else
  : ACE_Parse_Node (str_ops == 0 ? "<unknown>" : str_ops->name ()),
#endif /* defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
    node_ (str_ops),
    mods_ (str_mods)
{
  ACE_TRACE ("ACE_Stream_Node::ACE_Stream_Node");
}


ACE_Stream_Node::~ACE_Stream_Node (void)
{
  ACE_TRACE ("ACE_Stream_Node::~ACE_Stream_Node");
  ACE_Static_Node *n = ACE_const_cast (ACE_Static_Node *,
                                       this->node_);
  delete n;
  ACE_Parse_Node *m = ACE_const_cast (ACE_Parse_Node *,
                                      this->mods_);
  delete m;
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

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("svc = %s\n"),
              this->name ()));

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

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did suspend on %s, error = %d\n"),
                this->name (),
                ace_yyerrno));
}

void
ACE_Resume_Node::apply (void)
{
  ACE_TRACE ("ACE_Resume_Node::apply");
  if (ACE_Service_Config::resume (this->name ()) == -1)
    ace_yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did resume on %s, error = %d\n"),
                this->name (),
                ace_yyerrno));
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

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did remove on %s, error = %d\n"),
                this->name (),
                ace_yyerrno));
}

ACE_Dynamic_Node::ACE_Dynamic_Node (const ACE_Service_Type *sr,
                                    char *parms)
  : ACE_Static_Node (sr->chname (), parms),
    record_ (sr)
{
  ACE_TRACE ("ACE_Dynamic_Node::ACE_Dynamic_Node");
}

const ACE_Service_Type *
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

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did dynamic on %s, error = %d\n"),
                this->name (),
                ace_yyerrno));
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

const ACE_Service_Type *
ACE_Static_Node::record (void) const
{
  ACE_TRACE ("ACE_Static_Node::record");
  ACE_Service_Type *sr;

  if (ACE_Service_Repository::instance()->find
      (ASYS_WIDE_STRING (this->name ()),
       (const ACE_Service_Type **) &sr) == -1)
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

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did static on %s, error = %d\n"),
                this->name (),
                ace_yyerrno));
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
  : pathname_ (0),
    handle_ (0),
    symbol_ (0)
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

ACE_SHLIB_HANDLE
ACE_Location_Node::handle (void) const
{
  ACE_TRACE ("ACE_Location_Node::handle");
  return this->handle_;
}

void
ACE_Location_Node::set_symbol (void *s)
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

ACE_SHLIB_HANDLE
ACE_Location_Node::open_handle (void)
{
  ACE_TRACE ("ACE_Location_Node::open_handle");

  ASYS_TCHAR dl_pathname[MAXPATHLEN + 1];

  // Transform the pathname into the appropriate dynamic link library
  // by searching the ACE_LD_SEARCH_PATH.
  int result = ACE::ldfind (ASYS_WIDE_STRING (this->pathname ()),
                            dl_pathname,
                            (sizeof dl_pathname / sizeof (ASYS_TCHAR)));

  // Check for errors
  if (result != 0)
    return 0;

  // Set the handle
  this->handle (ACE_OS::dlopen (dl_pathname));

  if (this->handle () == 0)
    {
      ace_yyerrno++;

      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("dlopen failed for %s"),
                  dl_pathname));

      ASYS_TCHAR *errmsg = ACE_OS::dlerror ();

      if (errmsg != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT (": %s\n"),
                           errmsg),
                          0);
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("\n")),
                          0);
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

void *
ACE_Object_Node::symbol (ACE_Service_Object_Exterminator *)
{
  ACE_TRACE ("ACE_Object_Node::symbol");
  if (this->open_handle () != 0)
    {
      ASYS_TCHAR *object_name = ACE_const_cast (ASYS_TCHAR *, this->object_name_);

      this->symbol_ = (void *)
        ACE_OS::dlsym ((ACE_SHLIB_HANDLE) this->handle (),
                       object_name);

      if (this->symbol_ == 0)
        {
          ace_yyerrno++;

          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("dlsym failed for object %s\n"),
                      object_name));

          ASYS_TCHAR *errmsg = ACE_OS::dlerror ();

          if (errmsg != 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT (": %s\n"),
                               errmsg),
                              0);
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("\n")),
                              0);
        }
      return this->symbol_;
    }

  return 0;
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

void *
ACE_Function_Node::symbol (ACE_Service_Object_Exterminator *gobbler)
{
  ACE_TRACE ("ACE_Function_Node::symbol");
  if (this->open_handle () != 0)
    {
      void *(*func) (ACE_Service_Object_Exterminator *) = 0;
      this->symbol_ = 0;

      // Locate the factory function <function_name> in the shared
      // object.

      char *function_name = ACE_const_cast (char *,
                                            this->function_name_);

      func = (void *(*)(ACE_Service_Object_Exterminator *))
          ACE_OS::dlsym ((ACE_SHLIB_HANDLE) this->handle (),
                       ASYS_WIDE_STRING (function_name));

      if (func == 0)
        {
          ace_yyerrno++;

          if (this->symbol_ == 0)
            {
              ace_yyerrno++;

              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("dlsym failed for function %s\n"),
                          ASYS_WIDE_STRING (function_name)));

              ASYS_TCHAR *errmsg = ACE_OS::dlerror ();

              if (errmsg != 0)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ASYS_TEXT (": %s\n"),
                                   errmsg),
                                  0);
              else
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ASYS_TEXT ("\n")),
                                  0);
            }
        }
      // Invoke the factory function and record it's return value.
      this->symbol_ = (*func) (gobbler);

      if (this->symbol_ == 0)
        {
          ace_yyerrno++;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("%p\n"),
                             ASYS_WIDE_STRING (this->function_name_)),
                            0);
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
  : ACE_Parse_Node (static_node->name ()),
    node_ (static_node),
    mods_ (str_mods)
{
  ACE_TRACE ("ACE_Dummy_Node::ACE_Dummy_Node");
}

void
ACE_Dummy_Node::apply (void)
{
  ACE_TRACE ("ACE_Dummy_Node::apply");

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("did operations on stream %s, error = %d\n"),
                this->name (),
                ace_yyerrno));
}

ACE_Dummy_Node::~ACE_Dummy_Node (void)
{
  ACE_TRACE ("ACE_Dummy_Node::~ACE_Dummy_Node");
  ACE_Static_Node *n = ACE_const_cast (ACE_Static_Node *,
                                       this->node_);
  delete n;
  ACE_Parse_Node *m = ACE_const_cast (ACE_Parse_Node *,
                                      this->mods_);
  delete m;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Static_Function_Node)

void
ACE_Static_Function_Node::dump (void) const
{
  ACE_TRACE ("ACE_Static_Function_Node::dump");
}

ACE_Static_Function_Node::ACE_Static_Function_Node (const char *func_name)
  : function_name_ (func_name)
{
  ACE_TRACE ("ACE_Static_Function_Node::ACE_Static_Function_Node");
  this->must_delete_ = 1;
}

void *
ACE_Static_Function_Node::symbol (ACE_Service_Object_Exterminator *gobbler)
{
  ACE_TRACE ("ACE_Static_Function_Node::symbol");

  void *(*func)(ACE_Service_Object_Exterminator *) = 0;
  this->symbol_ = 0;

  // Locate the factory function <function_name> in the statically
  // linked svcs.

  ACE_Static_Svc_Descriptor **ssdp = 0;
  ACE_STATIC_SVCS &svcs = *ACE_Service_Config::static_svcs ();
  char *function_name = ACE_const_cast (char *, this->function_name_);

  for (ACE_STATIC_SVCS_ITERATOR iter (svcs);
       iter.next (ssdp) != 0;
       iter.advance ())
    {
      ACE_Static_Svc_Descriptor *ssd = *ssdp;
      if (ACE_OS::strcmp (ssd->name_,
                          ASYS_WIDE_STRING (function_name)) == 0)
        func = (void *(*)(ACE_Service_Object_Exterminator*)) ssd->alloc_;
    }

  if (func == 0)
    {
      ace_yyerrno++;

      if (this->symbol_ == 0)
        {
          ace_yyerrno++;

          ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("no static service registered for function %s\n"),
                             ASYS_WIDE_STRING (function_name)),
                             0);
        }
    }

  // Invoke the factory function and record it's return value.
  this->symbol_ = (*func) (gobbler);

  if (this->symbol_ == 0)
    {
      ace_yyerrno++;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("%p\n"),
                         ACE_WIDE_STRING (this->function_name_)),
                        0);
    }

  return this->symbol_;
}

ACE_Static_Function_Node::~ACE_Static_Function_Node (void)
{
  ACE_TRACE ("ACE_Static_Function_Node::~ACE_Static_Function_Node");
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
