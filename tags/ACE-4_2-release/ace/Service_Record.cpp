// Service_Record.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Service_Record.h"

#if !defined (__ACE_INLINE__)
#include "ace/Service_Record.i"
#endif /* __ACE_INLINE__ */

ACE_Service_Object_Type::ACE_Service_Object_Type (ACE_Service_Object *so, 
						  const char *s_name, 
						  unsigned int f)
  : ACE_Service_Type ((const void *) so, s_name, f)
{
  ACE_TRACE ("ACE_Service_Object_Type::ACE_Service_Object_Type");
}

int
ACE_Service_Object_Type::init (int argc, char *argv[]) const
{
  ACE_TRACE ("ACE_Service_Object_Type::init");
  const void *obj = this->object ();
  ACE_Service_Object *so = (ACE_Service_Object *) obj;

  if (so == 0)
    return -1;
  else
    return so->init (argc, argv);
}

ACE_ALLOC_HOOK_DEFINE(ACE_Module_Type)

void
ACE_Module_Type::dump (void) const
{
  ACE_TRACE ("ACE_Module_Type::dump");
}

ACE_Module_Type::ACE_Module_Type (MT_Module *m, 
				  const char *m_name, 
				  u_int f)
  : ACE_Service_Type ((const void *) m, m_name, f)
{
  ACE_TRACE ("ACE_Module_Type::ACE_Module_Type");
}

int
ACE_Module_Type::init (int argc, char *argv[]) const
{
  ACE_TRACE ("ACE_Module_Type::init");
  const void *obj = this->object ();
  MT_Module *mod = (MT_Module *) obj;
  MT_Task *reader = mod->reader ();
  MT_Task *writer = mod->writer ();

  if (reader->init (argc, argv) == -1
      || writer->init (argc, argv) == -1)
    return -1;
  else
    return 0;
}

int
ACE_Module_Type::suspend (void) const
{
  ACE_TRACE ("ACE_Module_Type::suspend");
  const void *obj = this->object ();
  MT_Module *mod = (MT_Module *) obj;
  MT_Task *reader = mod->reader ();
  MT_Task *writer = mod->writer ();

  if (reader->suspend () == -1
      || writer->suspend () == -1)
    return -1;
  else
    return 0;
}

int
ACE_Module_Type::resume (void) const
{
  ACE_TRACE ("ACE_Module_Type::resume");
  const void *obj = this->object ();
  MT_Module *mod = (MT_Module *) obj;
  MT_Task *reader = mod->reader ();
  MT_Task *writer = mod->writer ();

  if (reader->resume () == -1
      || writer->resume () == -1)
    return -1;
  else
    return 0;
}

// Note, these operations are somewhat too familiar with the
// implementation of ACE_Module and ACE_Module::close...

int
ACE_Module_Type::fini (void) const
{
  ACE_TRACE ("ACE_Module_Type::fini");

  const void *obj = this->object ();
  MT_Module *mod = (MT_Module *) obj;
  MT_Task *reader = mod->reader ();
  MT_Task *writer = mod->writer ();

  if (reader != 0)
    reader->fini ();

  if (writer != 0)
    writer->fini ();

  // Close the module and delete the memory.
  mod->close (MT_Module::M_DELETE);
  return ACE_Service_Type::fini ();     
}

