// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#ifndef TMCAST_PROTOCOL_HPP
#define TMCAST_PROTOCOL_HPP

namespace ACE_TMCast
{
  namespace Protocol
  {
    constexpr unsigned long MEMBER_ID_LENGTH = 38;

    struct MemberId
    {
      char id[MEMBER_ID_LENGTH];
      /*
        unsigned long  ip;
        unsigned short port;
      */
    };

    using TransactionId = unsigned short;

    using TransactionStatus = unsigned char;

    constexpr TransactionStatus TS_BEGIN    = 1;
    constexpr TransactionStatus TS_COMMIT   = 2;
    constexpr TransactionStatus TS_ABORT    = 3;
    constexpr TransactionStatus TS_COMMITED = 4;
    constexpr TransactionStatus TS_ABORTED  = 5;

    struct Transaction
    {
      TransactionId id;
      TransactionStatus status;
    };

    // Transaction List (TL)

    // unsigned long const TL_LENGTH = 1;

    // typedef Transaction TransactionList[TL_LENGTH];


    struct MessageHeader
    {
      unsigned long length;

      unsigned long check_sum;

      MemberId member_id;

      Transaction current;

      //TransactionList transaction_list;
    };

    constexpr unsigned long MAX_MESSAGE_SIZE = 768;

    constexpr unsigned long
    MAX_PAYLOAD_SIZE = MAX_MESSAGE_SIZE - sizeof (MessageHeader);

    // Protocol timing
    //
    //

    constexpr unsigned long SYNC_PERIOD = 30000;    // in mks

    constexpr unsigned short VOTING_FRAME = 4;      // in SYNC_PERIOD's
    constexpr unsigned short SEPARATION_FRAME = 5;  // in SYNC_PERIOD's

    // FATAL_SILENCE_FRAME in SYNC_PERIOD's
    // Generally it's a good idea to set it to < VOTING_FRAME + SEPARATION_FRAME
    //

    constexpr short FATAL_SILENCE_FRAME = VOTING_FRAME + SEPARATION_FRAME - 2;

    // short const FATAL_SILENCE_FRAME = 10000;

    // Helpers

    // std::string
    // tslabel (Protocol::TransactionStatus s);

    // std::ostream&
    // operator << (std::ostream& o, Transaction const& t);
  }
}

#endif // TMCAST_PROTOCOL_HPP
