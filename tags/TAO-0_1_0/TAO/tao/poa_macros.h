// Convenient macro for testing for deadlock, as well as for detecting
// when mutexes fail.
#define TAO_POA_WRITE_GUARD(MUTEX,OBJ,LOCK,ENV) \
  do { \
    ACE_Write_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) \
      { \
        CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO); \
        ENV.exception (exception); \
        return; \
      } \
  } \
  while (0);
#define TAO_POA_READ_GUARD(MUTEX,OBJ,LOCK,ENV) \
  do { \
    ACE_Read_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) \
      { \
        CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO); \
        ENV.exception (exception); \
        return; \
      } \
  } \
  while (0);
#define TAO_POA_WRITE_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN,ENV) \
  do { \
    ACE_Write_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) \
      { \
        CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO); \
        ENV.exception (exception); \
        return RETURN; \
      } \
  } \
  while (0);
#define TAO_POA_READ_GUARD_RETURN(MUTEX,OBJ,LOCK,RETURN,ENV) \
  do { \
    ACE_Read_Guard<MUTEX> OBJ (LOCK); \
    if (OBJ.locked () == 0) \
      { \
        CORBA::Exception *exception = new CORBA::OBJ_ADAPTER (CORBA::COMPLETED_NO); \
        ENV.exception (exception); \
        return RETURN; \
      } \
  } \
  while (0);

