/*
 * $Id$
 */

/*
 * A small program to test RPC round-trip delays.
 */
program PINGPROG {
  version PINGVERS {
    int PING (int) = 1;
  } = 1;
} = 0x20000001;
