Tue Jul 31 18:47:47 UTC 2007  ace\Auto_Functor.h  <ace\Auto_Functor.inl>

        * ace/ATM_Acceptor.inl:
        * ace/ATM_Stream.cpp:
        * ace/Asynch_Pseudo_Task.cpp:
        * ace/Auto_Functor.h:
        * ace/Auto_Functor.inl:
        * ace/Auto_Functor.cpp:
        * ace/Codeset_Registry.inl:
        * ace/Global_Macros.h:
        * ace/Hash_Cache_Map_Manager_T.cpp:
        * ace/IOStream_T.inl:
        * ace/IOStream_T.cpp:
        * ace/Lib_Find.cpp:
        * ace/Log_Msg.cpp:
        * ace/Message_Queue_Vx.cpp:
        * ace/Metrics_Cache_T.cpp:
        * ace/Notification_Queue.cpp:
        * ace/OS_NS_Thread.inl:
        * ace/OS_NS_unistd.inl:
        * ace/Object_Manager_Base.cpp:
        * ace/Process_Mutex.cpp:
        * ace/RW_Process_Mutex.cpp:
        * ace/SOCK_Connector.cpp:
        * ace/SOCK_Dgram_Bcast.cpp:
        * ace/SOCK_SEQPACK_Connector.cpp:

        * ace/SSL/SSL_Asynch_BIO.cpp:

        * ace/Service_Config.cpp:
        * ace/Service_Gestalt.cpp:
        * ace/Service_Manager.cpp:
        * ace/Sock_Connect.cpp:
        * ace/Svc_Conf_Lexer.cpp:
        * ace/Svc_Conf_y.cpp:
        * ace/TLI_Acceptor.cpp:
        * ace/Throughput_Stats.cpp:
        * ace/Timeprobe_T.cpp:
        * ace/XTI_ATM_Mcast.cpp:
        * ace/ace_wchar.inl:

        * bin/fuzz.pl:

        * contrib/FaCE/CE_Screen_Output.cpp:

        * contrib/minizip/crypt.h:
        * contrib/minizip/unzip.c:

        * examples/Reactor/TP_Reactor/client.cpp:

// $Id$

#ifndef ACE_AUTO_FUNCTOR_CPP
#define ACE_AUTO_FUNCTOR_CPP

#include "ace/Auto_Functor.h"

#if !defined(__ACE_INLINE__)
# include "ace/Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename X, typename Functor>
ACE_Utils::Auto_Functor<X,Functor>::~Auto_Functor()
{
  reset(0);
}

template<typename X, typename Functor> void
ACE_Utils::Auto_Functor<X,Functor>::reset(X * p)
{
  if(p_ != 0)
  {
    f_(p_);
  }
  p_ = p;
}

template<typename X, typename Functor>void
ACE_Utils::Auto_Functor<X,Functor>::reset(X * p, Functor f)
{
  reset(p);
  f_ = f;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /*ACE_AUTO_FUNCTOR_CPP*/
