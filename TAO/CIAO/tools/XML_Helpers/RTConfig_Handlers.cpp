// $Id$

#include "RTConfig_Handlers.h"

#if !defined (__ACE_INLINE__)
# include "RTConfig_Handlers.inl"
#endif /* __ACE_INLINE__ */

void
CIAO::RTConfig_Handler::startElement (const ACEXML_Char *namespaceURI,
                                      const ACEXML_Char *localName,
                                      const ACEXML_Char *qName,
                                      ACEXML_Attributes *alist
                                      ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  switch (this->state_)
    {
    case START:
      if (ACE_OS::strcmp (qName, "rtrecources") == 0)
        {
          this->state_ = RTRESOURCES;
        }
      else if (ACE_OS::strcmp (qName, "rtpolicyset") == 0)
        {
          this->state_ = RTPOLICYSET;
          // @@ fix me          this->create_new_rtpolicyset (alist);
        }
      else if (ACE_OS::strcmp (qName, "rtcad-ext") == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Start parsing RT cad file extension\n"));
        }
      return;

    case RTRESOURCES:
      if (ACE_OS::strcmp (qName, "threadpool") == 0)
        {
          this->parse_threadpool_attrs (alist
                                        ACEXML_ENV_ARG_PARAMETER);
        }
      else if (ACE_OS::strcmp (qName, "threadpoolwithlanes") == 0)
        {
          this->state_ = TPWLANES;
          this->parse_threadpoollanes_attrs (alist
                                             ACEXML_ENV_ARG_PARAMETER);
        }
      else if (ACE_OS::strcmp (qName, "connectionbands") == 0)
        {
          this->bands_config_ = new CIAO::RTConfiguration::PriorityBands_Configuration;
          const char *id;
          XML_Utils::get_single_attribute ("id",
                                           id,
                                           alist
                                           ACEXML_ENV_ARG_PARAMETER);
          ACEXML_CHECK;

          this->bands_config_->name = id;
          this->state_ = CONNBANDS;
        }
      return;

    case RTPOLICYSET:
      if (ACE_OS::strcmp (qName, "priority_model_policy") == 0)
        {
          // @@ parse attributes and add it to the policy set
        }
      else if (ACE_OS::strcmp (qName, "threadpool_policy") == 0)
        {
          // @@ parse the attributes and add it to the policy set
        }
      else if (ACE_OS::strcmp (qName, "banded_connection_policy") == 0)
        {
          // @@ parse the attributes and add it to the policy set
        }
      else if (ACE_OS::strcmp (qName, "rtpolicyset") == 0)
        {
          // @@ parse the attributes and add it to the policy set
        }
      return;

    case TPWLANES:
      if (ACE_OS::strcmp (qName, "lane") == 0)
        {
          this->add_threadpoollane (alist
                                    ACEXML_ENV_ARG_PARAMETER);
        }
      return;

    case CONNBANDS:
      if (ACE_OS::strcmp (qName, "band") == 0)
        {
          this->add_priorityband (alist
                                  ACEXML_ENV_ARG_PARAMETER);
        }
      return;

    default:
      ACEXML_THROW (ACEXML_SAXException
                    (ACE_TEXT ("RTConfig handler internal error")));
      return;
    }
  ACEXML_THROW (ACEXML_SAXException
                (ACE_TEXT ("RTConfig handler internal error")));
}

void
CIAO::RTConfig_Handler::endElement (const ACEXML_Char *,
                                    const ACEXML_Char *,
                                    const ACEXML_Char *qName
                                    ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  switch (this->state_)
    {
    case START:
      if (ACE_OS::strcmp (qName, "rtcad-ext") == 0)
        {
          ACE_DEBUG ((LM_DEBUG, "End parsing RT cad file extension\n"));
        }
      return;

    case RTRESOURCES:
      if (ACE_OS::strcmp (qName, "rtresources") == 0)
        this->state_ = RTPOLICYSET;
      return;

    case TPWLANES:
      if (ACE_OS::strcmp (qName, "threadpoolwithlanes") == 0)
        {
          // Store the good parsed results.
          CORBA::ULong len = this->rtresources.tpl_configs.length ();
          this->rtresources.tpl_configs.length (len+1);
          this->rtresources.tpl_configs[len-1] = this->tpl_config_;
        }
      return;

    case CONNBANDS:
      if (ACE_OS::strcmp (qName, "connectionbands") == 0)
        {
          // @@ wrap up band sequence.
        }
      return;

    case RTPOLICYSET:
      if (ACE_OS::strcmp (qName, "rtpolicyset") == 0)
        {
          // @@ add the policy set to the sequence.
        }
      return;

    default:
      ACEXML_THROW (ACEXML_SAXException
                    (ACE_TEXT ("RTConfig handler internal error")));
      return;
    }
  ACEXML_THROW (ACEXML_SAXException
                (ACE_TEXT ("RTConfig handler internal error")));
}

