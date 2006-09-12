# !/bin/sh
export ARMS_ROOT=. 
export DRM_ROOT=./../../../
export ONS_PORT=5002
export ONS_HOST=localhost
export BBLOGPROP=cfg/BandwidthBroker/bbserverlog.prop
export DBCONFIG=cfg/BandwidthBroker/bbconfig.db.prop
export TOPOLOGPROP=cfg/BandwidthBroker/xmltopo.prop

export BB_DB_INIT_SQL=$SRC/db/arms-initdb.sql
export BB_DATA_SQL=$SRC/db/largenet-data.sql

export NETMONLOGPROP=cfg/BandwidthBroker/nmserverlog.prop
chmod +x bin/*sh
echo bbinit done
