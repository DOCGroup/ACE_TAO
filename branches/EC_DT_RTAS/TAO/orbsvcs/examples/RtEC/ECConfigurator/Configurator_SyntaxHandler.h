// $Id$

//=============================================================================
/**
 *  @file    Configurator_SyntaxHandler.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef CONFIGURATOR_SYNTAXHANDLER_H
#define CONFIGURATOR_SYNTAXHANDLER_H

#include "SyntaxVisitor.h"
#include "SyntaxTree.h"
#include "RtSchedEventChannelC.h"

#include "ACEXML/common/SAXExceptions.h"

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"

#include <vector>

// Forward decls
class ECDriver;
class Kokyu_EC;
class Gateway_Initializer;
class ECSupplier;
class ECConsumer;

/**
 * @class Configurator_SyntaxHandler
 *
 * @brief Configurator_SyntaxHandler configures the EC based on a
 * syntax tree produced by Configurator_ParseHandler.
 *
 */
class Configurator_SyntaxHandler : public SyntaxVisitor
{
public:
  typedef std::vector<RtecEventComm::EventType> EventTypeVector;

  /*
   * Default constructor.
   */
  Configurator_SyntaxHandler (void);

  int init(CORBA::ORB_var orb, PortableServer::POA_var poa);

  /*
   * Default destructor.
   */
  virtual ~Configurator_SyntaxHandler (void);

  virtual int parseVisitableSyntax (VisitableSyntax *vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual int parseECConfiguration (ECConfiguration* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseEvent (Event* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseCriticality (Criticality* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseImportance (Importance* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTimeout (Timeout* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseLocalEventChannel (LocalEventChannel* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseRemoteEventChannel (RemoteEventChannel* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSchedulingStrategy (SchedulingStrategy* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseConsumer (Consumer* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSubscriptions (Subscriptions* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseDependants (Dependants* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSupplier (Supplier* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parsePublications (Publications* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTriggers (Triggers* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTestDriver (TestDriver* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseStartCondition (StartCondition* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseStopCondition (StopCondition* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual int parseEventName (EventName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseSupplierName (SupplierName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTimeoutName (TimeoutName* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseIORFile (IORFile* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseWorstExecution (WorstExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTypicalExecution (TypicalExecution* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parsePeriod (Period* vs, void *arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parsePhase (Phase* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseTime (Time* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));
  virtual int parseValue (Value* vs, void* arg)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual VisitableSyntax* getRootNode (void);

  virtual void setRootNode(VisitableSyntax *vs);

  virtual void parse (void)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  ECDriver *getDriver (void);

  void setNameTable(NameTable &nt);

  typedef ACE_Hash_Map_Manager<ACE_CString,RtEventChannelAdmin::SchedInfo,ACE_Null_Mutex> QoSTable;

  typedef std::vector<RtEventChannelAdmin::RtSchedEventChannel_var> ECVector;
  typedef std::vector<Kokyu_EC*> KokyuECVector;
  typedef std::vector<Gateway_Initializer*> GatewayInitVector;

  typedef ACE_Hash_Map_Manager<ACE_CString,ECSupplier*,ACE_Null_Mutex> SupplierMap;
  typedef ACE_Hash_Map_Manager<ACE_CString,ECConsumer*,ACE_Null_Mutex> ConsumerMap;
private:

  NameTable nametable;
  QoSTable eventqostable;
  QoSTable timeoutqostable;

  VisitableSyntax *root;

  ECDriver *driver;

  // ORB stuff
  CORBA::ORB_var orb;
  PortableServer::POA_var poa;

  // EC stuff
  KokyuECVector kokyuECs; // for convenience
  ECVector localECs;
  ECVector remoteECs;
  GatewayInitVector ginitv;

  // Consumer/Supplier stuff
  SupplierMap suppliermap;
  ConsumerMap consumermap;
};

#endif /* CONFIGURATOR_SYNTAXHANDLER_H */
