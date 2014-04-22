//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from the codeprovided in the book
 *  @author "Advanced CORBA Programming with C++" by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
 *  @author Reading
 *  @author MA.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#include    "CCSC.h"        // ORB-specific
#include    <iostream>

using namespace std;

// This inserter may or may not be needed for your ORB.
#if !defined (GEN_OSTREAM_OPS)

// Show the details for a thermometer or thermostat.
static std::ostream &
operator<<(std::ostream & os, CCS::Thermometer_ptr t)
{
    // Check for nil.
    if (CORBA::is_nil(t)) {
        os << "Cannot show state for nil reference." << endl;
        return os;
    }

    // Try to narrow and print what kind of device it is.
    CCS::Thermostat_var tmstat = CCS::Thermostat::_narrow(t);
    os << (CORBA::is_nil(tmstat.in()) ? "Thermometer:" : "Thermostat:")
       << endl;

    // Show attribute values.
    CCS::ModelType_var model = t->model();
    CCS::LocType_var location = t->location();
    os << "\tAsset number: " << t->asset_num() << endl;
    os << "\tModel       : " << model.in() << endl;

    os << "\tLocation    : " << location.in() << endl;
    os << "\tTemperature : " << t->temperature() << endl;

    // If device is a thermostat, show nominal temperature.
    if (!CORBA::is_nil(tmstat.in()))
        os << "\tNominal temp: " << tmstat->get_nominal() << endl;
    return os;
}

//----------------------------------------------------------------

// Show the information in a BtData struct.

static std::ostream &
operator<<(std::ostream & os, const CCS::Thermostat::BtData & btd)
{
    os << "CCS::Thermostat::BtData details:" << endl;
    os << "\trequested    : " << btd.requested << endl;
    os << "\tmin_permitted: " << btd.min_permitted << endl;
    os << "\tmax_permitted: " << btd.max_permitted << endl;
    os << "\terror_msg    : " << btd.error_msg << endl;
    return os;
}

//----------------------------------------------------------------

// Loop over the sequence of records in an EChange exception and
// show the details of each record.

static std::ostream &
operator<<(std::ostream & os, const CCS::Controller::EChange & ec)
{
    for (CORBA::ULong i = 0; i < ec.errors.length(); i++) {
        os << "Change failed:" << endl;
        os << ec.errors[i].tmstat_ref.in(); // Overloaded <<
        os << ec.errors[i].info << endl;    // Overloaded <<
    }
    return os;
}

//----------------------------------------------------------------

// Generic ostream inserter for exceptions. Inserts the exception
// name, if available, and the repository ID otherwise.
#if 0

static std::ostream &
operator<<(std::ostream & os, const CORBA::Exception & e)
{
    CORBA::Any tmp;
    tmp <<= e;

    CORBA::TypeCode_var tc = tmp.type();
    const char * p = tc->name();
    if (*p != '\0')
        os << p;
    else
        os << tc->id();
    return os;
}
#endif

#endif

//----------------------------------------------------------------

// Change the temperature of a thermostat.

static void
set_temp(CCS::Thermostat_ptr tmstat, CCS::TempType new_temp)
{
    if (CORBA::is_nil(tmstat))  // Don't call via nil reference
        return;

    CCS::AssetType anum = tmstat->asset_num();
    try
    {
      std::cout << "Setting thermostat " << anum
                << " to " << new_temp << " degrees." << std::endl;
      CCS::TempType old_nominal = tmstat->set_nominal(new_temp);
      std::cout << "Old nominal temperature was: "
                << old_nominal << std::endl;
      std::cout << "New nominal temperature is: "
                << tmstat->get_nominal() << std::endl;
    }
    catch (const CCS::Thermostat::BadTemp & bt)
    {
      std::cerr << "Setting of nominal temperature failed." << std::endl;
      std::cerr << bt.details << std::endl;             // Overloaded <<
    }
}

