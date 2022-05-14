#include <time.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"

TruthTable *initTruthTable(size_t n) {
    TruthTable *newTt = malloc(sizeof(TruthTable));
    newTt->dimension = n;
    newTt->elements = malloc(sizeof(size_t) * 1L << n);
    return newTt;
}

void destroyTruthTable(TruthTable *tt) {
    free(tt->elements);
    free(tt);
}

void printTruthTable(TruthTable *tt) {
    printf("Dimension: %zu \n", tt->dimension);
    printf("Truth table: ");

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        printf("%zu ", tt->elements[i]);
    }
    printf("\n");
}

void writeTruthTable(FILE *fp, TruthTable *tt) {
    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        fprintf(fp, "%zu ", tt->elements[i]);
    }
    fprintf(fp, "\n");
}

TruthTable * parseTruthTable(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Requested file does not exists in system or is not found. \n");
        printf("File: %s\n", filename);
        fclose(file);
        exit(1);
    }

    size_t dimension;
    fscanf(file, "%zd", &dimension);
    TruthTable *tt = initTruthTable(dimension);

    for (size_t i = 0; i < 1L << tt->dimension; ++i) {
        fscanf(file, "%zd", &tt->elements[i]);
    }
    fclose(file);
    return tt;
}

Partition *initPartition(size_t size) {
    Partition *newPartition = malloc(sizeof(Partition));
    newPartition->numberOfClasses = 0;
    newPartition->multiplicities = malloc(sizeof(size_t) * size);
    newPartition->classSizes = malloc(sizeof(size_t) * size);
    newPartition->classes = malloc(sizeof(size_t *) * size);
}

Partition *partitionFunction(TruthTable *function, size_t k) {
    if (k % 2 != 0) {
        printf("k is odd, the function partitionFunction works only for even numbers.");
        exit(1);
    }

    size_t *multiplicities = calloc(sizeof(size_t), 1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) multiplicities[i] = 0;
    findAllMultiplicities(k, 0, multiplicities, function, 0, 0);

    Partition *partition = initPartition(1L << function->dimension);

    for (size_t i = 0; i < 1L << function->dimension; ++i) {
        size_t numBuckets = partition->numberOfClasses;
        size_t multiplicity = multiplicities[i];
        // Check if multiplicity is in bucket, if false; add multiplicity to bucket
        bool multiplicityInBuckets = false;
        for (size_t b = 0; b < numBuckets; ++b) {
            if (partition->multiplicities[b] == multiplicity) {
                multiplicityInBuckets = true;
                partition->classes[b][partition->classSizes[b]] = i;
                partition->classSizes[b] += 1;
                break;
            }
        }
        if (!multiplicityInBuckets) {
            // Add a new bucket to the classes list
            partition->classes[numBuckets] = calloc(sizeof(size_t), 1L << function->dimension);
            partition->classSizes[numBuckets] = 1;
            partition->multiplicities[numBuckets] = multiplicity;
            partition->classes[numBuckets][0] = i;
            partition->numberOfClasses += 1;
        }
    }
    free(multiplicities);

    return partition;
}

void printPartitionInfo(Partition *partition) {
    printf("Partition:\n%zu\n", partition->numberOfClasses);
    for (int i = 0; i < partition->numberOfClasses; ++i) {
        for (int j = 0; j < partition->classSizes[i]; ++j) {
            if (j == partition->classSizes[i] - 1) {
                printf("%zu\n", partition->classes[i][j]);
            } else {
                printf("%zu ", partition->classes[i][j]);
            }
        }
    }
    printf("\n");
}

void writePartition(Partition *partition, FILE *fp) {
    fprintf(fp, "%zu\n", partition->numberOfClasses);
    for (int i = 0; i < partition->numberOfClasses; ++i) {
        for (int j = 0; j < partition->classSizes[i]; ++j) {
            if (j == partition->classSizes[i] - 1) {
                fprintf(fp, "%zu\n", partition->classes[i][j]);
            } else {
                fprintf(fp, "%zu ", partition->classes[i][j]);
            }
        }
    }
}

