#include "server_i.h"

#include "tao/ImR_Client/ImR_Client.h"

#include "orbsvcs/CosNamingC.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/streams.h"

class test_i : public virtual POA_test
{
  int n_;
public:
  test_i ()
    : n_(0)
  {
  }

  virtual ~test_i ()
  {
  }

  virtual CORBA::Long get (void)
  {
    ++n_;
    return n_;
  }
};

Server_i::Server_i (void)
  : server_name_()
  , second_name_()
{
}

Server_i::~Server_i()
{

  this->root_poa_->destroy(1, 1);
  this->orb_->destroy();
}

int
Server_i::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'd':
          this->second_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
          break;
        case 's':
          this->server_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s"
                             " [-d <name>] name of the server to depend on."
                             " -s <name> name of this server."
                             "\n",
                             argv[0]),1);
        }
    }

  return 0;
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
      this->root_poa_ = PortableServer::POA::_narrow (obj.in ());
      ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

      PortableServer::POAManager_var poa_manager =
        this->root_poa_->the_POAManager ();

      // If -orbuseimr 1 is specified then all persistent poas will be
      // registered with the imr.
      CORBA::PolicyList policies (2);
      policies.length (2);
      policies[0] = this->root_poa_->create_id_assignment_policy (PortableServer::USER_ID);
      policies[1] = this->root_poa_->create_lifespan_policy (PortableServer::PERSISTENT);

      test_i* test_svt;
      ACE_NEW_RETURN (test_svt, test_i, -1);
      PortableServer::ServantBase_var scoped_svt(test_svt);

      PortableServer::ObjectId_var server_id =
        PortableServer::string_to_ObjectId ("Test");

      PortableServer::POA_var poa =
        this->root_poa_->create_POA (this->server_name_.c_str(),
                                     poa_manager.in (),
                                     policies);

      poa->activate_object_with_id (server_id.in (), test_svt);
      obj = poa->id_to_reference (server_id.in ());

      policies[0]->destroy();
      policies[1]->destroy();

      CORBA::Object_var nsobj =
        this->orb_->resolve_initial_references ("NameService");
      CosNaming::NamingContext_var nc =
        CosNaming::NamingContext::_narrow (nsobj.in ());
      CosNaming::Name n(1);
      n.length (1);
      n[0].id = CORBA::string_dup (this->server_name_.c_str ());

      try
        {
          nsobj = nc->resolve (n);
        }
      catch (CosNaming::NamingContext::NotFound &)
        {
          nc->bind (n, obj.in ());
          return 1;
        }
      if (this->second_name_.length ())
        {
          n[0].id = CORBA::string_dup (this->second_name_.c_str ());

          try
            {
              nsobj = nc->resolve (n);
            }
          catch (CosNaming::NamingContext::NotFound &)
            {
              return 1;
            }
          test_var test = test::_narrow(nsobj.in());
          test->get();
        }
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

      {
        ACE_CString status = this->server_name_ + ACE_CString(".status");
        ofstream out(status.c_str());
        out << "started" << endl;
      }

      this->orb_->run ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server_i::run");
      throw;
    }


  return 0;
}
