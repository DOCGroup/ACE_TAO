/**
 * Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __CLIENTCONTEXT_H_
#define __CLIENTCONTEXT_H_

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

using Map = ACE_Hash_Map_Manager<char const *, void *, ACE_Null_Mutex>;

// Listing 1 code/ch14
// Client-specific context information.
class ClientContext
{
public:
  ClientContext () = default;
  ClientContext (ClientContext const &) = delete;
  ClientContext (ClientContext &&) = delete;
  ClientContext &operator= (ClientContext const &) = delete;
  ClientContext &operator= (ClientContext &&) = delete;

  void *get_attribute (char const *name);
  void set_attribute (char const *name, void *value);

private:
  Map attributeMap_;
};
// Listing 1

#endif /* __CLIENTCONTEXT_H_ */
