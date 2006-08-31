#ifndef SIMLBONX_H
#define SIMLBONX_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
class GatewayGeneratorVisitor;
///EUP

namespace SIML
{

DECLARE_BONEXTENSION( BON::Model, SystemImpl, System );
DECLARE_BONEXTENSION( BON::Atom, ModuleImpl, Module );
DECLARE_BONEXTENSION( BON::Connection, PortProxyImpl, PortProxy );

//*******************************************************************
//   C  L  A  S  S   SystemImpl
//*******************************************************************
class SystemImpl :
    virtual public BON::ModelImpl
{
public:

  //
  // kind and role getters
  virtual std::set<Module>                getModule();
  virtual std::set<PortProxy>             getPortProxy();

  ///BUP
  virtual void accept (GatewayGeneratorVisitor* visitor);
  ///EUP
};

//*******************************************************************
//   C  L  A  S  S   ModuleImpl
//*******************************************************************
class ModuleImpl :
    virtual public BON::AtomImpl
{
public:

  //
  // connectionEnd getters
  virtual std::set<PortProxy>             getInPortProxyLinks();
  virtual std::set<PortProxy>             getOutPortProxyLinks();
  virtual std::multiset<Module>           getPortProxyDsts();
  virtual std::set<PortProxy>             getPortProxyLinks();
  virtual std::multiset<Module>           getPortProxySrcs();

  ///BUP
  virtual void accept (GatewayGeneratorVisitor* visitor);
  ///EUP
};

//*******************************************************************
//   C  L  A  S  S   PortProxyImpl
//*******************************************************************
class PortProxyImpl :
    virtual public BON::ConnectionImpl
{
public:
  typedef enum
    {
      CPP_language_Type,
      Java_language_Type,
      CSharp_language_Type
    } language_Type;

  //
  // attribute getters and setters
  virtual std::string getdstPortName() ;
  virtual PortProxyImpl::language_Type    getlanguage();
  virtual std::string getsrcPortName() ;
  virtual void        setdstPortName( const std::string& val);
  virtual void        setlanguage( PortProxyImpl::language_Type val);
  virtual void        setsrcPortName( const std::string& val);
  //
  // connectionEnd getters
  virtual Module      getDst();
  virtual Module      getSrc();

  ///BUP
  virtual void accept (GatewayGeneratorVisitor* visitor);
  ///EUP
};



} // namespace SIML

///BUP
// add your additional class definitions here
///EUP

#endif // SIMLBONX_H
