ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T> ACE_INLINE
ACE_Env_Value<T>::operator T ()
{
  return value_;
}

template <class T> ACE_INLINE
ACE_Env_Value<T>::ACE_Env_Value ()
  : varname_ (0)
{
}

template <class T> ACE_INLINE
ACE_Env_Value<T>::ACE_Env_Value (const ACE_TCHAR *varname,
                                 const T &defval)
  : varname_ (varname),
    value_(defval)
{
  this->fetch_value ();
}

template <class T> ACE_INLINE void
ACE_Env_Value<T>::open (const ACE_TCHAR *varname,
                        const T &defval)
{
  this->varname_ = varname;
  this->value_ = defval;
  this->fetch_value ();
}

template <class T> ACE_INLINE void
ACE_Env_Value<T>::fetch_value ()
{
#if defined (ACE_WIN32)
  const ACE_TCHAR *env = ACE_OS::getenv (this->varname_);
  if (env != 0)
    ACE_Convert (env, value_);
#else
  char *nenv = ACE_OS::getenv (ACE_TEXT_ALWAYS_CHAR (this->varname_));
  if (nenv != 0)
    ACE_Convert (ACE_TEXT_CHAR_TO_TCHAR (nenv), this->value_);
#endif
}

template <class T> ACE_INLINE const ACE_TCHAR*
ACE_Env_Value<T>::varname () const
{
  return this->varname_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
