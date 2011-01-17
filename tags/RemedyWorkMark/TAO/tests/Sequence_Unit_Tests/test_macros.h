// $Id$

#include "ace/Log_Msg.h"

#define FAIL_RETURN_IF(CONDITION) \
      if (CONDITION) \
      { \
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
        return 1; \
      }

#define FAIL_RETURN_IF_NOT(CONDITION, X) \
      if (!(CONDITION)) \
      { \
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
        return 1; \
      }
#define CHECK_EQUAL(X, Y) \
      if ((X) != (Y)) \
      { \
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
        return 1; \
      }
#define CHECK(X) \
      if (!(X)) \
      { \
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
        return 1; \
      }

#define CHECK_THROW(X, Y) \
    try { \
           X; \
        } \
    catch (Y const &) \
        { \
        } \
    catch (...) \
        { \
          return 1; \
        } \


