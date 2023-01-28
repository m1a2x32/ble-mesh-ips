#include "BEC_Element.h"

// <temp> OOB not supported since connection to dashboard is not real-time.
static uint8_t dev_uuid[16] = { 0xdd, 0x01 };

struct k_sem mesh_sem;
uint16_t dst;
bool prov_pending = false;

static void unprovisioned_beacon_detected(uint8_t uuid[16], bt_mesh_prov_oob_info_t oob_info, uint32_t *uri_hash)
{
	if (prov_pending == false)
	{
		int err;
		if (uuid[14] == 0xA0 && uuid[15] == 0x00)
		{
			for (uint16_t availableAddr = BEACON_RANGE_START; availableAddr < BEACON_RANGE_END; availableAddr++)
			{
				if(bt_mesh_cdb_node_get(availableAddr) == NULL)
				{
					err = bt_mesh_provision_adv(uuid, BT_MESH_NET_PRIMARY, availableAddr, 0);
					printk("Provisioned recUuid (%04x) with err code %d \n", availableAddr, err);
					prov_pending = true;
					return;
				}
			}
		}
		else if (uuid[14] == 0xB0 && uuid[15] == 0x00)
		{
			for (uint16_t availableAddr = ASSET_RANGE_START; availableAddr < ASSET_RANGE_END; availableAddr++)
			{
				if(bt_mesh_cdb_node_get(availableAddr) == NULL)
				{
					err = bt_mesh_provision_adv(uuid, BT_MESH_NET_PRIMARY, availableAddr, 0);
					printk("Provisioned recUuid (%04x) with err code %d \n", availableAddr, err);
					prov_pending = true;
					return;
				}
			}
		}
	}
}

static void local_prov_complete(uint16_t net_idx, uint16_t addr)
{ // prov for local node - **gets triggered when settings restored**
    printk("Local node provisioned, net_idx 0x%04x address 0x%04x\n", net_idx, addr);
	k_sem_init(&mesh_sem, 0, 1);
	dst = LOCAL;
	k_sem_give(&mesh_sem);
}

static void prov_node_added(uint16_t net_idx, uint8_t uuid[16], uint16_t addr,
			    uint8_t num_elem)
{ // Provision for external nodes
	printk("Node provisioned, net_idx 0x%04x address "
			"0x%04x elements %d\n", net_idx, addr, num_elem);
	dst = addr;
	prov_pending = false;
	k_sem_give(&mesh_sem);
}

static void prov_reset(void)
{
	bt_mesh_reset();
}

struct bt_mesh_prov node_provisioning = {
    .uuid = dev_uuid,
    .complete = local_prov_complete,
	.node_added = prov_node_added,
	.reset = prov_reset,
    .unprovisioned_beacon = unprovisioned_beacon_detected,
    .output_actions = 0,
};
