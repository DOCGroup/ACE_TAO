/* -*- C++ -*- */
// $Id$

#define T_1 class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES
#define T_2 KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::bind (const KEY &key,
                                       const VALUE &value)
{
  return ACE_HCMM_BASE::bind (key,
                              value);
}

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::rebind (const KEY &key,
                                         const VALUE &value)
{
  return ACE_HCMM_BASE::rebind (key,
                                value);
}

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::rebind (const KEY &key,
                                         const VALUE &value,
                                         VALUE &old_value)
{
  return ACE_HCMM_BASE::rebind (key,
                                value,
                                old_value);
}

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::rebind (const KEY &key,
                                         const VALUE &value,
                                         KEY &old_key,
                                         VALUE &old_value)
{
  return ACE_HCMM_BASE::rebind (key,
                                value,
                                old_key,
                                old_value);
}

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::trybind (const KEY &key,
                                          VALUE &value)
{
  return ACE_HCMM_BASE::trybind (key,
                                 value);
}

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::unbind (const KEY &key)
{
  return ACE_HCMM_BASE::unbind (key);
}

template <T_1> ACE_INLINE int
ACE_Hash_Cache_Map_Manager<T_2>::unbind (const KEY &key,
                                         VALUE &value)
{
  return ACE_HCMM_BASE::unbind (key,
                                value);
}

#undef T_1
#undef T_2
