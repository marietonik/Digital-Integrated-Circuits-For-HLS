#include <iostream>
#include <cstdlib>
#include <ctime>

#include "ac_int.h"
#include "ac_math.h"
#include "k-means.h"

#include "mc_scverify.h"

template<int N, int M>
bool CCS_BLOCK(kmeans)(Point points[N], int ID[N], Point center[M]) {

POINTS: for (int i = 0; i < N; i++) {

ac_int<16,false> x = points[i].x;
ac_int<16,false> y = points[i].y;
ac_int<17,false> min_distance = -1;

    DIST: for (int j = 0; j < M; j++) { 
	
	ac_int<16,false> dist_x, dist_y;
	ac_math::ac_abs(x - center[j].x,dist_x);
	ac_math::ac_abs(y - center[j].y, dist_y);
	
    ac_int<17,false> distance;
    distance = dist_x + dist_y;

        if (distance < min_distance) {

            min_distance = distance;
            ID[i] = j;

        }
    }
}

// update centers
bool flag = true;
for (int j = 0; j < M; j++) { 

ac_int<32,false> sum_x = 0;
ac_int<32,false> sum_y = 0;
ac_int<16,false> new_centers_x[M];
ac_int<16,false> new_centers_y[M];
ac_int<16,false> error = 1;
ac_int<16,false> count = 0;

    for (int i = 0; i < N; i++) {

        ac_int<16,false> x = points[i].x;
        ac_int<16,false> y = points[i].y;

        if (j == ID[i]) {
			count++;
            sum_x += x;
            sum_y += y;
        }
    }
    
    ac_int<16,false> temp_x, temp_y;
    ac_math::ac_div(sum_x, count, temp_x);
    ac_math::ac_div(sum_y, count, temp_y);

    new_centers_x[j] = temp_x;
    new_centers_y[j] = temp_y;

	ac_int<16,false> diff_x, diff_y;
	ac_math::ac_abs(center[j].x - new_centers_x[j],diff_x);
	ac_math::ac_abs(center[j].y - new_centers_y[j],diff_y);
	
    if ((diff_x < error) && (diff_y < error)) {

        flag = false;
    }
    else {

        center[j].x = new_centers_x[j];
        center[j].y = new_centers_y[j];
        flag = true;
    }
}

// if new and old centers are almost similar return false, else return true

return flag;
}


CCS_MAIN(int argc, char** argv) {

std::srand(std::time(NULL));

// initialize points[N]
int ID[N]={0};
Point points[N];

POINTS: for (int i = 0; i < N; i++) {
points[i].x = std::rand() % 10;
points[i].y = std::rand() % 10;
}

// initialize center[M]
Point center[M];

CENTER: for (int j = 0; j < M; j++) {
center[j].x = std::rand() % 10;
center[j].y = std::rand() % 10;
}

bool go_on = true;
while (go_on) {
go_on = kmeans<N,M>(points,ID,center);
}

std::cout << "The 3 centers are:" << std::endl;
std::cout << center[0].x << ", " << center[0].y << std::endl;
std::cout << center[1].x << ", " << center[1].y << std::endl;
std::cout << center[2].x << ", " << center[2].y << std::endl;

CCS_RETURN(0);
}
