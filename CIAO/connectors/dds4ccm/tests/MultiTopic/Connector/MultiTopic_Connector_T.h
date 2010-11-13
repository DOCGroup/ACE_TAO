// $Id$

#ifndef DDS_MT_CONNECTOR_T
#define DDS_MT_CONNECTOR_T

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_MT_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>
{
    //square
    virtual
    typename CCM_TYPE::writer_type::_ptr_type
    get_supplier_sq_data (void)
    {
      return CCM_TYPE::writer_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datawriter_type::_ptr_type
    get_supplier_sq_dds_entity (void)
    {
      return CCM_TYPE::datawriter_type::_nil ();
    }

    virtual
    typename CCM_TYPE::getter_type::_ptr_type
    get_pull_consumer_sq_fresh_data (void)
    {
      return CCM_TYPE::getter_type::_nil ();
    }

    virtual
    typename CCM_TYPE::reader_type::_ptr_type
    get_pull_consumer_sq_data (void)
    {
      return CCM_TYPE::reader_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datareader_type::_ptr_type
    get_pull_consumer_sq_dds_entity (void)
    {
      return CCM_TYPE::datareader_type::_nil ();
    }

    virtual
    CCM_DDS::QueryFilter*
    pull_consumer_sq_filter (void)
    {
      return 0;
    }
    virtual
    void
    pull_consumer_sq_filter (const CCM_DDS::QueryFilter&)
    {
    }
    virtual
    ::CCM_DDS::CCM_ContentFilterSetting_ptr
     //CCM_DDS::CCM_ContentFilterSetting*
    get_pull_consumer_sq_filter_config(void)
    {
      return CCM_DDS::CCM_ContentFilterSetting::_nil ();
    }
    virtual
    CCM_DDS::CCM_ContentFilterSetting*
    get_push_consumer_sq_filter_config(void)
    {
      return CCM_DDS::CCM_ContentFilterSetting::_nil ();
    }

    virtual
    CCM_DDS::QueryFilter *
    push_consumer_sq_filter (void)
    {
      return 0;
    }

    virtual
    void
    push_consumer_sq_filter (const ::CCM_DDS::QueryFilter &)
    {
    }

    virtual
    typename CCM_TYPE::reader_type::_ptr_type
    get_push_consumer_sq_data (void)
    {
      return CCM_TYPE::reader_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datalistenercontrol_type::_ptr_type
    get_push_consumer_sq_data_control (void)
    {
      return CCM_TYPE::datalistenercontrol_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datareader_type::_ptr_type
    get_push_consumer_sq_dds_entity (void)
    {
      return CCM_TYPE::datareader_type::_nil ();
    }

//triangel
    virtual
    typename CCM_TYPE::writer_type::_ptr_type
    get_supplier_tr_data (void)
    {
      return CCM_TYPE::writer_type::_nil ();
    }
    virtual
    typename CCM_TYPE::datawriter_type::_ptr_type
    get_supplier_tr_dds_entity (void)
    {
      return CCM_TYPE::datawriter_type::_nil ();
    }
    virtual
    typename CCM_TYPE::getter_type::_ptr_type
    get_pull_consumer_tr_fresh_data (void)
    {
      return CCM_TYPE::getter_type::_nil ();
    }
    virtual
    typename CCM_TYPE::reader_type::_ptr_type
    get_pull_consumer_tr_data (void)
    {
      return CCM_TYPE::reader_type::_nil ();
    }
    virtual
    typename CCM_TYPE::datareader_type::_ptr_type
    get_pull_consumer_tr_dds_entity (void)
    {
      return CCM_TYPE::datareader_type::_nil ();
    }

    virtual
    CCM_DDS::QueryFilter*
    pull_consumer_tr_filter (void)
    {
      return 0;
    }
    virtual
    void
    pull_consumer_tr_filter (const CCM_DDS::QueryFilter&)
    {
    }
    virtual
    CCM_DDS::CCM_ContentFilterSetting*
    get_pull_consumer_tr_filter_config(void)
    {
      return CCM_DDS::CCM_ContentFilterSetting::_nil ();
    }
    virtual
    CCM_DDS::CCM_ContentFilterSetting*
    get_push_consumer_tr_filter_config(void)
    {
      return CCM_DDS::CCM_ContentFilterSetting::_nil ();
    }

    virtual
    CCM_DDS::QueryFilter *
    push_consumer_tr_filter (void)
    {
      return 0;
    }

    virtual
    void
    push_consumer_tr_filter (const ::CCM_DDS::QueryFilter &)
    {
    }

    virtual
    typename CCM_TYPE::reader_type::_ptr_type
    get_push_consumer_tr_data (void)
    {
      return CCM_TYPE::reader_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datalistenercontrol_type::_ptr_type
    get_push_consumer_tr_data_control (void)
    {
      return CCM_TYPE::datalistenercontrol_type::_nil ();
    }
    virtual
    typename CCM_TYPE::datareader_type::_ptr_type
    get_push_consumer_tr_dds_entity (void)
    {
      return CCM_TYPE::datareader_type::_nil ();
    }

  //circle

    virtual
    typename CCM_TYPE::writer_type::_ptr_type
    get_supplier_cq_data (void)
    {
      return CCM_TYPE::writer_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datawriter_type::_ptr_type
    get_supplier_cq_dds_entity (void)
    {
      return CCM_TYPE::datawriter_type::_nil ();
    }

    virtual
    typename CCM_TYPE::getter_type::_ptr_type
    get_pull_consumer_cq_fresh_data (void)
    {
      return CCM_TYPE::getter_type::_nil ();
    }

    virtual
    typename CCM_TYPE::reader_type::_ptr_type
    get_pull_consumer_cq_data (void)
    {
      return CCM_TYPE::reader_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datareader_type::_ptr_type
    get_pull_consumer_cq_dds_entity (void)
    {
      return CCM_TYPE::datareader_type::_nil ();
    }

    virtual
    CCM_DDS::QueryFilter*
    pull_consumer_cq_filter (void)
    {
      return 0;
    }
    virtual
    void
    pull_consumer_cq_filter (const CCM_DDS::QueryFilter&)
    {
    }
    virtual
    CCM_DDS::CCM_ContentFilterSetting*
    get_pull_consumer_cq_filter_config(void)
    {
      return CCM_DDS::CCM_ContentFilterSetting::_nil ();
    }
    virtual
    CCM_DDS::CCM_ContentFilterSetting*
    get_push_consumer_cq_filter_config(void)
    {
      return CCM_DDS::CCM_ContentFilterSetting::_nil ();
    }

    virtual
    CCM_DDS::QueryFilter *
    push_consumer_cq_filter (void)
    {
      return 0;
    }

    virtual
    void
    push_consumer_cq_filter (const ::CCM_DDS::QueryFilter &)
    {
    }

    virtual
    typename CCM_TYPE::reader_type::_ptr_type
    get_push_consumer_cq_data (void)
    {
      return CCM_TYPE::reader_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datalistenercontrol_type::_ptr_type
    get_push_consumer_cq_data_control (void)
    {
      return CCM_TYPE::datalistenercontrol_type::_nil ();
    }

    virtual
    typename CCM_TYPE::datareader_type::_ptr_type
    get_push_consumer_cq_dds_entity (void)
    {
      return CCM_TYPE::datareader_type::_nil ();
    }
};

#endif
