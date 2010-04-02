// $Id$

#include "partition_qos_test.h"

PartitionPolicyTest::PartitionPolicyTest ()
{
}

bool
PartitionPolicyTest::check (const ::DDS_PartitionQosPolicy & dds_qos,
                            const ::DDS::PartitionQosPolicy & qos)
{
  if (static_cast <CORBA::ULong> (dds_qos.name.length ()) !=
      qos.name.length ())
    {
      ACE_ERROR ((LM_ERROR, "ERROR PartitionPolicyTest : "
                            "Length name sequence is not equal "
                            "to original length.\n"));
      return false;
    }
  for (DDS_Long i = 0; i < dds_qos.name.length (); ++i)
    {
      if (ACE_OS::strcmp (dds_qos.name[i], qos.name[i]) != 0)
        {
          ACE_ERROR ((LM_ERROR, "ERROR PartitionPolicyTest : "
                            "Item %d of name sequence is not equal "
                            "to original.\n",
                            i));
          return false;
        }
    }
  return true;
}
