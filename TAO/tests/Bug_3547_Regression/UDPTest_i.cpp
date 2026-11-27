#include "UDPTest_i.h"
#include "ace/Log_Msg.h"
#include <algorithm>
#include <array>

std::array<CORBA::Octet, 1000> buf {};

void UDPTest_i::send (::UDPTestI::BufferT const Msg)
{
  std::copy_n (Msg, buf.size (), buf.begin ());
  buf.fill (CORBA::Octet {});
  this->received_send_.store (true, std::memory_order_release);
}
