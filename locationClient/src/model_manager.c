#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include "model_manager.h"
#include <stdio.h>

beacon bcns[3];
static int counter = 0;
// Health Server
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
	.attn_off = attention_off,
};

static struct bt_mesh_health_srv health_srv = {
	.cb = &health_srv_cb,
};

BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

static struct bt_mesh_loc_cli_handlers loc_cli_cb = {
	.global_status = gen_loc_status_global,
	.local_status = gen_loc_status_local,
}; 
static struct bt_mesh_loc_cli locCli = BT_MESH_LOC_CLI_INIT(&loc_cli_cb);

static struct bt_mesh_loc_srv_handlers loc_srv_cb = {
	.local_get = gen_loc_get_local,
	.local_set = gen_loc_set_local,
	.global_get = gen_loc_get_global,
	.global_set = gen_loc_set_global
};
static struct bt_mesh_loc_srv locSrv = BT_MESH_LOC_SRV_INIT(&loc_srv_cb);


// Composition
static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, 
		BT_MESH_MODEL_LIST(
			BT_MESH_MODEL_CFG_SRV,
			BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
			BT_MESH_MODEL_LOC_SRV(&locSrv),
			BT_MESH_MODEL_LOC_CLI(&locCli)
		), 
		BT_MESH_MODEL_NONE),
};

// Node
static struct bt_mesh_comp comp = {
		.cid = 0xFFFF, 	// Company ID
		.elem = elements,
		.elem_count = ARRAY_SIZE(elements),
};

const struct bt_mesh_comp *model_handler_init(void)
{
	// comp.pid = 0x00;
    // if(strcmp(pid, "beacon") == 0)
    // {
    //     comp.pid = 0x1;
    // }
    // else if(strcmp(pid, "whiteboard") == 0)
    // {
    //     comp.pid = 0x2;
    // }
	return &comp;
}

// Location Server
//-------------------------------------------------------------------------------------------------------
void gen_loc_get_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *rsp)
{
	printk("\n[%02x]->[SRV] RSSI:%d X:%d Y:%d\n", 
		ctx->addr, ctx->recv_rssi, rsp->north, rsp->east);
	bt_mesh_loc_cli_local_get(&locCli, NULL, NULL);
}

void gen_loc_set_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *loc)
{ 

}

void gen_loc_get_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *rsp)
{
	printf("\n[%02x]->[SRV] RSSI:%d X:%f Y:%f\n", 
		ctx->addr, ctx->recv_rssi, rsp->longitude, rsp->latitude);
	bt_mesh_loc_cli_global_get(&locCli, NULL, NULL);
}

void gen_loc_set_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *loc)
{

}

// Location Client
// -------------------------------------------------------------------------------------------------------
void gen_loc_status_global(struct bt_mesh_loc_cli *cli, struct bt_mesh_msg_ctx *ctx, const struct bt_mesh_loc_global *status)
{
	// printf("\n[%02x]->[CLI] RSSI:%d X:%f Y:%f\n", 
	// ctx->addr, ctx->recv_rssi, status->longitude, status->latitude);
	// for (int i = 0; i < counter; i++)
	// {
	// 	if (ctx->addr == bcns[i].uuid)
	// 	{
	// 		//empty buffer (restart)
	// 		printk("Node msg repeated/n");
	// 		return;
	// 	}
	// }
	
	
	// bcns[counter].uuid = ctx->addr;
	// bcns[counter].rssi = ctx->recv_rssi;
	// memcpy(&bcns[counter].gPos, status, sizeof( struct bt_mesh_loc_global));
	// counter++;
	
	// if (counter >= 3)
	// {
	// 	counter = 0;
	// 	trilateratePos(&global, &bcns[0], &bcns[1], &bcns[2]);
	// 	if(bt_mesh_loc_srv_global_pub(&locSrv, NULL, &global) == 0)
	// 		printf("\nmy pos (x,y): %f,%f\n", global.longitude, global.latitude);
	//}
}

void gen_loc_status_local(struct bt_mesh_loc_cli *cli, struct bt_mesh_msg_ctx *ctx, const struct bt_mesh_loc_local *status)
{ // store data in ringbuffer, once size becomes three or larger estrimate pos and send 
	printk("\n[%02x]->[CLI] RSSI:%d X:%d Y:%d\n", 
	ctx->addr, ctx->recv_rssi, status->east, status->north);
	for (int i = 0; i < counter; i++)
	{
		if (ctx->addr == bcns[i].uuid)
		{
			//empty buffer (restart)
			printk("Node msg repeated/n");
			return;
		}
	}
	
	
	bcns[counter].uuid = ctx->addr;
	bcns[counter].rssi = ctx->recv_rssi;
	memcpy(&bcns[counter].pos, status, sizeof( struct bt_mesh_loc_local));
	counter++;
	
	if (counter >= 3)
	{
		counter = 0;
		trilateratePos(&local, &bcns[0], &bcns[1], &bcns[2]);
		if(bt_mesh_loc_srv_local_pub(&locSrv, NULL, &local) == 0)
			printk("\nmy pos (x,y): %d,%d\n", local.east, local.north);
	}
}