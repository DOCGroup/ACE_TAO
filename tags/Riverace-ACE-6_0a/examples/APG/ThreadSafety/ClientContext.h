/**
 * $Id$
 *
 * Sample code from The ACE Programmer's Guide,
 * copyright 2003 Addison-Wesley. All Rights Reserved.
 */

#ifndef __CLIENTCONTEXT_H_
#define __CLIENTCONTEXT_H_

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"

typedef ACE_Hash_Map_Manager<const char *, void *, ACE_Null_Mutex>
Map;

// Listing 1 code/ch14
// Client-specific context information.
class ClientContext
{
public:
  void *get_attribute (const char *name);
  void set_attribute (const char *name, void *value);

private:
  Map attributeMap_;
};
// Listing 1

#endif /* __CLIENTCONTEXT_H_ */
