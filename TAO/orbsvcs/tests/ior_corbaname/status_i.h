
#ifndef STATUS_I_H
#define STATUS_I_H

#include "corbanameS.h"

class corbaname_Status_i : public POA_corbaname::Status {
public:
  corbaname_Status_i () = default;
  corbaname_Status_i (corbaname_Status_i const &) = delete;
  corbaname_Status_i (corbaname_Status_i &&) = delete;
  corbaname_Status_i &operator= (corbaname_Status_i const &) = delete;
  corbaname_Status_i &operator= (corbaname_Status_i &&) = delete;

  CORBA::Boolean print_status () override;
};

#endif /* STATUS_I_H */
