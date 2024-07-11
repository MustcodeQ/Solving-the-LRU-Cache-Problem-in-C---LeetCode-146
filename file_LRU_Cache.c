#include <stdio.h>
#include <stdlib.h>

// Node structure for doubly linked list
typedef struct Node
{
    int key;
    int value;
    struct Node *prev;
    struct Node *next;
} Node;

// LRUCache structure
typedef struct
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    Node **hashMap;
} LRUCache;

// Function to create a new node
Node *createNode(int key, int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// Function to create an LRUCache
LRUCache *lRUCacheCreate(int capacity)
{
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->hashMap = (Node **)malloc(10000 * sizeof(Node *));
    for (int i = 0; i < 10000; i++)
    {
        cache->hashMap[i] = NULL;
    }
    return cache;
}

// Function to move a node to the head
void moveToHead(LRUCache *cache, Node *node)
{
    if (node == cache->head)
    {
        return;
    }

    if (node == cache->tail)
    {
        cache->tail = cache->tail->prev;
        cache->tail->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node->next = cache->head;
    cache->head->prev = node;
    cache->head = node;
    node->prev = NULL;
}

// Function to remove the tail node
void removeTail(LRUCache *cache)
{
    if (cache->tail == NULL)
    {
        return;
    }

    Node *node = cache->tail;
    cache->tail = cache->tail->prev;
    if (cache->tail != NULL)
    {
        cache->tail->next = NULL;
    }
    else
    {
        cache->head = NULL;
    }

    cache->hashMap[node->key] = NULL;
    free(node);
    cache->size--;
}

// Function to get the value of a key
int lRUCacheGet(LRUCache *obj, int key)
{
    Node *node = obj->hashMap[key];
    if (node == NULL)
    {
        return -1;
    }

    moveToHead(obj, node);
    return node->value;
}

// Function to put a key-value pair
void lRUCachePut(LRUCache *obj, int key, int value)
{
    Node *node = obj->hashMap[key];
    if (node != NULL)
    {
        node->value = value;
        moveToHead(obj, node);
    }
    else
    {
        node = createNode(key, value);
        if (obj->size == obj->capacity)
        {
            removeTail(obj);
        }

        node->next = obj->head;
        if (obj->head != NULL)
        {
            obj->head->prev = node;
        }
        obj->head = node;
        if (obj->tail == NULL)
        {
            obj->tail = node;
        }
        obj->hashMap[key] = node;
        obj->size++;
    }
}

// Function to free the LRUCache
void lRUCacheFree(LRUCache *obj)
{
    Node *current = obj->head;
    while (current != NULL)
    {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(obj->hashMap);
    free(obj);
}

int main()
{
    LRUCache *obj = lRUCacheCreate(2);
    lRUCachePut(obj, 1, 1);
    lRUCachePut(obj, 2, 2);
    printf("%d\n", lRUCacheGet(obj, 1)); // returns 1
    lRUCachePut(obj, 3, 3);              // evicts key 2
    printf("%d\n", lRUCacheGet(obj, 2)); // returns -1 (not found)
    lRUCachePut(obj, 4, 4);              // evicts key 1
    printf("%d\n", lRUCacheGet(obj, 1)); // returns -1 (not found)
    printf("%d\n", lRUCacheGet(obj, 3)); // returns 3
    printf("%d\n", lRUCacheGet(obj, 4)); // returns 4
    lRUCacheFree(obj);

    return 0;
}