void **mapPartitionClasses(Partition *partitionF, Partition *partitionG, size_t dimension, MappingOfBuckets *mappingOfClasses) {
    // Check if the Partition has the same number of classes. If the sizes of the classes is different,
    // we have an invalid solution.
    if (partitionF->numberOfClasses != partitionG->numberOfClasses) {
        printf("The partition of function F and G is not compatible!\n");
        printf("Partition F has %zu number of classes and partition G has %zu number of classes.\n",
               partitionF->numberOfClasses, partitionG->numberOfClasses);
        exit(1);
    }

    // Find all the domains for the different classes.
    // Example where we have several mappings:
    // [(0,2),(1),(0,2),(3,4),(3,4),(5)]
    struct Node **domains = malloc(sizeof(struct Node) * partitionF->numberOfClasses);
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        domains[i] = initLinkedList();
    }
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        bool sameSizes = false;
        for (int j = 0; j < partitionG->numberOfClasses; ++j) {
            if (partitionF->classSizes[i] == partitionG->classSizes[j]) {
                addToLinkedList(domains[i], j);
                sameSizes = true;
            }
        }
        if (!sameSizes) {
            printf("Partition F and G does not have the same class sizes!");
            exit(1);
        }
    }

    // Find out how many mappings there is
    size_t numOfMappings = 1; // There is always at least one mapping
    bool *isCalculated = malloc(sizeof(bool) * partitionF->numberOfClasses);
    for (int i = 0; i < partitionF->numberOfClasses; ++i) isCalculated[i] = false;
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        struct Node *current = (struct Node *) domains[i]->next;
        if (!isCalculated[current->data]) {
            numOfMappings = numOfMappings * factorial(countNodes(domains[i]));
            isCalculated[current->data] = true;
        }
    }
    free(isCalculated);

    // Create a list of different domain mappings
    mappingOfClasses->domains = malloc(sizeof(size_t *) * numOfMappings);
    mappingOfClasses->mappings = malloc(sizeof(size_t *) * numOfMappings);

    createMappings(mappingOfClasses, domains, partitionG, dimension, numOfMappings);

    // Free domains
    for (int i = 0; i < partitionF->numberOfClasses; ++i) {
        destroyLinkedList(domains[i]);
    }
    free(domains);
}

void destroyPartitions(Partition *p) {
    for (size_t i = 0; i < p->numberOfClasses; ++i) {
        free(p->classes[i]);
    }
    free(p->classSizes);
    free(p->multiplicities);
    free(p->classes);
    free(p);
}

MappingOfBuckets *initMappingsOfClasses() {
    MappingOfBuckets *newMap = malloc(sizeof(MappingOfBuckets));
    newMap->numOfMappings = 0;
    return newMap;
}

void addToMOC(MappingOfBuckets *moc, size_t *mapping, size_t *domain, size_t mappingSize, size_t domainSize) {
    moc->mappings[moc->numOfMappings] = malloc(sizeof(size_t *) * mappingSize);
    moc->domains[moc->numOfMappings] = malloc(sizeof (size_t *) * domainSize);
    memcpy(moc->mappings[moc->numOfMappings], mapping, mappingSize * sizeof(size_t));
    memcpy(moc->domains[moc->numOfMappings], domain, domainSize * sizeof(size_t));
    moc->numOfMappings += 1;
}

void destroyMappingOfClasses(MappingOfBuckets *mappingsOfClasses) {
    for (int i = 0; i < mappingsOfClasses->numOfMappings; ++i) {
        free(mappingsOfClasses->domains[i]);
        free(mappingsOfClasses->mappings[i]);
    }
    free(mappingsOfClasses->domains);
    free(mappingsOfClasses->mappings);
    free(mappingsOfClasses);
}

void findAllMultiplicities(size_t k, int i, size_t *multiplicities, TruthTable *function, size_t x, size_t value) {
    if (i == k - 1) {
        size_t newValue = value ^ function->elements[x];
        multiplicities[newValue] += 1;
        return;
    }
    for (int y = 0; y < 1L << function->dimension; ++y) {
        size_t newX = x ^ y;
        size_t newValue = value ^ function->elements[y];
        findAllMultiplicities(k, i + 1, multiplicities, function, newX, newValue);
    }
}

void createMappings(MappingOfBuckets *mappingOfClasses, Node **domains, Partition *partitionG, size_t dimension, size_t numOfMappings) {
      size_t *newList = malloc(sizeof(size_t) * 1L << dimension);
      bool *chosen = malloc(sizeof(bool) * partitionG->numberOfClasses); // A boolean map with the size of number of classes
      size_t *currentDomain = malloc(sizeof(size_t) * partitionG->numberOfClasses);
      for (int j = 0; j < partitionG->numberOfClasses; ++j) {
	  chosen[j] = false;
      }
      selectRecursive(0, newList, currentDomain, chosen, domains, partitionG, dimension, mappingOfClasses);
      free(currentDomain);
      free(newList);
      free(chosen);
}

Node *initLinkedList() {
    Node *newNode = malloc(sizeof(Node));
    newNode->data = 0;
    newNode->next = NULL;
    return newNode;
}

void addToLinkedList(Node *head, size_t data) {
    Node *newNode = initLinkedList();
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}

