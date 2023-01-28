#ifndef BEC_AUTOMATED_PROV
#define BEC_AUTOMATED_PROV

#include <bluetooth/mesh/cdb.h>
#include <zephyr/settings/settings.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include <zephyr/bluetooth/mesh.h>

#define LOCAL 0x1
#define BEACON_RANGE_START (uint16_t)0xA01
#define BEACON_RANGE_END BEACON_RANGE_START+0xF
#define ASSET_RANGE_START (uint16_t)0xB01
#define ASSET_RANGE_END ASSET_RANGE_START+0xF

#define DEF_ATTENTION_DURATION 1

extern uint16_t locationBuf[];
extern size_t locBufCount;
extern uint16_t dst;
extern struct k_sem mesh_sem;

// for node composition
extern struct bt_mesh_loc_local local_loc;
extern struct bt_mesh_loc_cli locCli;

extern struct bt_mesh_comp node_composition;
extern struct bt_mesh_prov node_provisioning;


enum {
    INDEX0,
    OK,
    NODE_NOT_FOUND,
    NODE_NOT_PROVISIONED,
};

enum {
   TIMEOUT,
   NODEUNPROV,
   NODEPROV
};
// MISSING API FUNCTIONS
// identify



/* Desc:    Adds net key to selected node. Function is redundant as the provisioning process already adds netkey. 
            Could be used in the future when more subnets are implemented. 
   Input:   UUID - unique address
   Output:  Error code  */
extern void configure_node_ips(uint16_t uuid);

/* Desc:    Adds indoor application key to such selected node. 
   Input:   UUID - unique address
   Output:  Error code  */
extern void add_ips_app_key(uint16_t uuid);

/* Desc:    Detects type of node and configures subscription and publication values accordingly. 
   Input:   UUID - unique address
   Output:  Error code  */
extern uint8_t mod_ips_pub_sub(uint16_t uuid);

/* Desc:    Configures location of a specified beacon. 
   Input:   UUID - unique address
            loc  - specified location
   Output:  Error code sn  */
extern void set_location(uint16_t uuid, struct bt_mesh_loc_local loc);

/* Desc:    Gets location of a specified node. Could target beacons, but that location should already be known. 
   Input:   UUID - unique address
   Output:  Error code  */
extern void get_location(uint16_t uuid);

/* Desc:    Removes node from mesh network. 
   Input:   UUID - unique address
   Output:  Error code  */
extern void delete(uint16_t uuid);

/* Desc:    Prints a list of available net/app keys and nodes.   */
extern void print_cdb(void);

extern void setup_cdb(void);



#endif