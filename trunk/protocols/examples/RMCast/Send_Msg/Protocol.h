// file      : Protocol.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef PROTOCOL_H
#define PROTOCOL_H

unsigned short const payload_size = 512;
unsigned long const message_count = 10000;

struct Message
{
  unsigned long sn;

  unsigned short payload[payload_size];
};

#endif  // PROTOCOL_H
