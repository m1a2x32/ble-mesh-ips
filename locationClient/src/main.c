/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include "bt_mesh.h"
#include "model_manager.h"
#include <errno.h>


static void bt_ready(int err)
{
	if (err)
	{
		printk("bt_enable init failed with err %d", err);
		return;
	}
    printk("Bluetooth initialised OK\n");

	err = bt_mesh_init(prov_init(), model_handler_init());

	if (err)
	{
		printk("bt_mesh_init failed with err %d", err);
		return;
	}
	
	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}
	// bt_mesh_reset();
    if (!bt_mesh_is_provisioned()) {
    	printk("Node has not been provisioned - beaconing\n");
		bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
	} else {
    	printk("Node has already been provisioned\n");
	}
}

void main(void)
{
	int err = bt_enable(bt_ready);
	if (err)
	{
		printk("bt_enable failed with err %d", err);
	}
}
