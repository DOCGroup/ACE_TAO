// $Id$

#include "ace/OS.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Singleton.h"

#include "SS_Service_Handler.h"
#include "SS_State_READ.h"
#include "SS_State_PARSE.h"
#include "SS_State_WRITE.h"
#include "SS_State_ERROR.h"
#include "SS_State_DONE.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<TeraSS_Service_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Singleton<TeraSS_State_READ, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TeraSS_State_PARSE, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TeraSS_State_WRITE, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TeraSS_State_ERROR, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<TeraSS_State_DONE, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

