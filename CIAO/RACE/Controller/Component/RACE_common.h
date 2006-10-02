#ifndef RACE_COMMON_H
#define RACE_COMMON_H

#include "ace/Log_Msg.h"
#include "ciao/CIAO_common.h"

#define RACE_NORMAL 4

#define  RACE_VERBOSE 9

#define RACE_MIN 0

#define RACE_DEBUG(x,y...)                     \
  if (CIAO::debug_level () > x)                 \
    ACE_DEBUG ((LM_DEBUG, "RACE %N:%l: " y));

#define RACE_ERROR(x...)                          \
  ACE_ERROR ((LM_ERROR, "RACE %N:%l: ", x));


#endif /* RACE_COMMON_H */
