ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class T> ACE_INLINE
ACE_Env_Value<T>::operator T () const
{
  return value_;
}

template <class T> ACE_INLINE
ACE_Env_Value<T>::ACE_Env_Value (ACE_TCHAR const *varname,
                                 T const &defval)
  : varname_ (varname),
    value_(defval)
{
  this->fetch_value ();
}

template <class T> ACE_INLINE void
ACE_Env_Value<T>::open (ACE_TCHAR const *varname,
                        T const &defval)
{
  this->varname_ = varname;
  this->value_ = defval;
  this->fetch_value ();
}

template <class T> ACE_INLINE void
ACE_Env_Value<T>::fetch_value ()
{
#if defined (ACE_WIN32)
  ACE_TCHAR const *env = ACE_OS::getenv (this->varname_);
  if (env != nullptr)
    ACE_Convert (env, value_);
#else
  char *nenv = ACE_OS::getenv (ACE_TEXT_ALWAYS_CHAR (this->varname_));
  if (nenv != nullptr)
    ACE_Convert (ACE_TEXT_CHAR_TO_TCHAR (nenv), this->value_);
#endif
}

template <class T> ACE_INLINE ACE_TCHAR const *
ACE_Env_Value<T>::varname () const
{
  return this->varname_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
