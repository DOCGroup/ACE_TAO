//=============================================================================
/**
 *  @file    icp.cpp
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from the codeprovided in the book
 *  @author "Advanced CORBA Programming with C++" by MichiHenning and Steve Vinoski. Copyright 1999. Addison-Wesley
 *  @author Reading
 *  @author MA.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================





#include    <string>
#include    <map>
#include    <algorithm>
#include    <stdlib.h>
#include    <iostream>
#include    "icp.h"

using namespace std;

//----------------------------------------------------------------

enum DeviceType { thermometer, thermostat };

struct DeviceState {                // State for a device
    DeviceType      type;
    const char *    model;
    string          location;
    short           nominal_temp;   // For thermostats only
};
typedef map<unsigned long, DeviceState> StateMap;

//----------------------------------------------------------------

const size_t MAXSTR = 32;       // Max len of string including NUL

const short MIN_TEMP = 40;      // 40 F ==  4.44 C
const short MAX_TEMP = 90;      // 90 F == 32.22 C
const short DFLT_TEMP = 68;     // 68 F == 20.00 C

static StateMap dstate;         // Map of known devices

//----------------------------------------------------------------

// ICP_online() simulates adding a new device to the network by
// adding it to the dstate map.
//
// For this simple simulation, devices with odd asset numbers
// are thermometers and devices with even asset numbers
// are thermostats.
//
// Thermostats get an initial nominal temperature of DFLT_TEMP.
// The location string is intentionally left blank because it
// must be programmed by the controller after putting the device
// on-line (as should be the nominal temperature).
//
// If a device with the specified ID is on-line already, the
// return value is -1. A zero return value indicates success.

extern "C"
int
ICP_online(unsigned long id)
{
    // Look for id in state map.
    StateMap::iterator pos = dstate.find(id);
    if (pos != dstate.end())
        return -1;                          // Already exists

    // Fill in state.
    DeviceState ds;
    ds.type = (id % 2) ? thermometer : thermostat;
    ds.model = (ds.type == thermometer)
                ? "Sens-A-Temp" : "Select-A-Temp";
    ds.nominal_temp = DFLT_TEMP;

    // Insert new device into map
    dstate[id] = ds;

    return 0;
}

//----------------------------------------------------------------

// ICP_offline() simulates removing a device from the network by
// removing it from the dstate map. If the device isn't known, the
// return value is -1. A zero return value indicates success.

extern "C"
int
ICP_offline(unsigned long id)
{
    // Look for id in state map
    StateMap::iterator pos = dstate.find(id);
    if (pos == dstate.end())
        return -1;                              // No such device
    dstate.erase(id);
    return 0;
}

//----------------------------------------------------------------

// vary_temp() simulates the variation in actual temperature
// around a thermostat.The function randomly varies the
// temperature as a percentage of calls as follows:
//
//      3 degrees too cold:      5%
//      3 degrees too hot:       5%
//      2 degrees too cold:     10%
//      2 degrees too hot:      10%
//      1 degree too cold:      15%
//      1 degree too hot:       15%
//      exact temperature:      40%

static
long
vary_temp(long temp)
{
    long r = ACE_OS::rand() % 50;
    long delta;
    if (r < 5)
        delta = 3;
    else if (r < 15)
        delta = 2;
    else if (r < 30)
        delta = 1;
    else
        delta = 0;
    if (ACE_OS::rand() % 2)
        delta = -delta;
    return temp + delta;

}

//----------------------------------------------------------------

// Function object. Locates a thermostat that is in the same room
// as the device at position pos.

class ThermostatInSameRoom {
public:
            ThermostatInSameRoom(
                const StateMap::iterator & pos
            ) : m_pos(pos) {}
    bool    operator()(
                pair<const unsigned long, DeviceState> & p
            ) const
            {
                return(
                        p.second.type == thermostat
                        && p.second.location
                                == m_pos->second.location
                );
            }
private:
    const StateMap::iterator & m_pos;
};

//----------------------------------------------------------------

// actual_temp() is a helper function to determine the actual
// temperature returned by a particular thermometer or thermostat.
// The pos argument indicates the device.
//
// The function locates all thermostats that are in the same room
// as the device denoted by pos and computes the average of all
// the thermostats' nominal temperatures. (If no thermostats are
// in the same room as the device, the function assumes that the
// average of the nominal temperatures is DFLT_TEMP.)
//
// The returned temperature varies from the average as
// determined by vary_temp().

static
long
actual_temp(const StateMap::iterator & pos)
{
    long sum = 0;
    long count = 0;
    StateMap::iterator where = std::find_if(
                                    dstate.begin(), dstate.end(),
                                    ThermostatInSameRoom(pos)
                               );
    while (where != dstate.end()) {
        count++;
        sum += where->second.nominal_temp;
        where = std::find_if(
                    ++where, dstate.end(),
                    ThermostatInSameRoom(pos)
                );
    }
    return vary_temp(count == 0 ? DFLT_TEMP : sum / count);
}

//---------------------------------------------------------------

// ICP_get() returns an attribute value of the device with the
// given id. The attribute is named by the attr parameter. The
// value is copied into the buffer pointed to by the value
// pointer. The len parameter is the size of the passed buffer,
// so ICP_get can avoid overrunning the buffer.
//
// By default, thermometers report a temperature that varies
// somewhat around DFLT_TEMP. However, if there is another
// thermostat in the same room as the the thermometer, the
// thermometer reports a temperature that varies around that
// thermostat's temperature. For several thermostats that are in
// the same room, the thermometer reports around the average
// nominal temperature of all the thermostats.
//
// Attempts to read from a non-existent device or to read a
// non-existent attribute return -1. A return value of zero
// indicates success. If the supplied buffer is too short to hold
// a value, ICP_get() silently truncates the value and
// returns success.

extern "C"
int
ICP_get(
    unsigned long   id,
    const char *    attr,
    void *          value,
    size_t          len)
{
    // Look for id in state map
    StateMap::iterator pos = dstate.find(id);
    if (pos == dstate.end())
        return -1;                              // No such device

    // Depending on the attribute, return the
    // corresponding piece of state.
    if (ACE_OS::strcmp(attr, "model") == 0) {
        ACE_OS::strncpy((char *)value, pos->second.model, len);
    } else if (ACE_OS::strcmp(attr, "location") == 0) {
      ACE_OS::strncpy((char *)value, pos->second.location.c_str(), len);
    } else if (ACE_OS::strcmp(attr, "nominal_temp") == 0) {
        if (pos->second.type != thermostat)
            return -1;                      // Must be thermostat
        ACE_OS::memcpy(
            value, &pos->second.nominal_temp,
            ace_min(len, sizeof(pos->second.nominal_temp))
        );
    } else if (ACE_OS::strcmp(attr, "temperature") == 0) {
        long temp = actual_temp(pos);
        ACE_OS::memcpy(value, &temp, ace_min(len, sizeof(temp)));
    } else if (ACE_OS::strcmp(attr, "MIN_TEMP") == 0) {
        ACE_OS::memcpy(value, &MIN_TEMP, ace_min(len, sizeof(MIN_TEMP)));
    } else if (ACE_OS::strcmp(attr, "MAX_TEMP") == 0) {
        ACE_OS::memcpy(value, &MAX_TEMP, ace_min(len, sizeof(MAX_TEMP)));
    } else {
        return -1;                          // No such attribute
    }
    return 0;                               // OK
}

//----------------------------------------------------------------

// ICP_set() sets the the attributed specified by attr to the
// value specified by value for the device with ID id. Attempts to
// write a string longer than MAXSTR bytes (including the
// terminating NUL) result in silent truncation of the string.
// Attempts to access a non-existent device or attribute
// return -1. Attempts to set a nominal temperature outside the
// legal range also return -1. A zero return value
// indicates success.

extern "C"
int
ICP_set(unsigned long id, const char * attr, const void * value)
{
    // Look for id in state map
    StateMap::iterator pos = dstate.find(id);
    if (pos == dstate.end())
        return -1;                          // No such device

    // Change either location or nominal temp, depending on attr.
    if (ACE_OS::strcmp(attr, "location") == 0) {
        pos->second.location.assign(
            (const char *)value, MAXSTR - 1
        );
    } else if (ACE_OS::strcmp(attr, "nominal_temp") == 0) {
        if (pos->second.type != thermostat)
            return -1;                      // Must be thermostat
        short temp;
        ACE_OS::memcpy(&temp, value, sizeof(temp));
        if (temp < MIN_TEMP || temp > MAX_TEMP)
            return -1;
        pos->second.nominal_temp = temp;
    } else {
        return -1;                          // No such attribute
    }
    return 0;                               // OK
}

//----------------------------------------------------------------

#include <fstream>

class ICP_Persist {
public:
    ICP_Persist(const char * file);
    ~ICP_Persist();
private:
    string m_filename;
};

// Read device state from a file and initialize the dstate map.

ICP_Persist::
ICP_Persist(const char * file) : m_filename(file)
{
    // Open input file, creating it if necessary.
    std::ifstream db(m_filename.c_str(), std::ios::in|std::ios::out);//, 0666);
    if (!db) {
        std::cerr << "Error opening " << m_filename << std::endl;
        ACE_OS::exit(1);
    }

    // Read device details, one attribute per line.
    DeviceState ds;
    unsigned long id;
    while (db >> id) {
        // Read device type and set model string accordingly.
        int dtype;
        db >> dtype;
        ds.type = dtype == thermometer
                    ? thermometer : thermostat;
        ds.model = dtype == thermometer
                    ? "Sens-A-Temp" : "Select-A-Temp";
        char loc[MAXSTR];
        db.get(loc[0]);                 // Skip newline
        db.getline(loc, sizeof(loc));   // Read location
        ds.location = loc;
        if (ds.type == thermostat)
            db >> ds.nominal_temp;      // Read temperature
        dstate[id] = ds;                // Add entry to map
    }

    // db.close();
    // if (!db) {
    //    cerr << "Error closing " << m_filename << endl;
    //    ACE_OS::exit(1);
    // }
}

// Write device state to the file.

ICP_Persist::
~ICP_Persist()
{
    // Open input file, truncating it.
    std::ofstream db(m_filename.c_str());
    if (!db) {
        std::cerr << "Error opening " << m_filename << std::endl;
        ACE_OS::exit(1);
    }

    // Write the state details for each device.
    StateMap::iterator i;
    for (i = dstate.begin(); i != dstate.end(); i++) {
        db << i->first << std::endl;
        db << (unsigned long)(i->second.type) << std::endl;
        db << i->second.location << std::endl;
        if (i->second.type == thermostat)
            db << i->second.nominal_temp << std::endl;
    }
    if (!db) {
        std::cerr << "Error writing " << m_filename << std::endl;
        ACE_OS::exit(1);
    }

    // db.close();
    // if (!db) {
    //    cerr << "Error closing " << m_filename << endl;
    //    exit(1);
    // }
}

// Instantiate a single global instance of the class.
static ICP_Persist mydb("/tmp/CCS_DB");











