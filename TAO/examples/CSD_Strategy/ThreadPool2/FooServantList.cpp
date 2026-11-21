#include "FooServantList.h"
#include "Foo_i.h"
#include "OrbShutdownTask.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

FooServantList::FooServantList (ACE_TCHAR const *prefix,
                                unsigned num_servants,
                                unsigned num_clients,
                                CORBA::ORB_ptr orb)
  : servants_ (num_servants),
    prefix_ (prefix),
    num_clients_ (num_clients),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
FooServantList::create_and_activate (PortableServer::POA_ptr poa)
{
  for (unsigned i = 0; i < this->servants_.size (); ++i)
    {
      ACE_TCHAR buf[32] {};
      ACE_OS::snprintf (buf, sizeof (buf) / sizeof (ACE_TCHAR), ACE_TEXT ("%02u"), i + 1);
      ACE_TString const servant_name = this->prefix_ + ACE_TEXT ("_") + buf;

      this->servants_[i] = new Foo_i (servant_name.c_str (), this);

      PortableServer::ObjectId_var const id = PortableServer::string_to_ObjectId (
        ACE_TEXT_ALWAYS_CHAR (servant_name.c_str ()));

      poa->activate_object_with_id (id.in (), this->servants_[i].in ());

      CORBA::Object_var const obj = poa->id_to_reference (id.in ());

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                     "(%P|%t) Failed to activate servant (%s).\n",
                     servant_name.c_str ()));
          throw TestException ();
        }

      CORBA::String_var const ior = this->orb_->object_to_string (obj.in ());

      ACE_TString const filename = servant_name + ACE_TEXT (".ior");
      std::unique_ptr<FILE, int (*) (FILE *)> const ior_file (
        ACE_OS::fopen (filename.c_str (), "w"), ACE_OS::fclose);
      if (!ior_file)
        {
          ACE_ERROR ((LM_ERROR,
                     "(%P|%t) Cannot open output file (%s) for writing IOR.",
                     filename.c_str ()));
          throw TestException ();
        }

      ACE_OS::fprintf (ior_file.get (), "%s", ior.in ());
    }
}

void
FooServantList::client_done ()
{
  unsigned const num_left = this->num_clients_.fetch_sub (1, std::memory_order_acq_rel) - 1;

  if (num_left == 0)
    {
      if (TheOrbShutdownTask::instance ()->open (nullptr) != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) FooServantList::client_done: "
            "failed to create orb shutdown thread.\n"));
        }
    }
}
