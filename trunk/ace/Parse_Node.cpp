#include "ace/Parse_Node.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

#if !defined (__ACE_INLINE__)
#include "ace/Parse_Node.i"
#endif /* ____ */

#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Service_Types.h"
#include "ace/Task.h"
#include "ace/DLL.h"

ACE_RCSID (ace,
           Parse_Node,
           "$Id$")


ACE_ALLOC_HOOK_DEFINE (ACE_Stream_Node)

// Provide the class hierarchy that defines the parse tree of Service
// Nodes.


void
ACE_Stream_Node::dump (void) const
{
  ACE_TRACE ("ACE_Stream_Node::dump");
}

void
ACE_Stream_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Stream_Node::apply");

  if (ACE_Service_Config::initialize (this->node_->record (),
                                      this->node_->parameters ()) == -1)
    yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did stream on %s, error = %d\n"),
                this->node_->name (),
                yyerrno));
}

ACE_ALLOC_HOOK_DEFINE (ACE_Parse_Node)

void
ACE_Parse_Node::dump (void) const
{
  ACE_TRACE ("ACE_Parse_Node::dump");
}

const ACE_TCHAR *
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
  : ACE_Parse_Node (str_ops == 0 ? ACE_static_cast (ACE_TCHAR *,
                                                    ACE_LIB_TEXT ("<unknown>"))
                                 : ACE_static_cast (ACE_TCHAR *,
                                                    str_ops->name ())),
#else
  : ACE_Parse_Node ((str_ops == 0 ? ACE_LIB_TEXT ("<unknown>") : str_ops->name ())),
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
  : name_ (0),
    next_ (0)
{
  ACE_TRACE ("ACE_Parse_Node::ACE_Parse_Node");
}


ACE_Parse_Node::ACE_Parse_Node (const ACE_TCHAR *nm)
  : name_ (ACE::strnew (nm)),
    next_ (0)
{
  ACE_TRACE ("ACE_Parse_Node::ACE_Parse_Node");
}

void
ACE_Parse_Node::print (void) const
{
  ACE_TRACE ("ACE_Parse_Node::print");

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("svc = %s\n"),
              this->name ()));

  if (this->next_)
    this->next_->print ();
}


ACE_Parse_Node::~ACE_Parse_Node (void)
{
  ACE_TRACE ("ACE_Parse_Node::~ACE_Parse_Node");
  delete[] ACE_const_cast (ACE_TCHAR*, this->name_);
  delete this->next_;
}

ACE_ALLOC_HOOK_DEFINE (ACE_Suspend_Node)

void
ACE_Suspend_Node::dump (void) const
{
  ACE_TRACE ("ACE_Suspend_Node::dump");
}

ACE_Suspend_Node::ACE_Suspend_Node (const ACE_TCHAR *name)
  : ACE_Parse_Node (name)
{
  ACE_TRACE ("ACE_Suspend_Node::ACE_Suspend_Node");
}

ACE_ALLOC_HOOK_DEFINE (ACE_Resume_Node)

void
ACE_Resume_Node::dump (void) const
{
  ACE_TRACE ("ACE_Resume_Node::dump");
}

ACE_Resume_Node::ACE_Resume_Node (const ACE_TCHAR *name)
  : ACE_Parse_Node (name)
{
  ACE_TRACE ("ACE_Resume_Node::ACE_Resume_Node");
}

void
ACE_Suspend_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Suspend_Node::apply");

  if (ACE_Service_Config::suspend (this->name ()) == -1)
    yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did suspend on %s, error = %d\n"),
                this->name (),
                yyerrno));
}

void
ACE_Resume_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Resume_Node::apply");
  if (ACE_Service_Config::resume (this->name ()) == -1)
    yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did resume on %s, error = %d\n"),
                this->name (),
                yyerrno));
}

ACE_ALLOC_HOOK_DEFINE (ACE_Remove_Node)

void
ACE_Remove_Node::dump (void) const
{
  ACE_TRACE ("ACE_Remove_Node::dump");
}

ACE_Remove_Node::ACE_Remove_Node (const ACE_TCHAR *name)
  : ACE_Parse_Node (name)
{
  ACE_TRACE ("ACE_Remove_Node::ACE_Remove_Node");
}

void
ACE_Remove_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Remove_Node::apply");
  if (ACE_Service_Config::remove (this->name ()) == -1)
    yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did remove on %s, error = %d\n"),
                this->name (),
                yyerrno));
}

ACE_Dynamic_Node::ACE_Dynamic_Node (const ACE_Service_Type *sr,
                                    ACE_TCHAR *parms)
  : ACE_Static_Node (sr->name (), parms),
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
ACE_Dynamic_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Dynamic_Node::apply");

  if (ACE_Service_Config::initialize (this->record (),
                                      this->parameters ()) == -1)
    yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did dynamic on %s, error = %d\n"),
                this->name (),
                yyerrno));
}

ACE_ALLOC_HOOK_DEFINE (ACE_Dynamic_Node)

void
ACE_Dynamic_Node::dump (void) const
{
  ACE_TRACE ("ACE_Dynamic_Node::dump");
}

