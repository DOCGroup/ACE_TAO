//=============================================================================
/**
 *  @file    server.h
 *
 *  @author Source code used in TAO has been modified and adapted from the codeprovided in the book
 *  @author "Advanced CORBA Programming with C++" by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
 *  @author Reading
 *  @author MA.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================




#ifndef server_HH_
#define server_HH_

#include "CCSS.h"
#include <map>
#include <cassert>
#include <cstring>
#include <utility>

class Controller_impl;

class Thermometer_impl : public virtual POA_CCS::Thermometer {
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

    // Constructor and destructor
    Thermometer_impl (CCS::AssetType anum, char const *location);
    ~Thermometer_impl () override;

    static Controller_impl *    m_ctrl; // My controller

protected:
    CCS::AssetType const m_anum; // My asset number

private:
    // Helper functions
    CCS::ModelType  get_model();
    CCS::TempType   get_temp();
    CCS::LocType    get_loc();
    void set_loc (char const *new_loc);
};

class Thermostat_impl :
    public virtual POA_CCS::Thermostat,
    public virtual Thermometer_impl {
public:
    // CORBA operations
    CCS::TempType get_nominal () override;
    CCS::TempType set_nominal (CCS::TempType new_temp) override;

    // Constructor and destructor
    Thermostat_impl(
        CCS::AssetType  anum,
        char const *    location,
        CCS::TempType   nominal_temp
    );
    ~Thermostat_impl () override = default;

private:
    // Helper functions
    CCS::TempType get_nominal_temp();
    CCS::TempType set_nominal_temp(CCS::TempType new_temp);

    Thermostat_impl (Thermostat_impl const&) = delete;
    Thermostat_impl (Thermostat_impl&&) = delete;
    Thermostat_impl& operator= (Thermostat_impl const&) = delete;
    Thermostat_impl& operator= (Thermostat_impl&&) = delete;
};

class Controller_impl : public virtual POA_CCS::Controller {
public:
    // CORBA operations
    CCS::Controller::ThermometerSeq* list () override;
    void find (CCS::Controller::SearchSeq& slist) override;
    void change (CCS::Controller::ThermostatSeq const& tlist,
                 CORBA::Short delta) override;

    // Constructor and destructor
    Controller_impl () = default;
    ~Controller_impl () override = default;

    Controller_impl (Controller_impl const&) = delete;
    Controller_impl (Controller_impl&&) = delete;
    Controller_impl& operator= (Controller_impl const&) = delete;
    Controller_impl& operator= (Controller_impl&&) = delete;

    // Helper functions to allow thermometers and
    // thermostats to add themselves to the m_assets map
    // and to remove themselves again.
    void add_impl (CCS::AssetType anum, Thermometer_impl *tip);
    void remove_impl (CCS::AssetType anum);

private:
    // Map of known servants
    using AssetMap = std::map<CCS::AssetType, Thermometer_impl *>;
    AssetMap m_assets;

    // Function object for the find_if algorithm to search for
    // devices by location and model string.
    class StrFinder {
    public:
        StrFinder(
            CCS::Controller::SearchCriterion sc,
            char const *str
        ) : m_sc (sc), m_str (str) {}
        bool operator() (
            std::pair<CCS::AssetType const, Thermometer_impl *> const& p) const
        {
            switch (m_sc) {
            case CCS::Controller::LOCATION:
                return std::strcmp (p.second->location (), m_str) == 0;
                break;
            case CCS::Controller::MODEL:
                return std::strcmp (p.second->model (), m_str) == 0;
                break;
            default:
              assert (false);  // Precondition violation
            }
            return false;       // Stops compiler warning
        }
    private:
        CCS::Controller::SearchCriterion    m_sc;
        char const *m_str;
    };
};

#endif
