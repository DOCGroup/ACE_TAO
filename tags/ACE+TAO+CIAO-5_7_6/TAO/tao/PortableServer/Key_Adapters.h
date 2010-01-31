/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Key_Adapters.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
//=============================================================================

#ifndef TAO_KEY_ADAPTERS_H
#define TAO_KEY_ADAPTERS_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PS_ForwardC.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Active_Map_Manager_Key;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

///////////////////////////////////////////////////////////////////////////////

/**
 * @class TAO_Incremental_Key_Generator
 *
 * @brief Defines a key generator.
 *
 * This class is used in adapters of maps that do not produce keys.
 */
class TAO_Incremental_Key_Generator
{
public:

  TAO_Incremental_Key_Generator (void);

  int operator() (PortableServer::ObjectId &id);

protected:

  CORBA::ULong counter_;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * @class TAO_ObjectId_Hash
 *
 * @brief Hashing class for Object Ids.
 *
 * Define the hash() method for Object Ids.
 */
class TAO_PortableServer_Export TAO_ObjectId_Hash
{
public:

  /// Returns hash value.
  u_long operator () (const PortableServer::ObjectId &id) const;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * @class TAO_Ignore_Original_Key_Adapter
 *
 * @brief A key adapter (encode/decode) class.
 *
 * Define the encoding and decoding methods for converting
 * between Object Ids and active keys.  This class ignores the
 * <original_key> passed to it.
 */
class TAO_Ignore_Original_Key_Adapter
{
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

/**
 * @class TAO_Preserve_Original_Key_Adapter
 *
 * @brief A key adapter (encode/decode) class.
 *
 * Define the encoding and decoding methods for converting
 * between Object Ids and active keys.  This class remembers the
 * <original_key> passed to it.
 */
class TAO_Preserve_Original_Key_Adapter
{
public:

  int encode (const PortableServer::ObjectId &original_key,
              const ACE_Active_Map_Manager_Key &active_key,
              PortableServer::ObjectId &modified_key);

  int decode (const PortableServer::ObjectId &modified_key,
              ACE_Active_Map_Manager_Key &active_key);

  int decode (const PortableServer::ObjectId &modified_key,
              PortableServer::ObjectId &original_key);
};

///////////////////////////////////////////////////////////////////////////////

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_KEY_ADAPTERS_H */
