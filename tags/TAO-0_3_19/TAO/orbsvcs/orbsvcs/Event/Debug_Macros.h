// $Id$


#if defined (ACE_ES_NOLOGGING)
#define ACE_ES_DEBUG(X)
#define ACE_ES_DEBUG_ST(X)
#else
#define ACE_ES_DEBUG(X) \
  do { \
      ACE_Log_Msg::instance ()->log X; \
  } while (0)
#define ACE_ES_DEBUG_ST(X) X
#endif /* ACE_ES_NOLOGGING */
