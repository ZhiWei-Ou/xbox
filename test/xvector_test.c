#include "xvector.h"
#include "xlog.h"
#include <stdio.h>

struct MyStruct {
    XVector(int) v;
    XVector(char) v2;
};

int main() {
    XVector(int) v1;

    XVectorInit(&v1);
    XLOG_INFOF("v1 length: %d, capacity: %d", XVectorLen(&v1), XVectorCap(&v1));

    XVectorPush(&v1, 1);
    XVectorPush(&v1, 2);
    XVectorPush(&v1, 3);
    XVectorPush(&v1, 4);
    XLOG_INFOF("v1 length: %d, capacity: %d", XVectorLen(&v1), XVectorCap(&v1));
    XVectorForEach(&v1, i) {
        int num = XVectorAt(&v1, i);
        XLOG_INFOF("v1[%d]: %d", i, num);
    }

    XVectorFini(&v1);
    XLOG_INFOF("v1 length: %d, capacity: %d", XVectorLen(&v1), XVectorCap(&v1));
    
    return 0;
}
