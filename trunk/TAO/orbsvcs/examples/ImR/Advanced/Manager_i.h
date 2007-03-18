// -*- C++ -*-
//
// $Id$
#ifndef MANAGERI_H_
#define MANAGERI_H_

#include "ManagerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class  Manager_i
  : public virtual POA_Manager
{
public:
  Manager_i (long retryCount);

  virtual ~Manager_i (void);

  virtual CORBA::Long registerServer ();

  virtual void startRetry (CORBA::Long count);

  virtual CORBA::Long endRetry ();

private:

  long server_instance_;
  long retry_count_;
  long retry_attempt_;
};


#endif /* MANAGERI_H_  */
