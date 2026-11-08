#ifndef SIMPLEI_H_
#define SIMPLEI_H_

#include "simpleS.h"

class  Simple_i : public virtual POA_Simple
{
public:
  Simple_i (CORBA::ORB_ptr orb, int callback_count);

  ~Simple_i () override = default;

  CORBA::Long test_method (CORBA::Boolean do_callback) override;

  void callback_object (Callback_ptr cb) override;

  void shutdown () override;

  Simple_i (Simple_i const &) = delete;
  Simple_i (Simple_i &&) = delete;
  Simple_i &operator= (Simple_i const &) = delete;
  Simple_i &operator= (Simple_i &&) = delete;

  /// Not part of the CORBA interface. This method is called
  /// by our special orb event loop in server main().
  bool call_client ();

private:
  CORBA::ORB_var orb_;
  bool ready_for_callback_ {};
  int const callback_count_;
  Callback_var callback_;
};


#endif /* SIMPLEI_H_  */
