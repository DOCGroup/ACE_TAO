// -*- C++ -*-
#include "Framework_Component_DLL.h"
#include "ace/Service_Config.h"
#include "ace/Service_Object.h"
#include "ace/Framework_Component_T.h"
#include "ace/OS.h"

ACE_RCSID (tests,
           Framework_Component_DLL,
           "$Id$")

ACE_DLL_UNLOAD_POLICY (Framework_Component_DLL, ACE_DLL_UNLOAD_POLICY_LAZY)

Simple_Service::Simple_Service (void)
{
  FRAMEWORK_COMPONENT_DLL_TRACE ("Simple_Service::Simple_Service");
}

Simple_Service::~Simple_Service (void)
{
  FRAMEWORK_COMPONENT_DLL_TRACE ("Simple_Service::~Simple_Service");
}

const ACE_TCHAR *
Simple_Service::name (void)
{
  FRAMEWORK_COMPONENT_DLL_TRACE ("Simple_Service::dll_name");
  return ACE_TEXT ("Simple_Service");
}

/***************************************************************************/

/// This is just a simple server that can be loaded via the ACE
/// Service Configuration framework and uses the singleton that
/// also lives in this library.
template <int which>
class Server_T : public ACE_Service_Object
{
public:
  int init (int, ACE_TCHAR *[])
  {
    FRAMEWORK_COMPONENT_DLL_TRACE ("Server_T::init");

    // Initialize the singleton
    FWCT_DLL_Singleton_Adapter_T <Simple_Service> *ss = SS_SINGLETON::instance ();

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Server_T::init() dll_name: %s\n"),
                ss->dll_name ()));
    return 0;
  }

  int fini (void)
  {
    FRAMEWORK_COMPONENT_DLL_TRACE ("Server_T::fini");
    return 0;
  }
};

typedef Server_T <1> Server_1;
FRAMEWORK_COMPONENT_DLL_SINGLETON_DECLARATION(Server_T<1>;)
ACE_FACTORY_DEFINE (Framework_Component_DLL, Server_1)

typedef Server_T <2> Server_2;
FRAMEWORK_COMPONENT_DLL_SINGLETON_DECLARATION(Server_T<2>;)
ACE_FACTORY_DEFINE (Framework_Component_DLL, Server_2)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Server_T<1>;
template class Server_T<2>;
template class FWCT_DLL_Singleton_Adapter_T<Simple_Service>;
template class ACE_DLL_Singleton_T< FWCT_DLL_Singleton_Adapter_T <Simple_Service>,
                                    ACE_SYNCH_MUTEX >;
template class ACE_Framework_Component_T<ACE_DLL_Singleton_T<FWCT_DLL_Singleton_Adapter_T<Simple_Service>,
                                         ACE_SYNCH_MUTEX> >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Server_T<1>
#pragma instantiate Server_T<2>
#pragma instantiate FWCT_DLL_Singleton_Adapter_T<Simple_Service>
#pragma instantiate ACE_DLL_Singleton_T< FWCT_DLL_Singleton_Adapter_T <Simple_Service>,
                                         ACE_SYNCH_MUTEX >
#pragma instantiate ACE_Framework_Component_T<ACE_DLL_Singleton_T<FWCT_DLL_Singleton_Adapter_T<Simple_Service>,
                                               ACE_SYNCH_MUTEX> >;
#elif defined (__GNUC__) && (defined (_AIX) || defined (__hpux))

template ACE_DLL_Singleton_T<FWCT_DLL_Singleton_Adapter_T <Simple_Service>,
                             ACE_SYNCH_MUTEX> *
  ACE_DLL_Singleton_T<FWCT_DLL_Singleton_Adapter_T <Simple_Service>,
                      ACE_SYNCH_MUTEX>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