// =================================================================

void
CIAO::RTConfig_Handler::parse_threadpool_attrs (ACEXML_Attributes *alist
                                                ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  const STACKSIZE_READ = 1;
  const STATICTHR_READ = 2;
  const DYNAMICTHR_READ = 4;
  const DEFAULTPRIO_READ = 8;
  const YES_BUFFERING_READ = 0X10;
  const BUFFER_REQ_READ = 0X20;
  const BUFFER_SIZE_READ = 0X40;
  const TPNAME_READ = 0X80;

  // Initialize the attribute parse progress.  We will turn off these
  // bits one by one.  At the end, the att_status should be 0.
  int att_status =
    STACKSIZE_READ |
    STATICTHR_READ |
    DYNAMICTHR_READ |
    DEFAULTPRIO_READ |
    YES_BUFFERING_READ |
    BUFFER_REQ_READ |
    BUFFER_SIZE_READ |
    TPNAME_READ;

  char *endpos;
  CIAO::RTConfiguration::ThreadPool_Configuration_var tp_config
    = new CIAO::RTConfiguration::ThreadPool_Configuration;

  for (size_t i = 0; i < alist->getLength (); ++i)
    {
      const char *attName = alist->getQName (i);
      const char *attValue = alist->getValue (i);

      if (ACE_OS_String::strcmp (attName, "id") == 0)
        {
          tp_config->name = attValue;
          ACE_CLR_BITS (att_status,
                        TPNAME_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "stacksize") == 0)
        {
          tp_config->stacksize =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'stacksize' attribute in threadpool config."));

          ACE_CLR_BITS (att_status,
                        STACKSIZE_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "static_threads") == 0)
        {
          tp_config->static_threads =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'static_threads' attribute in threadpool config."));

          ACE_CLR_BITS (att_status,
                        STATICTHR_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "dynamic_threads") == 0)
        {
          tp_config->dynamic_threads =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'dynamic_threads' attribute in threadpool config."));

          ACE_CLR_BITS (att_status,
                        DYNAMICTHR_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "priority") == 0)
        {
          tp_config->default_priority =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'priority' attribute in threadpool config."));

          ACE_CLR_BITS (att_status,
                        DEFAULTPRIO_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "buffering") == 0)
        {
          // Assuming we have a validating parser.
          if (ACE_OS_String::strcmp (attName, "yes") == 0)
            tp_config->allow_request_buffering = 1;
          else
            tp_config->allow_request_buffering = 0;

          ACE_CLR_BITS (att_status,
                        YES_BUFFERING_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "max_buffer") == 0)
        {
          tp_config->max_buffered_requests =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'max_buffer' attribute in threadpool config."));

          ACE_CLR_BITS (att_status,
                        BUFFER_REQ_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "buffer_size") == 0)
        {
          tp_config->max_request_buffer_size =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'buffer_size' attribute in threadpool config."));

          ACE_CLR_BITS (att_status,
                        BUFFER_SIZE_READ);
        }
      else
        ACEXML_THROW
          (ACEXML_SAXException
           ("Invalid attribute found"));
    }
  if (att_status != 0)
        ACEXML_THROW
          (ACEXML_SAXException
           ("Not all ThreadPool_Configuration attributes are set."));

  // Store the good parsed results.
  CORBA::ULong len = this->rtresources.tp_configs.length ();
  this->rtresources.tp_configs.length (len+1);
  this->rtresources.tp_configs[len-1] = tp_config;
}

