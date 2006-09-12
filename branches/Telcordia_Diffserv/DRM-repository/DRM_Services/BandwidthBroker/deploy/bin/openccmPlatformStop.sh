#!/bin/bash

node_stop MainNode
factory_stop DefaultFactory
dci_stop DefaultDCI

ns_stop
ccm_deinstall
