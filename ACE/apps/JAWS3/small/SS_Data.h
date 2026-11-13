/* -*- c++ -*- */
#ifndef TERA_SS_DATA_H
#define TERA_SS_DATA_H

#include "ace/FILE_IO.h"
#include "ace/SOCK_Stream.h"
#include "ace/Message_Block.h"

class TeraSS_Service_Handler;

class TeraSS_Data
{
public:
  explicit TeraSS_Data (TeraSS_Service_Handler &handler);

  TeraSS_Data (TeraSS_Data const &) = delete;
  TeraSS_Data (TeraSS_Data &&) = delete;
  TeraSS_Data &operator= (TeraSS_Data const &) = delete;
  TeraSS_Data &operator= (TeraSS_Data &&) = delete;

  ACE_SOCK_Stream & peer ();
  ACE_Message_Block & mb ();
  ACE_FILE_IO & file_io ();

private:
  ACE_Message_Block mb_;
  /// Non-owning reference to the service handler containing the peer stream.
  TeraSS_Service_Handler &handler_;
  ACE_FILE_IO file_io_;
};

#endif /* TERA_SS_DATA_H */
