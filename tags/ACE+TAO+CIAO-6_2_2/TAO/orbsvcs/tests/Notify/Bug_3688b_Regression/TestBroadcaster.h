// $Id$
#ifndef testBroadcaster_h
#define testBroadcaster_h

#include "bug3688_export.h"

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include <string>

class bug3688_Export TestBroadcaster
{
  // public types and methods
  public:
    /// Default constructor.
    TestBroadcaster ( );

    /// Destructor.
    ~TestBroadcaster ( )
      throw ();

    bool connect(
      CORBA::ORB_ptr p_orb,
      std::string const & rc_channelIor
    );

    bool disconnect( );

    bool isConnected( ) const;

    bool sendData( );

  // protected types and methods
  protected:

  // private methods and instance variables
  private:
    CosNotifyChannelAdmin::SequenceProxyPushConsumer_var mv_sequenceProxyPushConsumer;

}; /* end of TestBroadcaster */


#endif /* testBroadcaster_h */
