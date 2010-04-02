//$Id$

#include "octet_seq.h"

OctetSequence::OctetSequence ()
{
}

bool
OctetSequence::check (const ::DDS_OctetSeq & dds_seq,
                      const ::DDS::OctetSeq & seq,
                      const char * test)
{
  if (static_cast < ::CORBA::ULong> (dds_seq.length ()) !=
      seq.length ())
    {
      ACE_ERROR ((LM_ERROR, "ERROR %C : "
                            "Length group_data is not equal to original length.\n",
                            test));
      return false;
    }

  for (::DDS_Long i = 0; i < dds_seq.length (); ++i)
    {
      if (seq[i] != dds_seq[i])
        {
          ACE_ERROR ((LM_ERROR, "ERROR %C : "
                            "Item %d of group_data is not equal to original.\n",
                            test, i));
          return false;
        }
    }
  return true;
}
