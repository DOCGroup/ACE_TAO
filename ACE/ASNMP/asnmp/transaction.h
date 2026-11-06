/* -*-C++-*- */
#ifndef TRANSACTION_
#define TRANSACTION_
//=============================================================================
/**
 *  @file    transaction.h
 *
 *  @brief
 *
 *  @author Michael R. MacFaden port to ACE / use Reactor pattern
 */
//=============================================================================


#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "asnmp/target.h"
#include "asnmp/pdu.h"
#include "asnmp/transaction_result.h"
#include "asnmp/wpdu.h"         // cmu adapter class
#include "ace/SOCK_Dgram.h"
#include "ace/Malloc_Base.h"
#include <memory>

/**
 * @class transaction
 *
 * @brief Used to manage the details of a particular transaction between
 * two SNMP agents. Uses SnmpTarget class to implement retry/timeout
 */
class ASNMP_Export transaction : public ACE_Event_Handler
{
  int retry_counter_ {};
  transaction_result *result_ {}; // borrowed callback

public:
  /// constructor
  /// destructor
  transaction(const Pdu& pdu, const UdpTarget& target, ACE_SOCK_Dgram& io);
  transaction(ACE_SOCK_Dgram& io);
  transaction (transaction const &) = delete;
  transaction &operator= (transaction const &) = delete;
  transaction (transaction &&) = delete;
  transaction &operator= (transaction &&) = delete;
  ~transaction();

  /// begin polling for values
  int run();
  int run(transaction_result *r); // Async interface, with callback object

  /// return pdu with result from agent after run() is completed rc = 0
  /// optionally get community str
  int result(Pdu& pdu, char *comm_str = 0, ACE_INET_Addr *from_addr = 0);

  /// called by reactor when data is ready to be read in from OS memory
  /// used for resend in asynchronous run()
  int handle_input (ACE_HANDLE fd) override;
  int handle_timeout (const ACE_Time_Value &, const void *) override;

  /// transmit buffer command to network...
  int send();

  /// pre: handle_input called
  /// retrieve the sender's from address from the last pkt
  const ACE_INET_Addr& get_from_addr() const;

  /// Return session_ handle.
  ACE_HANDLE get_handle () const override;

private:
  // ACE_SOCK_Dgram::recv allocates this buffer; release it through the matching allocator.
  struct Receive_Buffer_Deleter
  {
    void operator() (char *buffer) const
    {
      if (!buffer)
        return;
#if defined (ACE_HAS_ALLOC_HOOKS)
      ACE_Allocator::instance ()->free (buffer);
#else
      delete [] buffer;
#endif
    }
  };

  wpdu wp_;                     // wire pdu
  UdpTarget params_;            // params
  ACE_INET_Addr addr_;          // to address
  ACE_SOCK_Dgram session_;      // io object
  std::unique_ptr<char[], Receive_Buffer_Deleter> receive_buffer_ {};
  iovec receive_iovec_ {};      // non-owning view; recv() allocates receive_buffer_
  ACE_INET_Addr receive_addr_;  // from address
};

#endif // TRANSACTION_
