/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include "BEC_ELement.h"
#include "serial_protocol.h"



static int bt_ready(void)
{
	uint8_t net_key[16], dev_key[16];

	int err = bt_mesh_init(&node_provisioning, &node_composition);
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return err;
	}
	
	if (IS_ENABLED(CONFIG_BT_SETTINGS)) {// Load mesh settings
		printk("Loading stored settings\n");
		settings_load();
	}
	// bt_mesh_reset();
	// bt_mesh_cdb_clear();

	// Check if loaded cdb
	bt_rand(net_key, 16);
	err = bt_mesh_cdb_create(net_key);
	if (err == -EALREADY) {
		printk("Using stored CDB:\n");
	} else if (err) {
		printk("Failed to create CDB (err %d)\n", err);
		return err;
	} else {
		printk("Created CDB\n");
		setup_cdb();
	}

	// Check for local prov
	bt_rand(dev_key, 16);
	err = bt_mesh_provision(net_key, BT_MESH_NET_PRIMARY, 0, 0, LOCAL,
				dev_key);
	if (err == -EALREADY) {
		printk("Using stored settings\n");
	} else if (err) {
		printk("Provisioning failed (err %d)\n", err);
		return err;
	} else {
		printk("Provisioning completed\n");
	}

	return 0;
}

void main(void)
{
	initUART();

	int err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}
	printk("Bluetooth initialized\n");
	bt_ready();
	
	while (1)
	{ // main thread handling semaphores
		if(k_sem_take(&uart_sem, K_MSEC(200)) == 0) {
			parseCommand(rx_buf);
		}

		if (k_sem_take(&mesh_sem, K_SECONDS(1)) == 0)
		{
			configure_node_ips(dst);
			mod_ips_pub_sub(dst);
		}
	}
}
