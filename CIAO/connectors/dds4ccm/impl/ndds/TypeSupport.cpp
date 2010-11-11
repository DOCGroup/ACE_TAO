// $Id$

#include "dds4ccm/impl/ndds/TypeSupport.h"

std::map <ACE_CString, ::CIAO::NDDS::DDS_TypeFactory_i*> CIAO::NDDS::DDS_TypeSupport_i::type_factories;

namespace CIAO
{
  namespace NDDS
  {
    void
    DDS_TypeSupport_i::register_type (const char* type, DDS_TypeFactory_i* f, ::DDS::DomainParticipant_ptr)
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_TypeSupport_i::register_type - "
                    "Registered factory for type %C\n", type));

      type_factories [type] = f;
    }

    ::DDS::DataWriter_ptr
     DDS_TypeSupport_i::create_datawriter (DDSDataWriter* dw, ::DDS::DomainParticipant_ptr dp)
    {
      const char* type = dw->get_topic ()->get_type_name();
      DDS_TypeFactory_i *f = type_factories [type];
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datawriter - "
                        "Created DDSDataWriter for type %C\n", type));

          return f->create_datawriter (dw, dp);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::create_datawriter - "
                    "Error to create DDSDataWriter for type %C\n", type));

      return ::DDS::DataWriter::_nil ();
    }

    ::DDS::DataReader_ptr
     DDS_TypeSupport_i::create_datareader (DDSDataReader* dr, ::DDS::DomainParticipant_ptr dp)
    {
      const char* type = dr->get_topicdescription ()->get_type_name();
      DDS_TypeFactory_i *f = type_factories [type];
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datareader - "
                        "Created DDSDataReader for type %C\n", type));

          return f->create_datareader (dr, dp);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::create_datareader - "
                    "Error to create DDSDataReader for type %C\n", type));

      return ::DDS::DataReader::_nil ();
    }

    void
    DDS_TypeSupport_i::close (void)
    {
      for(typefactories::iterator i = type_factories.begin() ; i != type_factories.end(); ++i )
        {
          delete i->second;
          i->second = 0;
        }
      type_factories.clear ();
    }
  }
}

