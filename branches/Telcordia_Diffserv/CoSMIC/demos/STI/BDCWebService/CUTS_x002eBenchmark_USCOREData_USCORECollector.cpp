#include "CUTS_x002eBenchmark_USCOREData_USCORECollector.h"
#include <string>
using std::string;

CUTS_x002eBenchmark_USCOREData_USCORECollector::CUTS_x002eBenchmark_USCOREData_USCORECollector (CUTS::BDC_Control_Handle_ptr controls, 
                                                                                                CUTS::Path_Measurement_ptr online_measurements)
: controls_(CUTS::BDC_Control_Handle::_duplicate (controls)), 
online_measurements_(CUTS::Path_Measurement::_duplicate (online_measurements))
{
  CUTS_x002eBenchmark_USCOREData_USCORECollector_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

CUTS_x002eBenchmark_USCOREData_USCORECollector::CUTS_x002eBenchmark_USCOREData_USCORECollector ()
{
  CUTS_x002eBenchmark_USCOREData_USCORECollector_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

CUTS_x002eBenchmark_USCOREData_USCORECollector::CUTS_x002eBenchmark_USCOREData_USCORECollector (soap_mode iomode)
{
  CUTS_x002eBenchmark_USCOREData_USCORECollector_init(iomode, iomode);
}

CUTS_x002eBenchmark_USCOREData_USCORECollector::CUTS_x002eBenchmark_USCOREData_USCORECollector (soap_mode imode, soap_mode omode)
{
  CUTS_x002eBenchmark_USCOREData_USCORECollector_init(imode, omode);
}

void CUTS_x002eBenchmark_USCOREData_USCORECollector::CUTS_x002eBenchmark_USCOREData_USCORECollector_init (soap_mode imode, soap_mode omode)
{
  static const struct Namespace namespaces[] = 
  {
    {
      "SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL
    }

    ,
    {
      "SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL
    }

    ,
    {
      "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL
    }

    ,
    {
      "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL
    }

    ,
    {
      "CORBA", "http://www.omg.org/IDL-WSDL/1.0/", NULL, NULL
    }

    ,
    {
      "SLICE""urn:SLICE", NULL, NULL
    }

    ,
    {
      NULL, NULL, NULL, NULL
    }
  };

  soap_init2(this, imode, omode);
  if (!this->namespaces)
  this->namespaces = namespaces;
};


CUTS_x002eBenchmark_USCOREData_USCORECollector::~CUTS_x002eBenchmark_USCOREData_USCORECollector()
{
  soap_destroy(this);
  soap_end(this);
  soap_done(this);
}

int CUTS_x002eBenchmark_USCOREData_USCORECollector::serve()
{
  #ifndef WITH_FASTCGI
  unsigned int k = this->max_keep_alive;

  #endif
  do
  {
    soap_begin(this);

    #ifdef WITH_FASTCGI
    if (FCGI_Accept() < 0)
    {
        this->error = SOAP_EOF;

        return soap_send_fault(this);
    }

    #endif

    soap_begin(this);


    #ifndef WITH_FASTCGI
    if (this->max_keep_alive > 0 && !--k)
    this->keep_alive = 0;

    #endif

    if (soap_begin_recv(this))
    {
      if (this->error < SOAP_STOP)
      {
        #ifdef WITH_FASTCGI
        soap_send_fault(this);

        #else
        return soap_send_fault(this);

        #endif
      }

      soap_closesock(this);


      continue;
    }


    if (soap_envelope_begin_in(this)
        || soap_recv_header(this)
        || soap_body_begin_in(this)
        || dispatch() || (this->fserveloop && this->fserveloop(this)))
    {
      #ifdef WITH_FASTCGI
      soap_send_fault(this);

      #else
      return soap_send_fault(this);

      #endif
    }


    #ifdef WITH_FASTCGI
  }

   while (1);

  #else
}

 while (this->keep_alive);

#endif
return SOAP_OK;
}

static int serve_SLICE__execution_USCOREtime(CUTS_x002eBenchmark_USCOREData_USCORECollector*);

static int serve_SLICE__unbind_USCOREto_USCOREpath(CUTS_x002eBenchmark_USCOREData_USCORECollector*);

static int serve_SLICE__bind_USCOREto_USCOREpath(CUTS_x002eBenchmark_USCOREData_USCORECollector*);

static int serve_SLICE__collect_USCOREperformance_USCOREdata(CUTS_x002eBenchmark_USCOREData_USCORECollector*);

int CUTS_x002eBenchmark_USCOREData_USCORECollector::dispatch()
{
  if (soap_peek_element(this))
  return this->error;

  if (!soap_match_tag(this, this->tag, "SLICE:execution_time"))
  return serve_SLICE__execution_USCOREtime(this);

  if (!soap_match_tag(this, this->tag, "SLICE:unbind_to_path"))
  return serve_SLICE__unbind_USCOREto_USCOREpath(this);

  if (!soap_match_tag(this, this->tag, "SLICE:bind_to_path"))
  return serve_SLICE__bind_USCOREto_USCOREpath(this);

  if (!soap_match_tag(this, this->tag, "SLICE:collect_performance_data"))
  return serve_SLICE__collect_USCOREperformance_USCOREdata(this);

  return this->error = SOAP_NO_METHOD;
}

static int serve_SLICE__execution_USCOREtime(CUTS_x002eBenchmark_USCOREData_USCORECollector*soap)
{
  struct SLICE__execution_USCOREtime request;
  struct SLICE__execution_USCOREtimeResponse response;
  soap_default_SLICE__execution_USCOREtimeResponse (soap, &response);
  soap_default_SLICE__execution_USCOREtime(soap, &request);
  soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
  if (!soap_get_SLICE__execution_USCOREtime(soap, &request, "SLICE:execution_time", NULL))
  return soap->error;

  if (soap_body_end_in(soap)
      || soap_envelope_end_in(soap)
      || soap_end_recv(soap))
  return soap->error;

  soap->error = soap->execution_USCOREtime(request.path_USCOREid, 
                                           response);
  if (soap->error)
  return soap->error;

  soap_serializeheader(soap);

  soap_serialize_SLICE__execution_USCOREtimeResponse (soap, &response);

  if (soap_begin_count(soap))
  return soap->error;

    if (soap->mode & SOAP_IO_LENGTH)
  {
    if (soap_envelope_begin_out(soap)
        || soap_putheader(soap)
        || soap_body_begin_out(soap)
        || soap_put_SLICE__execution_USCOREtimeResponse (soap, &response, "SLICE:execution_timeResponse","")
        || soap_body_end_out(soap)
        || soap_envelope_end_out(soap))
    return soap->error;
  }

  if (soap_end_count(soap)
      || soap_response(soap, SOAP_OK)
      || soap_envelope_begin_out(soap)
      || soap_putheader(soap)
      || soap_body_begin_out(soap)
      || soap_put_SLICE__execution_USCOREtimeResponse (soap, &response, "SLICE:execution_timeResponse","")
      || soap_body_end_out(soap)
      || soap_envelope_end_out(soap)
      || soap_end_send(soap))
  return soap->error;

  return soap_closesock(soap);
}

static int serve_SLICE__unbind_USCOREto_USCOREpath(CUTS_x002eBenchmark_USCOREData_USCORECollector*soap)
{
  struct SLICE__unbind_USCOREto_USCOREpath request;
  struct SLICE__unbind_USCOREto_USCOREpathResponse response;
  soap_default_SLICE__unbind_USCOREto_USCOREpathResponse (soap, &response);
  soap_default_SLICE__unbind_USCOREto_USCOREpath(soap, &request);
  soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
  if (!soap_get_SLICE__unbind_USCOREto_USCOREpath(soap, &request, "SLICE:unbind_to_path", NULL))
  return soap->error;

  if (soap_body_end_in(soap)
      || soap_envelope_end_in(soap)
      || soap_end_recv(soap))
  return soap->error;

  soap->error = soap->unbind_USCOREto_USCOREpath(request.path_USCOREid, 
                                                 response);
  if (soap->error)
  return soap->error;

  soap_serializeheader(soap);

  soap_serialize_SLICE__unbind_USCOREto_USCOREpathResponse (soap, &response);

  if (soap_begin_count(soap))
  return soap->error;

    if (soap->mode & SOAP_IO_LENGTH)
  {
    if (soap_envelope_begin_out(soap)
        || soap_putheader(soap)
        || soap_body_begin_out(soap)
        || soap_put_SLICE__unbind_USCOREto_USCOREpathResponse (soap, &response, "SLICE:unbind_to_pathResponse","")
        || soap_body_end_out(soap)
        || soap_envelope_end_out(soap))
    return soap->error;
  }

  if (soap_end_count(soap)
      || soap_response(soap, SOAP_OK)
      || soap_envelope_begin_out(soap)
      || soap_putheader(soap)
      || soap_body_begin_out(soap)
      || soap_put_SLICE__unbind_USCOREto_USCOREpathResponse (soap, &response, "SLICE:unbind_to_pathResponse","")
      || soap_body_end_out(soap)
      || soap_envelope_end_out(soap)
      || soap_end_send(soap))
  return soap->error;

  return soap_closesock(soap);
}

static int serve_SLICE__bind_USCOREto_USCOREpath(CUTS_x002eBenchmark_USCOREData_USCORECollector*soap)
{
  struct SLICE__bind_USCOREto_USCOREpath request;
  struct SLICE__bind_USCOREto_USCOREpathResponse response;
  soap_default_SLICE__bind_USCOREto_USCOREpathResponse (soap, &response);
  soap_default_SLICE__bind_USCOREto_USCOREpath(soap, &request);
  soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
  if (!soap_get_SLICE__bind_USCOREto_USCOREpath(soap, &request, "SLICE:bind_to_path", NULL))
  return soap->error;

  if (soap_body_end_in(soap)
      || soap_envelope_end_in(soap)
      || soap_end_recv(soap))
  return soap->error;

  soap->error = soap->bind_USCOREto_USCOREpath(request.path, 
                                               request.flags, 
                                               response._USCOREreturn);
  if (soap->error)
  return soap->error;

  soap_serializeheader(soap);

  soap_serialize_SLICE__bind_USCOREto_USCOREpathResponse (soap, &response);

  if (soap_begin_count(soap))
  return soap->error;

    if (soap->mode & SOAP_IO_LENGTH)
  {
    if (soap_envelope_begin_out(soap)
        || soap_putheader(soap)
        || soap_body_begin_out(soap)
        || soap_put_SLICE__bind_USCOREto_USCOREpathResponse (soap, &response, "SLICE:bind_to_pathResponse","")
        || soap_body_end_out(soap)
        || soap_envelope_end_out(soap))
    return soap->error;
  }

  if (soap_end_count(soap)
      || soap_response(soap, SOAP_OK)
      || soap_envelope_begin_out(soap)
      || soap_putheader(soap)
      || soap_body_begin_out(soap)
      || soap_put_SLICE__bind_USCOREto_USCOREpathResponse (soap, &response, "SLICE:bind_to_pathResponse","")
      || soap_body_end_out(soap)
      || soap_envelope_end_out(soap)
      || soap_end_send(soap))
  return soap->error;

  return soap_closesock(soap);
}

static int serve_SLICE__collect_USCOREperformance_USCOREdata(CUTS_x002eBenchmark_USCOREData_USCORECollector*soap)
{
  struct SLICE__collect_USCOREperformance_USCOREdata request;
  struct SLICE__collect_USCOREperformance_USCOREdataResponse response;
  soap_default_SLICE__collect_USCOREperformance_USCOREdataResponse (soap, &response);
  soap_default_SLICE__collect_USCOREperformance_USCOREdata(soap, &request);
  soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
  if (!soap_get_SLICE__collect_USCOREperformance_USCOREdata(soap, &request, "SLICE:collect_performance_data", NULL))
  return soap->error;

  if (soap_body_end_in(soap)
      || soap_envelope_end_in(soap)
      || soap_end_recv(soap))
  return soap->error;

  soap->error = soap->collect_USCOREperformance_USCOREdata(response);
  if (soap->error)
  return soap->error;

  soap_serializeheader(soap);

  soap_serialize_SLICE__collect_USCOREperformance_USCOREdataResponse (soap, &response);

  if (soap_begin_count(soap))
  return soap->error;

    if (soap->mode & SOAP_IO_LENGTH)
  {
    if (soap_envelope_begin_out(soap)
        || soap_putheader(soap)
        || soap_body_begin_out(soap)
        || soap_put_SLICE__collect_USCOREperformance_USCOREdataResponse (soap, &response, "SLICE:collect_performance_dataResponse","")
        || soap_body_end_out(soap)
        || soap_envelope_end_out(soap))
    return soap->error;
  }

  if (soap_end_count(soap)
      || soap_response(soap, SOAP_OK)
      || soap_envelope_begin_out(soap)
      || soap_putheader(soap)
      || soap_body_begin_out(soap)
      || soap_put_SLICE__collect_USCOREperformance_USCOREdataResponse (soap, &response, "SLICE:collect_performance_dataResponse","")
      || soap_body_end_out(soap)
      || soap_envelope_end_out(soap)
      || soap_end_send(soap))
  return soap->error;

  return soap_closesock(soap);
}

int CUTS_x002eBenchmark_USCOREData_USCORECollector::execution_USCOREtime (int path_USCOREid, 
                                                                          struct SLICE__execution_USCOREtimeResponse& _param_1)
{
  try
  {
    CUTS::Event_Time_Info _return = this->online_measurements_->execution_time (path_USCOREid);
    _param_1._USCOREreturn = new SLICE__CUTS_x002eEvent_USCORETime_USCOREInfo();
    _param_1._USCOREreturn->min_USCOREevents = _return.min_events;
    _param_1._USCOREreturn->max_USCOREevents = _return.max_events;
    _param_1._USCOREreturn->time = new SLICE__CUTS_x002eTime_USCOREInfo();
    _param_1._USCOREreturn->time->min_ = _return.time.min;
    _param_1._USCOREreturn->time->max_ = _return.time.max;
    _param_1._USCOREreturn->time->total = _return.time.total;
  }

  catch (CUTS::ID_Not_Found& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "execution_time");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  catch (CUTS::Operation_Failed& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "execution_time");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  catch (CORBA::Exception& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "execution_time");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  return SOAP_OK;
}


int CUTS_x002eBenchmark_USCOREData_USCORECollector::unbind_USCOREto_USCOREpath (int path_USCOREid, 
                                                                                struct SLICE__unbind_USCOREto_USCOREpathResponse& _param_1)
{
  try
  {
    this->online_measurements_->unbind_to_path (path_USCOREid);
  }

  catch (CUTS::ID_Not_Found& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "unbind_to_path");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  catch (CORBA::Exception& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "unbind_to_path");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  return SOAP_OK;
}


int CUTS_x002eBenchmark_USCOREData_USCORECollector::bind_USCOREto_USCOREpath (CUTS_x002e_USCORESE_USCOREPath_USCORESequence* path, 
                                                                              unsigned short flags, 
                                                                              int& _USCOREreturn)
{
  try
  {
    SLICE__CUTS_x002ePath_USCOREElement** array = path->__ptritem;
    int array_len = path->__size;
    CUTS::Path_Sequence pathSeq;
    for (int i = 0; i < array_len; ++i)
    {
      SLICE__CUTS_x002ePath_USCOREElement element = *(*(array+i));
      CUTS::Path_Element pathSeqElement;
      pathSeqElement.src = CORBA::string_dup (element.src);
      pathSeqElement.node = CORBA::string_dup (element.node);
      pathSeqElement.dst = CORBA::string_dup (element.dst);
      CORBA::ULong len = pathSeq.length();
      pathSeq.length(len + 1);
      pathSeq[len] = pathSeqElement;
    }

    _USCOREreturn = this->online_measurements_->bind_to_path (pathSeq, flags);
  }

  catch (CUTS::Binding_Failed& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "bind_to_path");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  catch (CORBA::Exception& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "bind_to_path");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  return SOAP_OK;
}


int CUTS_x002eBenchmark_USCOREData_USCORECollector::collect_USCOREperformance_USCOREdata (struct SLICE__collect_USCOREperformance_USCOREdataResponse& _param_1)
{
  try
  {
    this->controls_->collect_performance_data ();
  }

  catch (CORBA::Exception& ex)
  {
    ACE_PRINT_EXCEPTION (ex, "collect_performance_data");
    string msg (ex._info().c_str());
    return soap_receiver_fault (this,"An exception occurred: ", msg.c_str());
  }

  return SOAP_OK;
}


