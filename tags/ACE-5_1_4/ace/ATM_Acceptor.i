// $Id$

// ATM_Acceptor.i

ACE_INLINE
ACE_ATM_Acceptor::ACE_ATM_Acceptor (const ACE_Addr &remote_sap,
                                    int backlog,
                                    ACE_ATM_Params params)
{
  ACE_TRACE ("ACE_ATM_Acceptor::ACE_ATM_Acceptor");
  if (open (remote_sap,
            backlog,
            params) < 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("ACE_ATM_Acceptor::ACE_ATM_Acceptor")));
}

ACE_INLINE
int
ACE_ATM_Acceptor::close (void)
{
#if defined (ACE_HAS_FORE_ATM_XTI) || defined (ACE_HAS_FORE_ATM_WS2) || defined (ACE_HAS_LINUX_ATM)
  return (acceptor_.close());
#else
  return 0;
#endif // ACE_HAS_FORE_ATM_XTI || ACE_HAS_FORE_ATM_WS2 || ACE_HAS_LINUX_ATM
}


