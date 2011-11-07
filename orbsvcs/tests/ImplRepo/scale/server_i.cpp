// $Id$

#include "server_i.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ImR_Client/ImR_Client.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/streams.h"

class test_i
  : public virtual POA_test
{
  int n_;
  CORBA::ORB_var orb_;
public:
  test_i (CORBA::ORB_ptr orb)
    : n_(0)
    , orb_(CORBA::ORB::_duplicate(orb))
  {
  }
  virtual ~test_i () {
  }
  virtual CORBA::Long get (void)
  {
    ++n_;
    CORBA::Object_var obj = orb_->resolve_initial_references("POACurrent");
    PortableServer::Current_var cur = PortableServer::Current::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(cur.in()));
    PortableServer::POA_var poa = cur->get_POA();
    CORBA::String_var poaname = poa->the_name();

    ACE_DEBUG((LM_DEBUG, "%s: get() %d\n", poaname.in(), n_));
    return n_;
  }
};

// The server name of the Aiprlane Server
static const char DEFAULT_SERVER_NAME[] = "TestObject";

Server_i::Server_i (void)
: server_name_(DEFAULT_SERVER_NAME)
, count_(1)
{
}

Server_i::~Server_i()
{
}

int
Server_i::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:c:h"));
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
    case 'p':  // prefix for all created objects
      this->server_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
      break;
    case 'c':  // Number of imr-ified objects to create.
      this->count_ = ACE_OS::atoi(get_opts.opt_arg());
      break;
    case '?':  // display help for use of the server.
    case 'h':
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
        "usage:  %s"
        " [-c] <count=1> ImR-ified objects."
        " [-p] <prefix=%s> To all ImR-ified object names."
        "\n",
        argv[0],
        DEFAULT_SERVER_NAME),1);
    }
  }

  return 0;
}

namespace
{
  ACE_CString toStr(int n)
  {
    char buf[20];
    return ACE_OS::itoa(n, buf, 10);
  }
}

int
Server_i::init (int argc, ACE_TCHAR** argv)
{
  try
  {
    this->orb_ = CORBA::ORB_init (argc, argv);

    int retval = this->parse_args (argc, argv);
    if (retval != 0)
      return retval;

    CORBA::Object_var obj =
      this->orb_->resolve_initial_references ("RootPOA");
    this->root_poa_ =
      PortableServer::POA::_narrow (obj.in ());
    ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager ();

    obj = this->orb_->resolve_initial_references ("IORTable");
    IORTable::Table_var ior_table =
      IORTable::Table::_narrow (obj.in ());
    ACE_ASSERT(! CORBA::is_nil(ior_table.in()));

    // If -orbuseimr 1 is specified then all persistent poas will be
    // registered with the imr.
    CORBA::PolicyList policies (2);
    policies.length (2);
    policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
    policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

    test_i* test_svt;
    ACE_NEW_RETURN (test_svt, test_i(orb_.in()), -1);
    PortableServer::ServantBase_var scoped_svt(test_svt);

    PortableServer::ObjectId_var server_id =
      PortableServer::string_to_ObjectId ("Test");

    // Create count_ POAs, activate an object in each, and register the object with
    // the IORTable.
    for (int i = 0; i < this->count_; ++i)
    {
      ACE_CString name = this->server_name_ + "_" + toStr(i);

      PortableServer::POA_var poa =
        this->root_poa_->create_POA (name.c_str(),
        poa_manager.in (),
        policies);

      poa->activate_object_with_id (server_id.in (), test_svt);

      TAO_Root_POA* tmp_poa = dynamic_cast<TAO_Root_POA*>(poa.in());
      obj = tmp_poa->id_to_reference_i (server_id.in (), false);

      CORBA::String_var ior = this->orb_->object_to_string (obj.in ());

      ior_table->bind (name.c_str(), ior.in ());
    }

    policies[0]->destroy();
    policies[1]->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Server_i::init");
    throw;
  }


  return 0;
}

int
Server_i::run (void)
{
  try
  {
    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager ();

    poa_manager->activate ();

    // We have potentially lots of IORs, so just write out a simple text
    // file that the run_test.pl can use to know we're done.
    {
      ACE_CString status = this->server_name_ + ACE_CString(".status");
      ofstream out(status.c_str());
      out << "started" << endl;
    }

    ACE_DEBUG ((LM_DEBUG,
      "\n  Started Server %s with %d imr-ified objects.\n\n",
      this->server_name_.c_str(),
      this->count_));

    this->orb_->run ();

    this->root_poa_->destroy(1, 1);
    this->orb_->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Server_i::run");
    throw;
  }


  return 0;
}

