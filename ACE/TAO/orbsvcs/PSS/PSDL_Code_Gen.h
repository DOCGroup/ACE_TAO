/* -*- C++ -*- */
// $Id$
//
// ==================================================================
//
// = LIBRARY
//    PSS
//
// = FILENAME
//    PSDL_Code_Gen
//
// = DESCRIPTION
//    This class acts as an interface to the database which is used
//    for the saving the state of the objects. Right now, the database
//    is a normal file.
//
// = AUTHOR
//    Priyanka Gontla <gontla_p@ociweb.com>
//
// ==================================================================

#ifndef TAO_PSDL_CODE_GEN_H
#define TAO_PSDL_CODE_GEN_H

#include /**/ "ace/pre.h"

#include "psdl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IOPC.h"
#include "tao/ORB.h"

class TAO_PSDL_Datastore;

class TAO_PSDL_Export TAO_PSDL_Code_Gen
{
 public:

  TAO_PSDL_Code_Gen (CORBA::ORB_ptr orb);

  ~TAO_PSDL_Code_Gen (void);

  /// Initializes a IOP::CodecFactory and IOP::Codec to take care of
  /// the marshalling and demarshalling of data.
  int set_codec (void);
  
  /// Method to save the name-stringified object reference pair
  /// to the database. Returns -1 on failure.
  int set_name_obj_ref (const char *name,
                        const char *string_obj_ref
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the stringified form of the object reference given the name
  /// of the object.
  const char *get_obj_ref (const char *name
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:

  /// Helper method which serializes the data and saves it to the
  /// database.
  CORBA::OctetSeq *encode (const char *string_obj_ref
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));


  /// Helper method to get the octet sequence
  const char *decode (const CORBA::OctetSeq &data
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// File where the persistent data is stored.
  const char *file_name_;

  /// Pointer to the class which accesses the database
  TAO_PSDL_Datastore *psdl_datastore_;

  /// Pointer to ORB
  CORBA::ORB_var orb_;

  /// CDR encapsulation codec useful for encoding and decoding the
  /// data
  IOP::Codec_var codec_;
};

#include /**/ "ace/post.h"

#endif /* TAO_PSDL_CODE_GEN_H */
