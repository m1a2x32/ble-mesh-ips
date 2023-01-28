# ble-mesh-ips
# Code Distribution 
- BLE-BEC = Network automatic provisioner. Forwards data via uart with format specified in 
[https://sites.google.com/view/fontysips/design]
- locationClient = Self locating node. BEC will automatically assign this node to its dedicated group addresses to
be able to communicate with locationServer nodes.
- locationServer = Reference nodes. BEC will automatically assign ... locationClient nodes.