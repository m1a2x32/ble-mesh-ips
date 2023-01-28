#include "BEC_Element.h"
#include "serial_protocol.h"
#include <zephyr/sys/util.h>
static const uint16_t ips_net_idx = 0;
static const uint16_t ips_app_idx = 0;

void configure_node_ips(uint16_t uuid)
{
	struct bt_mesh_cdb_app_key *key;
	uint8_t status = 0;
	int err = 0;
	struct bt_mesh_cdb_node *node = bt_mesh_cdb_node_get(uuid);
	if (node == NULL)
	{
		printk("Node [0x%04x] not found in cdb\n", ips_app_idx);
		return;
	}
	
	key = bt_mesh_cdb_app_key_get(ips_app_idx);
	if (key == NULL) {
		printk("No app-key 0x%04x\n", ips_app_idx);
		return;
	}

	if (uuid == LOCAL)
	{
		/* Add Application Key */
		err = bt_mesh_cfg_app_key_add(node->net_idx, node->addr, node->net_idx,
						ips_app_idx, key->keys[0].app_key, &status);
		if (err || status) {
			printk("Failed to add app-key (err %d, status %d)\n", err,
				status);
		} else {
			printk("Added app-key (err %d, status %d)\n", err,
				status);
		}

		/* Bind app key to loc client*/
		err = bt_mesh_cfg_mod_app_bind(node->net_idx, node->addr, node->addr,
						ips_app_idx, BT_MESH_MODEL_ID_GEN_LOCATION_CLI,
						&status);
		if (err || status) {
			printk("Failed to bind app-key (err %d, status %d)\n", err,
				status);
		} else {
			printk("Binded app-key (err %d, status %d)\n", err,
				status);
		}

		atomic_set_bit(node->flags, BT_MESH_CDB_NODE_CONFIGURED);

		if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
			bt_mesh_cdb_node_store(node);
		}
	}
	else {
		NET_BUF_SIMPLE_DEFINE(buf, BT_MESH_RX_SDU_MAX);
		struct bt_mesh_comp_p0_elem elem;
		struct bt_mesh_comp_p0 comp;
		int elem_addr;
		/* Add Application Key */
		err = bt_mesh_cfg_app_key_add(ips_net_idx, node->addr, ips_net_idx, ips_app_idx,
						key->keys[0].app_key, &status);
		if (err || status) {
			printk("Failed to add app-key (err %d status %d)\n", err, status);
		}

		/* Get the node's composition data and bind all models to the appkey */
		err = bt_mesh_cfg_comp_data_get(ips_net_idx, node->addr, 0, &status, &buf);
		if (err || status) {
			printk("Failed to get Composition data (err %d, status: %d)\n",
				err, status);
		}

		err = bt_mesh_comp_p0_get(&comp, &buf);
		if (err) {
			printk("Unable to parse composition data (err: %d)\n", err);
		}

		elem_addr = node->addr;
		while (bt_mesh_comp_p0_elem_pull(&comp, &elem)) {
			printk("Element @ 0x%04x: %u + %u models\n", elem_addr,
				elem.nsig, elem.nvnd);
			for (int i = 0; i < elem.nsig; i++) {
				uint16_t id = bt_mesh_comp_p0_elem_mod(&elem, i);

				if (id == BT_MESH_MODEL_ID_CFG_CLI ||
					id == BT_MESH_MODEL_ID_CFG_SRV) {
					continue;
				}
				printk("Binding AppKey to model 0x%03x:%04x\n",
					elem_addr, id);

				err = bt_mesh_cfg_mod_app_bind(ips_net_idx, node->addr,
								elem_addr, ips_app_idx, id,
								&status);
				if (err || status) {
					printk("Failed (err: %d, status: %d)\n", err,
						status);
				}
			}

			for (int i = 0; i < elem.nvnd; i++) {
				struct bt_mesh_mod_id_vnd id =
					bt_mesh_comp_p0_elem_mod_vnd(&elem, i);

				printk("Binding AppKey to model 0x%03x:%04x:%04x\n",
					elem_addr, id.company, id.id);

				err = bt_mesh_cfg_mod_app_bind_vnd(ips_net_idx, node->addr,
								elem_addr, ips_app_idx,
								id.id, id.company,
								&status);
				if (err || status) {
					printk("Failed (err: %d, status: %d)\n", err,
						status);
				}
			}

		elem_addr++;
	}
	atomic_set_bit(node->flags, BT_MESH_CDB_NODE_CONFIGURED);

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_cdb_node_store(node);
	}
	
	}
}

