#ifndef STATUS_I_H
#define STATUS_I_H

#include "corbalocS.h"
#include "ace/SString.h"

class corbaloc_Status_i : public POA_corbaloc::Status
{
public:
  corbaloc_Status_i () = default;

  corbaloc_Status_i (corbaloc_Status_i const &) = delete;
  corbaloc_Status_i &operator= (corbaloc_Status_i const &) = delete;
  corbaloc_Status_i (corbaloc_Status_i &&) = delete;
  corbaloc_Status_i &operator= (corbaloc_Status_i &&) = delete;

  CORBA::Boolean print_status () override;

  void set_name (char const *name) { server_name_ = name; }

private:
  ACE_CString server_name_ {};
};

#endif /* STATUS_I_H */
