#include "../include/bt_mesh.h"
#include <zephyr/bluetooth/crypto.h>

static uint8_t dev_uuid[16];

static int provisioning_output_pin(bt_mesh_output_action_t action, uint32_t number)
{
	printk("OOB Number: %u\n", number);
	return 0;
} 

static void provisioning_complete(uint16_t net_idx, uint16_t addr) {
    printk("Provisioning completed\n");
}

static void provisioning_reset(void)
{
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
}

static const struct bt_mesh_prov prov = {
	.uuid = dev_uuid,
#if defined(CONFIG_BT_MESH_DK_PROV_OOB_LOG) || defined(CONFIG_BT_MESH_DK_PROV_OOB_BLINK)
	.output_size = 1,
#endif
	.output_actions = (0
#ifdef CONFIG_BT_MESH_DK_PROV_OOB_LOG
		| BT_MESH_DISPLAY_NUMBER
		| BT_MESH_DISPLAY_STRING
#endif
#ifdef CONFIG_BT_MESH_DK_PROV_OOB_BLINK
		| BT_MESH_BLINK
#endif
		),
	.output_number = provisioning_output_pin,
#ifdef CONFIG_BT_MESH_DK_PROV_OOB_BUTTON
	.input_size = 1,
	.input_actions = BT_MESH_PUSH,
#endif
	.complete = provisioning_complete,
	.reset = provisioning_reset,
};

const struct bt_mesh_prov *prov_init(void)
{
    bt_rand(&dev_uuid, sizeof(dev_uuid));
    dev_uuid[14] = 0xB0;
	dev_uuid[15] = 0x00;
    printk("\n%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n\n",
            dev_uuid[15], dev_uuid[14], dev_uuid[13], dev_uuid[12],dev_uuid[11], dev_uuid[10], dev_uuid[9], dev_uuid[8],
            dev_uuid[7], dev_uuid[6], dev_uuid[5], dev_uuid[4],dev_uuid[3], dev_uuid[2], dev_uuid[1], dev_uuid[0]);
	return &prov;
}
