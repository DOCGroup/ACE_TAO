
//=============================================================================
/**
 *  @file    server.h
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from thecode provided in the book
 *  @author "Advanced CORBA Programming with C++"by Michi Henning and Steve Vinoski. Copyright1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission ofAddison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#ifndef server_HH_
#define server_HH_

#include "CCSS.h"
#include "icp.h"      // mine
#include "assert.h"
#include <map>
#include <list>
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

using namespace std;

// The following headers are #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <assert.h>
//#include <corba/poaS.h>

class Controller_impl;

class Thermometer_impl : public virtual POA_CCS::Thermometer
{
public:
  // CORBA attributes
  virtual CCS::ModelType  model ();
  virtual CCS::AssetType  asset_num ();
  virtual CCS::TempType   temperature ();
  virtual CCS::LocType    location ();
  virtual void            location (const char *loc);
  virtual void            remove ();

  // Constructor & destructor
  Thermometer_impl (CCS::AssetType anum);
  virtual ~Thermometer_impl ();

  static Controller_impl *    m_ctrl; // My controller
  const CCS::AssetType        m_anum; // My asset number

private:
  // Helper functions
  CCS::ModelType  get_model ();
  CCS::TempType   get_temp ();
  CCS::LocType    get_loc ();
  void            set_loc (const char * new_loc);

  // Copy and assignment not supported
  Thermometer_impl (const Thermometer_impl &);
  void operator= (const Thermometer_impl &);
};

class Thermostat_impl : public virtual POA_CCS::Thermostat,
                        public virtual Thermometer_impl
{
public:
  // CORBA operations
  virtual CCS::TempType   get_nominal ();
  virtual CCS::TempType   set_nominal (CCS::TempType new_temp);

  // Constructor and destructor
  Thermostat_impl (CCS::AssetType anum);
  virtual ~Thermostat_impl ();

private:
  // Helper functions
  CCS::TempType   get_nominal_temp ();
  CCS::TempType   set_nominal_temp (CCS::TempType new_temp);

  // Copy and assignment not supported
  Thermostat_impl (const Thermostat_impl &);
  void operator= (const Thermostat_impl &);
};

class Controller_impl : public virtual POA_CCS::Controller
{
public:
  // CORBA operations
  virtual CCS::Controller::ThermometerSeq* list ();
  virtual void
  find (CCS::Controller::SearchSeq & slist);
  virtual void change (const CCS::Controller::ThermostatSeq & tlist,
                       CORBA::Short                           delta);
  virtual CCS::Thermometer_ptr
  create_thermometer (CCS::AssetType      anum,
                      const char*     loc);
  virtual CCS::Thermostat_ptr
  create_thermostat (CCS::AssetType      anum,
                     const char*     loc,
                     CCS::TempType       temp);

  // Constructor and destructor
  Controller_impl (PortableServer::POA_ptr poa,
                   const char *            asset_file);
  virtual ~Controller_impl ();

  // Helper functions to allow access to the object map
  void                add_impl (CCS::AssetType      anum,
                                Thermometer_impl *  tip);
  void                remove_impl (CCS::AssetType anum);
  bool                exists (CCS::AssetType anum);

private:
  // Map of existing assets. The servant pointer is null the
  // corresponding servant is not in memory.
  typedef map<CCS::AssetType, Thermometer_impl *> AssetMap;
  AssetMap m_assets;

  // POA for thermometers and thermostats
  PortableServer::POA_var m_poa;

  // Name of asset number file
  CORBA::String_var m_asset_file;

  // Copy and assignment not supported
  Controller_impl (const Controller_impl &);
  void operator= (const Controller_impl &);

  // Function object for the find_if algorithm to search for
  // devices by location and model string.
  class StrFinder
  {
  public:
    StrFinder (CCS::Controller::SearchCriterion    sc,
               const char *                        str)
      : m_sc (sc), m_str (str) {}
    bool operator () (pair<const CCS::AssetType, Thermometer_impl *> & p) const
    {
      char buf[32];
      switch  (m_sc)
        {
        case CCS::Controller::LOCATION:
          ICP_get (p.first, "location", buf, sizeof (buf));
          break;
        case CCS::Controller::MODEL:
          ICP_get (p.first, "model", buf, sizeof (buf));
          break;
        default:
          assert (0);  // Precondition violation
        }
      return strcmp (buf, m_str) == 0;
    }
  private:
    CCS::Controller::SearchCriterion    m_sc;
    const char *                        m_str;
  };
};

class DeviceLocator_impl :
    public virtual PortableServer::ServantLocator
{
public:
  DeviceLocator_impl (Controller_impl * ctrl);

  virtual PortableServer::Servant
  preinvoke (const PortableServer::ObjectId & oid,
             PortableServer::POA_ptr          poa,
             const char *                     operation,
             void * &                         cookie);
  virtual void
  postinvoke (const PortableServer::ObjectId & /*oid*/,
              PortableServer::POA_ptr          /*poa*/,
              const char *                     /*operation*/,
              void *                           /*cookie*/,
              PortableServer::Servant          /*servant*/) {}
private:
  Controller_impl *                   m_ctrl;

  typedef list<Thermometer_impl *>    EvictorQueue;
  typedef map<CCS::AssetType, EvictorQueue::iterator>
  ActiveObjectMap;

  static const unsigned int           MAX_EQ_SIZE;// = 100;
  EvictorQueue                        m_eq;
  ActiveObjectMap                     m_aom;

  // Copy and assignment not supported
  DeviceLocator_impl (const DeviceLocator_impl &);
  void operator= (const DeviceLocator_impl &);
};

#endif
