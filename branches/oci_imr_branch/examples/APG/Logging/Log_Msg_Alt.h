// $Id$

#ifndef LOG_MSG_ALT_H
#define LOG_MSG_ALT_H

#include "ace/Log_Msg.h"

// Listing 1 code/ch03
#define MY_DEBUG      LM_DEBUG,     ACE_TEXT ("DEBUG%I")
#define MY_INFO       LM_INFO,      ACE_TEXT ("INFO%I")
#define MY_NOTICE     LM_NOTICE,    ACE_TEXT ("NOTICE%I")
#define MY_WARNING    LM_WARNING,   ACE_TEXT ("WARNING%I")
#define MY_ERROR      LM_ERROR,     ACE_TEXT ("ERROR%I")
#define MY_CRITICAL   LM_CRITICAL,  ACE_TEXT ("CRITICAL%I")
#define MY_ALERT      LM_ALERT,     ACE_TEXT ("ALERT%I")
#define MY_EMERGENCY  LM_EMERGENCY, ACE_TEXT ("EMERGENCY%I")
// Listing 1

#endif /* LOG_ALT_H */
