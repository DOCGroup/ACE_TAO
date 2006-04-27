/* -*- C++ -*- */
// $Id$

#ifndef COMMAND_H
#define COMMAND_H

#include "ace/SString.h"
#include "ace/Message_Block.h"

// Listing 01 code/ch18
class Command : public ACE_Data_Block
{
public:
  // Result Values
  enum {
    RESULT_PASS    = 1,
    RESULT_SUCCESS = 0,
    RESULT_FAILURE = -1
  };

  // Commands
  enum {
    CMD_UNKNOWN            = -1,
    CMD_ANSWER_CALL        = 10,
    CMD_RETRIEVE_CALLER_ID,
    CMD_PLAY_MESSAGE,
    CMD_RECORD_MESSAGE
  } commands;

  int flags_;
  int command_;

  void *extra_data_;

  int numeric_result_;
  ACE_TString result_;
};
// Listing 01

#endif /* COMMAND_H */
