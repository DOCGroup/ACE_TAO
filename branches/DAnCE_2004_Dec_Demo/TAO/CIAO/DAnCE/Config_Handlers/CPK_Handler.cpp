//$Id$

#include "CPK_Handler.h"
#include "tao/Exception.h"
#include "Utils.h"

using CIAO::Config_Handler::Utils;

void
CIAO::Config_Handler::CPK_Handler::
process_CCMComponentPortKind (DOMNodeIterator * iter,
                              Deployment::CCMComponentPortKind &kind)
{
  char* facet = XMLString::transcode (XStr ("Facet"));
  char* simplex_receptacle = XMLString::transcode (XStr ("SimplexReceptacle"));
  char* multiplex_receptacle = XMLString::transcode 
                                 (XStr ("MultiplexReceptacle"));
  char* event_emitter = XMLString::transcode (XStr ("EventEmitter"));
  char* event_publisher = XMLString::transcode (XStr ("EventPublisher"));
  char* event_consumer = XMLString::transcode (XStr ("EventConsumer"));
  char* rtec_event_emitter = XMLString::transcode (XStr ("rtecEventEmitter"));
  char* rtec_event_publisher = XMLString::transcode (XStr ("rtecEventPublisher"));
  char* cos_event_emitter = XMLString::transcode (XStr ("ecEventEmitter"));
  char* cos_event_publisher = XMLString::transcode (XStr ("ecEventPublisher"));
  char* ns_event_emitter = XMLString::transcode (XStr ("nsEventEmitter"));
  char* ns_event_publisher = XMLString::transcode (XStr ("nsEventPublisher"));
  char * temp = Utils::parse_string (iter);
  XStr kind_str = XStr (temp);
  XMLString::release (&temp);

  char* kind_str_tr = XMLString::transcode (kind_str);

  if ((ACE_OS::strcmp (kind_str_tr, facet)) == 0)
    {
      kind = Deployment::Facet;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, simplex_receptacle)) == 0)
    {
      kind = Deployment::SimplexReceptacle;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, multiplex_receptacle)) == 0)
    {
      kind = Deployment::MultiplexReceptacle;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, event_emitter)) == 0)
    {
      kind = Deployment::EventEmitter;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, event_publisher)) == 0)
    {
      kind = Deployment::EventPublisher;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, event_consumer)) == 0)
    {
      kind = Deployment::EventConsumer;
    }
  // RTEC event supplier
  else if ((ACE_OS::strcmp (kind_str_tr, rtec_event_emitter)) == 0)
    {
      kind = Deployment::rtecEventEmitter;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, rtec_event_publisher)) == 0)
    {
      kind = Deployment::rtecEventPublisher;
    }
  // COS EC event supplier
  else if ((ACE_OS::strcmp (kind_str_tr, cos_event_emitter)) == 0)
    {
      kind = Deployment::ecEventEmitter;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, cos_event_publisher)) == 0)
    {
      kind = Deployment::ecEventPublisher;
    }
  // NS event supplier
  else if ((ACE_OS::strcmp (kind_str_tr, ns_event_emitter)) == 0)
    {
      kind = Deployment::nsEventEmitter;
    }
  else if ((ACE_OS::strcmp (kind_str_tr, ns_event_publisher)) == 0)
    {
      kind = Deployment::nsEventPublisher;
    }
  else
    {
      // Something wrong here.. Throw exception
      XMLString::release (&kind_str_tr);
      XMLString::release (&facet);
      XMLString::release (&simplex_receptacle);
      XMLString::release (&multiplex_receptacle);
      XMLString::release (&event_emitter);
      XMLString::release (&event_publisher);
      XMLString::release (&event_consumer);

      ACE_DEBUG ((LM_DEBUG,
                  "Config_Handler::CPK_Handler::process_CCMComponentPortKing \
                   illegal <CCMComponentPortKind> value found \n"));
      ACE_THROW (CORBA::INTERNAL ());
    }

  XMLString::release (&kind_str_tr);
  XMLString::release (&facet);
  XMLString::release (&simplex_receptacle);
  XMLString::release (&multiplex_receptacle);
  XMLString::release (&event_emitter);
  XMLString::release (&event_publisher);
  XMLString::release (&event_consumer);
}