//----------------------------------------------------------------

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
    try
    {
        // Initialize the ORB
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        // Check arguments
        if (argc != 2)
        {
            std::cerr << "Usage: client IOR_string" << std::endl;
            throw 0;
        }

        // Get controller reference from argv
        // and convert to object.
        CORBA::Object_var obj = orb->string_to_object(argv[1]);
        if (CORBA::is_nil(obj.in()))
        {
            std::cerr << "Nil controller reference" << std::endl;
            throw 0;
        }

        // Try to narrow to CCS::Controller.
        CCS::Controller_var ctrl;
        try
        {
            ctrl = CCS::Controller::_narrow(obj.in());
        }
        catch (const CORBA::SystemException & se)
        {
            std::cerr << "Cannot narrow controller reference: "
                      << se
                      << std::endl;
            throw 0;
        }
        if (CORBA::is_nil(ctrl.in()))
        {
            std::cerr << "Wrong type for controller ref." << std::endl;
            throw 0;
        }

        // Get list of devices
        CCS::Controller::ThermometerSeq_var list = ctrl->list();

        // Show number of devices.
        CORBA::ULong len = list->length();
        std::cout << "Controller has " << len << " device";
        if (len != 1)
          std::cout << "s";
        std::cout << "." << std::endl;

        // If there are no devices at all, we are finished.
        if (len == 0)
            return 0;

        // Show details for each device.
        for (CORBA::ULong i = 0; i < list->length(); i++)
          std::cout << list[i].in();
        std::cout << std::endl;

        // Change the location of first device in the list
        CCS::AssetType anum = list[(CORBA::ULong) 0]->asset_num();
        std::cout << "Changing location of device "
                  << anum << "." << std::endl;
        list[(CORBA::ULong) 0]->location("Earth");
        // Check that the location was updated
        std::cout << "New details for device "
                  << anum << " are:" << std::endl;
        CCS::Thermometer_ptr tx = list[0u];
        std::cout << tx << std::endl;

        // Find first thermostat in list.
        CCS::Thermostat_var tmstat;
        for (   CORBA::ULong j = 0;
                j < list->length() && CORBA::is_nil(tmstat.in());
                j++)
        {
            tmstat = CCS::Thermostat::_narrow(list[j]);
        }

        // Check that we found a thermostat on the list.
        if (CORBA::is_nil(tmstat.in()))
        {
          std::cout << "No thermostat devices in list." << std::endl;
        }
        else
        {
            // Set temperature of thermostat to
            // 50 degrees (should work).
            set_temp(tmstat.inout(), 50);
            std::cout << std::endl;

            // Set temperature of thermostat to
            // -10 degrees (should fail).
            set_temp(tmstat.inout(), -10);
        }

        // Look for device in Rooms Earth and HAL. This must
        // locate at least one device because we earlier changed
        // the location of the first device to Room Earth.
        std::cout << "Looking for devices in Earth and HAL." << std::endl;
        CCS::Controller::SearchSeq ss;
        ss.length(2);
        ss[0].key.loc(CORBA::string_dup("Earth"));
        ss[1].key.loc(CORBA::string_dup("HAL"));
        ctrl->find(ss);

        // Show the devices found in that room.
        for (CORBA::ULong k = 0; k < ss.length(); k++)
          std::cout << ss[k].device.in();      // Overloaded <<
        std::cout << std::endl;

        // Increase the temperature of all thermostats
        // by 40 degrees. First, make a new list (tss)
        // containing only thermostats.
        std::cout << "Increasing thermostats by 40 degrees." << std::endl;
        CCS::Controller::ThermostatSeq tss;
        for (CORBA::ULong l = 0; l < list->length(); l++)
        {
            tmstat = CCS::Thermostat::_narrow(list[l]);
            if (CORBA::is_nil(tmstat.in()))
                continue;                   // Skip thermometers
            len = tss.length();
            tss.length(len + 1);
            tss[len] = tmstat;
        }

        // Try to change all thermostats.
        try
        {
            ctrl->change(tss, 40);
        }
        catch (const CCS::Controller::EChange & ec)
        {
          std::cerr << ec;                     // Overloaded <<
        }
    }
    catch (const CORBA::Exception & e)
    {
        std::cerr << "Uncaught CORBA exception: "
                  << e
                  << std::endl;
        return 1;
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}
