#ifndef CALLBACKI_H_
#define CALLBACKI_H_

#include "callbackS.h"

class  Callback_i : public virtual POA_Callback
{
public:
  explicit Callback_i (CORBA::ORB_ptr orb);

  ~Callback_i () override = default;

  void shutdown () override;

  void callback_method () override;

  Callback_i (Callback_i const &) = delete;
  Callback_i (Callback_i &&) = delete;
  Callback_i &operator= (Callback_i const &) = delete;
  Callback_i &operator= (Callback_i &&) = delete;

private:
  CORBA::ORB_var orb_;
};


#endif /* CALLBACKI_H_  */
