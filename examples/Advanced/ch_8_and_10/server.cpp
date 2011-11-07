
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from the codeprovided in the book
 *  @author "Advanced CORBA Programming with C++" by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
 *  @author Reading
 *  @author MA.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#include    "server.h"
#include    <algorithm>
#include    "icp.h"
//#include    <iostream.h>
//#include    <assert.h>
//----------------------------------------------------------------


// Generic ostream inserter for exceptions. Inserts the exception
// name, if available, and the repository ID otherwise.

#if 0    //This inserter may or may not be needed for your ORB.

static ostream &
operator<< (ostream & os, const CORBA::Exception & e)
{
    CORBA::Any tmp;
    tmp <<= e;
    CORBA::TypeCode_var tc = tmp.type ();
    const char * p = tc->name ();
    if (*p != '\0')
        os << p;
    else
        os << tc->id ();
    return os;
}

#endif

//----------------------------------------------------------------

Controller_impl * Thermometer_impl::m_ctrl; // static member

// Helper function to read the model string from a device.

CCS::ModelType
Thermometer_impl::
get_model ()
{
    char buf[32];
    assert (ICP_get (m_anum, "model", buf, sizeof (buf)) == 0);
    return CORBA::string_dup (buf);
}

// Helper function to read the temperature from a device.

CCS::TempType
Thermometer_impl::
get_temp ()
{
    short temp;
    assert (
        ICP_get (m_anum, "temperature", &temp, sizeof (temp)
    ) == 0);
    return temp;
}

// Helper function to read the location from a device.

CCS::LocType
Thermometer_impl::
get_loc ()
{
    char buf[32];
    assert (ICP_get (m_anum, "location", buf, sizeof (buf)) == 0);
    return CORBA::string_dup (buf);
}

// Helper function to set the location of a device.

void
Thermometer_impl::
set_loc (const char * loc)
{
    long result = 0;
    result = ICP_set (m_anum, "location", loc);
    assert (result == 0);
}

// Constructor.

Thermometer_impl::
Thermometer_impl (
    CCS::AssetType      anum,
    const char *        location
) : m_anum (anum)
{
    assert (ICP_online (anum) == 0);  // Mark device as on-line
    set_loc (location);
    m_ctrl->add_impl (anum, this);   // Add self to controller's map
}

// Destructor.

Thermometer_impl::
~Thermometer_impl ()
{
    try {
        m_ctrl->remove_impl (m_anum); // Remove self from map
        ICP_offline (m_anum);         // Mark device as off-line
    }
    catch (...) {
        assert (0);      // Prevent exceptions from escaping
    }
}

// IDL model attribute.

CCS::ModelType
Thermometer_impl::model ()
{
    return get_model ();
}

// IDL asset_num attribute.

CCS::AssetType
Thermometer_impl::asset_num ()
{
    return m_anum;
}

// IDL temperature attribute.

CCS::TempType
Thermometer_impl::temperature ()
{
    return get_temp ();
}

// IDL location attribute accessor.

CCS::LocType
Thermometer_impl::location ()
{
    return get_loc ();
}

// IDL location attribute modifier.

void
Thermometer_impl::location (const char * loc)
{
    set_loc (loc);
}

//----------------------------------------------------------------

// Helper function to get a thermostat's nominal temperature.

CCS::TempType
Thermostat_impl::get_nominal_temp ()
{
    short temp;
    assert (
        ICP_get (m_anum, "nominal_temp", &temp, sizeof (temp)
    ) == 0);
    return temp;
}

// Helper function to set a thermostat's nominal temperature.

CCS::TempType
Thermostat_impl::
set_nominal_temp (CCS::TempType new_temp)
{
    short old_temp;

    // We need to return the previous nominal temperature,
    // so we first read the current nominal temperature before
    // changing it.
    assert (ICP_get (
                m_anum, "nominal_temp",
                &old_temp, sizeof (old_temp)
           ) == 0
    );

    // Now set the nominal temperature to the new value.
    if (ICP_set (m_anum, "nominal_temp", &new_temp) != 0) {

        // If ICP_set () failed, read this thermostat's minimum
        // and maximum so we can initialize the BadTemp exception.
        CCS::Thermostat::BtData btd;
        ICP_get (
            m_anum, "MIN_TEMP",
            &btd.min_permitted, sizeof (btd.min_permitted)
        );
        ICP_get (
            m_anum, "MAX_TEMP",
            &btd.max_permitted, sizeof (btd.max_permitted)
        );
        btd.requested = new_temp;
        btd.error_msg = CORBA::string_dup (
            new_temp > btd.max_permitted ? "Too hot" : "Too cold"
        );
        throw CCS::Thermostat::BadTemp (btd);
    }
    return old_temp;
}

// Constructor.

Thermostat_impl::
Thermostat_impl (
    CCS::AssetType      anum,
    const char *        location,
    CCS::TempType       nominal_temp
) : Thermometer_impl (anum, location)
{
    // Base Thermometer_impl constructor does most of the
    // work, so we need only set the nominal temperature here.
    set_nominal_temp (nominal_temp);
}

// IDL get_nominal operation.

CCS::TempType
Thermostat_impl::get_nominal ()
{
    return get_nominal_temp ();
}

// IDL set_nominal operation.

CCS::TempType
Thermostat_impl::
set_nominal (CCS::TempType new_temp)
{
    return set_nominal_temp (new_temp);
}

//----------------------------------------------------------------

// Helper function for thermometers and thermostats to
// add themselves to the m_assets map.

void
Controller_impl::
add_impl (CCS::AssetType anum, Thermometer_impl * tip)
{
    m_assets[anum] = tip;
}

