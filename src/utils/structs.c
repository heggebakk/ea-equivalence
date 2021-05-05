#include <stdlib.h>
#include "structs.h"

void freeTruthTable(truthTable tt) {
    free(tt.elements);
}

void freeBuckets(partitions *partition) {
    for (int i = 0; i < partition->numBuckets; ++i) {
        free(partition->buckets[i]->elements);
    }
}

void freePartition(partitions p) {
    for(int i = 0; i < p.numBuckets; ++i) {
        free(p.buckets[i]);
    }
    free(p.buckets);
}

void freeImagesOfElements(imagesOfElements *images) {
    free(images->elements);
    free(images);
}
