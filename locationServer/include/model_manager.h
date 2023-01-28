#ifndef BT_MESH_MODEL_MANEGER__
#define BT_MESH_MODEL_MANEGER__

#include <zephyr/bluetooth/mesh.h>

#define STORAGE_NODE_LABEL storage

extern struct bt_mesh_loc_srv locSrv;
const struct bt_mesh_comp *model_handler_init(void);


#endif /* BT_MESH_MODEL_MANEGER__ */