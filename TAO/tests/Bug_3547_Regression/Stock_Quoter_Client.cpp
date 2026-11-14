#include "tao/Strategies/advanced_resource.h"

// Stock Quoter Stub
#include "Stock_QuoterC.h"
#include "UDPTestC.h"
#include "ace/streams.h"
#include "ace/Task.h"
#include <algorithm>
#include <array>

class OrbTask : public ACE_Task_Base
{
public:
  explicit OrbTask (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }
  OrbTask (OrbTask const &) = delete;
  OrbTask (OrbTask &&) = delete;
  OrbTask &operator= (OrbTask const &) = delete;
  OrbTask &operator= (OrbTask &&) = delete;

  int svc () override
  {
    try
      {
        this->orb_->run ();
      }
    catch (CORBA::Exception const &)
      {
      }
    return 0;
  }

private:
  CORBA::ORB_var const orb_;
};

constexpr int thread_count {1};
constexpr int buffer_size {1000};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB
    CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

    // Use a simple ObjectKey to access the Stock Quoter.
    // (Client ORB must be initialized with
    // -ORBInitRef MyStockQuoter=corbaloc:...)
    CORBA::Object_var const udp_obj =
      orb->resolve_initial_references ("UDPTest");

    CORBA::Object_var const stock_quoter_obj =
      orb->resolve_initial_references ("MyStockQuoter");

    OrbTask task (orb.in ());

    if (task.activate (THR_NEW_LWP | THR_JOINABLE,
                          thread_count) != 0)
       ACE_ERROR_RETURN ((LM_ERROR,
                          "Cannot activate threads\n"),
                         1);

    UDPTestI_var const server = UDPTestI::_narrow (udp_obj.in ());

    Stock_Quoter_var const quoter = Stock_Quoter::_narrow (stock_quoter_obj.in ());

    std::array<CORBA::Octet, buffer_size> message {};
    std::fill (message.begin (), message.end (), CORBA::Octet {1});
    //UDP->send( 10 );

    server->send (message.data ());

    try
      {
        CORBA::Float const current_stock_price = quoter->get_quote ("BA");
        cout << "Stock Id: BA, Price = " << current_stock_price << endl;
      }
      catch (Bad_Ticker_Symbol const &e)
      {
        cerr << "Caught a bad ticker symbol exception: "
             << e.symbol << endl;
      }

    quoter->shutdown ();

    orb->shutdown (true);
    task.wait ();

    orb->destroy ();
  }
  catch (CORBA::Exception const &e)
  {
    e._tao_print_exception ("Exception caught:");
    return 1;
  }

  return 0;
}
