// $Id$

#include "Identification_Service.h"
#include "../Utils/UUID.h"

ACE_RCSID (FTRT_EventChannel,
           Identification_Service,
           "$Id$")


namespace FTRTEC {

  namespace {
    Identification_Service* service;
    const char oid[] = "\xF8\xB3\xB1\xFE\xAC\xC6\x07\x11\x02\x0C\x02\x50\xEB\x05\x77\xD0";
  }

  Identification_Service::Identification_Service()
  {
  }

  Identification_Service::~Identification_Service()
  {
  }


  Identification_Service* Identification_Service::instance()
  {
    return service;
  }

  int Identification_Service::init(int argc, ACE_TCHAR* argv[])
  {
    if (service != 0)
      return 0;
    name_.length(1);
    name_[0].id = CORBA::string_dup("FT_EventService");

    while (argc > 1) {
      if ( ACE_OS::strcasecmp (argv[0], ACE_LIB_TEXT("-Object_ID")) == 0) {
        --argc; ++argv;
        if (argv[0][0] == '-') continue;
        else if (argv[0][0] != '$') {
          UUID uuid(argv[0]);
          if (!uuid.is_valid())
            ACE_ERROR_RETURN((LM_ERROR, "Invalid Object_ID\n"), -1);
          object_id_.length(16);
          uuid.to_binary(&object_id_[0]);
        }
        --argc; ++argv;
      }
      else if (ACE_OS::strcasecmp (argv[0], ACE_LIB_TEXT("-Name")) ==0) {
        --argc; ++argv;
        if (argv[0][0] == '-') continue;
        else if (argv[0][0] != '$') {
          name_[0].id = CORBA::string_dup(argv[0]);
        }
        --argc; ++argv;
      }
    }

    if (object_id_.length() == 0) {
      // assign an default value for object id
      object_id_.length(16);
      memcpy(&object_id_[0], oid, 16);
    }
    service = this;
    return 0;
  }


  const PortableServer::ObjectId& Identification_Service::object_id() const
  {
    return object_id_;
  }


  const CosNaming::Name& Identification_Service::name() const
  {
    return name_;
  }

  ACE_FACTORY_DEFINE (TAO_FTRTEC, Identification_Service);

  ACE_STATIC_SVC_DEFINE (Identification_Service,
    ACE_TEXT ("FTRTEC_Identification"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (Identification_Service),
    ACE_Service_Type::DELETE_THIS
    | ACE_Service_Type::DELETE_OBJ,
    0);
}
