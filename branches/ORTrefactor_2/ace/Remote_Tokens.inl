/* -*- C++ -*- */
// $Id$

#if defined (ACE_HAS_TOKENS_LIBRARY)

ACE_INLINE
ACE_Remote_Mutex::ACE_Remote_Mutex (void)
{
  ACE_TRACE ("ACE_Remote_Mutex::ACE_Remote_Mutex");
}

ACE_INLINE
ACE_Remote_Mutex::ACE_Remote_Mutex (const ACE_TCHAR *token_name,
				    int ignore_deadlock,
				    int debug)
{
  ACE_TRACE ("ACE_Remote_Mutex::ACE_Remote_Mutex");
  this->open (token_name, ignore_deadlock, debug);
}

ACE_INLINE ACE_Token_Proxy *
ACE_Remote_Mutex::clone (void) const
{
  ACE_Token_Proxy *temp;
  ACE_NEW_RETURN (temp,
                  ACE_Remote_Mutex (this->name (),
			                              ignore_deadlock_,
			                              debug_),
                  0);
  return temp;
}

ACE_INLINE ACE_Tokens *
ACE_Remote_Mutex::create_token (const ACE_TCHAR *name)
{
  ACE_Tokens *temp;
  ACE_NEW_RETURN (temp,
                  ACE_Mutex_Token (name),
                  0);
  return temp;
}

// ************************************************************

ACE_INLINE
ACE_Remote_RLock::ACE_Remote_RLock (const ACE_TCHAR *token_name,
				    int ignore_deadlock,
				    int debug)
{
  ACE_TRACE ("ACE_Remote_RLock::ACE_Remote_RLock");
  this->open (token_name, ignore_deadlock, debug);
}

ACE_INLINE ACE_Tokens *
ACE_Remote_RLock::create_token (const ACE_TCHAR *name)
{
  ACE_Tokens *temp = 0;
  ACE_NEW_RETURN (temp,
                  ACE_RW_Token (name),
                  0);
  return temp;
}

ACE_INLINE int
ACE_Remote_RLock::type (void) const
{
  return ACE_RW_Token::READER;
}

ACE_INLINE ACE_Token_Proxy *
ACE_Remote_RLock::clone (void) const
{
  ACE_Token_Proxy *temp = 0;
  ACE_NEW_RETURN (temp,
                  ACE_Remote_RLock (this->name (),
			            ignore_deadlock_,
			            debug_),
                  0);
  return temp;
}

// ************************************************************

ACE_INLINE
ACE_Remote_WLock::ACE_Remote_WLock (const ACE_TCHAR *token_name,
				    int ignore_deadlock,
				    int debug)
{
  ACE_TRACE ("ACE_Remote_WLock::ACE_Remote_WLock");
  this->open (token_name, ignore_deadlock, debug);
}


ACE_INLINE ACE_Tokens *
ACE_Remote_WLock::create_token (const ACE_TCHAR *name)
{
  ACE_Tokens *temp = 0;
  ACE_NEW_RETURN (temp,
                  ACE_RW_Token (name),
                  0);
  return temp;
}

ACE_INLINE int
ACE_Remote_WLock::type (void) const
{
  return ACE_RW_Token::WRITER;
}

ACE_INLINE ACE_Token_Proxy *
ACE_Remote_WLock::clone (void) const
{
  ACE_Token_Proxy *temp = 0;
  ACE_NEW_RETURN (temp,
                  ACE_Remote_WLock (this->name (),
			                              ignore_deadlock_,
			                              debug_),
                  0);
  return temp;
}

#endif /* ACE_HAS_TOKENS_LIBRARY */
