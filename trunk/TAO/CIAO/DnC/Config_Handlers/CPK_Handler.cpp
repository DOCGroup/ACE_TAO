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
  XStr facet ("Facet");
  XStr simplex_receptacle ("SimplexReceptacle");
  XStr multiplex_receptacle ("MultiplexReceptacle");
  XStr event_emitter ("EventEmitter");
  XStr event_publisher ("EventPublisher");
  XStr event_consumer ("EventConsumer");



  XStr kind_str = XStr (Utils::parse_string (iter));

  if ((ACE_OS::strcmp (XMLString::transcode (kind_str),
                       XMLString::transcode (facet))) == 0)
  {
    kind = Deployment::Facet;
  }
  else if ((ACE_OS::strcmp (XMLString::transcode (kind_str),
                       XMLString::transcode (simplex_receptacle))) == 0)
  {
    kind = Deployment::SimplexReceptacle;
  }
  else if ((ACE_OS::strcmp (XMLString::transcode (kind_str),
                       XMLString::transcode (multiplex_receptacle))) == 0)
  {
    kind = Deployment::MultiplexReceptacle;
  }
  else if ((ACE_OS::strcmp (XMLString::transcode (kind_str),
                       XMLString::transcode (event_emitter))) == 0)
  {
    kind = Deployment::EventEmitter;
  }
  else if ((ACE_OS::strcmp (XMLString::transcode (kind_str),
                       XMLString::transcode (event_publisher))) == 0)
  {
    kind = Deployment::EventPublisher;
  }
  else if ((ACE_OS::strcmp (XMLString::transcode (kind_str),
                       XMLString::transcode (event_consumer))) == 0)
  {
    kind = Deployment::EventConsumer;
  }
  else
  {
     // Something wrong here.. Throw exception
  ACE_DEBUG ((LM_DEBUG,
              "Config_Handler::CPK_Handler::process_CCMComponentPortKing illegal <CCMComponentPortKind> value found \n"));
  ACE_THROW (CORBA::INTERNAL ());
  }
}