ACE_Dynamic_Node::~ACE_Dynamic_Node (void)
{
  ACE_TRACE ("ACE_Dynamic_Node::~ACE_Dynamic_Node");
}

ACE_ALLOC_HOOK_DEFINE (ACE_Static_Node)

void
ACE_Static_Node::dump (void) const
{
  ACE_TRACE ("ACE_Static_Node::dump");
}

ACE_Static_Node::ACE_Static_Node (const ACE_TCHAR *nm,
                                  ACE_TCHAR *params)
  : ACE_Parse_Node (nm),
    parameters_ (ACE::strnew (params))
{
  ACE_TRACE ("ACE_Static_Node::ACE_Static_Node");
}

const ACE_Service_Type *
ACE_Static_Node::record (void) const
{
  ACE_TRACE ("ACE_Static_Node::record");
  ACE_Service_Type *sr;

  if (ACE_Service_Repository::instance()->find
      (this->name (),
       (const ACE_Service_Type **) &sr) == -1)
    return 0;
  else
    return sr;
}

ACE_TCHAR *
ACE_Static_Node::parameters (void) const
{
  ACE_TRACE ("ACE_Static_Node::parameters");
  return this->parameters_;
}

void
ACE_Static_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Static_Node::apply");
  if (ACE_Service_Config::initialize (this->name (),
                                      this->parameters ()) == -1)
    yyerrno++;

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did static on %s, error = %d\n"),
                this->name (),
                yyerrno));
}

ACE_Static_Node::~ACE_Static_Node (void)
{
  ACE_TRACE ("ACE_Static_Node::~ACE_Static_Node");
  delete[] this->parameters_;
}

ACE_ALLOC_HOOK_DEFINE (ACE_Location_Node)

void
ACE_Location_Node::dump (void) const
{
  ACE_TRACE ("ACE_Location_Node::dump");
}

ACE_Location_Node::ACE_Location_Node (void)
  : pathname_ (0),
    symbol_ (0)
{
  ACE_TRACE ("ACE_Location_Node::ACE_Location_Node");
}

ACE_Location_Node::~ACE_Location_Node (void)
{
  ACE_TRACE ("ACE_Location_Node::~ACE_Location_Node");
}

const ACE_DLL &
ACE_Location_Node::dll (void)
{
  return this->dll_;
}

const ACE_TCHAR *
ACE_Location_Node::pathname (void) const
{
  ACE_TRACE ("ACE_Location_Node::pathname");
  return this->pathname_;
}

void
ACE_Location_Node::pathname (const ACE_TCHAR *p)
{
  ACE_TRACE ("ACE_Location_Node::pathname");
  this->pathname_ = p;
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

int
ACE_Location_Node::open_dll (int & yyerrno)
{
  ACE_TRACE ("ACE_Location_Node::open_dll");

  if (-1 == this->dll_.open (this->pathname ()))
    {
      yyerrno++;

      ACE_TCHAR *errmsg = this->dll_.error ();
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("ACE_DLL::open failed for %s: %s\n"),
                  this->pathname (),
                  errmsg ? errmsg : ACE_LIB_TEXT ("no error reported")));
      return -1;
    }

  return 0;

}

ACE_ALLOC_HOOK_DEFINE (ACE_Object_Node)

void
ACE_Object_Node::dump (void) const
{
  ACE_TRACE ("ACE_Object_Node::dump");
}

ACE_Object_Node::ACE_Object_Node (const ACE_TCHAR *path,
                                  const ACE_TCHAR *obj_name)
  : object_name_ (ACE::strnew (obj_name))
{
  ACE_TRACE ("ACE_Object_Node::ACE_Object_Node");
  this->pathname (ACE::strnew (path));
  this->must_delete_ = 0;
}

void *
ACE_Object_Node::symbol (int & yyerrno,
                         ACE_Service_Object_Exterminator *)
{
  ACE_TRACE ("ACE_Object_Node::symbol");
  if (this->open_dll (yyerrno) == 0)
    {
      ACE_TCHAR *object_name = ACE_const_cast (ACE_TCHAR *, this->object_name_);

      this->symbol_ = this->dll_.symbol (object_name);
      if (this->symbol_ == 0)
        {
          yyerrno++;

          ACE_TCHAR *errmsg = this->dll_.error ();
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("ACE_DLL::symbol failed for object %s: %s\n"),
                      object_name,
                      errmsg ? errmsg : ACE_LIB_TEXT ("no error reported")));
          return 0;
        }

      return this->symbol_;
    }

  return 0;
}

ACE_Object_Node::~ACE_Object_Node (void)
{
  ACE_TRACE ("ACE_Object_Node::~ACE_Object_Node");
  delete[] ACE_const_cast (ACE_TCHAR *, this->object_name_);
}

ACE_ALLOC_HOOK_DEFINE (ACE_Function_Node)

void
ACE_Function_Node::dump (void) const
{
  ACE_TRACE ("ACE_Function_Node::dump");
}

