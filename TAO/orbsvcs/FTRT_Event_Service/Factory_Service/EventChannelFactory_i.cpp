#include "orbsvcs/Log_Macros.h"
#include "EventChannelFactory_i.h"
#include "ace/Process.h"
#include "ace/Read_Buffer.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "orbsvcs/FtRtEvent/Utils/UUID.h"
#include "ace/OS_NS_sys_time.h"

#include <cstring>
#include <memory>

constexpr int WAIT_FOR_REGISTRATION_TIME = 10; //sec

EventChannelFactory_i::EventChannelFactory_i (std::string const &conf_filename,
                                              CORBA::ORB_ptr orb_ptr)
  : conf_file (conf_filename)
  , id {}
  , orb (orb_ptr)
{
}

CORBA::Object_ptr EventChannelFactory_i::create_object (
  char const *type_id,
  PortableGroup::Criteria const &the_criteria,
  PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id)
{
  ORBSVCS_DEBUG((LM_DEBUG,"EventChannelFactory_i::create_object\n"));
  std::unique_ptr<FILE, int (*) (FILE *)> file {
    ACE_OS::fopen (this->conf_file.c_str (), "r"), ACE_OS::fclose
  };
  if (!file)
    throw PortableGroup::NoFactory();

  ACE_Read_Buffer read_buf (file.get ());
  std::unique_ptr<char, void (*) (char *)> id_str {
    nullptr,
    [] (char *buffer) { ACE_Allocator::instance ()->free (buffer); }
  };
  std::unique_ptr<char, void (*) (char *)> prog {
    nullptr,
    [] (char *buffer) { ACE_Allocator::instance ()->free (buffer); }
  };

  for (;;)
    {
      id_str.reset (read_buf.read (' '));
      if (!id_str)
        break;

      prog.reset (read_buf.read ('\n'));
      if (!prog)
        break;

      id_str.get ()[std::strlen (id_str.get ()) - 1] = '\0';
      if (std::strcmp (id_str.get (), type_id) == 0)
        return create_process (prog.get (), the_criteria, factory_creation_id);
    }

  throw PortableGroup::ObjectNotCreated ();
}

void EventChannelFactory_i::delete_object (
  PortableGroup::GenericFactory::FactoryCreationId const &factory_creation_id)
{
  ACE_TRACE("EventChannelFactory_i::delete_object");
  CORBA::ULong object_id {};
  factory_creation_id >>= object_id;
  CORBA::Object_var obj {};
  if (objects.find (object_id, obj) == 0)
    {
      objects.unbind (object_id);
      //obj->shutdown();
    }
}

CORBA::Object_ptr EventChannelFactory_i::create_process (
  std::string const &process_str,
  PortableGroup::Criteria const &the_criteria,
  PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id)
{
  ACE_TRACE("EventChannelFactory_i::create_process");

  CORBA::Object_ptr result { CORBA::Object::_nil () };

  // fill the factory_creation_id

  ACE_NEW_RETURN(factory_creation_id,
    PortableGroup::GenericFactory::FactoryCreationId,
    CORBA::Object::_nil());
  *factory_creation_id <<= static_cast<CORBA::ULong> (++this->id);

  // create an acceptor and get the listen address

  ACE_SOCK_Acceptor acceptor {};
  ACE_INET_Addr server_addr {};
  acceptor.open(server_addr);
  acceptor.get_local_addr(server_addr);

  ACE_Process_Options options {};
  ACE_TString str {};

  std::string::size_type const slash = process_str.find_last_of ('/');
  if (slash != std::string::npos)
    options.working_directory (process_str.substr (0, slash).c_str ());
  str = ACE_TEXT_CHAR_TO_TCHAR (process_str.c_str ());

  constexpr int ENV_BUF_LEN = 512;
  ACE_TCHAR buf[ENV_BUF_LEN] {};
  server_addr.addr_to_string (buf, ENV_BUF_LEN, 0);
  options.setenv (ACE_TEXT("EventChannelFactoryAddr"),
                 ACE_TEXT("%") ACE_TEXT_PRIs, buf);

  // extract the object ID from the criteria
  for (CORBA::ULong i {}; i < the_criteria.length (); ++i)
    {
      CosNaming::Name const &name = the_criteria[i].nam;
      if (name.length () > 0)
        {
          char const *val {};
          char const *id_str = name[0].id.in ();
          the_criteria[i].val >>= val;
          if (id_str[0] != '-') // environment variable
            options.setenv (ACE_TEXT_CHAR_TO_TCHAR (id_str),
                            ACE_TEXT("%") ACE_TEXT_PRIs, val);
          else // command line option
            {
              ACE_OS::sprintf (buf, ACE_TEXT(" %s %s"), id_str, val);
              str += buf;
            }
        }
    }

  ORBSVCS_DEBUG((LM_DEBUG, "Command Line : %s\n", str.c_str()));

  options.command_line (ACE_TEXT("%") ACE_TEXT_PRIs, str.c_str ());

  // Try to create a new process running date.
  ACE_Process new_process {};

  ACE_Time_Value timeout { WAIT_FOR_REGISTRATION_TIME };
  timeout += ACE_OS::gettimeofday();
  if (new_process.spawn (options) == -1)
  {
    ORBSVCS_ERROR ((LM_ERROR,
      "%p errno = %d.\n",
      str.c_str(),
      ACE_OS::last_error ()));
    return result;
  }

  ACE_INET_Addr client_addr {};
  ACE_SOCK_Stream stream {};

  ORBSVCS_DEBUG((LM_DEBUG, "accepting connection from event channel\n"));
  if (acceptor.accept(stream, &client_addr, &timeout) == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR, "accept fail\n"), 0);

  ORBSVCS_DEBUG((LM_DEBUG, "Factory Connect established with %s:%d\n",
    client_addr.get_host_name(), client_addr.get_port_number() ));

  // receive the ior string from the created object

  constexpr int IOR_BUFFER_SIZE = 5000;
  char ior[IOR_BUFFER_SIZE + 1] {};
  int n {};
  int byte_read {};
  while (byte_read < IOR_BUFFER_SIZE &&
         (n = stream.recv (ior + byte_read, IOR_BUFFER_SIZE - byte_read)) > 0)
    byte_read += n;

  if (n < 0 || byte_read == 0)
    return result;

  try
    {
      CORBA::Object_var object { this->orb->string_to_object (ior) };
      if (this->objects.bind (this->id, object) == 0)
        return object._retn ();
    }
  catch (...)
    {
    }

  return nullptr;
}
