// $Id$

#include "ace/OS.h"
#include "ace/ACE.h"
#include "ace/Synch.h"

#include "ace_Cache_Manager.h"

ACE_RCSID(ace, Cache_Manager, "$Id$")

ACE_String_Hash_Functor::ACE_String_Hash_Functor (const char *s)
  : i_ (0)
{
  this->i_ = ACE::hash_pjw (s);
}

ACE_String_Hash_Functor::operator int (void) const
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Bucket_Item<char const *, ACE_Cache_Object *>;
template class ACE_Hash_Bucket_DLCStack<char const *, ACE_Cache_Object *>;
template class ACE_Hash_Bucket_DLCStack_Iterator<char const *,
                                                 ACE_Cache_Object *>;
template class ACE_Hash_Bucket_Manager<const char *,
                                       ACE_Cache_Object *,
                                       ACE_String_Equal_Functor>;
template class ACE_Cache_Hash<const char *,
                              ACE_String_Hash_Functor,
                              ACE_String_Equal_Functor>;
template class ACE_Cache_Heap_Item<char const *,
                                   ACE_Referenced_Cache_Object_Factory,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>;
template class ACE_Cache_Heap<const char *,
                              ACE_Referenced_Cache_Object_Factory,
                              ACE_String_Hash_Functor,
                              ACE_String_Equal_Functor>;
template class ACE_Cache_Manager<const char *,
                                 ACE_Referenced_Cache_Object_Factory,
                                 ACE_String_Hash_Functor,
                                 ACE_String_Equal_Functor>;
template class ACE_Cache_Heap_Item<char const *,
                                   ACE_Counted_Cache_Object_Factory,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>;
template class ACE_Cache_Heap<const char *,
                              ACE_Counted_Cache_Object_Factory,
                              ACE_String_Hash_Functor,
                              ACE_String_Equal_Functor>;
template class ACE_Cache_Manager<const char *,
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

#pragma instantiate ACE_Hash_Bucket_Item<char const *, ACE_Cache_Object *>
#pragma instantiate ACE_Hash_Bucket_DLCStack<char const *, ACE_Cache_Object *>
#pragma instantiate ACE_Hash_Bucket_DLCStack_Iterator<char const *,
                                                      ACE_Cache_Object *>
#pragma instantiate ACE_Hash_Bucket_Manager<const char *,
                                            ACE_Cache_Object *,
                                            ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Hash<const char *,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap_Item<char const *,
                                        ACE_Referenced_Cache_Object_Factory,
                                        ACE_String_Hash_Functor,
                                        ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap<const char *,
                                   ACE_Referenced_Cache_Object_Factory,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Manager<const char *,
                                      ACE_Referenced_Cache_Object_Factory,
                                      ACE_String_Hash_Functor,
                                      ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap_Item<char const *,
                                        ACE_Counted_Cache_Object_Factory,
                                        ACE_String_Hash_Functor,
                                        ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Heap<const char *,
                                   ACE_Counted_Cache_Object_Factory,
                                   ACE_String_Hash_Functor,
                                   ACE_String_Equal_Functor>
#pragma instantiate ACE_Cache_Manager<const char *,
                                      ACE_Counted_Cache_Object_Factory,
                                      ACE_String_Hash_Functor,
                                      ACE_String_Equal_Functor>
#pragma instantiate ACE_Singleton<ACE_Referenced_Cache_Object_Factory,
                                  ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<ACE_Counted_Cache_Object_Factory,
                                  ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<ACE_String_Referenced_Cache_Manager,
                                  ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Singleton<ACE_String_Counted_Cache_Manager,
                                  ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
