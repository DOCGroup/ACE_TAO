/* -*-C++-*- */
// $Id$
#ifndef TRANSACTION_RESULT_H_
#define TRANSACTION_RESULT_H_
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    transaction_result.h
//
// = DESCRIPTION
//  An object respresenting a request/reply operation between mgr/agent
//
// = AUTHOR
//    Michael R. MacFaden
//
// ============================================================================

class transaction;
class ACE_Export transaction_result
{
  public:
    virtual ~transaction_result();
    virtual void result(transaction * trans, int) = 0;
};

#endif
