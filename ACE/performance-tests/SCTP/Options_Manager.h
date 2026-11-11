// -*- C++ -*-

// The types defined in ACE_CDR are more complete that those in Basic_Types.h


#ifndef OPTIONS_MANAGER_H
#define OPTIONS_MANAGER_H

#include <array>
#include <cstddef>
#include <string>

#ifndef ACE_CDR_BASE_H
#include "ace/CDR_Base.h"
#endif

#include "ace/streams.h"
#include "ace/OS_NS_stdio.h"

class Options_Manager
{
public:
  using string_type = std::basic_string<ACE_TCHAR>;

  Options_Manager(int argc, ACE_TCHAR **argv, ACE_TCHAR const * const opts_set);

  // constant string size
  enum { string_len = 100 };

  static ACE_CDR::ULong test_iterations;
  static ACE_CDR::Boolean test_enable_nagle;
  static ACE_CDR::Long test_transport_protocol;

  static ACE_CDR::Double histogram_min_bin;
  static ACE_CDR::Double histogram_max_bin;
  static ACE_CDR::ULong histogram_bin_count;
  static ACE_CDR::ULong histogram_num_outliers;

  static ACE_CDR::UShort client_port;
  static ACE_CDR::ULong  client_connect_addr;

  static ACE_CDR::UShort server_port;
  static string_type server_host;
  static ACE_CDR::ULong server_accept_addr;

  static ACE_CDR::UShort payload_size_power_of_2;

  // Secondary client_connect_addrs for SCTP SOCK_SEQPACK test
  static constexpr std::size_t max_num_secondary_connect_addrs = 100;
  static std::array<ACE_CDR::ULong, max_num_secondary_connect_addrs> secondary_connect_addrs;
  static std::size_t num_secondary_connect_addrs;

  // Secondary server_accept_addrs for SCTP SOCK_SEQPACK test
  static constexpr std::size_t max_num_secondary_accept_addrs = 100;
  static std::array<ACE_CDR::ULong, max_num_secondary_accept_addrs> secondary_accept_addrs;
  static std::size_t num_secondary_accept_addrs;

  static ACE_CDR::UShort _error;
  static const ACE_TCHAR* _error_message;

  static ACE_CDR::UShort _usage;
  void _show_usage(FILE* out, ACE_TCHAR const * const opts_set);

private:
  static ACE_CDR::Boolean __initialized;
  static string_type __program_name;
};

#endif /* OPTIONS_MANAGER_H */
