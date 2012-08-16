// $Id$

#include "dds4ccm/impl/ndds/TypeSupport.h"

namespace CIAO
{
  namespace NDDS
  {
    DDS_TypeFactory_i_var::DDS_TypeFactory_i_var (
      DDS_TypeFactory_i* tf)
      : ref_count_ (1),
        tf_ (tf)
    {
      DDS4CCM_TRACE ("DDS_TypeFactory_i_var::DDS_TypeFactory_i_var");
    }

    DDS_TypeFactory_i_var::~DDS_TypeFactory_i_var (void)
    {
      DDS4CCM_TRACE ("DDS_TypeFactory_i_var::~DDS_TypeFactory_i_var");
    }

    DDS_TypeFactory_i*
    DDS_TypeFactory_i_var::get_factory ()
    {
      DDS4CCM_TRACE ("DDS_TypeFactory_i_var::get_typefactory");

      return tf_;
    }

    long
    DDS_TypeFactory_i_var::_inc_ref ()
    {
      DDS4CCM_TRACE ("DDS_TypeFactory_i_var::_inc_ref");

      return ++this->ref_count_;
    }

    long
    DDS_TypeFactory_i_var::_dec_ref ()
    {
      DDS4CCM_TRACE ("DDS_TypeFactory_i_var::_dec_ref");

      return --this->ref_count_;
    }

    std::map < ::DDS::DomainParticipant_ptr, ::CIAO::NDDS::DDS_TypeSupport_i::typefactories >
      CIAO::NDDS::DDS_TypeSupport_i::participant_factories;

    DDS_TypeFactory_i::~DDS_TypeFactory_i (void)
    {
    }

    DDS_TypeFactory_i*
    DDS_TypeSupport_i::get_factory_i (::DDS::DomainParticipant_ptr dp,
                                      const char* type)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::get_factory_i");

      DDS_TypeFactory_i* tfi = 0;
      participantfactories::iterator entry = participant_factories.find(dp);
      if (entry != participant_factories.end())
        {
          // We have found the domain participant, now search for a type factory
          // within the domain participant
          typefactories tf = entry->second;
          typefactories::iterator it = tf.find(type);
          if (it != tf.end())
            {
              tfi = it->second->get_factory ();
            }
        }