ACE_Function_Node::ACE_Function_Node (const ACE_TCHAR *path,
                                      const ACE_TCHAR *func_name)
  : function_name_ (ACE::strnew (func_name))
{
  ACE_TRACE ("ACE_Function_Node::ACE_Function_Node");
  this->pathname (ACE::strnew (path));
  this->must_delete_ = 1;
}

void *
ACE_Function_Node::symbol (int & yyerrno,
                           ACE_Service_Object_Exterminator *gobbler)
{
  typedef ACE_Service_Object *(*ACE_Service_Factory_Ptr)
    (ACE_Service_Object_Exterminator *);

  ACE_TRACE ("ACE_Function_Node::symbol");
  if (this->open_dll (yyerrno) == 0)
    {
      ACE_Service_Factory_Ptr func = 0;
      this->symbol_ = 0;

      // Locate the factory function <function_name> in the shared
      // object.
      ACE_TCHAR *function_name = ACE_const_cast (ACE_TCHAR *,
                                                 this->function_name_);
      void *func_p = this->dll_.symbol (function_name);
      if (func_p == 0)
        {
          yyerrno++;

          if (this->symbol_ == 0)
            {
              yyerrno++;

              ACE_TCHAR *errmsg = this->dll_.error ();
              ACE_ERROR ((LM_ERROR,
                          ACE_LIB_TEXT ("ACE_DLL::symbol failed for function %s: %s\n"),
                          function_name,
                          errmsg ? errmsg :
                                   ACE_LIB_TEXT ("no error reported")));
              return 0;
            }
        }
      ptrdiff_t temp_p = ACE_reinterpret_cast (ptrdiff_t, func_p);
      func = ACE_reinterpret_cast (ACE_Service_Factory_Ptr, temp_p);
      // Invoke the factory function and record it's return value.
      this->symbol_ = (*func) (gobbler);

      if (this->symbol_ == 0)
        {
          yyerrno++;
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("%p\n"),
                             this->function_name_),
                            0);
        }
    }
  return this->symbol_;
}

ACE_Function_Node::~ACE_Function_Node (void)
{
  ACE_TRACE ("ACE_Function_Node::~ACE_Function_Node");
  delete[] ACE_const_cast (ACE_TCHAR *, function_name_);
  delete[] ACE_const_cast (ACE_TCHAR *, pathname_);
}

ACE_ALLOC_HOOK_DEFINE (ACE_Dummy_Node)

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
ACE_Dummy_Node::apply (int & yyerrno)
{
  ACE_TRACE ("ACE_Dummy_Node::apply");

  if (ACE::debug ())
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("did operations on stream %s, error = %d\n"),
                this->name (),
                yyerrno));
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

ACE_ALLOC_HOOK_DEFINE (ACE_Static_Function_Node)

void
ACE_Static_Function_Node::dump (void) const
{
  ACE_TRACE ("ACE_Static_Function_Node::dump");
}

ACE_Static_Function_Node::ACE_Static_Function_Node (const ACE_TCHAR *func_name)
  : function_name_ (ACE::strnew (func_name))
{
  ACE_TRACE ("ACE_Static_Function_Node::ACE_Static_Function_Node");
  this->must_delete_ = 1;
}

void *
ACE_Static_Function_Node::symbol (int & yyerrno,
                                  ACE_Service_Object_Exterminator *gobbler)
{
  ACE_TRACE ("ACE_Static_Function_Node::symbol");

  void *(*func)(ACE_Service_Object_Exterminator *) = 0;
  this->symbol_ = 0;

  // Locate the factory function <function_name> in the statically
  // linked svcs.

  ACE_Static_Svc_Descriptor **ssdp = 0;
  ACE_STATIC_SVCS &svcs = *ACE_Service_Config::static_svcs ();
  ACE_TCHAR *function_name = ACE_const_cast (ACE_TCHAR *, this->function_name_);

  for (ACE_STATIC_SVCS_ITERATOR iter (svcs);
       iter.next (ssdp) != 0;
       iter.advance ())
    {
      ACE_Static_Svc_Descriptor *ssd = *ssdp;
      if (ACE_OS::strcmp (ssd->name_,
                          function_name) == 0)
        func = (void *(*)(ACE_Service_Object_Exterminator*)) ssd->alloc_;
    }

  if (func == 0)
    {
      yyerrno++;

      if (this->symbol_ == 0)
        {
          yyerrno++;

          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_LIB_TEXT ("no static service registered for function %s\n"),
                             function_name),
                             0);
        }
    }

  // Invoke the factory function and record it's return value.
  this->symbol_ = (*func) (gobbler);

  if (this->symbol_ == 0)
    {
      yyerrno++;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("%p\n"),
                         this->function_name_),
                        0);
    }

  return this->symbol_;
}

ACE_Static_Function_Node::~ACE_Static_Function_Node (void)
{
  ACE_TRACE ("ACE_Static_Function_Node::~ACE_Static_Function_Node");
  delete[] ACE_const_cast (ACE_TCHAR *, this->function_name_);
}

#endif /* ACE_USES_CLASSIC_SVC_CONF == 1 */
