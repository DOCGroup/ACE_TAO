/* -*- C++ -*- */
// $Id$

/* This file is used to provide a consolidated Options.handling facility. */

#ifndef _OPTIONS_H
#define _OPTIONS_H

/* Silly macros for handling message types. */
#define GET_PACKET_TYPE(P)      (ntohs (*((short *) P)))
#define SET_PACKET_TYPE(P,T)    ((*(short *) P) = ntohs (T))
#define SKIP_PACKET_TYPE(P)     ((P) + sizeof (short))
#define SUBTRACT_PACKET_TYPE(L) ((L) - sizeof (short)) 

class Options
{
  static void	        print_usage_and_die (int long_msg);
  static unsigned int	option_word;

public:
  enum Option_Types
    {
      REMOTE_USAGE	     = 01,
      PRINT_LOGIN_NAME	     = 02,
      DEBUG		     = 04,
      STAND_ALONE_SERVER     = 010,
      SORT_BY_LOGIN_NAME     = 020,
      SORT_BY_REAL_NAME	     = 040,
      USE_VERBOSE_FORMAT     = 0100,
      BE_A_DAEMON 	     = 0200
    };

  /* Different types of messages. */
  enum Protocol_Types
    { 
      PROTO_USR     = 1,		/* Only return info on one user. */
      PROTO_ALL     = 2,		/* Return info on all users logged in around the system. */
      PROTO_FLO     = 3,		/* Return info on friends logged in. */
      PROTO_RUSER   = 4,		/* Return info in ruser format! */

      PROTO_RWHO    = 5,		/* Return info in rwho format. */
      PROTO_WHO     = 6,		/* Return info in who format. */
      PROTO_RUPTIME = 7			/* Return info in ruptime format. */
    };
  
  static void           set_options (int argc, char *argv[]);
  static void		set_opt (Option_Types opt);
  static int		get_opt (Option_Types opt);

  static short          port_number;
  static Protocol_Types protocol_type;
  static int		max_server_timeout;
  static char		*program_name;
  static char		*friend_file;
  static char		*user_name;
};

#ifdef __OPTIMIZE__
inline void 
Options::set_opt (Option_Types opt)
{
  Options::option_word |= opt;
}

inline int
Options::get_opt (Option_Types opt)
{
  return Options::option_word & opt;
}
#endif /* __OPTIMIZE__ */
#endif				/* _OPTIONS_H */
