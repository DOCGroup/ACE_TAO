/* -*- C++ -*- */
// $Id$

// Local_Tokens_T.i

template <class TYPE> ACE_INLINE void 
ACE_Token_Name<TYPE>::operator= (const ACE_Token_Name<TYPE> &rhs)
{
  ACE_TRACE ("ACE_Token_Name<TYPE>::operator=");
  if (&rhs == this)
    return;
  else
    {
      this->type (rhs.type ());
      this->name (rhs.name ());
    }
}

template <class TYPE> ACE_INLINE int
ACE_Token_Name<TYPE>::operator== (const ACE_Token_Name<TYPE> &rhs) const
{
  ACE_TRACE ("ACE_Token_Name<TYPE>::operator==");

  // the name and type must be the same
  return (ACE_OS::strcmp (this->token_name_, rhs.name ()) == 0
	  && rhs.type () == type_);
}

template <class TYPE> ACE_INLINE const char*
ACE_Token_Name<TYPE>::name (void) const
{
  ACE_TRACE ("ACE_Token_Name<TYPE>::name");
  return this->token_name_;
}

template <class TYPE> ACE_INLINE void
ACE_Token_Name<TYPE>::name (const char *new_name)
{
  ACE_TRACE ("ACE_Token_Name<TYPE>::name");

  if (new_name == 0)
    return;

  int n = ACE_OS::strlen (new_name) + 1;

  if (n > ACE_MAXTOKENNAMELEN)
    n = ACE_MAXTOKENNAMELEN;

  ACE_OS::strncpy (this->token_name_, (char *) new_name, n);
}

template <class TYPE> ACE_INLINE TYPE
ACE_Token_Name<TYPE>::type (void) const
{
  ACE_TRACE ("ACE_Token_Name<TYPE>::type");
  return this->type_;
}

template <class TYPE> ACE_INLINE void
ACE_Token_Name<TYPE>::type (TYPE type)
{
  ACE_TRACE ("ACE_Token_Name<TYPE>::type");
  this->type_ = type;
}

