/* -*- C++ -*- */
// @(#)Semaphore_Test.h	1.1	10/18/96

#include "ace/Log_Msg.h"

#define SHMSZ 27
#define SEM_KEY_1 2345
#define SEM_KEY_2 4321
#define SHM_KEY 5678

#define ESD(MSG,LABEL) do { ACE_ERROR ((LM_ERROR, MSG)); goto LABEL; } while (0)
