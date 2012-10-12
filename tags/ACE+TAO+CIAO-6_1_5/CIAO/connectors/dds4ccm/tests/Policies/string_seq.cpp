//$Id$

#include "string_seq.h"

StringSequence::StringSequence ()
{
}

bool
StringSequence::check (const ::DDS_StringSeq & dds_seq,
                      const ::DDS::StringSeq & seq,
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
      if (ACE_OS::strcmp (seq[i], dds_seq[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR, "ERROR %C : "
                            "Item %d of group_data is not equal to original <%C>:<%C>.\n",
                            test, i, seq[i].in (), dds_seq[i]));
          return false;
        }
    }
  return true;
}
