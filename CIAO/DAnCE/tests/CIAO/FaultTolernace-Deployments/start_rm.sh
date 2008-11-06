$TAO_ROOT/orbsvcs/examples/FaultTolerance/FLARe/no-RT/ReplicationManager &
sleep 1
tao_nsadd --ns file://./ns.ior --name ReplicationManager --ior file://./rm.ior
