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
  virtual CORBA::Long get (ACE_ENV_SINGLE_ARG_DECL) ACE_THROW_SPEC ((CORBA::SystemException)) {
    ++n_;
    CORBA::Object_var obj = orb_->resolve_initial_references("POACurrent" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    PortableServer::Current_var cur = PortableServer::Current::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    ACE_ASSERT(! CORBA::is_nil(cur.in()));
    PortableServer::POA_var poa = cur->get_POA(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    CORBA::String_var poaname = poa->the_name(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

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
Server_i::parse_args (int argc, char* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:c:h");
  int c;

  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
    case 'p':  // prefix for all created objects
      this->server_name_ = get_opts.opt_arg ();
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
Server_i::init (int argc, char** argv ACE_ENV_ARG_DECL)
{
  ACE_TRY
  {
    this->orb_ = CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    int retval = this->parse_args (argc, argv);
    if (retval != 0)
      return retval;

    CORBA::Object_var obj =
      this->orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    this->root_poa_ =
      PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    obj = this->orb_->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    IORTable::Table_var ior_table =
      IORTable::Table::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(ior_table.in()));

    // If -orbuseimr 1 is specified then all persistent poas will be
    // registered with the imr.
    CORBA::PolicyList policies (2);
    policies.length (2);
    policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

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
        policies
        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa->activate_object_with_id (server_id.in (), test_svt ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Root_POA* tmp_poa = dynamic_cast<TAO_Root_POA*>(poa.in());
      obj = tmp_poa->id_to_reference_i (server_id.in (), false ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = this->orb_->object_to_string (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ior_table->bind (name.c_str(), ior.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }

    policies[0]->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    policies[1]->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i::init");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Server_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
  {
    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

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

    this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->root_poa_->destroy(1, 1);
    this->orb_->destroy();
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Server_i::run");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}

