// $Id$

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Synch.h"

#include "JAWS/Cache_Manager.h"
#include "JAWS/Cache_List_T.h"

ACE_String_Hash_Functor::ACE_String_Hash_Functor (const char *s)
  : i_ (0)
{
  this->i_ = ACE::hash_pjw (s);
}

ACE_String_Hash_Functor::operator unsigned long (void) const
{
  return this->i_;
}

ACE_String_Equal_Functor::ACE_String_Equal_Functor (const char *s1,
                                                    const char *s2)
  : i_ (0)
{
  this->i_ = ACE_OS::strcmp (s1, s2);
}

ACE_String_Equal_Functor::operator int (void) const
{
  return this->i_ == 0;
}

ACE_Strdup_String::ACE_Strdup_String (void)
  : c_ (0),
    s_ (0)
{
}

ACE_Strdup_String::ACE_Strdup_String (const char *s)
  : c_ (0),
    s_ (0)
{
  this->c_ = new int (1);
  this->s_ = ACE_OS::strdup (s);
}

ACE_Strdup_String::ACE_Strdup_String (const ACE_Strdup_String &s)
  : c_ (s.c_),
    s_ (s.s_)
{
  ++*(this->c_);
}

ACE_Strdup_String::~ACE_Strdup_String (void)
{
  if (this->c_ && --*(this->c_) == 0)
    {
      if (this->s_)
        ACE_OS::free (this->s_);
      delete this->c_;
    }
  this->s_ = 0;
  this->c_ = 0;
}

ACE_Strdup_String::operator const char * (void) const
{
  return this->s_;
}

void
ACE_Strdup_String::operator = (const char *s)
{
  if (this->c_ && --*(this->c_) == 0)
    {
      if (this->s_)
        ACE_OS::free (this->s_);
      delete this->c_;
    }
  this->c_ = new int (1);
  this->s_ = ACE_OS::strdup (s);
}

void
ACE_Strdup_String::operator = (const ACE_Strdup_String &s)
{
  if (this->c_ && --*(this->c_) == 0)
    {
      if (this->s_)
        ACE_OS::free (this->s_);
      delete this->c_;
    }
  this->c_ = s.c_;
  this->s_ = s.s_;
  ++*(this->c_);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Bucket_Item<ACE_Strdup_String, ACE_Cache_Object *>;
template class ACE_Hash_Bucket_DLCStack<ACE_Strdup_String, ACE_Cache_Object *>;
template class ACE_Hash_Bucket_DLCStack_Iterator<ACE_Strdup_String,
                                                 ACE_Cache_Object *>;
template class ACE_Hash_Bucket_Manager<ACE_Strdup_String,
                                       ACE_Cache_Object *,
                                       ACE_String_Equal_Functor>;
template class ACE_Cache_Hash<ACE_Strdup_String,
                              ACE_String_Hash_Functor,
                              ACE_String_Equal_Functor>;
template class ACE_Cache_List_Item<ACE_Strdup_String,
                                   ACE_Referenced_Cache_Object_Factory,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>;
template class ACE_Cache_List<ACE_Strdup_String,
                              ACE_Referenced_Cache_Object_Factory,
                              ACE_String_Hash_Functor,
                              ACE_String_Equal_Functor>;
template class ACE_Cache_Manager<ACE_Strdup_String,
                                 ACE_Referenced_Cache_Object_Factory,
                                 ACE_String_Hash_Functor,
                                 ACE_String_Equal_Functor>;
template class ACE_Cache_List_Item<ACE_Strdup_String,
                                   ACE_Counted_Cache_Object_Factory,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>;
template class ACE_Cache_List<ACE_Strdup_String,
                              ACE_Counted_Cache_Object_Factory,
                              ACE_String_Hash_Functor,
                              ACE_String_Equal_Functor>;
template class ACE_Cache_Manager<ACE_Strdup_String,
                                 ACE_Counted_Cache_Object_Factory,
                                 ACE_String_Hash_Functor,
                                 ACE_String_Equal_Functor>;
template class ACE_Singleton<ACE_Referenced_Cache_Object_Factory,
                             ACE_SYNCH_MUTEX>;
template class ACE_Singleton<ACE_Counted_Cache_Object_Factory,
                             ACE_SYNCH_MUTEX>;
template class ACE_Singleton<ACE_String_Referenced_Cache_Manager,
                             ACE_SYNCH_MUTEX>;
template class ACE_Singleton<ACE_String_Counted_Cache_Manager,
                             ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Bucket_Item<ACE_Strdup_String, ACE_Cache_Object *>
#pragma instantiate ACE_Hash_Bucket_DLCStack<ACE_Strdup_String, \
                                             ACE_Cache_Object *>
#pragma instantiate ACE_Hash_Bucket_DLCStack_Iterator<ACE_Strdup_String, \
                                                      ACE_Cache_Object *>
#pragma instantiate ACE_Hash_Bucket_Manager<ACE_Strdup_String, \
                                            ACE_Cache_Object *, \
                                            ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Hash<ACE_Strdup_String, \
                                   ACE_String_Hash_Functor, \
                                   ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap_Item<ACE_Strdup_String, \
                                        ACE_Referenced_Cache_Object_Factory, \
                                        ACE_String_Hash_Functor, \
                                        ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap<ACE_Strdup_String, \
                                   ACE_Referenced_Cache_Object_Factory, \
                                   ACE_String_Hash_Functor, \
                                   ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Manager<ACE_Strdup_String, \
                                      ACE_Referenced_Cache_Object_Factory, \
                                      ACE_String_Hash_Functor, \
                                      ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap_Item<ACE_Strdup_String, \
                                        ACE_Counted_Cache_Object_Factory, \
                                        ACE_String_Hash_Functor, \
                                        ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap<ACE_Strdup_String, \
                                   ACE_Counted_Cache_Object_Factory, \
                                   ACE_String_Hash_Functor, \
                                   ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Manager<ACE_Strdup_String, \
                                      ACE_Counted_Cache_Object_Factory, \
                                      ACE_String_Hash_Functor, \
                                      ACE_String_Equal_Functor>
#pragma instantiate ACE_Singleton<ACE_Referenced_Cache_Object_Factory, \
                                  ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<ACE_Counted_Cache_Object_Factory, \
                                  ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<ACE_String_Referenced_Cache_Manager, \
                                  ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<ACE_String_Counted_Cache_Manager, \
                                  ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
