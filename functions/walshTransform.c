#include "walshTransform.h"
#include "../utils/truthTable.h"

typedef struct vbfWalshTransform walshTransform;

_Bool dot(size_t a, size_t b);

size_t truthTableWalshTransform(truthTable tt, size_t a, size_t b);

walshTransform truthTableToWalshTransform(truthTable tt);

_Bool dot(size_t a, size_t b) {
    return __builtin_popcountl(a & b) % 2;
}

size_t truthTableWalshTransform(truthTable tt, size_t a, size_t b) {
    size_t sum = 0;
    for (size_t x = 0; x < 1L << tt.dimension; ++x) {
        sum += dot(a, x) ^ dot(b, tt.elements[x]) ? -1 : 1;
    }
    return sum;
}

//walshTransform truthTableToWalshTransform(truthTable tt) {
//    walshTransform wt;
//    size_t dimension = tt.dimension;
//    wt.
//}
