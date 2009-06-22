// $Id$

#include "MessengerC.h"
#include "ClientInitializer.h"

#include "tao/ORBInitializer_Registry.h"
// Ensure that the PI library is linked in when building statically
#include "tao/PI/PI.h"
#include <iostream>

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      ClientInitializer* temp_initializer = new ClientInitializer;

      PortableInterceptor::ORBInitializer_var orb_initializer =
        temp_initializer;

      PortableInterceptor::register_orb_initializer (orb_initializer.in ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "Client ORB");

      // Now that the ORB is initialized (and subsequently the
      // PICurrent), we can set the slot data on the PICurrent for our
      // interceptor initializer.
      temp_initializer->set_slot_data ();

      CORBA::Object_var obj = orb->string_to_object( "file://Messenger.ior" );
      if ( CORBA::is_nil(obj.in() ) ) {
        std::cerr << "Nil Messenger reference" << std::endl;
        return 1;
      }
      // Narrow
      Messenger_var messenger = Messenger::_narrow( obj.in() );
      if( CORBA::is_nil( messenger.in() ) ) {
        std::cerr << "Not a Messenger reference" << std::endl;
        return 1;
      }

      CORBA::String_var message = CORBA::string_dup( "Hello!" );
      messenger->send_message( "TAO User", "TAO Test", message.inout() );
    }

  catch(const CORBA::Exception& ex)
    {
      std::cerr << "client Caught CORBA exception: " << ex << std::endl;
      return 1;
    }

  return 0;
}
