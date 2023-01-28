#include <zephyr/bluetooth/bluetooth.h>
#include <model_manager.h>
#include <persistant_store_manager.h>

static void gen_loc_set_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *loc);
static void gen_loc_get_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *rsp);
static void gen_loc_set_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *loc);
static void gen_loc_get_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *rsp);

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

static struct bt_mesh_loc_srv_handlers loc_srv_cb = {
	.local_get = gen_loc_get_local,
	.local_set = gen_loc_set_local,
	.global_get = gen_loc_get_global,
	.global_set = gen_loc_set_global
};
struct bt_mesh_loc_srv locSrv = BT_MESH_LOC_SRV_INIT(&loc_srv_cb);

// Composition
static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, 
		BT_MESH_MODEL_LIST(
			BT_MESH_MODEL_CFG_SRV,
			BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
			BT_MESH_MODEL_LOC_SRV(&locSrv)
		), 
		BT_MESH_MODEL_NONE),
};

// node
static struct bt_mesh_comp comp = {
		.cid = 0xFFFF, 	// Company ID
		.elem = elements,
		.elem_count = ARRAY_SIZE(elements),
};

const struct bt_mesh_comp *model_handler_init()
{
	return &comp;
}

// Location Server
// -----------------------------------------------------------------
static void gen_loc_get_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *rsp)
{

}

static void gen_loc_set_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *loc)
{
	settings_save_one("pos/local/x", &local.east, sizeof(local.east)); 
	settings_save_one("pos/local/y", &local.north, sizeof(local.north)); 
	settings_load_subtree("pos/local");
	settings_save();
}

static void gen_loc_get_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *rsp)
{
}

void gen_loc_set_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *loc)
{
}