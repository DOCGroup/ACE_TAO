// -*- C++ -*-
// $Id$

#include "PSDL_Code_Gen.h"
#include "PSDL_Datastore.h"
#include "tao/OctetSeqC.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/CodecFactory/CodecFactory.h"



TAO_PSDL_Code_Gen::TAO_PSDL_Code_Gen (CORBA::ORB_ptr orb)
  : file_name_ ("Data_Store"),
    psdl_datastore_ (),
    orb_ (orb),
    codec_ (0)
{
  this->set_codec ();
  ACE_NEW (this->psdl_datastore_,
           TAO_PSDL_Datastore);

}

TAO_PSDL_Code_Gen::~TAO_PSDL_Code_Gen (void)
{
  delete this->psdl_datastore_;
}

int
TAO_PSDL_Code_Gen::set_codec (void)
{

  // Obtain a reference to the CodecFactory.
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("CodecFactory");

  IOP::CodecFactory_var codec_factory =
    IOP::CodecFactory::_narrow (obj.in ());

  // Set up a structure that contains information necessary to
  // create a GIOP 1.1 CDR encapsulation Codec.
  IOP::Encoding encoding;
  encoding.format = IOP::ENCODING_CDR_ENCAPS;
  encoding.major_version = 1;
  encoding.minor_version = 1;

  // Obtain the CDR encapsulation Codec.
  this->codec_ =
    codec_factory->create_codec (encoding);

  if (this->codec_.in () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "codec pointer not set correctly\n"));
      return -1;
    }
  return 0;
}

int
TAO_PSDL_Code_Gen::set_name_obj_ref (const char *name,
                                     const char *string_obj_ref)
{
  // Invoke the helper encode method which will
  // convert the stringified object reference to a CORBA::OctetSeq.
  // Insert the name-CORBA::OCtetSeq pair to a hash_map and save the
  // hash_map to the database.

  // Encode the stringified object reference to a CORBA::OctetSeq *
  CORBA::OctetSeq_var octet_seq = this->encode (string_obj_ref);

  // Insert the new entry to the hash map which contains all the
  // name-octet_seq entries. And, write the hash_map to a file.
  int result = this->psdl_datastore_->bind (name,
                                            octet_seq.in ());

  if (result == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Bind not done successfully\n"));
    }
  else if (result == 1)
    {
      /*ACE_DEBUG ((LM_DEBUG,
                    "Bind already done.\n"));
      */
      return 0;
    }

  return result;
}

const char *
TAO_PSDL_Code_Gen::get_obj_ref (const char *name)
{
  // Get from the hash_map saved in the database, the corresponding entry
  // (CORBA::OctetSeq *) for the name. Then, decode the octetseq to
  // get the stringified object reference and return it.

  CORBA::OctetSeq octet_seq;

  // Find the octet_seq for the name.
  int result = this->psdl_datastore_->find (name,
                                            octet_seq);

  if (result == 0)
    {
      // Decode the octet_seq.
      const char *obj_ref = this->decode (octet_seq);

      return CORBA::string_dup (obj_ref);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "An entry for name %s is not found\n",
                  name));
      return 0;
    }
}


CORBA::OctetSeq *
TAO_PSDL_Code_Gen::encode (const char *string_obj_ref)
{
  CORBA::Any data;
  data <<= string_obj_ref;

  CORBA::OctetSeq *encoded_data = 0;

  encoded_data = this->codec_->encode (data);

  CORBA::OctetSeq_var safe_encoded_data = encoded_data;

  return safe_encoded_data._retn ();
}

const char *
TAO_PSDL_Code_Gen::decode (const CORBA::OctetSeq &data)
{
  const char *extracted_value;

  // Extract the data from the octet sequence.
  CORBA::Any_var decoded_data =
    this->codec_->decode (data);

  decoded_data.in() >>= extracted_value;

  return CORBA::string_dup (extracted_value);
}
