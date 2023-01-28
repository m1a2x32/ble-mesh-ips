#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <bluetooth/mesh/models.h>

typedef struct {
    uint16_t uuid;
    //struct bt_mesh_loc_global pos;
    struct bt_mesh_loc_local pos;
    int8_t rssi;
} beacon;


double calcDistance(int8_t correctedRssi);
void trilateratePos(struct bt_mesh_loc_local *loc, beacon *a, beacon *b, beacon *c);



#endif