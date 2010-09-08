// $Id$

#include "EventChannelFactory_i.h"
#include "ace/Process.h"
#include "ace/Read_Buffer.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "orbsvcs/FtRtEvent/Utils/UUID.h"

const int WAIT_FOR_REGISTRATION_TIME = 10; //sec
const int MAX_ID_STRING = 10;

EventChannelFactory_i::EventChannelFactory_i(const char* conf_filename, CORBA::ORB_ptr orb_ptr)
: conf_file(conf_filename), id(0), orb(orb_ptr)
{
}

CORBA::Object_ptr EventChannelFactory_i::create_object (
  const char * type_id,
  const PortableGroup::Criteria & the_criteria,
  PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id
  )
{

  ACE_DEBUG((LM_DEBUG,"EventChannelFactory_i::create_object\n"));
  FILE* file = 0;
  char *id_str=0, *prog=0;

  try{

    file = ACE_OS::fopen(conf_file, "r");
    if (file == 0)
      throw PortableGroup::NoFactory();

    ACE_Read_Buffer read_buf(file);

    while ((id_str = read_buf.read(' ')) != 0 &&
      (prog = read_buf.read('\n')) != 0) {
        id_str[strlen(id_str)-1] = '\0';
        if (ACE_OS::strcmp(id_str, type_id) == 0) {
          return create_process(prog, the_criteria, factory_creation_id);
      }
    }
  }
  catch (...){
    if (file) ACE_OS::fclose(file);
    if (id_str) ACE_Allocator::instance()->free(id_str);
    if (prog) ACE_Allocator::instance()->free(prog);
    throw;
  }

    throw PortableGroup::ObjectNotCreated();
}

void EventChannelFactory_i::delete_object (
  const PortableGroup::GenericFactory::FactoryCreationId & factory_creation_id
  )
{
  ACE_TRACE("EventChannelFactory_i::delete_object");
  CORBA::ULong object_id;
  factory_creation_id >>= object_id;
  CORBA::Object_var obj;
  if (objects.find(object_id, obj) == 0) {
    objects.unbind(object_id);
    //obj->shutdown();
  }
}

CORBA::Object_ptr EventChannelFactory_i::create_process (
  char * process_str,
  const PortableGroup::Criteria & the_criteria,
  PortableGroup::GenericFactory::FactoryCreationId_out factory_creation_id)
{
  ACE_TRACE("EventChannelFactory_i::create_process");

  CORBA::Object_ptr result = CORBA::Object::_nil();

  // fill the factory_creation_id

  ACE_NEW_RETURN(factory_creation_id,
    PortableGroup::GenericFactory::FactoryCreationId,
    CORBA::Object::_nil());
  *factory_creation_id <<= (CORBA::ULong) ++id;

  // create an acceptor and get the listen address

  ACE_SOCK_Acceptor acceptor;
  ACE_INET_Addr server_addr;
  acceptor.open(server_addr);
  acceptor.get_local_addr(server_addr);

  ACE_Process_Options options;
  ACE_TString str;

  char* pos = ACE_OS::strrchr(process_str, '/');
  if (pos !=0) { //
    *pos = '\0';
    options.working_directory(process_str);
    *pos = '/';
  }
  str = ACE_TEXT_CHAR_TO_TCHAR(process_str);

  const int ENV_BUF_LEN = 512;
  ACE_TCHAR buf[ENV_BUF_LEN];
  server_addr.addr_to_string(buf,ENV_BUF_LEN,0);
  options.setenv(ACE_TEXT("EventChannelFactoryAddr"), buf);

  // extract the object ID from the criteria
  for (size_t i = 0; i < the_criteria.length(); ++i)
  {
    const CosNaming::Name& name = the_criteria[i].nam;
    if (name.length() > 0) {
      const char* val;
      const char* id_str = name[0].id.in();
      the_criteria[i].val >>= val;
      if (id_str[0] != '-') // environment variable
        options.setenv(ACE_TEXT_CHAR_TO_TCHAR(id_str), ACE_TEXT("%s"), val);
      else {// command line option
        ACE_OS::sprintf(buf, ACE_TEXT(" %s %s"), id_str, val);
        str += buf;
      }
    }
  }

  ACE_DEBUG((LM_DEBUG, "Command Line : %s\n", str.c_str()));

  options.command_line(str.c_str());

  // Try to create a new process running date.
  ACE_Process new_process;

  ACE_Time_Value timeout(WAIT_FOR_REGISTRATION_TIME);
  timeout += ACE_OS::gettimeofday();
  if (new_process.spawn (options) == -1)
  {
    int error = ACE_OS::last_error ();
    ACE_ERROR ((LM_ERROR,
      "%p errno = %d.\n",
      str.c_str(),
      error));
    return result;
  }

  ACE_INET_Addr client_addr;
  ACE_SOCK_Stream stream;

  ACE_DEBUG((LM_DEBUG, "accepting connection from event channel\n"));
  if (acceptor.accept(stream, &client_addr, &timeout) == -1)
     ACE_ERROR_RETURN((LM_ERROR, "accept fail\n"), 0);


  ACE_DEBUG((LM_DEBUG, "Factory Connect established with %s:%d\n",
    client_addr.get_host_name(), client_addr.get_port_number() ));

  // receive the ior string from the created object

  char ior[5000] = {'0'};
  int n = 0;
  int byteRead=0;
  while (0 != (n = stream.recv(ior+byteRead, 5000-byteRead)))  {
    byteRead += n;
  }

  if (ACE_OS::strlen(ior) == 0)
    return result;

  try{
    CORBA::Object_var result  = orb->string_to_object(ior);

    if (objects.bind(id, result) == 0){
      return result._retn();
    }
  }
  catch (...){
  }

  return 0;
}

