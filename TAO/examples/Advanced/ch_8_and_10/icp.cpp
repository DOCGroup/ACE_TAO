//=============================================================================
/**
 *  @file    icp.cpp
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
//#include    <stdlib.h>
#include    "icp.h"
#include    "ace/OS_NS_stdlib.h"
#include    "ace/OS_NS_string.h"

//----------------------------------------------------------------

enum class DeviceType { thermometer, thermostat };

struct DeviceState {                // State for a device
    DeviceType type {DeviceType::thermometer};
    char const *model {};
    std::string location {};
    short nominal_temp {};   // For thermostats only
};
using StateMap = std::map<unsigned long, DeviceState>;

//----------------------------------------------------------------

constexpr size_t MAXSTR {32};       // Max len of string including NUL

constexpr long MIN_TEMP {40};      // 40 F ==  4.44 C
constexpr long MAX_TEMP {90};      // 90 F == 32.22 C
constexpr long DFLT_TEMP {68};     // 68 F == 20.00 C

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
// If a device with the specified ID is already on-line, the
// return value is -1. A zero return value indicates success.

extern "C"
int
ICP_online(unsigned long id)
{
    // Look for id in state map.
    StateMap::iterator pos = dstate.find (id);
    if (pos != dstate.end ())
        return -1;                          // Already exists

    // Fill in state.
    DeviceState ds {};
    ds.type = (id % 2) ? DeviceType::thermometer : DeviceType::thermostat;
    ds.model = (ds.type == DeviceType::thermometer)
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
    StateMap::iterator pos = dstate.find (id);
    if (pos == dstate.end ())
        return -1;                              // No such device
    dstate.erase (id);
    return 0;
}

//----------------------------------------------------------------

// vary_temp() simulates the variation in actual temperature
// around a thermostat. The function randomly varies the
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
vary_temp (long temp)
{
    long const r = ACE_OS::rand () % 50;
    long delta {};
    if (r < 5)
        delta = 3;
    else if (r < 15)
        delta = 2;
    else if (r < 30)
        delta = 1;
    else
        delta = 0;
    if (ACE_OS::rand () % 2)
        delta = -delta;
    return temp + delta;
}

//----------------------------------------------------------------

// Function object. Locates a thermostat that is in the same room
// as the device at position pos.

class ThermostatInSameRoom {
public:
    explicit ThermostatInSameRoom (StateMap::const_iterator pos)
      : m_pos (pos)
    {
    }

    bool operator() (std::pair<unsigned long const, DeviceState> const& p) const
    {
      return p.second.type == DeviceType::thermostat
        && p.second.location == m_pos->second.location;
    }

private:
    StateMap::const_iterator m_pos;
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
actual_temp (StateMap::const_iterator pos)
{
    long sum {};
    long count {};
    StateMap::const_iterator where = std::find_if (dstate.cbegin (),
                                                   dstate.cend (),
                                                   ThermostatInSameRoom (pos));
    while (where != dstate.cend ()) {
        count++;
        sum += where->second.nominal_temp;
        where = std::find_if (++where, dstate.cend (), ThermostatInSameRoom (pos));
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
// thermostat in the same room as the thermometer, the
// thermometer reports a temperature that varies around that
// thermostat's temperature. For several thermostats that are in
// the same room, the thermometer reports a temperature that
// varies around the average nominal temperature of all the
// thermostats.
//
// Attempts to read from a non-existent device or to read a
// non-existent attribute return -1. A return value of zero
// indicates success. If the supplied buffer is too short to hold
// a value, ICP_get() silently truncates the value and
// returns success.

extern "C"
int
ICP_get (
    unsigned long   id,
    char const *    attr,
    void *          value,
    size_t          len)
{
    // Look for id in state map
    StateMap::const_iterator pos = dstate.find (id);
    if (pos == dstate.end ())
        return -1;                              // No such device

    // Depending on the attribute, return the
    // corresponding piece of state.
    if (ACE_OS::strcmp (attr, "model") == 0) {
        ACE_OS::strncpy (static_cast<char *> (value), pos->second.model, len);
    } else if (ACE_OS::strcmp (attr, "location") == 0) {
        ACE_OS::strncpy (static_cast<char *> (value), pos->second.location.c_str (), len);
    } else if (ACE_OS::strcmp (attr, "nominal_temp") == 0) {
        if (pos->second.type != DeviceType::thermostat)
            return -1;                      // Must be thermostat
        ACE_OS::memcpy(
            value, &pos->second.nominal_temp,
            (std::min)(len, sizeof(pos->second.nominal_temp))
        );
    } else if (ACE_OS::strcmp (attr, "temperature") == 0) {
        long const temp = actual_temp (pos);
        ACE_OS::memcpy (value, &temp, (std::min) (len, sizeof (temp)));
    } else if (ACE_OS::strcmp (attr, "MIN_TEMP") == 0) {
      ACE_OS::memcpy (value, &MIN_TEMP, (std::min) (len, sizeof (MIN_TEMP)));
    } else if (ACE_OS::strcmp (attr, "MAX_TEMP") == 0) {
      ACE_OS::memcpy (value, &MAX_TEMP, (std::min) (len, sizeof (MAX_TEMP)));
    } else {
        return -1;                          // No such attribute
    }
    return 0;                               // OK
}

//----------------------------------------------------------------

// ICP_set() sets the attribute specified by attr to the
// value specified by value for the device with ID id. Attempts to
// write a string longer than MAXSTR bytes (including the
// terminating NUL) result in silent truncation of the string.
// Attempts to access a non-existent device or attribute
// return -1. Attempts to set a nominal temperature outside the
// legal range also return -1. A zero return value
// indicates success.

extern "C"
int
ICP_set (unsigned long id, char const *attr, void const *value)
{
    // Look for id in state map
    StateMap::iterator pos = dstate.find (id);
    if (pos == dstate.end ())
        return -1;                          // No such device

    // Change either location or nominal temp, depending on attr.
    if (ACE_OS::strcmp (attr, "location") == 0) {
        pos->second.location.assign(
            static_cast<char const *> (value), MAXSTR - 1
        );
    } else if (ACE_OS::strcmp (attr, "nominal_temp") == 0) {
        if (pos->second.type != DeviceType::thermostat)
            return -1;                      // Must be thermostat
        short temp {};
        ACE_OS::memcpy(&temp, value, sizeof(temp));
        if (temp < MIN_TEMP || temp > MAX_TEMP)
            return -1;
        pos->second.nominal_temp = temp;
    } else {
        return -1;                          // No such attribute
    }
    return 0;                               // OK
}
