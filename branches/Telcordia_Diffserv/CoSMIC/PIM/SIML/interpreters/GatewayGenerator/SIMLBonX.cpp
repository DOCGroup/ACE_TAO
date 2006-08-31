#include "stdafx.h"
#include "SIMLBonX.h"
#include "GatewayGeneratorVisitor.h"

namespace BON
{

IMPLEMENT_BONEXTENSION( SIML::System, "SIML::System" );
IMPLEMENT_BONEXTENSION( SIML::Module, "SIML::Module" );
IMPLEMENT_BONEXTENSION( SIML::PortProxy, "SIML::PortProxy" );

} // end namespace BON

namespace SIML
{

void
SystemImpl::accept (GatewayGeneratorVisitor* visitor)
{
  System system (this);
  visitor->visitSystem (system);
}

void
ModuleImpl::accept (GatewayGeneratorVisitor* visitor)
{
  Module module (this);
  visitor->visitModule (module);
}

void
PortProxyImpl::accept (GatewayGeneratorVisitor* visitor)
{
  PortProxy portProxy (this);
  visitor->visitPortProxy (portProxy);
}

//********************************************************************************
// getter for role "Module" among "Module"s
//********************************************************************************
std::set<Module> SystemImpl::getModule()
{
  std::set<Module> res;
  std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("SIML::Module");
  for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
    {
      Module elem(*i);
      ASSERT(elem);
      res.insert(elem);
    }
  return res;
}


//********************************************************************************
// getter for role "PortProxy" among "PortProxy"s
//********************************************************************************
std::set<PortProxy> SystemImpl::getPortProxy()
{
  std::set<PortProxy> res;
  std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("SIML::PortProxy");
  for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
    {
      PortProxy elem(*i);
      ASSERT(elem);
      res.insert(elem);
    }
  return res;
}

//********************************************************************************
//
//********************************************************************************
std::set<PortProxy> ModuleImpl::getInPortProxyLinks()
{
  std::set<PortProxy> result;
  std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
  std::set<BON::Connection>::iterator it = conns.begin();
  for( ; it != conns.end(); ++it)
    {
      PortProxy c( *it);
      if (c)
        result.insert( c);
    }
  return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PortProxy> ModuleImpl::getOutPortProxyLinks()
{
  std::set<PortProxy> result;
  std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
  std::set<BON::Connection>::iterator it = conns.begin();
  for( ; it != conns.end(); ++it)
    {
      PortProxy c( *it);
      if (c)
        result.insert( c);
    }
  return result;
}


//********************************************************************************
// returns dst Modules
//********************************************************************************
std::multiset<Module> ModuleImpl::getPortProxyDsts()
{
  std::multiset<Module> res;
  {
    std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("SIML::PortProxy");
    for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
      {
        Module dst( *cit );
        ASSERT(dst);
        res.insert( dst);
      }
  }
  return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<PortProxy> ModuleImpl::getPortProxyLinks()
{
  std::set<PortProxy> result;
  std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
  std::set<BON::Connection>::iterator it = conns.begin();
  for( ; it != conns.end(); ++it)
    {
      PortProxy c( *it);
      if (c)
        result.insert( c);
    }
  return result;
}


//********************************************************************************
// returns src Modules
//********************************************************************************
std::multiset<Module> ModuleImpl::getPortProxySrcs()
{
  std::multiset<Module> res;
  {
    std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("SIML::PortProxy");
    for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
      {
        Module dst( *cit );
        ASSERT(dst);
        res.insert( dst);
      }
  }
  return res;
}

//********************************************************************************
//
//********************************************************************************
std::string PortProxyImpl::getdstPortName()
{
  return FCOImpl::getAttribute("dstPortName")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
PortProxyImpl::language_Type PortProxyImpl::getlanguage()
{
  std::string val = FCOImpl::getAttribute("language")->getStringValue();

  if ( val == "CPP") return CPP_language_Type;
  else if ( val == "Java") return Java_language_Type;
  else if ( val == "CSharp") return CSharp_language_Type;
  else throw("None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
std::string PortProxyImpl::getsrcPortName()
{
  return FCOImpl::getAttribute("srcPortName")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void PortProxyImpl::setdstPortName( const std::string& val)
{
  FCOImpl::getAttribute("dstPortName")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void PortProxyImpl::setlanguage( PortProxyImpl::language_Type val)
{
  std::string str_val = "";

  if ( val == CPP_language_Type) str_val = "CPP";
  else if ( val == Java_language_Type) str_val = "Java";
  else if ( val == CSharp_language_Type) str_val = "CSharp";
  else throw("None of the possible items");

  FCOImpl::getAttribute("language")->setStringValue( str_val);
}


//********************************************************************************
//
//********************************************************************************
void PortProxyImpl::setsrcPortName( const std::string& val)
{
  FCOImpl::getAttribute("srcPortName")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Module
//********************************************************************************
Module PortProxyImpl::getDst()
{
  BON::ConnectionEnd ce = ConnectionImpl::getDst();
  Module sp( ce);
  if ( sp)
    return sp;

  Module empty;
  return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Module
//********************************************************************************
Module PortProxyImpl::getSrc()
{
  BON::ConnectionEnd ce = ConnectionImpl::getSrc();
  Module sp( ce);
  if ( sp)
    return sp;

  Module empty;
  return empty;
}


} // namespace SIML
