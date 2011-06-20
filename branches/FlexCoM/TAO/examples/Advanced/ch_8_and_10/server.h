//=============================================================================
/**
 *  @file    server.h
 *
 *  $Id$
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
#include <iostream>
#include <assert.h>

using namespace std;

class Controller_impl;

class Thermometer_impl : public virtual POA_CCS::Thermometer {
public:
    // CORBA attributes
    virtual CCS::ModelType  model();
    virtual CCS::AssetType  asset_num();
    virtual CCS::TempType   temperature();
    virtual CCS::LocType    location();
    virtual void            location(const char * loc);

    // Constructor and destructor
    Thermometer_impl(CCS::AssetType anum, const char * location);
    virtual ~Thermometer_impl();

    static Controller_impl *    m_ctrl; // My controller

protected:
    const CCS::AssetType        m_anum; // My asset number

private:
    // Helper functions
    CCS::ModelType  get_model();
    CCS::TempType   get_temp();
    CCS::LocType    get_loc();
    void            set_loc(const char * new_loc);

    // Copy and assignment not supported
    Thermometer_impl(const Thermometer_impl &);
    void operator=(const Thermometer_impl &);
};

class Thermostat_impl :
    public virtual POA_CCS::Thermostat,
    public virtual Thermometer_impl {
public:
    // CORBA operations
    virtual CCS::TempType get_nominal();
    virtual CCS::TempType set_nominal(CCS::TempType new_temp);

    // Constructor and destructor
    Thermostat_impl(
        CCS::AssetType  anum,
        const char *    location,
        CCS::TempType   nominal_temp
    );
    virtual ~Thermostat_impl() {}

private:
    // Helper functions
    CCS::TempType get_nominal_temp();
    CCS::TempType set_nominal_temp(CCS::TempType new_temp);

    // Copy and assignment not supported
    Thermostat_impl(const Thermostat_impl &);
    void operator=(const Thermostat_impl &);
};

class Controller_impl : public virtual POA_CCS::Controller {
public:
    // CORBA operations
    virtual CCS::Controller::ThermometerSeq* list();
    virtual void find(CCS::Controller::SearchSeq & slist);
    virtual void change(const CCS::Controller::ThermostatSeq & tlist,
                        CORBA::Short                           delta);

    // Constructor and destructor
    Controller_impl() {}
    virtual ~Controller_impl() {}

    // Helper functions to allow thermometers and
    // thermostats to add themselves to the m_assets map
    // and to remove themselves again.
    void    add_impl(CCS::AssetType anum, Thermometer_impl * tip);
    void    remove_impl(CCS::AssetType anum);

private:
    // Map of known servants
    typedef map<CCS::AssetType, Thermometer_impl *> AssetMap;
    AssetMap m_assets;

    // Copy and assignment not supported
    Controller_impl(const Controller_impl &);
    void operator=(const Controller_impl &);

    // Function object for the find_if algorithm to search for
    // devices by location and model string.
    class StrFinder {
    public:
        StrFinder(
            CCS::Controller::SearchCriterion    sc,
            const char *                        str
        ) : m_sc(sc), m_str(str) {}
        bool operator()(
            pair<const CCS::AssetType, Thermometer_impl *> & p
        ) const
        {
            switch (m_sc) {
            case CCS::Controller::LOCATION:
                return strcmp(p.second->location(), m_str) == 0;
                break;
            case CCS::Controller::MODEL:
                return strcmp(p.second->model(), m_str) == 0;
                break;
            default:
              assert(0);  // Precondition violation
            }
            return 0;       // Stops compiler warning
        }
    private:
        CCS::Controller::SearchCriterion    m_sc;
        const char *                        m_str;
    };
};

#endif
