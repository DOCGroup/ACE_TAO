// This program sends logging records directly to the server, rather
// than going through the client logging daemon.

#include "ace/SOCK_Connector.h"
#include "ace/Log_Record.h"
#include "ace/Log_Msg.h"
#include "ace/Truncate.h"
#include "ace/CDR_Stream.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"

#include <array>
#include <cstddef>

constexpr u_short default_logger_port = ACE_DEFAULT_SERVER_PORT;
ACE_TCHAR const * const default_logger_host = ACE_DEFAULT_SERVER_HOST;
constexpr std::size_t default_max_iterations = 10;

struct Logger_Options
{
  ACE_TCHAR const *host {default_logger_host};
  u_short port {default_logger_port};
  std::size_t max_iterations {default_max_iterations};
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Logger_Options options;
  if (argc > 1)
    options.host = argv[1];
  if (argc > 2)
    options.port = static_cast<u_short> (ACE_OS::atoi (argv[2]));
  if (argc > 3)
    {
      int const parsed_iterations = ACE_OS::atoi (argv[3]);
      if (parsed_iterations < 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("iterations must not be negative\n")),
                          1);
      options.max_iterations = static_cast<std::size_t> (parsed_iterations);
    }

  ACE_SOCK_Stream logger;
  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (options.port, options.host);

  if (connector.connect (logger, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);

  for (std::size_t i = 0; i < options.max_iterations; ++i)
    {
      ACE_Log_Record log_record (LM_DEBUG,
                                 ACE_OS::time (nullptr),
                                 ACE_OS::getpid ());

      std::array<ACE_TCHAR, BUFSIZ> buf {};
      ACE_OS::sprintf (buf.data (), ACE_TEXT ("message = %u\n"),
                       static_cast<unsigned int> (i + 1));
      log_record.msg_data (buf.data ());

      std::size_t const max_payload_size =
        4 // type()
        + 8 // timestamp
        + 4 // process id
        + 4 // data length
        + ACE_Log_Record::MAXLOGMSGLEN // data
        + ACE_CDR::MAX_ALIGNMENT; // padding;

      // Insert contents of <log_record> into payload stream.
      ACE_OutputCDR payload (max_payload_size);
      payload << log_record;

      // Get the number of bytes used by the CDR stream.
      ACE_CDR::ULong const length =
        ACE_Utils::truncate_cast<ACE_CDR::ULong> (payload.total_length ());

      // Send a header so the receiver can determine the byte order and
      // size of the incoming CDR stream.
      ACE_OutputCDR header (ACE_CDR::MAX_ALIGNMENT + 8);
      header << ACE_OutputCDR::from_boolean (ACE_CDR_BYTE_ORDER);

      // Store the size of the payload that follows
      header << ACE_CDR::ULong (length);

      // Use an iovec to send both buffer and payload simultaneously.
      std::array<iovec, 2> iov {};
      iov[0].iov_base = header.begin ()->rd_ptr ();
      iov[0].iov_len  = 8;
      iov[1].iov_base = payload.begin ()->rd_ptr ();
      iov[1].iov_len  = length;

      if (logger.sendv_n (iov.data (), static_cast<int> (iov.size ())) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "send"), -1);
    }

  if (logger.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("close")), -1);

  return 0;
}
