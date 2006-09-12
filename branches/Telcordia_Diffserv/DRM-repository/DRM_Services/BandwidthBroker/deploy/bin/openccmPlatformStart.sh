#!/bin/bash

ccm_install
ns_start

dci_start DefaultDCI
factory_start DefaultFactory
node_start MainNode

