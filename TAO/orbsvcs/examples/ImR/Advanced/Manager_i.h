// -*- C++ -*-
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
  explicit Manager_i (long retry_count);

  ~Manager_i () override = default;

  CORBA::Long registerServer () override;

  void startRetry (CORBA::Long count) override;

  CORBA::Long endRetry () override;

  Manager_i (Manager_i const &) = delete;
  Manager_i (Manager_i &&) = delete;
  Manager_i &operator= (Manager_i const &) = delete;
  Manager_i &operator= (Manager_i &&) = delete;

private:
  long server_instance_ {};
  long retry_count_;
  long retry_attempt_ {};
};

#endif /* MANAGERI_H_  */
