// -*- C++ -*-
//
// $Id$

/* Handles timeouts. */

#ifndef _HANDLE_TIMEOUT_H
#define _HANDLE_TIMEOUT_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"
#include "ace/Log_Msg.h"
#include "ace/svc_export.h"

class ACE_Svc_Export Handle_Timeout : public ACE_Service_Object
{
public:
  Handle_Timeout (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  virtual ACE_HANDLE get_handle (void) const;
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
