ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::bind (
  KEY const &key,
  VALUE const &value)
{
  return ACE_HCMM_BASE::bind (key, value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::rebind (
  KEY const &key,
  VALUE const &value)
{
  return ACE_HCMM_BASE::rebind (key, value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::rebind (
  KEY const &key,
  VALUE const &value,
  VALUE &old_value)
{
  return ACE_HCMM_BASE::rebind (key, value, old_value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::rebind (
  KEY const &key,
  VALUE const &value,
  KEY &old_key,
  VALUE &old_value)
{
  return ACE_HCMM_BASE::rebind (key,
                                value,
                                old_key,
                                old_value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::trybind (
  KEY const &key,
  VALUE &value)
{
  return ACE_HCMM_BASE::trybind (key, value);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::unbind (KEY const &key)
{
  return ACE_HCMM_BASE::unbind (key);
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
ACE_INLINE int
ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES>::unbind (KEY const &key,
                                         VALUE &value)
{
  return ACE_HCMM_BASE::unbind (key, value);
}

ACE_END_VERSIONED_NAMESPACE_DECL
