#ifndef TEMPERATURE_MONITOR_OPTIONS_H
#define TEMPERATURE_MONITOR_OPTIONS_H

class Temperature_Monitor_Options
  {
  public:
    Temperature_Monitor_Options (int, ACE_TCHAR *[])
    { }

    const char *thermometer_address ()
    {
      return "serial:// s0/0x3e52";
    }

    int poll_interval ()
    {
      return 10; // every 10 seconds
    }

    int reset_interval ()
    {
      return 60; // sixty seconds
    }

    int excessive_resets ()
    {
      return 5; // no response in 5 minutes
    }

    const char *admin_email ()
    {
      return "root@localhost";
    }

    const char *email_from ()
    {
      return "temperature monitor";
    }

    int history_size()
    {
      return 10;
    }
  };

#endif /* TEMPERATURE_MONITOR_OPTIONS_H */
