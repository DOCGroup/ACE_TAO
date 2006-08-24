#!/bin/sh

. /opt/path.sh

case "$1" in
    start)
	echo "Starting RACE...."
	$CIAO_ROOT/DAnCE/Plan_Launcher/plan_launcher -p RACE.cdp -n -o RACE_DAM.ior
	echo "RACE Started!"
	;;
    stop)
	echo "Stopping RACE...."
	$CIAO_ROOT/DAnCE/Plan_Launcher/plan_launcher -n -i file://RACE_DAM.ior
	echo "Race stopped."
	;;
    control)
	case "$2" in
	    on)
		$CIAO_ROOT/DAnCE/RACE/Controller/Component/starter -s file://Controller.ior
		;;
	    off)
		$CIAO_ROOT/DAnCE/RACE/Controller/Component/starter -k file://Controller.ior
		;;
	    *)
		echo "Usage: $0 $1 {on|off}"
	    esac
	;;
    inject)
	$CIAO_ROOT/RACE/Input_Adapters/Interactive_Input_Adapter/injector -k file://Interactive_Input_Adapter.ior -p $2 -t $3
	;;

    restart)
	sudo rm -f /opt/descriptors/deployed
	sudo rm -f /opt/descriptors/output
	sudo rm -f /opt/descriptors/RT1H_FT.cdp
	sudo /etc/init.d/Naming_Service restart
	# ssh isislab@blade10.isislab.vanderbilt.edu /home/isislab/node.sh &
	/opt/descriptors/prepare-host.py --sequential -v -c"sudo killall -9 NodeApplication" mako root@blade29 root@blade30 root@blade31 root@blade32 blade35 blade36 blade37 blade38 blade39 blade40 blade41 blade42 blade43 blade44
	/opt/descriptors/prepare-host.py --sequential -v -c"sudo /etc/init.d/NodeManager restart" mako root@blade29 root@blade30 root@blade31 root@blade32 blade35 blade36 blade37 blade38 blade39 blade40 blade41 blade42 blade43 blade44
	sudo /etc/init.d/Execution_Manager restart
	;;
    stopnm)
	/opt/descriptors/prepare-host.py --sequential -v -c"sudo /etc/init.d/NodeManager restart" mako root@blade29 root@blade30 root@blade31 root@blade32 blade35 blade36 blade37 blade38 blade39 blade40 blade41 blade42 blade43 blade44
	;;
    status)
	/etc/init.d/Naming_Service status
	/etc/init.d/Execution_Manager status
	/opt/descriptors/prepare-host.py --sequential  -c"sudo /etc/init.d/NodeManager status" mako root@blade29 root@blade30 root@blade31 root@blade32 blade35 blade36 blade37 blade38 blade39 blade40 blade41 blade42 blade43 blade44
	;;
    nastatus)
	/opt/descriptors/prepare-host.py --sequential  -c"ps aux | grep \"NodeApplication -k\"" mako root@blade29 root@blade30 root@blade31 root@blade32 blade35 blade36 blade37 blade38 blade39 blade40 blade41 blade42 blade43 blade44
	;;
    xwindow)
	/opt/descriptors/prepare-host.py --sequential  -c"-Y xterm" mako root@blade29 root@blade30 root@blade31 root@blade32 blade35 blade36 blade37 blade38 blade39 blade40 blade41 blade42 blade43 blade44
	;;
    *)
	echo "Usage: $0 {start|stop|control|inject|status|nastatus}"
esac

exit 0
