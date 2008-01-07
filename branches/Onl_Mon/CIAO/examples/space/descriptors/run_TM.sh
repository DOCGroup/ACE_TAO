#!/bin/bash
$CIAO_ROOT/DAnCE/TM_Daemon/TM_Daemon -ORBInitRef NameService=corbaloc:iiop:roon.dre.vanderbilt.edu:40000/NameService -k SPACE.cdp -m NodeManagerMap_2.dat -N CIAO_TM_Daemon -d Domain_2.cdd
