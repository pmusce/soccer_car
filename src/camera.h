#define CAMERA_MAX_NUM 3

#define CAMERA_MOUSE 0
#define CAMERA_BALL 1
#define CAMERA_CAR 2

extern float angle;
extern float viewAlpha, viewBeta; // angoli che definiscono la vista
extern float eyeDist;

void setCamera();
int nextCamera();