// $Id$

#include "RTI_IO.h"
#include "ace/streams.h"

void
print_RtiPacket (Cubit::RtiPacket const &arg)
{
  ACE_DEBUG ((LM_DEBUG, 
              "packetHeader.packetLength %d\n"
              "packetHeader.federationHandle %d\n"
              "packetHeader.channelHandle %d\n"
              "packetHeader.packetColor %d\n",
              arg.packetHeader.packetLength,
              arg.packetHeader.federationHandle,
              arg.packetHeader.channelHandle,
              arg.packetHeader.packetColor));

  for (CORBA::ULong j = 0; j < arg.msgs.length (); ++j) 
    {
      ACE_DEBUG ((LM_DEBUG,
                  "* message * %d\n",
                  j));

      Cubit::RtiObjectUpdateMessageHeader const &oumh = arg.msgs[j].oumh ();

      ACE_DEBUG ((LM_DEBUG,
                  "\tupdateLength: %d\n" 
                  "\tupdateTag: %d\n"
                  "\tobjectHandle: %d\n" 
                  "\ttimestamp: %f\n"
                  //"\teventRetractionHandle: \n"
                  "\tclassHandle: %d\n" 
                  "\tsendingFederateHandle: %d\n"
                  "\tuserTag: %s\n"
                  "\ttransportationHandle: %x\n"
                  "\torderingHandle: %x\n",
                  oumh.updateLength,
                  oumh.updateTag,
                  oumh.objectHandle,
                  oumh.timestamp,
                  //oumh.eventRetractionHandle,
                  oumh.classHandle,
                  oumh.sendingFederateHandle,
                  oumh.userTag.in (),
                  oumh.transportationHandle,
                  oumh.orderingHandle));

      for (CORBA::ULong k = 0; k < oumh.messagePayload.length (); ++k) 
        {
          Cubit::HandleValuePair const & hvp=oumh.messagePayload[k];
          ACE_DEBUG ((LM_DEBUG, "\t\thandle: %d\n", hvp.handle));
        }
    }

}

#if 0
ostream &
operator<< (ostream &o, Cubit::RtiPacket const &arg)
{
  o << "packetHeader.packetLength" << arg.packetHeader.packetLength    << endl
    << "packetHeader.federationHandle" << arg.packetHeader.federationHandle
    << endl
    << "packetHeader.channelHandle" << arg.packetHeader.channelHandle
    << endl
    << "packetHeader.packetColor" << arg.packetHeader.packetColor
    << endl;

  for (int j = 0; j < arg.msgs.length (); ++j) 
    {
      o << "message " << j << endl;

      Cubit::RtiObjectUpdateMessageHeader const &oumh = arg.msgs[j].oumh ();

      o << "\tupdateLength: " << oumh.updateLength
        << endl
        << "\tupdateTag: " << oumh.updateTag
        << endl
        << "\tobjectHandle: " << oumh.objectHandle
        << endl
        << "\ttimestamp: " << oumh.timestamp
        << endl
        << "\teventRetractionHandle: " << oumh.eventRetractionHandle
        << endl
        << "\tclassHandle: " << oumh.classHandle
        << endl
        << "\tsendingFederateHandle: " << oumh.sendingFederateHandle
        << endl
        << "\tuserTag: " << oumh.userTag
        << endl
        //      << "\tregionData: " << oumh.regionData
        //      << endl
        << "\ttransportationHandle: " << oumh.transportationHandle
        << endl
        << "\torderingHandle: " << oumh.orderingHandle
        << endl;

      for (int k = 0; k < oumh.messagePayload.length (); ++k) 
        {
          Cubit::HandleValuePair const & hvp=oumh.messagePayload[k];
          o << "\t\thandle: " << hvp.handle
            << endl;
          // bag the data for now.
        }
    }

  return o;
}
#endif