uint8_t mod_ips_pub_sub(uint16_t uuid)
{
	int err = 0;
	uint8_t status = 0;
	struct bt_mesh_cfg_mod_pub pub = {
		.addr = 0,
		.app_idx = BT_MESH_NET_PRIMARY,
		.cred_flag = 0,
		.period = 0,
		.transmit = 0,
		.ttl =	0,
		.uuid = NULL,
	};
	
	if (uuid == LOCAL)
	{ // Check if self node
	/// CHANGE SUB ADDRESS to d002 ||| d001 IS USED FOR TESTING
		err = bt_mesh_cfg_mod_sub_add(BT_MESH_NET_PRIMARY, uuid, uuid, 0xD001, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &status);
		if (err | status) {
			printk("Failed to add sub addr (err %d | status %d)\n", err, status);
		}
		err = bt_mesh_cfg_mod_sub_add(BT_MESH_NET_PRIMARY, uuid, uuid, 0xC001, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &status);
		if (err | status) {
			printk("Failed to add sub addr (err %d | status %d)\n", err, status);
		}
		err = bt_mesh_cfg_mod_sub_add(BT_MESH_NET_PRIMARY, uuid, uuid, 0xD002, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &status);
		if (err | status) {
			printk("Failed to add sub addr (err %d | status %d)\n", err, status);
		}
	}
	else if(uuid <= BEACON_RANGE_START && uuid >= BEACON_RANGE_END) { // Check uuid for BEACON
		// Modify sub params
		err = bt_mesh_cfg_mod_sub_add(BT_MESH_NET_PRIMARY, uuid, uuid, 0xC001, BT_MESH_MODEL_ID_GEN_LOCATION_SETUPSRV, &status);
		if (err | status) {
			printk("Failed to add sub addr (err %d | status %d)\n", err, status);
		}
		// Modify pub params
		pub.addr = 0xD001;
		err = bt_mesh_cfg_mod_pub_set(BT_MESH_NET_PRIMARY, uuid, uuid, BT_MESH_MODEL_ID_GEN_LOCATION_SRV, &pub, &status);
		if (err | status) {
			printk("Failed to mod pub addr (err %d | status %d)\n", err, status);
		}
	}
	else if(uuid <= ASSET_RANGE_START && uuid >= ASSET_RANGE_END) { // Check uuid for ASSET
        // Modify sub params
		err = bt_mesh_cfg_mod_sub_add(BT_MESH_NET_PRIMARY, uuid, uuid, 0xC002, BT_MESH_MODEL_ID_GEN_LOCATION_SRV, &status);
		if (err | status) {
			printk("Failed to add sub addr (err %d | status %d)\n", err, status);
		}
		err = bt_mesh_cfg_mod_sub_add(BT_MESH_NET_PRIMARY, uuid, uuid, 0xD001, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &status);
		if (err | status) {
			printk("Failed to add sub addr (err %d | status %d)\n", err, status);
		}
		// Modify pub params
		pub.addr = 0xC001;
		err = bt_mesh_cfg_mod_pub_set(BT_MESH_NET_PRIMARY, uuid, uuid, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &pub, &status);
		if (err | status) {
			printk("Failed to mod pub addr (err %d | status %d)\n", err, status);
		}
		pub.addr = 0xD002;
		err = bt_mesh_cfg_mod_pub_set(BT_MESH_NET_PRIMARY, uuid, uuid, BT_MESH_MODEL_ID_GEN_LOCATION_SRV, &pub, NULL);
		if (err | status) {
			printk("Failed to mod pub addr (err %d | status %d)\n", err, status);
		}
	}
	return status;
}

void setup_cdb(void)
{
	struct bt_mesh_cdb_app_key *key;

	key = bt_mesh_cdb_app_key_alloc(ips_net_idx, ips_app_idx);
	if (key == NULL) {
		printk("Failed to allocate app-key 0x%04x\n", ips_app_idx);
		return;
	}

	bt_rand(key->keys[0].app_key, 16);

	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {
		bt_mesh_cdb_app_key_store(key);
	}
}


struct bt_mesh_loc_local local_loc;

void set_location(uint16_t uuid, struct bt_mesh_loc_local loc)
{
	int err;
	uint8_t status = 0;
	struct bt_mesh_cfg_mod_pub pub = {
		.addr = uuid,
		.app_idx = BT_MESH_NET_PRIMARY,
		.cred_flag = 0,
		.period = 0,
		.transmit = 0,
		.ttl =	0,
		.uuid = NULL,
	};
	// mod pub set of local to directed uuid
	err = bt_mesh_cfg_mod_pub_set(BT_MESH_NET_PRIMARY, LOCAL, LOCAL, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &pub, &status);
	if (err | status) {
		printk("Failed to mod pub addr (err %d)\n", err);
		send_ack_uart(SET_LOCATION, &uuid, 1, NODE_NOT_PROVISIONED);
		return;
	}
	err = bt_mesh_loc_cli_local_set(&locCli, NULL, &loc, &local_loc);
	if (err) {
		printk("Failed setting location (err %d)\n", err);
		send_ack_uart(SET_LOCATION, &uuid, 1, NODE_NOT_FOUND);
		return;
	}
	printk("Setting location to %d: %d %d\n",uuid, loc.east, loc.north);
	send_ack_uart(SET_LOCATION, &uuid, 1, OK);
}

