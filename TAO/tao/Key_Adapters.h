/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Key_Adapters.h
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#ifndef TAO_KEY_ADAPTERS_H
#define TAO_KEY_ADAPTERS_H

#include "tao/corbafwd.h"
#include "ace/Map.h"
#include "tao/POAC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

////////////////////////////////////////////////////////////////////////////////

class TAO_Incremental_Key_Generator
{
  // = TITLE
  //     Defines a key generator.
  //
  // = DESCRIPTION
  //     This class is used in adapters of maps that do not produce keys.
public:

  TAO_Incremental_Key_Generator (void);

  int operator() (PortableServer::ObjectId &id);

protected:

  CORBA::ULong counter_;
};

////////////////////////////////////////////////////////////////////////////////

class TAO_Export TAO_ObjectId_Hash
{
  // = TITLE
  //     Hashing class for Object Ids.
  //
  // = DESCRIPTION
  //     Define the hash() method for Object Ids.
public:

  u_long operator () (const PortableServer::ObjectId &id) const;
  // Returns hash value.
};

////////////////////////////////////////////////////////////////////////////////

class TAO_Ignore_Original_Key_Adapter
{
  // = TITLE
  //     A key adapter (encode/decode) class.
  //
  // = DESCRIPTION
  //     Define the encoding and decoding methods for converting
  //     between Object Ids and active keys.  This class ignores the
  //     <original_key> passed to it.
public:

  int encode (const PortableServer::ObjectId &original_key,
              const ACE_Active_Map_Manager_Key &active_key,
              PortableServer::ObjectId &modified_key);

  int decode (const PortableServer::ObjectId &modified_key,
              ACE_Active_Map_Manager_Key &active_key);

  int decode (const PortableServer::ObjectId &modified_key,
              PortableServer::ObjectId &original_key);
};

////////////////////////////////////////////////////////////////////////////////

class TAO_Preserve_Original_Key_Adapter
{
  // = TITLE
  //     A key adapter (encode/decode) class.
  //
  // = DESCRIPTION
  //     Define the encoding and decoding methods for converting
  //     between Object Ids and active keys.  This class remembers the
  //     <original_key> passed to it.
public:

  int encode (const PortableServer::ObjectId &original_key,
              const ACE_Active_Map_Manager_Key &active_key,
              PortableServer::ObjectId &modified_key);

  int decode (const PortableServer::ObjectId &modified_key,
              ACE_Active_Map_Manager_Key &active_key);

  int decode (const PortableServer::ObjectId &modified_key,
              PortableServer::ObjectId &original_key);
};

////////////////////////////////////////////////////////////////////////////////

// Comparison of Object Ids. Defined in Stub.cpp for TAO_opaque (an
// alias of PortableServer::ObjectId).
extern TAO_Export int operator== (const PortableServer::ObjectId &l,
                                  const PortableServer::ObjectId &r);

extern TAO_Export int operator!= (const PortableServer::ObjectId &l,
                                  const PortableServer::ObjectId &r);

////////////////////////////////////////////////////////////////////////////////

#if defined (__ACE_INLINE__)
# include "tao/Key_Adapters.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_KEY_ADAPTERS_H */
