// $Id$

template <class T> ACE_INLINE
ACE_Env_Value<T>::operator const T (void) const
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

template <class T> ACE_INLINE
ACE_Env_Value<T>::~ACE_Env_Value (void)
{
}

// Default calls a CTOR on type T of the form 'T::T(const char*)', but
// users can feel free to create their own specialized conversion
// functions if necessary, as shown below.  Note that for 'char*' the
// default is used because a simple cast will be performed and no
// conversion will be necessary.

template <class T> ACE_INLINE void
ACE_Convert (const char *s, T &t)
{
  t = T (s);
}

ACE_INLINE void
ACE_Convert (const char *s, char *&v)
{
  v = (char *) s;
}

ACE_INLINE void
ACE_Convert (const char *s, short &si)
{
  si = ACE_OS::strtol (s, 0, 10);
}

ACE_INLINE void
ACE_Convert (const char *s, long &l)
{
  l = ACE_OS::strtol (s, 0, 10);
}

ACE_INLINE void
ACE_Convert (const char *s, int &i)
{
  i = ACE_OS::strtol (s, 0, 10);
}

ACE_INLINE void
ACE_Convert (const char *s, u_long &ul)
{
  ul = ACE_OS::strtoul (s, 0, 10);
}

ACE_INLINE void
ACE_Convert (const char *s, double &d)
{
  d = ACE_OS::strtod (s, 0);
}
