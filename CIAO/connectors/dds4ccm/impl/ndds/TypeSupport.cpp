// $Id$

#include "dds4ccm/impl/ndds/TypeSupport.h"

std::map < ::DDS::DomainParticipant_ptr, ::CIAO::NDDS::DDS_TypeSupport_i::typefactories >
  CIAO::NDDS::DDS_TypeSupport_i::participant_factories;

namespace CIAO
{
  namespace NDDS
  {
    DDS_TypeFactory_i::~DDS_TypeFactory_i (void)
    {
    }

    DDS_TypeFactory_i*
    DDS_TypeSupport_i::get_factory (const char* type,
                                    ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::get_factory");

      participantfactories::iterator entry = participant_factories.find(dp);
      if (entry != participant_factories.end())
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_TypeSupport_i::get_factory - "
                        "A factory of type <%C> and with DomainParticipant <%@> "
                        "is registred\n",
                        type,
                        dp));
          if (entry->second.size () == 0)
            return 0;

          // Return factory belonging to the specified type.
          return entry->second[type];
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_TypeSupport_i::get_factory - "
                    "A factory of type <%C> and with DomainParticipant <%@> "
                    "was not registred\n",
                    type,
                    dp));
      return 0;
    }

    bool
    DDS_TypeSupport_i::set_factory (const char* type,
                                    DDS_TypeFactory_i* f,
                                    ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::set_factory");

      participantfactories::iterator dp_entry = participant_factories.find(dp);
      if (dp_entry == participant_factories.end())
        { // entry not found -> insert.
          // first create a new type factory for this dp.
          typefactories tf;
          std::pair <typefactories::iterator, bool> tf_ret =
            tf.insert(typefactories::value_type (type, f));
          if (tf_ret.second == false)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::set_factory - "
                            "Unable to create new Type factory for type <%C>\n",
                            type));
              return false;
            }

          std::pair <participantfactories::iterator, bool> dp_ret =
            participant_factories.insert(
              std::pair< ::DDS::DomainParticipant_ptr, typefactories > (dp, tf));
          if (dp_ret.second == false)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::set_factory - "
                            "Unable to create new DomainParticipant entry type <%C>\n",
                            type));
              return false;
            }
        }
      return true;
    }

    void
    DDS_TypeSupport_i::remove_participant_factory(const char * type,
                                                  ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::remove_participant_factory");

      participantfactories::iterator dp_entry = participant_factories.find(dp);

      if (dp_entry != participant_factories.end())
        {
          // initialize every factory to nil
          typefactories tf = dp_entry->second;
          typefactories::iterator it = tf.find(type);
          if (it != tf.end())
            {
              it->second = 0;
            }
          else
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::remove_participant_factory - "
                            "Could not find the correct factory belonging to participant <%@> "
                            "and type <%C>. Unable to remove.\n",
                            dp,
                            type));
            }

          // erase the participant from the map
          participant_factories.erase(dp_entry);

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_TypeSupport_i::remove_participant_factory - "
                        "Erased entry for participant <%@>\n",
                        dp,
                        type));
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TypeSupport_i::remove_participant_factory - "
                        "Could not find the correct factory belonging to participant <%@>. "
                        "Unable to remove.\n",
                        dp));
        }
    }


    bool
    DDS_TypeSupport_i::register_type (const char* type, DDS_TypeFactory_i* f,
                                      ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::register_type");

      if (get_factory (type, dp))
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::register_type - "
                        "Won't register factory for type %C since it already exist\n",
                        type));
          return false;
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_TypeSupport_i::register_type - "
                    "Registered factory for type %C and participant <%@>\n",
                    type, dp));

      return set_factory (type, f, dp);
    }

    DDS_TypeFactory_i*
    DDS_TypeSupport_i::unregister_type (const char* type, ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::unregister_type");

      DDS_TypeFactory_i * f = get_factory(type, dp);
      remove_participant_factory (type, dp);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_TypeSupport_i::unregister_type - "
                    "Unregistered factory for type %C and participant <%@>\n",
                    type, dp));
      return f;
    }

    ::DDS::DataWriter_ptr
     DDS_TypeSupport_i::create_datawriter (DDSDataWriter* dw,
                                           ::DDS::DomainParticipant_ptr dp,
                                           ::DDS::Publisher_ptr pub)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::create_datawriter");

      const char* type = dw->get_topic ()->get_type_name();

      DDS_TypeFactory_i *f = get_factory (type, dp);
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datawriter - "
                        "Created DDSDataWriter for type %C\n", type));

          return f->create_datawriter (dw, dp, pub);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::create_datawriter - "
                    "Error creating DDSDataWriter for type %C\n", type));

      return ::DDS::DataWriter::_nil ();
    }

    ::DDS::DataReader_ptr
     DDS_TypeSupport_i::create_datareader (DDSDataReader* dr,
                                           ::DDS::DomainParticipant_ptr dp,
                                           ::DDS::Subscriber_ptr sub)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::create_datareader");

      const char* type = dr->get_topicdescription ()->get_type_name();
      DDS_TypeFactory_i *f = get_factory (type, dp);
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datareader - "
                        "Created DDSDataReader for type %C\n", type));

          return f->create_datareader (dr, dp, sub);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::create_datareader - "
                    "Error creating DDSDataReader for type %C\n", type));

      return ::DDS::DataReader::_nil ();
    }

    void
    DDS_TypeSupport_i::close (void)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::close");

      for(participantfactories::iterator i = participant_factories.begin();
          i != participant_factories.end();
          ++i )
        {
          typefactories tf = i->second;
          for(typefactories::iterator j = tf.begin();
              j != tf.end();
              ++j )
            {
              delete j->second;
              j->second = 0;
            }
        }
      participant_factories.clear ();
    }
  }
}

