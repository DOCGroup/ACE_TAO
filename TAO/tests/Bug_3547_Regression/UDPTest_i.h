#ifndef UDPTest_i_h
#define UDPTest_i_h

#include "UDPTestS.h"
#include <atomic>

// This class provides the implementation for Stock_Quoter interface.
class UDPTest_i : public virtual POA_UDPTestI
{
public:
  UDPTest_i () = default;
  UDPTest_i (UDPTest_i const &) = delete;
  UDPTest_i (UDPTest_i &&) = delete;
  UDPTest_i &operator= (UDPTest_i const &) = delete;
  UDPTest_i &operator= (UDPTest_i &&) = delete;
  ~UDPTest_i () override = default;

  void send (::UDPTestI::BufferT const Msg) override;

  std::atomic<bool> received_send_ {};
};

#endif /* Stock_Quoter_i_h */
