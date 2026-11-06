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
    TestBroadcaster ();
    TestBroadcaster (TestBroadcaster const&) = delete;
    TestBroadcaster (TestBroadcaster&&) = delete;
    TestBroadcaster& operator= (TestBroadcaster const&) = delete;
    TestBroadcaster& operator= (TestBroadcaster&&) = delete;

    /// Destructor.
    ~TestBroadcaster () noexcept;

    bool connect (
      CORBA::ORB_ptr orb,
      std::string const& channel_ior);

    bool disconnect ();

    bool is_connected () const;

    bool send_data ();

  // protected types and methods
  protected:
  // private methods and instance variables
  private:
    CosNotifyChannelAdmin::SequenceProxyPushConsumer_var sequence_proxy_push_consumer {};
}; /* end of TestBroadcaster */


#endif /* testBroadcaster_h */
