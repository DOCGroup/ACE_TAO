// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/examples/Advanced/ch_18
//
// = FILENAME
//    server.h
//
// = AUTHORS
//   Source code used in TAO has been modified and adapted from the code
//   provided in the book, "Advanced CORBA Programming with C++" by Michi
//   Henning and Steve Vinoski. Copyright 1999. Addison-Wesley, Reading,
//   MA.
//
//   Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
//
// ============================================================================




#ifndef server_HH_
#define server_HH_

#include "CCSS.h"
#include "icp.h"
#include <map>
#include <list>
//#include <assert.h>
//#include <corba/poaS.h>


class Controller_impl;

class Thermometer_impl : public virtual POA_CCS::Thermometer {
public:
    // CORBA attributes
    virtual CCS::ModelType  model()
                                throw(CORBA::SystemException);
    virtual CCS::AssetType  asset_num()
                                throw(CORBA::SystemException);
    virtual CCS::TempType   temperature()
                                throw(CORBA::SystemException);
    virtual CCS::LocType    location()
                                throw(CORBA::SystemException);
    virtual void            location(const char *loc)
                                throw(CORBA::SystemException);
    virtual void            remove()
                                throw(CORBA::SystemException);

    // Constructor & destructor
    Thermometer_impl(CCS::AssetType anum);
    virtual ~Thermometer_impl();

    static Controller_impl *    m_ctrl; // My controller
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
    virtual CCS::TempType   get_nominal()
                                throw(CORBA::SystemException);
    virtual CCS::TempType   set_nominal(
                                CCS::TempType new_temp
                            ) throw(
                                CORBA::SystemException,
                                CCS::Thermostat::BadTemp
                            );

    // Constructor and destructor
    Thermostat_impl(CCS::AssetType anum);
    virtual ~Thermostat_impl();

private:
    // Helper functions
    CCS::TempType   get_nominal_temp();
    CCS::TempType   set_nominal_temp(CCS::TempType new_temp)
                        throw(CCS::Thermostat::BadTemp);

    // Copy and assignment not supported
    Thermostat_impl(const Thermostat_impl &);
    void operator=(const Thermostat_impl &);
};

class Controller_impl : public virtual POA_CCS::Controller {
public:
    // CORBA operations
    virtual CCS::Controller::ThermometerSeq *
                list() throw(CORBA::SystemException);
    virtual void
                find(CCS::Controller::SearchSeq & slist)
                    throw(CORBA::SystemException);
    virtual void
                change(
                    const CCS::Controller::ThermostatSeq & tlist,
                    CORBA::Short                           delta
                ) throw(
                    CORBA::SystemException,
                    CCS::Controller::EChange
                );
    virtual CCS::Thermometer_ptr
                create_thermometer(
                    CCS::AssetType      anum,
                    const char*     loc
                ) throw(
                    CORBA::SystemException,
                    CCS::Controller::DuplicateAsset
                );
    virtual CCS::Thermostat_ptr
                create_thermostat(
                    CCS::AssetType      anum,
                    const char*     loc,
                    CCS::TempType       temp
                ) throw(
                    CORBA::SystemException,
                    CCS::Controller::DuplicateAsset,
                    CCS::Thermostat::BadTemp
                );

    // Constructor and destructor
    Controller_impl(
        PortableServer::POA_ptr poa,
        const char *            asset_file
    ) throw(int);
    virtual ~Controller_impl();

    // Helper functions to allow access to the object map
    void                add_impl(
                            CCS::AssetType      anum,
                            Thermometer_impl *  tip
                        );
    void                remove_impl(CCS::AssetType anum);
    bool                exists(CCS::AssetType anum);

private:
    // Map of existing assets. The servant pointer is null
    // the corresponding servant is not in memory.
    typedef map<CCS::AssetType, Thermometer_impl *> AssetMap;
    AssetMap m_assets;

    // POA for thermometers and thermostats
    PortableServer::POA_var m_poa;

    // Name of asset number file
    CORBA::String_var m_asset_file;

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
            char buf[32];
            switch (m_sc) {
            case CCS::Controller::LOCATION:
                ICP_get(p.first, "location", buf, sizeof(buf));
                break;
            case CCS::Controller::MODEL:
                ICP_get(p.first, "model", buf, sizeof(buf));
                break;
            default:
                assert(0);  // Precondition violation
            }
            return strcmp(buf, m_str) == 0;
        }
    private:
        CCS::Controller::SearchCriterion    m_sc;
        const char *                        m_str;
    };
};

class DeviceLocator_impl :
    public virtual PortableServer::ServantLocator {
public:
    DeviceLocator_impl(Controller_impl * ctrl);

    virtual PortableServer::Servant
                preinvoke(
                    const PortableServer::ObjectId & oid,
                    PortableServer::POA_ptr          poa,
                    const char *                     operation,
                    void * &                         cookie
                ) throw(
                    CORBA::SystemException,
                    PortableServer::ForwardRequest);
    virtual void
                postinvoke(
                    const PortableServer::ObjectId & /* oid */,
                    PortableServer::POA_ptr          /* poa */,
                    const char *                     /* operation */,
                    void *                           /* cookie */,
                    PortableServer::Servant          /* servant */
                ) throw(CORBA::SystemException) {}

private:
    Controller_impl *                   m_ctrl;

    typedef list<Thermometer_impl *>    EvictorQueue;
    typedef map<CCS::AssetType, EvictorQueue::iterator>
                                        ActiveObjectMap;

    static const unsigned int           MAX_EQ_SIZE = 100;
    EvictorQueue                        m_eq;
    ActiveObjectMap                     m_aom;

    // Copy and assignment not supported
    DeviceLocator_impl(const DeviceLocator_impl &);
    void operator=(const DeviceLocator_impl &);
};

#endif
