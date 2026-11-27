// file      : Protocol.h
// author    : Boris Kolpackov <boris@kolpackov.net>
#ifndef PROTOCOL_H
#define PROTOCOL_H

constexpr unsigned short payload_size = 200;
constexpr unsigned long message_count = 100;

struct Message
{
  unsigned long sn;

  unsigned short payload[payload_size];
};

#endif  // PROTOCOL_H
