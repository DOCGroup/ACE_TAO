/* -*- C++ -*- */
// $Id$

/* Handles timeouts. */

#if !defined (_HANDLE_TIMEOUT_H)
#define _HANDLE_TIMEOUT_H

#include "ace/Service_Config.h"
#include "ace/Service_Types.h"

class Handle_Timeout : public ACE_Service_Object
{
public:
  Handle_Timeout (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  virtual int get_handle (void) const;
  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg);

  int count;
};

extern ACE_Service_Object_Type t1;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_Timeout.i"
#else
#define ACE_INLINE 
#endif /* __ACE_INLINE__ */

#endif /* _HANDLE_TIMEOUT_H */
