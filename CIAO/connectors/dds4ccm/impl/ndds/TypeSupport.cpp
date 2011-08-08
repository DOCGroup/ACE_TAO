// $Id$

#include "dds4ccm/impl/ndds/TypeSupport.h"

namespace CIAO
{
  namespace NDDS
  {
    std::map < ::DDS::DomainParticipant_ptr, ::CIAO::NDDS::DDS_TypeSupport_i::typefactories >
      CIAO::NDDS::DDS_TypeSupport_i::participant_factories;

    DDS_TypeFactory_i::~DDS_TypeFactory_i (void)
    {
    }

    DDS_TypeFactory_i*
    DDS_TypeSupport_i::get_factory_i (const char* type,
                                      ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::get_factory_i");

      participantfactories::iterator entry = participant_factories.find(dp);
      if (entry != participant_factories.end())
        {
          if (entry->second.size () == 0UL)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                            "DDS_TypeSupport_i::get_factory_i - "
                            "No type-factory combinations for DomainParticipant <%@> found\n",
                            dp));
              return 0;
            }

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_TypeSupport_i::get_factory_i - "
                        "A factory of type <%C> and with DomainParticipant <%@> "
                        "is registered\n",
                        type,
                        dp));

          // Return factory belonging to the specified type.
          typefactories tf = entry->second;
          typefactories::iterator it = tf.find(type);
          if (it != tf.end())
            return it->second;
          else
            return 0;
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_TypeSupport_i::get_factory_i - "
                    "A factory of type <%C> and with DomainParticipant <%@> "
                    "is not registered\n",
                    type,
                    dp));
      return 0;
    }

    bool
    DDS_TypeSupport_i::register_factory_i (const char* type,
                                          DDS_TypeFactory_i* f,
                                          ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::register_factory_i");

      participantfactories::iterator dp_entry = participant_factories.find(dp);

      if (dp_entry == participant_factories.end())
        { // entry not found -> insert.
          // Add the DomainParticipant.
          typefactories tf;
          std::pair <participantfactories::iterator, bool> dp_ret = participant_factories.insert(
            std::pair< ::DDS::DomainParticipant_ptr, typefactories > (dp, tf));
          if (dp_ret.second == false)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::register_factory_i - "
                            "Unable to create new DomainParticipant entry: "
                            "type <%C> - DomainParticipant <%@>\n",
                            type,
                            dp));
              return false;
            }
        }

      // DomainParticipant has been added
      dp_entry = participant_factories.find(dp);
      typefactories tf = dp_entry->second;
      // tf contains the list of type-factory combinations.
      // search for the given type
      typefactories::iterator tf_entry = tf.find(type);
      if (tf_entry == tf.end())
        { // Factory not registered for specified type.
          // Register it
          std::pair <typefactories::iterator, bool> tf_ret =
            tf.insert(typefactories::value_type (type, f));
          if (tf_ret.second == false)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::register_factory_i - "
                            "Unable to create new factory entry type <%C>\n",
                            type));
              return false;
            }
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_TypeSupport_i::register_factory_i - "
                        "Created new factory entry for type <%C>\n",
                        type));
          // assign the type-factory combination to the DomainParticipant entry.
          dp_entry->second = tf;
          return true;
        }
      // Unable to register type-factory combination since it already exists.
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::register_factory_i - "
                    "Unable to register type-factory combination since "
                    "it already exists for type <%C>\n",
                    type));
      return false;
    }

    void
    DDS_TypeSupport_i::unregister_participant_factory_i (const char * type,
                                                        ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::unregister_participant_factory_i");

      participantfactories::iterator dp_entry = participant_factories.find(dp);

      if (dp_entry != participant_factories.end())
        {
          typefactories tf = dp_entry->second;
          typefactories::iterator it = tf.find(type);
          if (it != tf.end())
            {
              tf.erase(it);
              if (tf.size () == 0UL)
                { // no more entries -> remove the participant from
                  // the list
                  participant_factories.erase(dp_entry);

                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                                "DDS_TypeSupport_i::unregister_participant_factory_i - "
                                "Erased entry for participant <%@>\n",
                                dp,
                                type));
                }
              else
                {
                  // assign the type-factory combination to the DomainParticipant entry.
                  dp_entry->second = tf;
                }
            }
          else
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::unregister_participant_factory_i - "
                            "Could not find the correct factory belonging to participant <%@> "
                            "and type <%C>. Unable to remove.\n",
                            dp,
                            type));
            }
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TypeSupport_i::unregister_participant_factory_i - "
                        "Could not find the entry for participant <%@>. "
                        "Unable to remove.\n",
                        dp));
        }
    }


    bool
    DDS_TypeSupport_i::register_type (const char* type, DDS_TypeFactory_i* f,
                                      ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::register_type");

      if (get_factory_i (type, dp))
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::register_type - "
                        "Won't register factory for type <%C> since it already exist\n",
                        type));
          return false;
        }
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_TypeSupport_i::register_type - "
                    "Registered factory for type <%C> and participant <%@>\n",
                    type, dp));

      return register_factory_i (type, f, dp);
    }

    DDS_TypeFactory_i*
    DDS_TypeSupport_i::unregister_type (const char* type, ::DDS::DomainParticipant_ptr dp)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::unregister_type");

      DDS_TypeFactory_i * f = get_factory_i(type, dp);
      unregister_participant_factory_i(type, dp);

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                    "DDS_TypeSupport_i::unregister_type - "
                    "Unregistered factory for type <%C> and participant <%@>\n",
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

      DDS_TypeFactory_i *f = get_factory_i(type, dp);
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datawriter - "
                        "Created DDSDataWriter for type %C\n", type));

          return f->create_datawriter (dw, dp, pub);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::create_datawriter - "
                    "Error creating DDSDataWriter for type <%C>\n", type));

      return ::DDS::DataWriter::_nil ();
    }

    ::DDS::DataReader_ptr
     DDS_TypeSupport_i::create_datareader (DDSDataReader* dr,
                                           ::DDS::DomainParticipant_ptr dp,
                                           ::DDS::Subscriber_ptr sub)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::create_datareader");

      const char* type = dr->get_topicdescription ()->get_type_name();
      DDS_TypeFactory_i *f = get_factory_i(type, dp);
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datareader - "
                        "Created DDSDataReader for type <%C>\n", type));

          return f->create_datareader (dr, dp, sub);
        }

      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TypeSupport_i::create_datareader - "
                    "Error creating DDSDataReader for type <%C>\n", type));

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

          for(size_t j = tf.size() - 1; j != 0; j--)
            {
              delete tf[j];
            }
          tf.clear();
        }
      participant_factories.clear ();
    }
  }
}

