
//=============================================================================
/**
 *  @file    server.h
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
#include <cassert>
#include <cstring>
#include <map>
#include <list>
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

// The following headers are #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <assert.h>
//#include <corba/poaS.h>

class Controller_impl;

class Thermometer_impl : public virtual POA_CCS::Thermometer
{
public:
  Thermometer_impl (Thermometer_impl const&) = delete;
  Thermometer_impl (Thermometer_impl&&) = delete;
  Thermometer_impl& operator= (Thermometer_impl const&) = delete;
  Thermometer_impl& operator= (Thermometer_impl&&) = delete;

  // CORBA attributes
  CCS::ModelType model () override;
  CCS::AssetType asset_num () override;
  CCS::TempType temperature () override;
  CCS::LocType location () override;
  void location (char const *loc) override;
  void remove () override;

  // Constructor & destructor
  explicit Thermometer_impl (CCS::AssetType anum);
  ~Thermometer_impl () override;

  static Controller_impl *m_ctrl; // My controller
  CCS::AssetType const m_anum; // My asset number

private:
  // Helper functions
  CCS::ModelType get_model () const;
  CCS::TempType get_temp () const;
  CCS::LocType get_loc () const;
  void set_loc (char const *new_loc);
};

class Thermostat_impl : public virtual POA_CCS::Thermostat,
                        public virtual Thermometer_impl
{
public:
  Thermostat_impl (Thermostat_impl const&) = delete;
  Thermostat_impl (Thermostat_impl&&) = delete;
  Thermostat_impl& operator= (Thermostat_impl const&) = delete;
  Thermostat_impl& operator= (Thermostat_impl&&) = delete;

  // CORBA operations
  CCS::TempType get_nominal () override;
  CCS::TempType set_nominal (CCS::TempType new_temp) override;

  // Constructor and destructor
  explicit Thermostat_impl (CCS::AssetType anum);
  ~Thermostat_impl () override = default;

private:
  // Helper functions
  CCS::TempType get_nominal_temp () const;
  CCS::TempType set_nominal_temp (CCS::TempType new_temp);
};

class Controller_impl : public virtual POA_CCS::Controller
{
public:
  Controller_impl (Controller_impl const&) = delete;
  Controller_impl (Controller_impl&&) = delete;
  Controller_impl& operator= (Controller_impl const&) = delete;
  Controller_impl& operator= (Controller_impl&&) = delete;

  // CORBA operations
  CCS::Controller::ThermometerSeq* list () override;
  void find (CCS::Controller::SearchSeq& slist) override;
  void change (CCS::Controller::ThermostatSeq const& tlist,
               CORBA::Short delta) override;
  CCS::Thermometer_ptr create_thermometer (CCS::AssetType anum,
                                           char const *loc) override;
  CCS::Thermostat_ptr create_thermostat (CCS::AssetType anum,
                                         char const *loc,
                                         CCS::TempType temp) override;

  // Constructor and destructor
  Controller_impl (PortableServer::POA_ptr poa,
                   char const *asset_file);
  ~Controller_impl () override;

  // Helper functions to allow access to the object map
  void add_impl (CCS::AssetType anum, Thermometer_impl *tip);
  void remove_impl (CCS::AssetType anum);
  bool exists (CCS::AssetType anum) const;

private:
  // Map of existing assets. The servant pointer is null the
  // corresponding servant is not in memory.
  using AssetMap = std::map<CCS::AssetType, Thermometer_impl *>;
  AssetMap m_assets;

  // POA for thermometers and thermostats
  PortableServer::POA_var m_poa;

  // Name of asset number file
  CORBA::String_var m_asset_file;

  // Function object for the find_if algorithm to search for
  // devices by location and model string.
  class StrFinder
  {
  public:
    StrFinder (CCS::Controller::SearchCriterion sc,
               char const *str)
      : m_sc (sc), m_str (str) {}
    bool operator () (std::pair<CCS::AssetType const, Thermometer_impl *> const& p) const
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
          assert (false);  // Precondition violation
          return false;
        }
      return std::strcmp (buf, m_str) == 0;
    }
  private:
    CCS::Controller::SearchCriterion    m_sc;
    char const *m_str;
  };
};

class DeviceLocator_impl :
    public virtual PortableServer::ServantLocator
{
public:
  DeviceLocator_impl (DeviceLocator_impl const&) = delete;
  DeviceLocator_impl (DeviceLocator_impl&&) = delete;
  DeviceLocator_impl& operator= (DeviceLocator_impl const&) = delete;
  DeviceLocator_impl& operator= (DeviceLocator_impl&&) = delete;

  explicit DeviceLocator_impl (Controller_impl *ctrl);

  PortableServer::Servant
  preinvoke (PortableServer::ObjectId const& oid,
             PortableServer::POA_ptr poa,
             char const *operation,
             void *&cookie) override;
  void
  postinvoke (PortableServer::ObjectId const& /*oid*/,
              PortableServer::POA_ptr /*poa*/,
              char const * /*operation*/,
              void * /*cookie*/,
              PortableServer::Servant /*servant*/) override {}
private:
  Controller_impl *m_ctrl;

  using EvictorQueue = std::list<Thermometer_impl *>;
  using ActiveObjectMap = std::map<CCS::AssetType, EvictorQueue::iterator>;

  static constexpr unsigned int MAX_EQ_SIZE = 100;
  EvictorQueue m_eq;
  ActiveObjectMap m_aom;
};

#endif
