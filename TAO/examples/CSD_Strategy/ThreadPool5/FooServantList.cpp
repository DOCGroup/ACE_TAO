#include "FooServantList.h"
#include "Foo_i.h"
#include "Callback_i.h"
#include "ClientTask.h"
#include "OrbShutdownTask.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include <memory>

FooServantList::FooServantList(ACE_TCHAR const *prefix,
                               unsigned    num_servants,
                               unsigned    num_clients,
                               bool        collocated_test,
                               int         servant_to_deactivate,
                               CORBA::ORB_ptr orb)
  : safe_servants_ (num_servants),
    prefix_(prefix),
    num_servants_(num_servants),
    num_clients_(num_clients),
    init_num_clients_ (num_clients_),
    collocated_test_ (collocated_test),
    servant_to_deactivate_ (servant_to_deactivate),
    orb_ (CORBA::ORB::_duplicate(orb))
{
}

FooServantList::~FooServantList() = default;

void
FooServantList::create_and_activate (PortableServer::POA_ptr poa)
{
  poa_ = PortableServer::POA::_duplicate (poa);

  for (unsigned i = 0; i < this->num_servants_; i++)
    {
      ACE_TCHAR buf[32];
      ACE_OS::sprintf(buf, ACE_TEXT("%02u"), i + 1);
      ACE_TString const servant_name = this->prefix_ + ACE_TEXT("_") + buf;

      this->safe_servants_[i] = new Foo_i (this);

      PortableServer::ObjectId_var id =
                    PortableServer::string_to_ObjectId(ACE_TEXT_ALWAYS_CHAR(servant_name.c_str()));

      poa->activate_object_with_id(id.in(),
                                   this->safe_servants_[i].in());

      CORBA::Object_var obj = poa->id_to_reference(id.in());

      if (CORBA::is_nil(obj.in()))
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) Failed to activate servant (%s).\n"),
                     servant_name.c_str()));
          throw TestException();
        }

      // create the collocated object reference.
      if (this->collocated_test_ && i == 0)
        {
          Foo_var collocated = Foo::_narrow (obj.in ());

          // Create the servant object.
          PortableServer::ServantBase_var safe_servant(new Callback_i ());

          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId("callback");

          poa->activate_object_with_id(id.in(), safe_servant.in());

          CORBA::Object_var callback_obj = poa->id_to_reference(id.in());

          if (CORBA::is_nil(callback_obj.in()))
            {
              ACE_ERROR((LM_ERROR,
                        ACE_TEXT("(%P|%t) Failed to activate servant (%s).\n"),
                        servant_name.c_str()));
              throw TestException();
            }

          Callback_var callback
            = Callback::_narrow (callback_obj.in ());

          this->collocated_client_ = std::make_unique<ClientTask> (
            this->orb_.in (), collocated.in (), callback.in (), true);
          if (collocated_client_->open() != 0)
            {
              ACE_ERROR((LM_ERROR,
                "(%P|%t) Failed to open the collocated client.\n"));
              throw TestException();
            }
        }

      CORBA::String_var ior
        = this->orb_->object_to_string(obj.in());

      ACE_TString const filename = servant_name + ACE_TEXT(".ior");
      std::unique_ptr<FILE, int (*) (FILE *)> ior_file (
        ACE_OS::fopen(filename.c_str(), ACE_TEXT("w")), ACE_OS::fclose);

      if (!ior_file)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) Cannot open output file (%s) for writing IOR."),
                     filename.c_str()));
          throw TestException();
        }
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("(%P|%t) writing IOR to file %s\n"),
                 filename.c_str()));
      ACE_OS::fprintf (ior_file.get (), "%s", ior.in ());
    }
}

void
FooServantList::client_done()
{
  unsigned num_left;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->num_clients_lock_);
    num_left = --this->num_clients_;
  }

  if (num_left == 0)
    {
      if (TheOrbShutdownTask::instance()->open(nullptr) != 0)
        {
          ACE_ERROR((LM_ERROR, "(%P|%t)FooServantList::client_done: "
            "failed to create orb shutdown thread.\n"));
        }
    }
}

// Returns a borrowed pointer; FooServantList owns the collocated client task.
ClientTask *
FooServantList::collocated_client () const
{
  return this->collocated_client_.get ();
}

void
FooServantList::deactivate_servant ()
{
  for (unsigned i = 0; i < this->num_servants_; i++)
    {
      if ((this->servant_to_deactivate_ == 0) ||
           ((this->servant_to_deactivate_ > 0) &&
            (i == static_cast<unsigned> (this->servant_to_deactivate_ - 1))))
        {
          Foo_i *const servant = dynamic_cast<Foo_i *> (safe_servants_[i].in ());
          if (servant->active())
            {
              servant->active(false);
              ACE_DEBUG((LM_DEBUG, "(%P|%t)FooServantList::deactivate_servant "
                "deactivate %uth servant\n", i + 1));

              PortableServer::ObjectId_var id =
                poa_->servant_to_id (safe_servants_[i].in ());

              poa_->deactivate_object (id.in ());

              if (this->num_servants_ == 1)
                {
                  // If there is only one servant and we deactivate it then
                  // all clients will catch exception and we need a way to
                  // shutdown the orb.
                  // Wait for 5 seconds so we can see the requests queued
                  // will be cancelled by deactivate servant.
                  ACE_OS::sleep (5);
                  ACE_DEBUG((LM_DEBUG, "(%P|%t)shutdown ORB\n"));
                  if (TheOrbShutdownTask::instance()->open(nullptr) != 0)
                    {
                      ACE_ERROR((LM_ERROR, "(%P|%t)FooServantList::deactivate_servant: "
                        "failed to create orb shutdown thread.\n"));
                    }
                }
              else
                {
                  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->num_clients_lock_);
                  // The clients that requests this deactivated servant
                  // will catch exception due to the deactivated servant.
                  // We need descrease the num_clients so the alived
                  // servant can be called to shutdown the orb.
                  this->num_clients_ -= this->init_num_clients_ / this->num_servants_;
                }
            }
        }
    }
}

