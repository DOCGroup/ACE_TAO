/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_Sig_Set::ACE_Sig_Set (sigset_t *ss)
  // : sigset_ ()
{
  ACE_TRACE ("ACE_Sig_Set::ACE_Sig_Set");

  if (ss == 0)
    ACE_OS::sigfillset (&this->sigset_);
  else
    // Structure assignment.
    this->sigset_ = *ss;
}

ACE_INLINE
ACE_Sig_Set::ACE_Sig_Set (int fill)
  // : sigset_ ()
{
  ACE_TRACE ("ACE_Sig_Set::ACE_Sig_Set");

  if (fill)
    ACE_OS::sigfillset (&this->sigset_);
  else
    ACE_OS::sigemptyset (&this->sigset_);
}

ACE_INLINE
ACE_Sig_Set::ACE_Sig_Set (ACE_Sig_Set *ss)
  // : sigset_ ()
{
  ACE_TRACE ("ACE_Sig_Set::ACE_Sig_Set");

  if (ss == 0)
    ACE_OS::sigfillset (&this->sigset_);
  else
    this->sigset_ = ss->sigset_;
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
  return ACE_OS::sigismember (ACE_const_cast (sigset_t *, &this->sigset_), signo);
}

ACE_INLINE
ACE_Sig_Set::operator sigset_t *(void)
{
  ACE_TRACE ("ACE_Sig_Set::operator sigset_t *");
  return &this->sigset_;
}

ACE_INLINE sigset_t
ACE_Sig_Set::sigset (void) const
{
  ACE_TRACE ("ACE_Sig_Set::sigset");
  return this->sigset_;
}

ACE_INLINE
ACE_Sig_Action::~ACE_Sig_Action (void)
{
  ACE_TRACE ("ACE_Sig_Action::~ACE_Sig_Action");
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
  if (ss != 0)
    this->sa_.sa_mask = *ss; // Structure assignment
}

ACE_INLINE void
ACE_Sig_Action::mask (ACE_Sig_Set &ss)
{
  ACE_TRACE ("ACE_Sig_Action::mask");
  this->sa_.sa_mask = ss.sigset (); // Structure assignment
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

#if 0
ACE_INLINE ACE_SignalHandler
ACE_Sig_Action::sigaction (void)
{
  ACE_TRACE ("ACE_Sig_Action::sigaction");
  return ACE_SignalHandler (this->sa_.sa_sigaction);
}

ACE_INLINE void
ACE_Sig_Action::sigaction (ACE_SignalHandler handler)
{
  ACE_TRACE ("ACE_Sig_Action::sigaction");
  this->sa_.sa_sigaction = (void (*)()) ACE_SignalHandlerV (handler);
}
#endif /* 0 */

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
  // : sa_ ()
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
  : omask_ ()
{
  //ACE_TRACE ("ACE_Sig_Guard::ACE_Sig_Guard");

#if defined (ACE_LACKS_UNIX_SIGNALS)
  ACE_UNUSED_ARG (mask);
#else
  // If MASK is 0 then block all signals!
  if (mask == 0)
    {
#  if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
      ACE_OS::sigprocmask (SIG_BLOCK,
                           ACE_OS_Object_Manager::default_mask (),
                           (sigset_t *) this->omask_);
#  else
      ACE_OS::thr_sigsetmask (SIG_BLOCK,
                              ACE_OS_Object_Manager::default_mask (),
                              (sigset_t *) this->omask_);
#  endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    }
  else
#  if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
    ACE_OS::sigprocmask (SIG_BLOCK,
                         (sigset_t *) *mask,
                         (sigset_t *)
                         this->omask_);
#  else
    ACE_OS::thr_sigsetmask (SIG_BLOCK,
                            (sigset_t *) *mask,
                            (sigset_t *)
                            this->omask_);
#  endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
#endif /* ACE_LACKS_UNIX_SIGNALS */
}

// Restore the signal mask.

ACE_INLINE
ACE_Sig_Guard::~ACE_Sig_Guard (void)
{
  //ACE_TRACE ("ACE_Sig_Guard::~ACE_Sig_Guard");
#if !defined (ACE_LACKS_UNIX_SIGNALS)
#if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  ACE_OS::sigprocmask (SIG_SETMASK,
                       (sigset_t *) this->omask_,
                       0);
#else
  ACE_OS::thr_sigsetmask (SIG_SETMASK,
                          (sigset_t *) this->omask_,
                          0);
#endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
#endif /* !ACE_LACKS_UNIX_SIGNALS */
}

#if defined (ACE_HAS_WINCE)
ACE_INLINE
ACE_Sig_Handler::ACE_Sig_Handler (void)
{
}

ACE_INLINE
ACE_Sig_Handler::~ACE_Sig_Handler (void)
{
}
#endif /* ACE_HAS_WINCE */

ACE_INLINE int
ACE_Sig_Handler::in_range (int signum)
{
  ACE_TRACE ("ACE_Sig_Handler::in_range");
  return signum > 0 && signum < ACE_NSIG;
}

ACE_INLINE
ACE_Sig_Adapter::~ACE_Sig_Adapter (void)
{
}
