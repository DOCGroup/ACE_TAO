// $Id$

#ifndef STATUS_I_H
#define STATUS_I_H

#include "corbalocS.h"
#include "ace/SString.h"

class corbaloc_Status_i : public POA_corbaloc::Status
{
public:
  corbaloc_Status_i ();

  CORBA::Boolean print_status (void);

  void set_name (const char *name) { server_name_ = name; }

private:
  ACE_CString server_name_;
};

#endif /* STATUS_I_H */
