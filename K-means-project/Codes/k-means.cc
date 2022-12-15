#include <iostream>
#include <cstdlib>
#include <ctime>

static const int N = 10;
static const int M = 3;

struct Point {
int x;
int y;
};

template<int N, int M>

bool kmeans(Point points[N], int ID[N], Point center[M]) {

int min_ID = 0;
int min_distance = INT_MAX;
int new_centers_x[M];
int new_centers_y[M];
float error = 0.5;
bool flag = true;

POINTS: for (int i = 0; i < N; i++) {

int x = points[i].x;
int y = points[i].y;

    DIST: for (int j = 0; j < M; j++) { 

    int distance = abs(x - center[j].x) + abs(y - center[j].y);

        if (distance < min_distance) {

            min_distance = distance;
            min_ID = j;

        }

    }

ID[i] = min_ID;   
}

// update centers

for (int j = 0; j < M; j++) { 

int sum_x = 0;
int sum_y = 0;

    for (int i = 0; i < N; i++) {

        int x = points[i].x;
        int y = points[i].y;

        if (j == ID[i]) {

            sum_x += x;
            sum_y += y;
        }
    }
    
    div_t temp_x = div(sum_x, N);
    div_t temp_y = div(sum_y, N);

    new_centers_x[j] = temp_x.quot;
    new_centers_y[j] = temp_y.quot;


    if ((abs(center[j].x - new_centers_x[j]) < error) && (abs(center[j].y - new_centers_y[j]) < error)) {

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


int main(int argc, char** argv) {

srand((unsigned) time(NULL));

// initialize points[N]
int ID[N]={};
Point points[N];

POINTS: for (int i = 0; i < N; i++) {
points[i].x = rand() % 10;
points[i].y = rand() % 10;
}

// initialize center[M]
Point center[M];

CENTER: for (int j = 0; j < M; j++) {
center[j].x = rand() % 10;
center[j].y = rand() % 10;
}

bool go_on = true;
while (go_on) {
go_on = kmeans<N,M>(points,ID,center);
}

std::cout << "The 3 centers are:" << std::endl;
std::cout << center[0].x << ", " << center[0].y << std::endl;
std::cout << center[1].x << ", " << center[1].y << std::endl;
std::cout << center[2].x << ", " << center[2].y << std::endl;

return 0;
}