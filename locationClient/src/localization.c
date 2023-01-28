#include "localization.h"
#include <math.h>

#define ENVFACTOR 	4.0

/* 
 * Description:
 *          Calculates distance based on a set of RSSI samples. Function removes outliers since due to RSSI instability.
 * PARAM:
 *          input   correctedRssi     Collected samples of RSSI
 */
double calcDistance(int8_t correctedRssi)
{
    double exponent = 0, distance = 0;
    
    exponent = (-60 - correctedRssi);
    exponent = exponent / (10*ENVFACTOR);

    distance = pow(10.0, exponent);

    return distance;
}

/* 
 * Description:
 *          Calculates distance based on a set of RSSI samples. \
 * PARAM:  
 *     input  beacon east
 */
void trilateratePos(struct bt_mesh_loc_local *loc, beacon *a, beacon *b, beacon *c)
{
    double distanceA = calcDistance(a->rssi), 
           distanceB = calcDistance(b->rssi), 
           distanceC = calcDistance(c->rssi);

    long double vb = ((pow(a->pos.east,2)-pow(b->pos.east,2)) + (pow(a->pos.north,2)-pow(b->pos.north,2)) + (pow(distanceB,2)-pow(distanceA,2))) / 2;

    long double va = ((pow(c->pos.east,2)-pow(b->pos.east,2)) + (pow(c->pos.north,2)-pow(b->pos.north,2)) + (pow(distanceB,2) - pow(distanceC,2))) / 2;

    loc->north = (vb*(b->pos.east - c->pos.east) - va*(b->pos.east - c->pos.east)) /
                (((a->pos.north - b->pos.north)*(b->pos.east - c->pos.east)) - ((c->pos.north - b->pos.north)*(b->pos.east - c->pos.east)));

    loc->east = (loc->north*(a->pos.north - b->pos.north) - vb) / (b->pos.east - a->pos.east);
}