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

  /// every 10 seconds
  int poll_interval () const
  {
    return 10;
  }

  /// sixty seconds
  int reset_interval () const
  {
    return 60;
  }

  /// no response in 5 minutes
  int excessive_resets () const
  {
    return 5;
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
