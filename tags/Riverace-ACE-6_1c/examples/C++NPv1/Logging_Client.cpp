/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS_NS_sys_time.h"
#include "ace/CDR_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Record.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdlib.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

#if defined (ACE_WIN32) && (!defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
                            (ACE_HAS_STANDARD_CPP_LIBRARY == 0) || \
                            defined (ACE_USES_OLD_IOSTREAMS))
#  include <stdio.h>
#else
#  include <string>
#endif

class Logging_Client {
private:
  ACE_SOCK_Stream logging_peer_;

public:
  ACE_SOCK_Stream &peer () { return logging_peer_; }

  //FUZZ: disable check_for_lack_ACE_OS
  int send (const ACE_Log_Record &log_record) {
  //FUZZ: enable check_for_lack_ACE_OS

    // Serialize the log record using a CDR stream, allocate
    // enough space for the complete <ACE_Log_Record>.
    const size_t max_payload_size =
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
    ACE_CDR::ULong length =
      ACE_Utils::truncate_cast<ACE_CDR::ULong> (payload.total_length ());

    // Send a header so the receiver can determine the byte
    // order and size of the incoming CDR stream.
    ACE_OutputCDR header (ACE_CDR::MAX_ALIGNMENT + 8);
    header << ACE_OutputCDR::from_boolean (ACE_CDR_BYTE_ORDER);

    // Store the size of the payload that follows
    header << ACE_CDR::ULong (length);
    // Use an iovec to send both buffer and payload simultaneously.
    iovec iov[2];
    iov[0].iov_base = header.begin ()->rd_ptr ();
    iov[0].iov_len  = 8;
    iov[1].iov_base = payload.begin ()->rd_ptr ();
    iov[1].iov_len  = length;

    // Send header and payload efficiently using "gather-write".
    return logging_peer_.sendv_n (iov, 2);
  }

  ~Logging_Client () { logging_peer_.close (); }
};


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  u_short logger_port = argc > 1 ? ACE_OS::atoi (argv[1]) : 0;
  const char *logger_host =
    argc > 2 ? argv[2] : ACE_DEFAULT_SERVER_HOST;
  int result;

  ACE_INET_Addr server_addr;

  if (logger_port != 0)
    result = server_addr.set (logger_port, logger_host);
  else
    result = server_addr.set ("ace_logger", logger_host);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "lookup %s, %p\n",
                       logger_port == 0 ? "ace_logger" : argv[1],
                       logger_host), 1);

  ACE_SOCK_Connector connector;
  Logging_Client logging_client;

  if (connector.connect (logging_client.peer (), server_addr) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "connect()"), 1);

#if defined (ACE_WIN32) && (!defined (ACE_HAS_STANDARD_CPP_LIBRARY) || \
                            (ACE_HAS_STANDARD_CPP_LIBRARY == 0) || \
                            defined (ACE_USES_OLD_IOSTREAMS))
  for (;;) {
    char user_input[ACE_Log_Record::MAXLOGMSGLEN];
    if (!gets (user_input))
      break;

    ACE_Time_Value now (ACE_OS::gettimeofday ());
    ACE_Log_Record log_record (LM_INFO, now, ACE_OS::getpid ());
    log_record.msg_data (user_input);
    if (logging_client.send (log_record) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n", "logging_client.send()"), 1);
  }
#else

  // Limit the number of characters read on each record
  cin.width (ACE_Log_Record::MAXLOGMSGLEN);

  for (;;) {

#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB == 0)
    string user_input;
    getline (cin, user_input, '\n');
#else
    std::string user_input;
    std::getline (cin, user_input, '\n');

#endif

    if (!cin || cin.eof ()) break;

    ACE_Time_Value now (ACE_OS::gettimeofday ());
    ACE_Log_Record log_record (LM_INFO, now, ACE_OS::getpid ());
    log_record.msg_data (user_input.c_str ());
    if (logging_client.send (log_record) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n", "logging_client.send()"), 1);
  }
#endif

  return 0; // Logging_Client destructor closes TCP connection.
}