// Helper function for thermometers and thermostats to
// remove themselves from the m_assets map.

void
Controller_impl::
remove_impl (CCS::AssetType anum)
{
    m_assets.erase (anum);
}

// IDL list operation.

CCS::Controller::ThermometerSeq *
Controller_impl::list ()
{
    // Create a new thermometer sequence. Because we know
    // the number of elements we will put onto the sequence,
    // we use the maximum constructor.
    CCS::Controller::ThermometerSeq_var listv
        = new CCS::Controller::ThermometerSeq (m_assets.size ());
    listv->length (m_assets.size ());

    // Loop over the m_assets map and create a
    // reference for each device.
    CORBA::ULong count = 0;
    AssetMap::iterator i;
    for (i = m_assets.begin (); i != m_assets.end (); i++)
        listv[count++] = i->second->_this ();
    return listv._retn ();
}

// IDL change operation.

void
Controller_impl::
change (
    const CCS::Controller::ThermostatSeq &  tlist,
    CORBA::Short                            delta
)
{
    CCS::Controller::EChange ec;    // Just in case we need it

    // We cannot add a delta value to a thermostat's temperature
    // directly, so for each thermostat, we read the nominal
    // temperature, add the delta value to it, and write
    // it back again.
    for (CORBA::ULong i = 0; i < tlist.length (); i++) {
        if (CORBA::is_nil (tlist[i]))
            continue;                       // Skip nil references

        // Read nominal temp and update it.
        CCS::TempType tnom = tlist[i]->get_nominal ();
        tnom += delta;
        try {
            tlist[i]->set_nominal (tnom);
        }
        catch (const CCS::Thermostat::BadTemp & bt) {
            // If the update failed because the temperature
            // is out of range, we add the thermostat's info
            // to the errors sequence.
            CORBA::ULong len = ec.errors.length ();
            ec.errors.length (len + 1);
            ec.errors[len].tmstat_ref = tlist[i];
            ec.errors[len].info = bt.details;
        }
    }

    // If we encountered errors in the above loop,
    // we will have added elements to the errors sequence.
    if (ec.errors.length () != 0)
        throw ec;
}

// IDL find operation

void
Controller_impl::
find (CCS::Controller::SearchSeq & slist)
{
    // Loop over input list and look up each device.
    CORBA::ULong listlen = slist.length ();
    for (CORBA::ULong i = 0; i < listlen; i++) {

        AssetMap::iterator where;   // Iterator for asset map
        int num_found = 0;          // Num matched per iteration

        // Assume we will not find a matching device.
        slist[i].device = CCS::Thermometer::_nil ();

        // Work out whether we are searching by asset,
        // model, or location.
        CCS::Controller::SearchCriterion sc = slist[i].key._d ();
        if (sc == CCS::Controller::ASSET) {
            // Search for matching asset number.
            where = m_assets.find (slist[i].key.asset_num ());
            if (where != m_assets.end ())
                slist[i].device = where->second->_this ();
        } else {
            // Search for model or location string.
            const char * search_str;
            if (sc == CCS::Controller::LOCATION)
                search_str = slist[i].key.loc ();
            else
                search_str = slist[i].key.model_desc ();

            // Find first matching device (if any).
            where = std::find_if (
                        m_assets.begin (), m_assets.end (),
                        StrFinder (sc, search_str)
                    );

            // While there are matches...
            while (where != m_assets.end ()) {
                if (num_found == 0) {
                    // First match overwrites reference
                    // in search record.
                    slist[i].device = where->second->_this ();
                } else {
                    // Each further match appends a new
                    // element to the search sequence.
                    CORBA::ULong len = slist.length ();
                    slist.length (len + 1);
                    slist[len].key = slist[i].key;
                    slist[len].device = where->second->_this ();
                }
                num_found++;

                // Find next matching device with this key.
                where = std::find_if (
                            ++where, m_assets.end (),
                            StrFinder (sc, search_str)
                        );
            }
        }
    }
    std::cerr << "end" << std::endl;
}

//----------------------------------------------------------------

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
    try {
        // Initialize orb
        CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

        // Get reference to Root POA.
        CORBA::Object_var obj
            = orb->resolve_initial_references ("RootPOA");
        PortableServer::POA_var poa
            = PortableServer::POA::_narrow (obj.in ());

        // Activate POA manager
        PortableServer::POAManager_var mgr
            = poa->the_POAManager ();
        mgr->activate ();

        // Create a controller and set static m_ctrl member
        // for thermostats and thermometers.
        Controller_impl ctrl_servant;
        Thermometer_impl::m_ctrl = &ctrl_servant;

        // Write controller stringified reference to stdout
        CCS::Controller_var ctrl = ctrl_servant._this ();
        CORBA::String_var str = orb->object_to_string (ctrl.in ());
        std::cout << str.in () << std::endl << std::endl;

        // Create a few devices. (Thermometers have odd asset
        // numbers, thermostats have even asset numbers.)
        Thermometer_impl thermo1 (2029, "Deep Thought");
        Thermometer_impl thermo2 (8053, "HAL");
        Thermometer_impl thermo3 (1027, "ENIAC");

        Thermostat_impl tmstat1 (3032, "Colossus", 68);
        Thermostat_impl tmstat2 (4026, "ENIAC", 60);
        Thermostat_impl tmstat3 (4088, "ENIAC", 50);
        Thermostat_impl tmstat4 (8042, "HAL", 40);

        // Accept requests
        orb->run ();
    }
    catch (const CORBA::Exception & e) {
        std::cerr << "Uncaught CORBA exception: "
                  << e
                  << std::endl;
        return 1;
    }
    catch (...) {
        assert (0);  // Unexpected exception, dump core
    }
    return 0;
}
