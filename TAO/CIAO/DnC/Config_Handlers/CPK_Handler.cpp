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

  if (kind_str == facet)
    kind = Deployment::Facet;
  if (kind_str == simplex_receptacle)
    kind = Deployment::SimplexReceptacle;
  if (kind_str == multiplex_receptacle)
    kind = Deployment::MultiplexReceptacle;
  if (kind_str == event_emitter)
    kind = Deployment::EventEmitter;
  if (kind_str == event_publisher)
    kind = Deployment::EventPublisher;
  if (kind_str == event_consumer)
    kind = Deployment::EventConsumer;

  // Something wrong here.. Throw exception
  ACE_DEBUG ((LM_DEBUG,
              "Config_Handler::CPK_Handler::process_CCMComponentPortKing illegal <CCMComponentPortKind> value found \n"));
  ACE_THROW (CORBA::INTERNAL ());
}
