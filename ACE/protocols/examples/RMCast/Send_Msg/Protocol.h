// file      : Protocol.h
// author    : Boris Kolpackov <boris@kolpackov.net>
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstddef>

constexpr size_t payload_size = 702;
constexpr size_t message_count = 80000;

struct Message
{
  unsigned int sn;

  unsigned short payload[payload_size];
};

#endif  // PROTOCOL_H