void
CIAO::RTConfig_Handler::parse_threadpoollanes_attrs (ACEXML_Attributes *alist
                                                     ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  const STACKSIZE_READ = 1;
  const YES_BORROWING_READ = 2;
  const YES_BUFFERING_READ = 4;
  const BUFFER_REQ_READ = 8;
  const BUFFER_SIZE_READ = 0X10;
  const TPNAME_READ = 0X20;

  // Initialize the attribute parse progress.  We will turn off these
  // bits one by one.  At the end, the att_status should be 0.
  int att_status =
    STACKSIZE_READ |
    YES_BORROWING_READ |
    YES_BUFFERING_READ |
    BUFFER_REQ_READ |
    BUFFER_SIZE_READ |
    TPNAME_READ;

  char *endpos;
  this->tpl_config_
    = new CIAO::RTConfiguration::ThreadPoolLanes_Configuration;

  for (size_t i = 0; i < alist->getLength (); ++i)
    {
      const char *attName = alist->getQName (i);
      const char *attValue = alist->getValue (i);

      if (ACE_OS_String::strcmp (attName, "id") == 0)
        {
          this->tpl_config_->name = attValue;
          ACE_CLR_BITS (att_status,
                        TPNAME_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "stacksize") == 0)
        {
          this->tpl_config_->stacksize =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'stacksize' attribute in threadpoolwithlanes config."));

          ACE_CLR_BITS (att_status,
                        STACKSIZE_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "borrowing") == 0)
        {
          // Assuming we have a validating parser.
          if (ACE_OS_String::strcmp (attName, "yes") == 0)
            this->tpl_config_->allow_borrowing = 1;
          else
            this->tpl_config_->allow_borrowing = 0;

          ACE_CLR_BITS (att_status,
                        YES_BUFFERING_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "buffering") == 0)
        {
          // Assuming we have a validating parser.
          if (ACE_OS_String::strcmp (attName, "yes") == 0)
            this->tpl_config_->allow_request_buffering = 1;
          else
            this->tpl_config_->allow_request_buffering = 0;

          ACE_CLR_BITS (att_status,
                        YES_BUFFERING_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "max_buffer") == 0)
        {
          this->tpl_config_->max_buffered_requests =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'max_buffer' attribute in threadpoolwitlanes config."));

          ACE_CLR_BITS (att_status,
                        BUFFER_REQ_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "buffer_size") == 0)
        {
          this->tpl_config_->max_request_buffer_size =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'buffer_size' attribute in threadpoolwithlanes config."));

          ACE_CLR_BITS (att_status,
                        BUFFER_SIZE_READ);
        }
      else
        ACEXML_THROW
          (ACEXML_SAXException
           ("Invalid attribute in element 'threadpoolwithlanes' found"));
    }
  if (att_status != 0)
        ACEXML_THROW
          (ACEXML_SAXException
           ("Not all ThreadPoolLanes_Configuration attributes are set."));
}

void
CIAO::RTConfig_Handler::add_threadpoollane (ACEXML_Attributes *alist
                                            ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  const PRIORITY_READ = 1;
  const STATICTHR_READ = 2;
  const DYNAMICTHR_READ = 4;

  // Initialize the attribute parse progress.  We will turn off these
  // bits one by one.  At the end, the att_status should be 0.
  int att_status =
    PRIORITY_READ |
    STATICTHR_READ |
    DYNAMICTHR_READ ;

  char *endpos;
  RTCORBA::ThreadpoolLane alane;

  for (size_t i = 0; i < alist->getLength (); ++i)
    {
      const char *attName = alist->getQName (i);
      const char *attValue = alist->getValue (i);

      if (ACE_OS_String::strcmp (attName, "priority") == 0)
        {
          alane.lane_priority =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'priority' attribute in lane config."));

          ACE_CLR_BITS (att_status,
                        PRIORITY_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "static_threads") == 0)
        {
          alane.static_threads =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'static_threads' attribute in lane config."));

          ACE_CLR_BITS (att_status,
                        STATICTHR_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "dynamic_threads") == 0)
        {
          alane.dynamic_threads =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'dynamic_threads' attribute in lane config."));

          ACE_CLR_BITS (att_status,
                        DYNAMICTHR_READ);
        }
      else
        ACEXML_THROW
          (ACEXML_SAXException
           ("Invalid attribute found in lane element"));
    }
  if (att_status != 0)
        ACEXML_THROW
          (ACEXML_SAXException
           ("Not all 'lane' attributes are set."));

  // Store the good parsed results.
  CORBA::ULong len = this->tpl_config_->lanes.length ();
  this->tpl_config_->lanes.length (len+1);
  this->tpl_config_->lanes[len-1] = alane;
}

void
CIAO::RTConfig_Handler::add_priorityband (ACEXML_Attributes *alist
                                          ACEXML_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  const LOW_READ = 1;
  const HIGH_READ = 2;

  // Initialize the attribute parse progress.  We will turn off these
  // bits one by one.  At the end, the att_status should be 0.
  int att_status =
    LOW_READ |
    HIGH_READ ;

  char *endpos;
  RTCORBA::PriorityBand band;

  for (size_t i = 0; i < alist->getLength (); ++i)
    {
      const char *attName = alist->getQName (i);
      const char *attValue = alist->getValue (i);

      if (ACE_OS_String::strcmp (attName, "low") == 0)
        {
          band.low =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'low' attribute in band config."));

          ACE_CLR_BITS (att_status,
                        LOW_READ);
        }
      else if (ACE_OS_String::strcmp (attName, "high") == 0)
        {
          band.high =
            strtoul (attValue, &endpos, 10);

          if (*endpos != 0)
            ACEXML_THROW
              (ACEXML_SAXException
               ("Invalid 'high' attribute in lane config."));

          ACE_CLR_BITS (att_status,
                        HIGH_READ);
        }
      else
        ACEXML_THROW
          (ACEXML_SAXException
           ("Invalid attribute found in band element"));
    }
  if (att_status != 0)
        ACEXML_THROW
          (ACEXML_SAXException
           ("Not all 'band' attributes are set."));

  // Store the good parsed results.
  CORBA::ULong len = this->bands_config_->bands.length ();
  this->bands_config_->bands.length (len+1);
  this->bands_config_->bands[len-1] = band;
}
