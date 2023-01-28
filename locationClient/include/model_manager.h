#ifndef BT_MESH_MODEL_MANEGER__
#define BT_MESH_MODEL_MANEGER__

#include <zephyr/bluetooth/mesh.h>
#include <localization.h>

//make beacon linked list + watch dog 
extern beacon bcns[];

const struct bt_mesh_comp *model_handler_init(void);
extern void gen_loc_set_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *loc);
extern void gen_loc_get_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *rsp);
extern void gen_loc_set_global(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_global *loc);
extern void gen_loc_get_local(struct bt_mesh_loc_srv *srv, struct bt_mesh_msg_ctx *ctx, struct bt_mesh_loc_local *rsp);

extern void gen_loc_status_local(struct bt_mesh_loc_cli *cli, struct bt_mesh_msg_ctx *ctx, const struct bt_mesh_loc_local *status);
extern void gen_loc_status_global(struct bt_mesh_loc_cli *cli, struct bt_mesh_msg_ctx *ctx, const struct bt_mesh_loc_global *status);

#endif /* BT_MESH_MODEL_MANEGER__ */