int
ACE_Module_Type::info (char **str, size_t len) const
{
  ACE_TRACE ("ACE_Module_Type::info");
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%s\t %s", this->name (), "# ACE_Module\n");

  if (*str == 0 && (*str = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*str, buf, len);
  return ACE_OS::strlen (buf);
}

void
ACE_Module_Type::link (ACE_Module_Type *n)
{
  ACE_TRACE ("ACE_Module_Type::link");
  this->next_ = n;
}

ACE_Module_Type *
ACE_Module_Type::link (void) const
{
  ACE_TRACE ("ACE_Module_Type::link");
  return this->next_;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Stream_Type)

void
ACE_Stream_Type::dump (void) const
{
  ACE_TRACE ("ACE_Stream_Type::dump");
}

int
ACE_Stream_Type::init (int, char *[]) const
{
  ACE_TRACE ("ACE_Stream_Type::init");
  return 0;
}

int
ACE_Stream_Type::suspend (void) const
{
  ACE_TRACE ("ACE_Stream_Type::suspend");
  for (ACE_Module_Type *m = this->head_; m != 0; m = m->link ())
    m->suspend ();

  return 0;
}

int
ACE_Stream_Type::resume (void) const
{
  ACE_TRACE ("ACE_Stream_Type::resume");
  for (ACE_Module_Type *m = this->head_; m != 0; m = m->link ())
    m->resume ();

  return 0;
}

ACE_Stream_Type::ACE_Stream_Type (MT_Stream *s, 
				  const char *s_name, 
				  unsigned int f)
  : ACE_Service_Type ((const void *) s, s_name, f), 
    head_ (0)
{
  ACE_TRACE ("ACE_Stream_Type::ACE_Stream_Type");
}

int
ACE_Stream_Type::info (char **str, size_t len) const
{
  ACE_TRACE ("ACE_Stream_Type::info");
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "%s\t %s", this->name (), "# STREAM\n");

  if (*str == 0 && (*str = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*str, buf, len);
  return ACE_OS::strlen (buf);
}

int
ACE_Stream_Type::fini (void) const
{
  ACE_TRACE ("ACE_Stream_Type::fini");
  const void *obj = this->object ();
  MT_Stream *str = (MT_Stream *) obj;

  for (ACE_Module_Type *m = this->head_; m != 0; )
    {
      ACE_Module_Type *t = m->link ();

      // Final arg is an indication to *not* delete the Module.
      str->remove (m->name (), MT_Module::M_DELETE_NONE);

      // Finalize the Module (this may delete it, but we don't really
      // care since we don't access it again).
      m->fini ();
      m = t;
    }

  str->close ();
  return ACE_Service_Type::fini ();
}

// Locate and remove <mod_name> from the ACE_Stream.

int
ACE_Stream_Type::remove (ACE_Module_Type *mod)
{
  ACE_TRACE ("ACE_Stream_Type::remove");
  ACE_Module_Type *prev = 0;
  const void *obj = this->object ();
  MT_Stream *str = (MT_Stream *) obj;
  int result = 0;

  for (ACE_Module_Type *m = this->head_; m != 0; )
    {
      // We need to do this first so we don't bomb out if we delete m!
      ACE_Module_Type *next = m->link ();

      if (m == mod)
	{
	  if (prev == 0)
	    this->head_ = next;
	  else	  
	    prev->link (next);

	  // Final arg is an indication to *not* delete the Module.
	  if (str->remove (m->name (), MT_Module::M_DELETE_NONE) == -1)
	    result = -1; 

	  // This call may end up deleting m, which is ok since we
	  // don't access it again!
	  m->fini (); 
	}
      else
	prev = m;

      m = next;
    }

  return result;
}

int
ACE_Stream_Type::push (ACE_Module_Type *new_module)
{
  ACE_TRACE ("ACE_Stream_Type::push");
  const void *obj = this->object ();
  MT_Stream *str = (MT_Stream *) obj;

  new_module->link (this->head_);
  this->head_ = new_module;
  obj = new_module->object ();
  return str->push ((MT_Module *) obj);
}

ACE_Module_Type *
ACE_Stream_Type::find (const char *mod_name) const
{
  ACE_TRACE ("ACE_Stream_Type::find");

  for (ACE_Module_Type *m = this->head_; 
       m != 0; 
       m = m->link ())
    if (ACE_OS::strcmp (m->name (), mod_name) == 0)
      return m;

  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Service_Record)

void
ACE_Service_Record::dump (void) const
{
  ACE_TRACE ("ACE_Service_Record::dump");
}

ACE_Service_Record::ACE_Service_Record (const char *n, 
					ACE_Service_Type *t, 
					const void *h, 
					int active)
  : name_ (0),
    type_ (t), 
    handle_ (h), 
    active_ (active)
{
  ACE_TRACE ("ACE_Service_Record::ACE_Service_Record");
  this->name (n);
}

ACE_Service_Record::~ACE_Service_Record (void)
{
  ACE_TRACE ("ACE_Service_Record::~ACE_Service_Record");
  this->type_->fini ();
  if (this->handle_ != 0)
    ACE_OS::dlclose ((void *) this->handle_);
  delete [] (char *) this->name_;
}

void 
ACE_Service_Record::suspend (void) const
{
  ACE_TRACE ("ACE_Service_Record::suspend");
  ((ACE_Service_Record *) this)->active_ = 0;
  this->type_->suspend ();
}

void 
ACE_Service_Record::resume (void) const
{
  ACE_TRACE ("ACE_Service_Record::resume");
  ((ACE_Service_Record *) this)->active_ = 1;
  this->type_->resume ();
}

int
ACE_Service_Object_Type::fini (void) const
{
  ACE_TRACE ("ACE_Service_Object_Type::fini");
  const void *obj = this->object ();
  ACE_Service_Object *so = (ACE_Service_Object *) obj;
  so->fini ();
  return ACE_Service_Type::fini ();
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Message_Queue<ACE_SYNCH>;
template class ACE_Module<ACE_SYNCH>;
template class ACE_Stream<ACE_SYNCH>;
template class ACE_Stream_Head<ACE_SYNCH>;
template class ACE_Stream_Tail<ACE_SYNCH>;
template class ACE_Task<ACE_SYNCH>;
template class ACE_Thru_Task<ACE_SYNCH>;

// Even with threads, these ACE_NULL_SYNCH specializations are necessary.
#if defined (ACE_HAS_THREADS)
  template class ACE_Message_Queue<ACE_NULL_SYNCH>;
  template class ACE_Module<ACE_NULL_SYNCH>;
  template class ACE_Task<ACE_NULL_SYNCH>;
  template class ACE_Thru_Task<ACE_NULL_SYNCH>;
#endif /* ACE_HAS_THREADS */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