      if (tfi)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_TypeSupport_i::get_factory_i - "
                        "A factory for domain participant <%@> of type <%C> "
                        "has been found\n",
                        dp,
                        type));
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TypeSupport_i::get_factory_i - "
                        "A factory for domain participant <%@> of type <%C> "
                        "has not been found\n",
                        dp,
                        type));
        }

      return tfi;
    }

    bool
    DDS_TypeSupport_i::register_type (::DDS::DomainParticipant_ptr dp,
                                      const char* type, DDS_TypeFactory_i* f)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::register_type");

      participantfactories::iterator dp_entry = participant_factories.find(dp);

      if (dp_entry == participant_factories.end())
        {
          // The domain participant has not been found, insert the domain
          // participant first
          typefactories tf;
          std::pair <participantfactories::iterator, bool> dp_ret = participant_factories.insert(
            std::pair< ::DDS::DomainParticipant_ptr, typefactories > (dp, tf));
          if (!dp_ret.second)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::register_type - "
                            "Unable to create new DomainParticipant entry: "
                            "type <%C> - DomainParticipant <%@>\n",
                            type,
                            dp));
              return false;
            }
        }

      // DomainParticipant is in the list or has been added
      dp_entry = participant_factories.find(dp);
      typefactories tf = dp_entry->second;
      // tf contains the list of type-factory combinations.
      // search for the given type
      typefactories::iterator tf_entry = tf.find(type);
      bool retval = false;
      if (tf_entry == tf.end())
        {
          // Factory not registered for specified type.
          // Register it
          DDS_TypeFactory_i_var *tf_var = 0;
          ACE_NEW_RETURN (tf_var,
                    DDS_TypeFactory_i_var (f),
                    false);

          std::pair <typefactories::iterator, bool> tf_ret =
            tf.insert(typefactories::value_type (type, tf_var));
          if (tf_ret.second)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                            "DDS_TypeSupport_i::register_type - "
                            "Created new factory entry for type <%C> for participant <%@>\n",
                            type, dp));
              // assign the type-factory combination to the DomainParticipant entry.
              dp_entry->second = tf;
              retval = true;
            }
          else
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::register_type - "
                            "Unable to create new factory entry type <%C> for participant <%@>\n",
                            type, dp));
              delete tf_var;
            }
        }
      else
        {
          long const refcount = tf_entry->second->_inc_ref ();
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                        "DDS_TypeSupport_i::register_type - "
                        "Incremented refcount to <%d> for type-factory for participant <%@> since "
                        "it already exists for type <%C>\n",
                        refcount, dp,
                        type));
        }
      return retval;
    }

    void
    DDS_TypeSupport_i::unregister_type (::DDS::DomainParticipant_ptr dp,
                                        const char* type)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::unregister_type");

      participantfactories::iterator dp_entry = participant_factories.find(dp);

      if (dp_entry != participant_factories.end())
        {
          // Found the domain participant
          typefactories tf = dp_entry->second;
          typefactories::iterator it = tf.find(type);
          if (it != tf.end())
            {
              // Let us drop the refcount on the type factory, when it drops
              // to zero we delete it
              long const refcount = it->second->_dec_ref ();
              if (refcount == 0)
                {
                  DDS_TypeFactory_i_var* f_var = it->second;
                  tf.erase(it);
                  dp_entry->second = tf;
                  DDS_TypeFactory_i* f = f_var->get_factory();
                  delete f;
                  delete f_var;
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                                "DDS_TypeSupport_i::unregister_type - "
                                "Deleted type factory for participant <%@> and type <%C> "
                                "refcount dropped to zero\n",
                                dp,
                                type));
                  if (tf.size () == 0UL)
                    { // no more entries -> remove the participant from
                      // the list
                      participant_factories.erase(dp_entry);

                      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                                    "DDS_TypeSupport_i::unregister_type - "
                                    "Erased participant entry for participant <%@>, "
                                    "no type factories left anymore\n",
                                    dp));
                    }
                }
              else
                {
                  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                                "DDS_TypeSupport_i::unregister_type - "
                                "Decremented refcount to <%d> for factory for participant <%@> and type <%C>\n",
                                refcount,
                                dp,
                                type));
                }
            }
          else
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TypeSupport_i::unregister_type - "
                            "Could not find the correct factory belonging to participant <%@> "
                            "and type <%C>. Unable to remove.\n",
                            dp,
                            type));
            }
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TypeSupport_i::unregister_type - "
                        "Could not find the entry for participant <%@>. "
                        "Unable to remove.\n",
                        dp));
        }
    }

    ::DDS::DataWriter_ptr
     DDS_TypeSupport_i::create_datawriter (DDSDataWriter* dw,
                                           ::DDS::DomainParticipant_ptr dp,
                                           ::DDS::Publisher_ptr pub)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::create_datawriter");

      const char* type = dw->get_topic ()->get_type_name();

      DDS_TypeFactory_i *f = get_factory_i(dp, type);
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datawriter - "
                        "Created DDSDataWriter for type <%C> for "
                        "participant <%@>\n", type, dp));

          return f->create_datawriter (dw, dp, pub);
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datawriter - "
                        "Error creating DDSDataWriter for type <%C> "
                        "for participant <%@>\n", type, dp));
        }

      return ::DDS::DataWriter::_nil ();
    }

    ::DDS::DataReader_ptr
     DDS_TypeSupport_i::create_datareader (DDSDataReader* dr,
                                           ::DDS::DomainParticipant_ptr dp,
                                           ::DDS::Subscriber_ptr sub)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::create_datareader");

      const char* type = dr->get_topicdescription ()->get_type_name();
      DDS_TypeFactory_i *f = get_factory_i(dp, type);
      if (f)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datareader - "
                        "Created DDSDataReader for type <%C> "
                        "for participant <%@>\n", type, dp));

          return f->create_datareader (dr, dp, sub);
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TypeSupport_i::create_datareader - "
                        "Error creating DDSDataReader for type <%C> "
                        "for participant <%@>\n", type, dp));
        }

      return ::DDS::DataReader::_nil ();
    }

    void
    DDS_TypeSupport_i::close (void)
    {
      DDS4CCM_TRACE ("DDS_TypeSupport_i::close");

      for(participantfactories::iterator i = participant_factories.begin();
          i != participant_factories.end();
          ++i)
        {
          typefactories tf = i->second;

          for(typefactories::iterator j = tf.begin();
              j != tf.end();
              ++j)
            {
              delete j->second;
            }
          tf.clear();
        }
      participant_factories.clear ();
    }
  }
}

