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
#include <utility>
#include <vector>

constexpr int WAIT_FOR_REGISTRATION_TIME = 10; //sec

namespace
{
  template <typename SOCKET>
  struct Socket_Closer
  {
    SOCKET &socket;

    ~Socket_Closer ()
    {
      if (this->socket.get_handle () != ACE_INVALID_HANDLE)
        {
          this->socket.close ();
        }
    }
  };
}

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
  {
    throw PortableGroup::NoFactory();
  }

  ACE_Read_Buffer read_buf (file.get ());
  ACE_Read_Buffer_Unique_Ptr id_str {nullptr, {read_buf.alloc ()}};
  ACE_Read_Buffer_Unique_Ptr prog {nullptr, {read_buf.alloc ()}};

  for (;;)
    {
      id_str.reset (read_buf.read (' '));
      if (!id_str)
      {
        break;
      }

      prog.reset (read_buf.read ('\n'));
      if (!prog)
      {
        break;
      }

      size_t const id_length = std::strlen (id_str.get ());
      if (id_length == 0)
      {
        continue;
      }
      id_str.get ()[id_length - 1] = '\0';
      if (std::strcmp (id_str.get (), type_id) == 0)
      {
        return create_process (prog.get (), the_criteria, factory_creation_id);
      }
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
  Socket_Closer<ACE_SOCK_Acceptor> const acceptor_closer {acceptor};
  ACE_INET_Addr server_addr {};
  if (acceptor.open (server_addr) == -1
      || acceptor.get_local_addr (server_addr) == -1)
    {
      ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Cannot create event factory listener: %p\n"),
                      ACE_TEXT ("acceptor")));
      return CORBA::Object::_nil ();
    }

  ACE_TString str {};
  str = ACE_TEXT_CHAR_TO_TCHAR (process_str.c_str ());
  std::vector<std::pair<char const *, char const *>> environment;

  // extract the object ID from the criteria
  for (CORBA::ULong i {}; i < the_criteria.length (); ++i)
    {
      CosNaming::Name const &name = the_criteria[i].nam;
      if (name.length () > 0)
        {
          char const *val {};
          char const *id_str = name[0].id.in ();
          if (!(the_criteria[i].val >>= val) || val == nullptr)
            {
              ORBSVCS_ERROR ((LM_ERROR,
                              ACE_TEXT ("Invalid event channel factory criterion\n")));
              return CORBA::Object::_nil ();
            }
          if (id_str[0] != '-')
            {
              // environment variable
              environment.emplace_back (id_str, val);
            }
          else // command line option
            {
              str += ACE_TEXT (" ");
              str += ACE_TEXT_CHAR_TO_TCHAR (id_str);
              str += ACE_TEXT (" ");
              str += ACE_TEXT_CHAR_TO_TCHAR (val);
            }
        }
    }

  ORBSVCS_DEBUG((LM_DEBUG, "Command Line : %s\n", str.c_str()));

  ACE_Process_Options options (true, str.length () + 1);
  std::string::size_type const slash = process_str.find_last_of ('/');
  if (slash != std::string::npos)
    {
      options.working_directory (process_str.substr (0, slash).c_str ());
    }

  constexpr int ENV_BUF_LEN = 512;
  ACE_TCHAR buf[ENV_BUF_LEN] {};
  if (server_addr.addr_to_string (buf, ENV_BUF_LEN, 0) == -1
      || options.setenv (ACE_TEXT ("EventChannelFactoryAddr"),
                         ACE_TEXT ("%") ACE_TEXT_PRIs, buf) == -1)
    {
      ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Cannot configure event factory address\n")));
      return CORBA::Object::_nil ();
    }

  for (std::pair<char const *, char const *> const &entry : environment)
    {
      if (options.setenv (ACE_TEXT_CHAR_TO_TCHAR (entry.first),
                          ACE_TEXT ("%") ACE_TEXT_PRIs,
                          ACE_TEXT_CHAR_TO_TCHAR (entry.second)) == -1)
        {
          ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Cannot set event factory environment\n")));
          return CORBA::Object::_nil ();
        }
    }

  if (options.command_line (ACE_TEXT ("%") ACE_TEXT_PRIs, str.c_str ()) == -1)
    {
      ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Cannot set event factory command line\n")));
      return CORBA::Object::_nil ();
    }

  // Try to create a new process running date.
  ACE_Process new_process {};

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
  Socket_Closer<ACE_SOCK_Stream> const stream_closer {stream};
  ACE_Time_Value timeout { WAIT_FOR_REGISTRATION_TIME };
  ACE_Time_Value const deadline = ACE_OS::gettimeofday () + timeout;

  ORBSVCS_DEBUG((LM_DEBUG, "accepting connection from event channel\n"));
  if (acceptor.accept (stream, std::addressof (client_addr),
                       std::addressof (timeout)) == -1)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR, "accept fail\n"), 0);
  }

  ORBSVCS_DEBUG((LM_DEBUG, "Factory Connect established with %C:%d\n",
    client_addr.get_host_name(), client_addr.get_port_number() ));

  // receive the ior string from the created object

  constexpr int IOR_BUFFER_SIZE = 5000;
  char ior[IOR_BUFFER_SIZE + 1] {};
  size_t byte_read {};
  while (byte_read < IOR_BUFFER_SIZE)
    {
      ACE_Time_Value const now = ACE_OS::gettimeofday ();
      if (now >= deadline)
        {
          return result;
        }
      ACE_Time_Value const remaining = deadline - now;
      ssize_t const received = stream.recv (ior + byte_read,
                                            IOR_BUFFER_SIZE - byte_read,
                                            std::addressof (remaining));
      if (received < 0)
        {
          return result;
        }
      if (received == 0)
        {
          break;
        }
      byte_read += static_cast<size_t> (received);
    }

  if (byte_read == 0)
  {
    return result;
  }

  try
    {
      CORBA::Object_var object { this->orb->string_to_object (ior) };
      if (this->objects.bind (this->id, object) == 0)
      {
        return object._retn ();
      }
    }
  catch (...)
    {
    }

  return nullptr;
}