void get_location(uint16_t uuid)
{
	int err;
	uint8_t status = 0;
	struct bt_mesh_cfg_mod_pub pub = {
		.addr = uuid,
		.app_idx = BT_MESH_NET_PRIMARY,
		.cred_flag = 0,
		.period = 0,
		.transmit = 0,
		.ttl =	0,
		.uuid = NULL,
	};

	// mod pub set of local to directed uuid
	err = bt_mesh_cfg_mod_pub_set(BT_MESH_NET_PRIMARY, LOCAL, LOCAL, BT_MESH_MODEL_ID_GEN_LOCATION_CLI, &pub, &status);
	if (err | status) {
		printk("Failed to mod pub addr (err %d)\n", err);
		send_ack_uart(GET_LOCATION, &uuid, 1, NODE_NOT_PROVISIONED);
		return;
	}

	err = bt_mesh_loc_cli_local_get(&locCli, NULL, &local_loc);
	if (err) {
		printk("Failed getting location (err %d)\n", err);
		send_ack_uart(GET_LOCATION, &uuid, 1, NODE_NOT_FOUND);
		return;
	}	
	K_MSEC(200);
	send_ack_uart(GET_LOCATION, locationBuf, locBufCount, OK);
	locBufCount = 0;
}

void delete(uint16_t uuid)
{
	bool status;
	struct bt_mesh_cdb_node *node = bt_mesh_cdb_node_get(uuid);
	if (node == NULL) {
		printk("No node with address 0x%04x\n", uuid);
		send_ack_uart(DELETE_ASSET, &uuid, 1, NODE_NOT_PROVISIONED);
		return;
	}

	int err = bt_mesh_cfg_node_reset(0, uuid, &status);
	if (err) {
		printk("0x%04x reset failed (err %d)\n", uuid, err);
		send_ack_uart(DELETE_ASSET, &uuid, 1, NODE_NOT_FOUND);
		return;
	}
	
	bt_mesh_cdb_node_del(node, true);
	send_ack_uart(DELETE_ASSET, &uuid, 1, OK);
}

void print_cdb(void)
{
	char key_hex_str[32 + 1];
	uint16_t availableUuids[CONFIG_BT_MESH_CDB_NODE_COUNT];
	struct bt_mesh_cdb_node *node;
	struct bt_mesh_cdb_subnet *subnet;
	struct bt_mesh_cdb_app_key *app_key;
	bool configured;
	size_t nodeCounter = 0;

	printk("NODES:\nAddress Elements	DevKey\n");
	// NODES
	for (int i = 0; i < ARRAY_SIZE(bt_mesh_cdb.nodes); ++i) {
		node = &bt_mesh_cdb.nodes[i];
		if (node->addr == BT_MESH_ADDR_UNASSIGNED) {
			continue;
		}

		configured = atomic_test_bit(node->flags,
					     BT_MESH_CDB_NODE_CONFIGURED);

		bin2hex(node->dev_key, 16, key_hex_str, sizeof(key_hex_str));
		availableUuids[nodeCounter] = node->addr;
		nodeCounter++;
		printk("0x%04x   %-8d   %s\n", node->addr, node->num_elem, key_hex_str);
	}
	send_ack_uart(GET_NODE, &availableUuids, nodeCounter, OK);

	// NETWORK KEYS
	printk("\nNETKEYS:\nNetIdx Key\n");
	for (int i = 0; i < ARRAY_SIZE(bt_mesh_cdb.subnets); ++i) {
		subnet = &bt_mesh_cdb.subnets[i];
		if (subnet->net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		bin2hex(subnet->keys[0].net_key, 16, key_hex_str,
			sizeof(key_hex_str));
		printk("0x%03x   %s\n", subnet->net_idx, key_hex_str);
	}

	// APPLICATION KEYS
	printk("\nAPPKEYS:\nAppIdx Key\n");
	for (int i = 0; i < ARRAY_SIZE(bt_mesh_cdb.app_keys); ++i) {
		app_key = &bt_mesh_cdb.app_keys[i];
		if (app_key->net_idx == BT_MESH_KEY_UNUSED) {
			continue;
		}

		bin2hex(app_key->keys[0].app_key, 16, key_hex_str,
			sizeof(key_hex_str));
		printk("0x%03x   0x%03x   %s\n", app_key->net_idx, app_key->app_idx, key_hex_str);
	}
}

