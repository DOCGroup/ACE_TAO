// $Id$

#include "RTI_IO.h"

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
