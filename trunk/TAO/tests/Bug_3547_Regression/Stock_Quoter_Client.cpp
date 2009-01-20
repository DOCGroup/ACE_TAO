// $Id$

#include "tao/Strategies/advanced_resource.h"

// Stock Quoter Stub
#include "Stock_QuoterC.h"
#include "UDPTestC.h"
#include "ace/streams.h"

unsigned char Msg[1000] = { 0 } ;

const ACE_TCHAR *iorFile = ACE_TEXT("file://gazza.ior");

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Use a simple ObjectKey to access the Stock Quoter.
    // (Client ORB must be initialized with
    // -ORBInitRef MyStockQuoter=corbaloc:...)
    CORBA::Object_var stock_quoter_obj =
      orb->resolve_initial_references ("MyStockQuoter");

    CORBA::Object_var object = orb->string_to_object (iorFile);
    UDPTestI_var server = UDPTestI::_narrow (object.in ());

    // Narrow the reference to the Stock_Quoter interface
    //UDPTestI_var UDP = UDPTestI::_narrow (UDP_obj.in());
    Stock_Quoter_var quoter = Stock_Quoter::_narrow (stock_quoter_obj.in());

    memset( Msg, 1, 1000 ) ;
    //UDP->send( 10 ) ;

    server->send( Msg ) ;

    try
      {
        CORBA::Float current_stock_price = quoter->get_quote ("BA");
        cout << "Stock Id: BA, Price = " << current_stock_price << endl;
      }
    catch (Bad_Ticker_Symbol& e)
      {
        cerr << "Caught a bad ticker symbol exception: "
             << e.symbol << endl;;
      }

      quoter->shutdown ();
  }
  catch (CORBA::Exception& e)
  {
    e._tao_print_exception ("Exception caught:");
    return 1;
  }

  return 0;
}
