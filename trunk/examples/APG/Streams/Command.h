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
    PASS    = 1,
    SUCCESS = 0,
    FAILURE = -1
  };

  // Commands
  enum {
    UNKNOWN            = -1,
    ANSWER_CALL        = 10,
    RETRIEVE_CALLER_ID,
    PLAY_MESSAGE,
    RECORD_MESSAGE
  } commands;

  int flags_;
  int command_;

  void *extra_data_;

  int numeric_result_;
  ACE_CString result_;
};
// Listing 01

#endif /* COMMAND_H */
