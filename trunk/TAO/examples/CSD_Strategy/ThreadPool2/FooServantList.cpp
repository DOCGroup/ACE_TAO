// $Id$
#include "FooServantList.h"
#include "Foo_i.h"
#include "OrbShutdownTask.h"
#include "ace/OS.h"


FooServantList::FooServantList(const ACE_TCHAR* prefix,
                               unsigned    num_servants,
                               unsigned    num_clients,
                               CORBA::ORB_ptr orb)
  : prefix_(prefix),
    num_servants_(num_servants),
    num_clients_(num_clients),
    orb_ (CORBA::ORB::_duplicate(orb))
{
  this->servants_ = new PortableServer::ServantBase_var[num_servants];
}


FooServantList::~FooServantList()
{
  delete [] this->servants_;
}


void
FooServantList::create_and_activate(PortableServer::POA_ptr poa)
{
  for (unsigned i = 0; i < this->num_servants_; i++)
    {
      ACE_TCHAR buf[32];
      ACE_OS::sprintf(buf, ACE_TEXT("%02d"), i + 1);
      ACE_TString servant_name = this->prefix_ + ACE_TEXT("_") + buf;

      this->servants_[i] = new Foo_i(servant_name.c_str(),this);

      PortableServer::ObjectId_var id =
                    PortableServer::string_to_ObjectId(ACE_TEXT_ALWAYS_CHAR(servant_name.c_str()));

      poa->activate_object_with_id(id.in(),
                                   this->servants_[i].in());

      CORBA::Object_var obj = poa->id_to_reference(id.in());

      if (CORBA::is_nil(obj.in()))
        {
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) Failed to activate servant (%s).\n",
                     servant_name.c_str()));
          throw TestException();
        }

      CORBA::String_var ior
        = this->orb_->object_to_string(obj.in());

      ACE_TString filename = servant_name + ACE_TEXT(".ior");
      FILE* ior_file = ACE_OS::fopen(filename.c_str(), "w");

      if (ior_file == 0)
        {
          ACE_ERROR((LM_ERROR,
                     "(%P|%t) Cannot open output file (%s) for writing IOR.",
                     filename.c_str()));
          throw TestException();
        }

      ACE_OS::fprintf(ior_file, "%s", ior.in());
      ACE_OS::fclose(ior_file);
    }
}


void
FooServantList::client_done(void)
{
  unsigned num_left = --this->num_clients_;

  if (num_left == 0)
    {
      if (TheOrbShutdownTask::instance()->open(0) != 0)
        {
          ACE_ERROR((LM_ERROR, "(%P|%t)FooServantList::client_done: "
            "failed to create orb shutdown thread.\n"));
        }
    }
}
