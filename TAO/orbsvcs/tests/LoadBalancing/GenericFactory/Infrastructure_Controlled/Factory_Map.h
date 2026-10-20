// -*- C++ -*-
#ifndef FACTORY_MAP_H
#define FACTORY_MAP_H

#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Factory_Struct.h"
#include "ace/Hash_Map_Manager_T.h"

using Factory_Map = ACE_Hash_Map_Manager_Ex<
  ACE_UINT32,
  Factory_Struct,
  ACE_Hash<ACE_UINT32>,
  ACE_Equal_To<ACE_UINT32>,
  ACE_Null_Mutex>;


#include /**/ "ace/post.h"
#endif /* FACTORY_MAP_H */
