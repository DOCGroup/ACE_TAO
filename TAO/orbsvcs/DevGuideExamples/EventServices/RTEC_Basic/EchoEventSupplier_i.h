// EchoEventSupplier_i.h
// Implements a PushSupplier.

#ifndef _EchoEventSupplier_i_h_
#define _EchoEventSupplier_i_h_

#include "orbsvcs/RtecEventCommS.h"// for POA_CosEventComm::PushSupplier

class EchoEventSupplier_i : public virtual POA_RtecEventComm::PushSupplier
{
  public:
    // Constructor
    EchoEventSupplier_i(CORBA::ORB_ptr orb);

    EchoEventSupplier_i (EchoEventSupplier_i const &) = delete;
    EchoEventSupplier_i (EchoEventSupplier_i &&) = delete;
    EchoEventSupplier_i &operator= (EchoEventSupplier_i const &) = delete;
    EchoEventSupplier_i &operator= (EchoEventSupplier_i &&) = delete;

    void disconnect_push_supplier () override;

  private:
    CORBA::ORB_var orb_;
};

#endif // _EchoEventSupplier_i_h_
