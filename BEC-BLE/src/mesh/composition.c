#include "BEC_Element.h"

// ------------------------- HEALTH SRV ---------------------
static void attention_on(struct bt_mesh_model *model)
{
	printk("Health Srv attention on\n");
}

static void attention_off(struct bt_mesh_model *model)
{
	printk("Health Srv attention off\n");
}

static const struct bt_mesh_health_srv_cb health_srv_cb = {
	.attn_on = attention_on,
	.attn_off = attention_off
};

static struct bt_mesh_health_srv health_srv = {
	.cb = &health_srv_cb,
};

BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

// ------------------------- HEALTH CLI ---------------------

static void health_attention_status(struct bt_mesh_health_cli *cli,
				    uint16_t addr, uint8_t attention)
{
	printk("Health Attention Status from 0x%04x: %u", addr, attention);
}

static void health_period_status(struct bt_mesh_health_cli *cli, uint16_t addr,
				 uint8_t period)
{
	printk("Health Fast Period Divisor Status from 0x%04x: %u", addr, period);
}

struct bt_mesh_health_cli health_cli = {
	.attention_status = health_attention_status,
	.period_status = health_period_status
};

// ------------------------- LOCATION CLI ---------------------
uint16_t locationBuf[CONFIG_BT_MESH_CDB_NODE_COUNT*3]; // each node has uuid, x, y values
size_t locBufCount = 0;
static void gen_loc_status_global(struct bt_mesh_loc_cli *cli, struct bt_mesh_msg_ctx *ctx, const struct bt_mesh_loc_global *status)
{
}

static void gen_loc_status_local(struct bt_mesh_loc_cli *cli, struct bt_mesh_msg_ctx *ctx, const struct bt_mesh_loc_local *status)
{
	locationBuf[locBufCount++] = ctx->addr;
	locationBuf[locBufCount++] = status->east;
	locationBuf[locBufCount++] = status->north; 
	printk("Addr [%d]: x:%d y:%d \n", ctx->addr, status->east, status->north);
}

static struct bt_mesh_loc_cli_handlers loc_cli_cb = {
	.global_status = gen_loc_status_global,
	.local_status = gen_loc_status_local,
};

struct bt_mesh_loc_cli locCli = BT_MESH_LOC_CLI_INIT(&loc_cli_cb);

static struct bt_mesh_cfg_cli cfg_cli;

static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, 
	BT_MESH_MODEL_LIST(
		BT_MESH_MODEL_CFG_SRV,
        BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
		BT_MESH_MODEL_CFG_CLI(&cfg_cli),
		BT_MESH_MODEL_HEALTH_CLI(&health_cli),
		BT_MESH_MODEL_LOC_CLI(&locCli),
	), 
	BT_MESH_MODEL_NONE),
};

struct bt_mesh_comp node_composition = {
	.cid = CONFIG_BT_COMPANY_ID,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};