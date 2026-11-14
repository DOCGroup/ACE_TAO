#ifndef TEMPERATURE_MONITOR_OPTIONS_H
#define TEMPERATURE_MONITOR_OPTIONS_H

class Temperature_Monitor_Options
{
public:
  Temperature_Monitor_Options (int, ACE_TCHAR *[])
  { }

  char const *thermometer_address () const
  {
    return "serial:// s0/0x3e52";
  }

  int poll_interval () const
  {
    return 10; // every 10 seconds
  }

  int reset_interval () const
  {
    return 60; // sixty seconds
  }

  int excessive_resets () const
  {
    return 5; // no response in 5 minutes
  }

  char const *admin_email () const
  {
    return "root@localhost";
  }

  char const *email_from () const
  {
    return "temperature monitor";
  }

  int history_size () const
  {
    return 10;
  }
};

#endif /* TEMPERATURE_MONITOR_OPTIONS_H */
