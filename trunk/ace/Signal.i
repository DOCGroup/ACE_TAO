/* -*- C++ -*- */
// $Id$

// Signal.i

ACE_INLINE
ACE_Sig_Set::ACE_Sig_Set (sigset_t *ss)
  : sigset_ (*ss) // Structure assignment
{
  ACE_TRACE ("ACE_Sig_Set::ACE_Sig_Set");
}

ACE_INLINE
ACE_Sig_Set::ACE_Sig_Set (int fill)
{
  ACE_TRACE ("ACE_Sig_Set::ACE_Sig_Set");
  if (fill)
    ACE_OS::sigfillset (&this->sigset_);
  else
    ACE_OS::sigemptyset (&this->sigset_);
}

ACE_INLINE
ACE_Sig_Set::~ACE_Sig_Set (void)
{
  ACE_TRACE ("ACE_Sig_Set::~ACE_Sig_Set");
  ACE_OS::sigemptyset (&this->sigset_);
}

ACE_INLINE int
ACE_Sig_Set::empty_set (void)
{
  ACE_TRACE ("ACE_Sig_Set::empty_set");
  return ACE_OS::sigemptyset (&this->sigset_);  
}

ACE_INLINE int
ACE_Sig_Set::fill_set (void)
{
  ACE_TRACE ("ACE_Sig_Set::fill_set");
  return ACE_OS::sigfillset (&this->sigset_);
}

ACE_INLINE int
ACE_Sig_Set::sig_add (int signo)
{
  ACE_TRACE ("ACE_Sig_Set::sig_add");
  return ACE_OS::sigaddset (&this->sigset_, signo);
}

ACE_INLINE int
ACE_Sig_Set::sig_del (int signo)
{
  ACE_TRACE ("ACE_Sig_Set::sig_del");
  return ACE_OS::sigdelset (&this->sigset_, signo);
}

ACE_INLINE int
ACE_Sig_Set::is_member (int signo) const
{
  ACE_TRACE ("ACE_Sig_Set::is_member");
  return ACE_OS::sigismember ((sigset_t *) &this->sigset_, signo);
}

ACE_INLINE
ACE_Sig_Set::operator sigset_t *(void)
{
  ACE_TRACE ("ACE_Sig_Set::operator sigset_t *");
  return &this->sigset_;
}

ACE_INLINE int
ACE_Sig_Action::flags (void)
{
  ACE_TRACE ("ACE_Sig_Action::flags");
  return this->sa_.sa_flags;
}

ACE_INLINE void
ACE_Sig_Action::flags (int flags)
{
  ACE_TRACE ("ACE_Sig_Action::flags");
  this->sa_.sa_flags = flags;
}

ACE_INLINE sigset_t *
ACE_Sig_Action::mask (void)
{
  ACE_TRACE ("ACE_Sig_Action::mask");
  return &this->sa_.sa_mask;
}

ACE_INLINE void
ACE_Sig_Action::mask (sigset_t *ss)
{
  ACE_TRACE ("ACE_Sig_Action::mask");
  this->sa_.sa_mask = *ss; // Structure assignment
}

ACE_INLINE ACE_SignalHandler
ACE_Sig_Action::handler (void)
{
  ACE_TRACE ("ACE_Sig_Action::handler");
  return ACE_SignalHandler (this->sa_.sa_handler);
}

ACE_INLINE void
ACE_Sig_Action::handler (ACE_SignalHandler handler)
{
  ACE_TRACE ("ACE_Sig_Action::handler");
#if !defined(ACE_HAS_TANDEM_SIGNALS)                                  
  this->sa_.sa_handler = ACE_SignalHandlerV (handler);
#else                                                                 
  this->sa_.sa_handler = (void (*)()) ACE_SignalHandlerV (handler);
#endif /* !ACE_HAS_TANDEM_SIGNALS */
}

ACE_INLINE void
ACE_Sig_Action::set (struct sigaction *sa)
{
  ACE_TRACE ("ACE_Sig_Action::set");
  this->sa_ = *sa; // Structure assignment.
}

ACE_INLINE struct sigaction *
ACE_Sig_Action::get (void)
{
  ACE_TRACE ("ACE_Sig_Action::get");
  return &this->sa_;
}

ACE_INLINE 
ACE_Sig_Action::operator ACE_SIGACTION * ()
{
  ACE_TRACE ("ACE_Sig_Action::operator ACE_SIGACTION *");
  return &this->sa_;
}

ACE_INLINE
ACE_Sig_Action::ACE_Sig_Action (const ACE_Sig_Action &s)
{
  ACE_TRACE ("ACE_Sig_Action::ACE_Sig_Action");
  *this = s; // structure copy.
}

ACE_INLINE int
ACE_Sig_Action::register_action (int signum, ACE_Sig_Action *oaction)
{
  ACE_TRACE ("ACE_Sig_Action::register_action");
  struct sigaction *sa = oaction == 0 ? 0 : oaction->get ();
  
  return ACE_OS::sigaction (signum, &this->sa_, sa);
}

ACE_INLINE int
ACE_Sig_Action::retrieve_action (int signum)
{
  ACE_TRACE ("ACE_Sig_Action::retrieve_action");
  return ACE_OS::sigaction (signum, 0, &this->sa_);
}

ACE_INLINE int
ACE_Sig_Action::restore_action (int signum, ACE_Sig_Action &oaction)
{
  ACE_TRACE ("ACE_Sig_Action::restore_action");
  this->sa_ = *oaction.get (); // Structure assignment
  return ACE_OS::sigaction (signum, &this->sa_, 0);
}

// Block out the signal MASK until the destructor is called. 

ACE_INLINE 
ACE_Sig_Guard::ACE_Sig_Guard (ACE_Sig_Set *mask)
{
  //ACE_TRACE ("ACE_Sig_Guard::ACE_Sig_Guard");

  // If MASK is 0 then block all signals!
  if (mask == 0)
    {
      ACE_Sig_Set smask (1);

#if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
      ACE_OS::sigprocmask (SIG_BLOCK, (sigset_t *) smask, (sigset_t *)
			   this->omask_); 
#else
      ACE_OS::thr_sigsetmask (SIG_BLOCK, (sigset_t *) smask, (sigset_t *)
			      this->omask_);
#endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    }
  else
#if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
    ACE_OS::sigprocmask (SIG_BLOCK, (sigset_t *) *mask, (sigset_t *)
			 this->omask_); 
#else
  ACE_OS::thr_sigsetmask (SIG_BLOCK, (sigset_t *) *mask, (sigset_t *)
			  this->omask_);
#endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
}

// Restore the signal mask.

ACE_INLINE 
ACE_Sig_Guard::~ACE_Sig_Guard (void)
{
  //ACE_TRACE ("ACE_Sig_Guard::~ACE_Sig_Guard");
#if (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  ACE_OS::sigprocmask (SIG_SETMASK, (sigset_t *) this->omask_, 0);
#else
  ACE_OS::thr_sigsetmask (SIG_SETMASK, (sigset_t *) this->omask_, 0);
#endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
}

ACE_INLINE int
ACE_Sig_Handler::in_range (int signum)
{
  ACE_TRACE ("ACE_Sig_Handler::in_range");
  return signum > 0 && signum < NSIG;
}
