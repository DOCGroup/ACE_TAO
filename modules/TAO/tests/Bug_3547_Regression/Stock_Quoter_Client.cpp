// $Id$

#include "tao/Strategies/advanced_resource.h"

// Stock Quoter Stub
#include "Stock_QuoterC.h"
#include "UDPTestC.h"
#include "ace/streams.h"
#include "ace/Task.h"

class OrbTask : public ACE_Task_Base
{
public:
  OrbTask(const CORBA::ORB_ptr orb)
      : orb_(CORBA::ORB::_duplicate(orb))
  {
  }

  virtual int svc()
  {
      try
        {
          this->orb_->run ();
        }
      catch (const CORBA::Exception&)
        {
        }
      return 0;
  }

private:
  CORBA::ORB_var orb_;
};

static int n_threads = 1;

unsigned char Msg[1000] = { 0 } ;

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB
    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    // Use a simple ObjectKey to access the Stock Quoter.
    // (Client ORB must be initialized with
    // -ORBInitRef MyStockQuoter=corbaloc:...)
    CORBA::Object_var udp_obj =
      orb->resolve_initial_references ("UDPTest");

    CORBA::Object_var stock_quoter_obj =
      orb->resolve_initial_references ("MyStockQuoter");

    OrbTask task(orb.in());

    if (task.activate (THR_NEW_LWP | THR_JOINABLE,
                          n_threads) != 0)
       ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot activate threads\n"),
                         1);

    UDPTestI_var server = UDPTestI::_narrow (udp_obj.in ());

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

    task.wait();

    orb->destroy ();
  }
  catch (CORBA::Exception& e)
  {
    e._tao_print_exception ("Exception caught:");
    return 1;
  }

  return 0;
}
