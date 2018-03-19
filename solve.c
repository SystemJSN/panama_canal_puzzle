#include <stdio.h>
#include <stdlib.h>

/* hash table size */
#define HASH_SIZE 100003
/* queue size */
#define QUEUE_SIZE 50000

/* position of the game */
typedef struct t_box {
    char s[2][6];
    int bx;
    int by;
    int depth;
    char c;
    int dir;
}box;

/* node in hash table */
typedef struct t_node {
    box b;
    struct t_node* next;
    struct t_node* last;
}node;

/* get hash value of the position */
unsigned long hash(char s[2][6]) {
    int i, j;
    int k = 1;
    unsigned long value = 0;
    for (i = 0; i < 2; ++i) {
        for (j = 0; j < 6; ++j) {
            value = (value + (k * (s[i][j] - 'a' + 1) >> 20)) % HASH_SIZE;
            k *= 7;
        }
    }
    return value;
}

/* compare two positions */
int same_box(box* b1, box* b2) {
    int i, j;
    for (i = 0; i < 2; ++i) {
        for (j = 0; j < 6; ++j) {
            if (b1->s[i][j] != b2->s[i][j]) {
                return 0;
            }
        }
    }
    if (b1->bx != b2->bx || b1->by != b2->by) {
        return 0;
    }
    return 1;
}

/* copy position */
void copy_box(box* b1, box* b2) {
    int i, j;
    for (i = 0; i < 2; ++i) {
        for (j = 0; j < 6; ++j) {
            b1->s[i][j] = b2->s[i][j];
        }
    }
    b1->bx = b2->bx;
    b1->by = b2->by;
    b1->depth = b2->depth;
    b1->c = b2->c;
    b1->dir = b2->dir;
}

/* move the white blank north */
void move_north(box* b, box* new_box) {
    int bx, by;
    if (b->bx == 0) {
        new_box->bx = -1;
        new_box->by = -1;
        return;
    }
    copy_box(new_box, b);
    bx = new_box->bx;
    by = new_box->by;
    new_box->s[bx][by] = new_box->s[bx - 1][by];
    new_box->s[bx - 1][by] = ' ';
    new_box->c = new_box->s[bx][by];
    new_box->dir = 0;
    new_box->bx--;
    new_box->depth++;
}

/* move the white blank south */
void move_south(box* b, box* new_box) {
    int bx, by;
    if (b->bx == 1) {
        new_box->bx = -1;
        new_box->by = -1;
        return;
    }
    copy_box(new_box, b);
    bx = new_box->bx;
    by = new_box->by;
    new_box->s[bx][by] = new_box->s[bx + 1][by];
    new_box->s[bx + 1][by] = ' ';
    new_box->c = new_box->s[bx][by];
    new_box->dir = 1;
    new_box->bx++;
    new_box->depth++;
}

/* move the white blank west */
void move_west(box* b, box* new_box) {
    int bx, by;
    if (b->by == 0) {
        new_box->bx = -1;
        new_box->by = -1;
        return;
    }
    copy_box(new_box, b);
    bx = new_box->bx;
    by = new_box->by;
    new_box->s[bx][by] = new_box->s[bx][by - 1];
    new_box->s[bx][by - 1] = ' ';
    new_box->c = new_box->s[bx][by];
    new_box->dir = 2;
    new_box->by--;
    new_box->depth++;
}

/* move the white blank east */
void move_east(box* b, box* new_box) {
    int bx, by;
    if (b->by == 5) {
        new_box->bx = -1;
        new_box->by = -1;
        return;
    }
    copy_box(new_box, b);
    bx = new_box->bx;
    by = new_box->by;
    new_box->s[bx][by] = new_box->s[bx][by + 1];
    new_box->s[bx][by + 1] = ' ';
    new_box->c = new_box->s[bx][by];
    new_box->dir = 3;
    new_box->by++;
    new_box->depth++;
}

/* move the white blank to get a new position */
void move(box* b, int dir, box* new_box) {
    if (dir == 0) {
        move_north(b, new_box);
    } else if (dir == 1) {
        move_south(b, new_box);
    } else if (dir == 2) {
        move_west(b, new_box);
    } else {
        move_east(b, new_box);
    }
}

