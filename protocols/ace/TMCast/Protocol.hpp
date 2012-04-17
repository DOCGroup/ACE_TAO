// $Id$
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>

#ifndef TMCAST_PROTOCOL_HPP
#define TMCAST_PROTOCOL_HPP

namespace ACE_TMCast
{
  namespace Protocol
  {
    unsigned long const MEMBER_ID_LENGTH = 38;

    struct MemberId
    {
      char id[MEMBER_ID_LENGTH];
      /*
        unsigned long  ip;
        unsigned short port;
      */
    };

    typedef unsigned short TransactionId;

    typedef unsigned char  TransactionStatus;

    TransactionStatus const TS_BEGIN    = 1;
    TransactionStatus const TS_COMMIT   = 2;
    TransactionStatus const TS_ABORT    = 3;
    TransactionStatus const TS_COMMITED = 4;
    TransactionStatus const TS_ABORTED  = 5;

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

    unsigned long const MAX_MESSAGE_SIZE = 768;

    unsigned long const
    MAX_PAYLOAD_SIZE = MAX_MESSAGE_SIZE - sizeof (MessageHeader);

    // Protocol timing
    //
    //

    unsigned long const SYNC_PERIOD = 30000;    // in mks

    unsigned short const VOTING_FRAME = 4;      // in SYNC_PERIOD's
    unsigned short const SEPARATION_FRAME = 5;  // in SYNC_PERIOD's

    // FATAL_SILENCE_FRAME in SYNC_PERIOD's
    // Generally it's a good idea to set it to < VOTING_FRAME + SEPARATION_FRAME
    //

    short const FATAL_SILENCE_FRAME = VOTING_FRAME + SEPARATION_FRAME - 2;

    // short const FATAL_SILENCE_FRAME = 10000;

    // Helpers

    // std::string
    // tslabel (Protocol::TransactionStatus s);

    // std::ostream&
    // operator << (std::ostream& o, Transaction const& t);
  }
}

#endif // TMCAST_PROTOCOL_HPP
