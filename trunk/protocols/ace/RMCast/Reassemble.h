// file      : ace/RMCast/Reassemble.h
// author    : Boris Kolpackov <boris@kolpackov.net>
// cvs-id    : $Id$

#ifndef ACE_RMCAST_REASSEMBLE_H
#define ACE_RMCAST_REASSEMBLE_H

#include "ace/Hash_Map_Manager.h"

#include "Stack.h"
#include "Protocol.h"
#include "Bits.h"

namespace ACE_RMCast
{
  class Reassemble : public Element
  {
  public:
    Reassemble ();

  public:
    virtual void
    recv (Message_ptr m);

  private:
    typedef
    ACE_Hash_Map_Manager_Ex<Address,
                            Data_ptr,
                            AddressHasher,
                            ACE_Equal_To<Address>,
                            ACE_Null_Mutex>
    Map;

    Map map_;
  };
}


#endif  // ACE_RMCAST_REASSEMBLE_H