/* whether the position is the final position */
int is_final_state(box* b) {
    char* tmp1 = "PANAMA";
    char* tmp2 = "CANAL ";
    int i;
    for (i = 0; i < 6; ++i) {
        if (tmp1[i] != b->s[0][i]) {
            return 0;
        }
    }
    for (i = 0; i < 6; ++i) {
        if (tmp2[i] != b->s[1][i]) {
            return 0;
        }
    }
    return 1;
}

/* position used or not */
int box_used(box* b, node* hash_table, unsigned long hash_value) {
    node* tmp_node = hash_table + hash_value;
    tmp_node = tmp_node->next;
    while (tmp_node != NULL) {
        if (same_box(&tmp_node->b, b)) {
            return 1;
        }
        tmp_node = tmp_node->next;
    }
    return 0;
}

/* insert the position to the hash table */
node* insert_box_to_table(box* b, node* hash_table, unsigned long hash_value) {
    node* new_node = (node*)malloc(sizeof(node));
    copy_box(&new_node->b, b);
    new_node->next = hash_table[hash_value].next;
    hash_table[hash_value].next = new_node;
    return new_node;
}

/* print result */
void print_res(node* n, FILE* fp) {
    int j, k;
    if (n->last) {
        print_res(n->last, fp);
        fprintf(fp, "Step %d, move %c ", n->b.depth, n->b.c);
        if (n->b.dir == 0) {
            fprintf(fp, "south:\n");
        } else if (n->b.dir == 1) {
            fprintf(fp, "north:\n");
        } else if (n->b.dir == 2) {
            fprintf(fp, "east:\n");
        } else {
            fprintf(fp, "west:\n");
        }
    } else {
        fprintf(fp, "Step 0:\n");
    }
    for (j = 0; j < 2; ++j) {
        for (k = 0; k < 6; ++k) {
            fprintf(fp, "%c ", n->b.s[j][k]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
}

/* free spaces of the hash table */
void free_hash_table(node* hash_table) {
    int i;
    node* tmp_node = NULL;
    for (i = 0; i < HASH_SIZE; ++i) {
        tmp_node = hash_table[i].next;
        while (tmp_node) {
            hash_table[i].next = tmp_node->next;
            free(tmp_node);
            tmp_node = hash_table[i].next;
        }
    }
    free(hash_table);
}

/* run the solution */
void run(box* init_box) {
    node* hash_table = (node*)malloc(sizeof(node) * HASH_SIZE);
    node* queue[QUEUE_SIZE];
    int queue_head = 0;
    int queue_tail = 0;
    unsigned long hash_value;
    box tmp_box;
    node* tmp_node = NULL;
    node* next_node = NULL;
    int i;
    node* res_node = NULL;
    FILE* fp = NULL;

    for (i = 0; i < HASH_SIZE; ++i) {
        hash_table[i].next = NULL;
    }

    hash_value = hash(init_box->s);
    tmp_node = insert_box_to_table(init_box, hash_table, hash_value);
    tmp_node->last = NULL;
    queue[queue_tail++] = tmp_node;

    while (queue_head != queue_tail) {
        tmp_node = queue[queue_head++];
        if (queue_head == QUEUE_SIZE) {
            queue_head = 0;
        }
        if (is_final_state(&tmp_node->b)) {
            res_node = tmp_node;
            break;
        }
        for (i = 0; i < 4; ++i) {
            move(&tmp_node->b, i, &tmp_box);
            if (tmp_box.bx != -1) {
                hash_value = hash(tmp_box.s);
                if (!box_used(&tmp_box, hash_table, hash_value)) {
                    next_node = insert_box_to_table(&tmp_box, hash_table, hash_value);
                    next_node->last = tmp_node;
                    queue[queue_tail++] = next_node;
                    if (queue_tail == QUEUE_SIZE) {
                        queue_tail = 0;
                    }
                }
            }
        }
    }

    fp = fopen("output.txt", "w");
    print_res(res_node, fp);
    fclose(fp);

    free_hash_table(hash_table);
}

int main(int argc, char** argv) {
    box init_box;
    int i;
    char* tmp1 = "CANAMA";
    char* tmp2 = "PANAL ";
    init_box.depth = 0;
    for (i = 0; i < 6; ++i) {
        init_box.s[0][i] = tmp1[i];
    }
    for (i = 0; i < 6; ++i) {
        init_box.s[1][i] = tmp2[i];
    }
    init_box.bx = 1;
    init_box.by = 5;
    run(&init_box);
    return 0;
}
