// $Id$

template <class T> ACE_INLINE
ACE_Env_Value<T>::operator T (void)
{
  return value_;
}

template <class T> ACE_INLINE
ACE_Env_Value<T>::ACE_Env_Value (void)
  : varname_ (0)
{
}

template <class T> ACE_INLINE
ACE_Env_Value<T>::ACE_Env_Value (const char *varname,
                                 const T &defval)
  : varname_ (varname),
    value_(defval)
{
  this->fetch_value ();
}

template <class T> ACE_INLINE void
ACE_Env_Value<T>::open (const char *varname,
                        const T &defval)
{
  varname_ = varname;
  value_ = defval;
  this->fetch_value ();
}

template <class T> ACE_INLINE void
ACE_Env_Value<T>::fetch_value (void)
{
  const char *env = ACE_OS::getenv (varname_);

  if (env != 0)
    ACE_Convert (env, value_);
}

template <class T> ACE_INLINE const char*
ACE_Env_Value<T>::varname (void) const
{
  return varname_;
}

template <class T> ACE_INLINE
ACE_Env_Value<T>::~ACE_Env_Value (void)
{
}
