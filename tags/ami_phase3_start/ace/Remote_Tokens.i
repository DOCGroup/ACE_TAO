/* -*- C++ -*- */
// $Id$

// Remote_Tokens.i

ACE_INLINE
ACE_Remote_Mutex::ACE_Remote_Mutex (void)
{
  ACE_TRACE ("ACE_Remote_Mutex::ACE_Remote_Mutex");
}

ACE_INLINE
ACE_Remote_Mutex::ACE_Remote_Mutex (const char *token_name,
				    int ignore_deadlock,
				    int debug)
{
  ACE_TRACE ("ACE_Remote_Mutex::ACE_Remote_Mutex");
  this->open (token_name, ignore_deadlock, debug);
}

ACE_INLINE ACE_Token_Proxy *
ACE_Remote_Mutex::clone (void) const
{
  return new ACE_Remote_Mutex (this->name (),
			       ignore_deadlock_,
			       debug_);
}

ACE_INLINE ACE_Tokens *
ACE_Remote_Mutex::create_token (const char *name)
{
  return new ACE_Mutex_Token (name);
}

// ************************************************************

ACE_INLINE
ACE_Remote_RLock::ACE_Remote_RLock (const char *token_name,
				    int ignore_deadlock,
				    int debug)
{
  ACE_TRACE ("ACE_Remote_RLock::ACE_Remote_RLock");
  this->open (token_name, ignore_deadlock, debug);
}

ACE_INLINE ACE_Tokens *
ACE_Remote_RLock::create_token (const char *name)
{
  return new ACE_RW_Token (name);
}

ACE_INLINE int
ACE_Remote_RLock::type (void) const
{
  return ACE_RW_Token::READER;
}

ACE_INLINE ACE_Token_Proxy *
ACE_Remote_RLock::clone (void) const
{
  return new ACE_Remote_RLock (this->name (),
			       ignore_deadlock_,
			       debug_);
}

// ************************************************************

ACE_INLINE
ACE_Remote_WLock::ACE_Remote_WLock (const char *token_name,
				    int ignore_deadlock,
				    int debug)
{
  ACE_TRACE ("ACE_Remote_WLock::ACE_Remote_WLock");
  this->open (token_name, ignore_deadlock, debug);
}


ACE_INLINE ACE_Tokens *
ACE_Remote_WLock::create_token (const char *name)
{
  return new ACE_RW_Token (name);
}

ACE_INLINE int
ACE_Remote_WLock::type (void) const
{
  return ACE_RW_Token::WRITER;
}

ACE_INLINE ACE_Token_Proxy *
ACE_Remote_WLock::clone (void) const
{
  return new ACE_Remote_WLock (this->name (),
			       ignore_deadlock_,
			       debug_);
}


