//$Id$
#include "XML_Helper.h"

// This file is needed only for proper instantiation of
// XML_Helper_Singleton in a dll.

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<CIAO::Config_Handlers::XML_Helper, ACE_Null_Mutex> *
ACE_Singleton<CIAO::Config_Handlers::XML_Helper, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