void printLinkedList(Node *head) {
    if (head->next == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    Node *current = head->next;
    printf("Nodes of linked list: ");
    while (current != NULL) {
        printf("%zu ", current->data);
        current = current->next;
    }
    printf("\n");
}

size_t countNodes(Node *head) {
    size_t count = 0;
    Node *current = head->next;

    while (current != NULL) {
        count += 1;
        current = current->next;
    }

    return count;
}

void destroyLinkedList(Node *head) {
    Node *current = NULL;
    while (head != NULL) {
        current = head;
        head = head->next;
        free(current);
    }
}

TtNode * initTtNode() {
    TtNode *newNode = malloc(sizeof(TtNode));
    newNode->next = NULL;
    newNode->data = NULL;
    return newNode;
}

void addNode(TtNode *head, TruthTable *data) {
    if (head->data == NULL) {
        head->data = initTruthTable(data->dimension);
        memcpy(head->data->elements, data->elements, sizeof(size_t) * 1L << data->dimension);
        return;
    }
    TtNode *newNode = malloc(sizeof(TtNode));
    newNode->data = initTruthTable(data->dimension);
    memcpy(newNode->data->elements, data->elements, sizeof(size_t) * 1L << data->dimension);
    newNode->next = head->next;
    head->next = newNode;
}

size_t countTtNodes(TtNode *head) {
    if (head->data == NULL) return 0;
    size_t count = 1;
    TtNode *current = head;
    while (current->next != NULL) {
        count += 1;
        current = current->next;
    }
    return count;
}

TruthTable * getNode(TtNode *head, size_t index) {
    if (index > countTtNodes(head)) {
        printf("The node is out of bounds\n");
        exit(1);
    }
    TtNode *current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

void printTtLinkedList(TtNode *head) {
    TtNode *current = head;
    if (head == NULL) {
        printf("Linked list is empty. \n");
        return;
    }
    printf("Nodes of linked list: ");
    while (current != NULL) {
        for (int i = 0; i < 1L << current->data->dimension; ++i) {
            if (i == (1L << current->data->dimension) - 1) {
                printf("%zu\n", current->data->elements[i]);
            } else {
                printf("%zu ", current->data->elements[i]);
            }
        }
        current = (TtNode *) current->next;
    }
    printf("\n");
}

void writeTtLinkedList(TtNode *head, FILE *fp) {
    TtNode *current = head;
    while (current != NULL) {
        for (int i = 0; i < 1L << current->data->dimension; ++i) {
            if (i == (1L << current->data->dimension) - 1) {
                fprintf(fp, "%zu\n", current->data->elements[i]);
            } else {
                fprintf(fp, "%zu ", current->data->elements[i]);
            }
        }
        current = current->next;
    }
}

void destroyTtLinkedList(TtNode *head) {
    if (head->data == NULL) {
        free(head);
        return;
    }
    while (head->data != NULL) {
        TtNode *current = head;
        head = head->next;
        destroyTruthTable(current->data);
        free(current);
        if (head == NULL) {
            free(head);
            break;
        }
    }
}

void
selectRecursive(size_t i, size_t *newList, size_t *currentDomain, bool *chosen, Node **domains, Partition *partitionG,
                size_t dimension, MappingOfBuckets *moc) {
    if (i >= partitionG->numberOfClasses) {
        // Add the new list and the matching domain map to the mappingOfClasses.
        addToMOC(moc, newList, currentDomain, 1L << dimension, partitionG->numberOfClasses);

        return;
    }
    Node *current = (Node *) domains[i]->next; // Tells us which bucket we are going through, starting with the first possible matching
    while (current != NULL) {
        if (!chosen[current->data]) {
            currentDomain[i] = current->data;
            for (int j = 0; j < partitionG->classSizes[current->data]; ++j) {
                newList[partitionG->classes[current->data][j]] = current->data;
            }
            chosen[current->data] = true;
            selectRecursive(i + 1, newList, currentDomain, chosen, domains, partitionG, dimension, moc);
            chosen[current->data] = false;
        }
        current = (Node *) current->next;
    }
}

size_t factorial(size_t value) {
    size_t fact = 1;
    for (int i = 1; i < value + 1; ++i) {
        fact *= i;
    }
    return fact;
}

void destroyWalshTransform(WalshTransform *wt) {
    for (size_t i = 0; i < 1L << wt->dimension; ++i) {
        free(wt->elements[i]);
    }
    free(wt->elements);
    free(wt);
}

_Bool dot(size_t a, size_t b) {
    return __builtin_popcountl(a & b) % 2;
}

size_t truthTableWalshTransform(TruthTable tt, size_t a, size_t b) {
    size_t sum = 0;
    for (size_t x = 0; x < 1L << tt.dimension; ++x) {
        sum += dot(a, x) ^ dot(b, tt.elements[x]) ? -1 : 1;
    }
    return sum;
}

WalshTransform * truthTableToWalshTransform(TruthTable tt) {
    WalshTransform *wt = malloc(sizeof(WalshTransform));
    size_t dimension = tt.dimension;
    wt->dimension = dimension;
    wt->elements = malloc(sizeof(size_t) * 1L << dimension);
    for (size_t a = 0; a < 1L << dimension; ++a) {
        wt->elements[a] = malloc(sizeof(size_t) * 1L << dimension);
        for (size_t b = 0; b < 1L << dimension; ++b) {
            wt->elements[a][b] = (size_t) truthTableWalshTransform(tt, a, b);
        }
    }
    return wt;
}

void printWalshTransformTable(WalshTransform wt) {
    printf("Walsh transform: \n");
    size_t entries = 1L << wt.dimension;
    for (size_t a = 0; a < entries; ++a) {
        for (size_t b = 0; b < entries; ++b) {
            printf("%ld\t", wt.elements[a][b]);
        }
        printf("\n");
    }
}

size_t walshTransformPowerMoment(WalshTransform wt, size_t k, size_t shiftA, size_t shiftB) {
    size_t sum = 0;
    for (size_t a = 0; a < 1L << wt.dimension; ++a) {
        for (size_t b = 0; b < 1L << wt.dimension; ++b) {
            size_t modifier = (dot(a, shiftA) ^ dot(b, shiftB)) ? -1 : 1;
            size_t base = wt.elements[a][b];
            size_t product = 1;
            for (size_t i = 0; i < k; ++i) {
                product *= base;
            }
            sum += product * modifier;
        }
    }
    return sum;
}

Partition *eaPartitionWalsh(TruthTable *tt, size_t k) {
    // Change function f and g from a truth table to a walsh transform table
    WalshTransform *wt = truthTableToWalshTransform(*tt);

    size_t *multiplicities = malloc(sizeof(size_t) * 1L << wt->dimension);
    size_t possibleValues [1L << wt->dimension];
    size_t currentPossibleValue = 0;

    for (size_t s = 0; s < 1L << wt->dimension; ++s) {
        size_t walsh = walshTransformPowerMoment(*wt, k, 0, s);
        // Note that the value of the power moment is 2^(2*n) times the multiplicity
        size_t multiplicity = walsh >> (2 * wt->dimension);
        multiplicities[s] = multiplicity;
        // Try to find this multiplicity in the list of recorded multiplicities. If it's note there, add a new entry.
        size_t k2 = 0;
        for (; k2 < currentPossibleValue; ++k2) {
            if (possibleValues[k2] == multiplicity) {
                break;
            }
        }
        if (k2 == currentPossibleValue) {
            possibleValues[currentPossibleValue++] = multiplicity;
        }
    }

    // At this point, we know what exact multiplicities define the partition, and can construct the structure
    Partition *p = malloc(sizeof(Partition));
    p->numberOfClasses = currentPossibleValue;
    p->classSizes = malloc(sizeof(size_t) * currentPossibleValue);
    p->classes = malloc(sizeof(size_t *) * currentPossibleValue);
    p->multiplicities = multiplicities;

    size_t currentClass[1L << wt->dimension];
    size_t currentClassIndex;

    for (size_t classIndex = 0; classIndex < currentPossibleValue; ++classIndex) {
        currentClassIndex = 0;
        for (size_t s = 0; s < 1L << wt->dimension; ++s) {
            if (multiplicities[s] == possibleValues[classIndex]) {
                currentClass[currentClassIndex++] = s;
            }
        }
        p->classSizes[classIndex] = currentClassIndex;
        p->classes[classIndex] = malloc(sizeof(size_t *) * currentClassIndex);
        memcpy(p->classes[classIndex], currentClass, sizeof(size_t) * currentClassIndex);
    }
    destroyWalshTransform(wt);
    return p;
}

RunTimes *initRunTimes() {
    RunTimes *newTime = malloc(sizeof(RunTimes));
    newTime->total = 0.0;
    newTime->partition = 0.0;
    newTime->outerPermutation = 0.0;
    newTime->innerPermutation = 0.0;
    return newTime;
}

double stopTime(double runTime, clock_t startParsing) {
    runTime += (double) (clock() - startParsing) / CLOCKS_PER_SEC;
    return runTime;
}

void printTimes(RunTimes *runTimes) {
    printf("Time spent partitioning: %f \n", runTimes->partition);
    printf("Time spent outer permutation: %f \n", runTimes->outerPermutation);
    printf("Time spent inner permutation: %f \n", runTimes->innerPermutation);
    printf("Total time spent: %f \n", runTimes->total);
}

void writeTimes(RunTimes *runTimes, FILE *fp) {
    fprintf(fp, "Time spent partitioning: %f \n", runTimes->partition);
    fprintf(fp, "Time spent outer permutation: %f \n", runTimes->outerPermutation);
    fprintf(fp, "Time spent inner permutation: %f \n", runTimes->innerPermutation);
    fprintf(fp, "Total time spent: %f \n", runTimes->total);
}

void destroyRunTimes(RunTimes *runTimes) {
    free(runTimes);
}