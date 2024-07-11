LRUCache Implementation in C
Overview
This program implements a Least Recently Used (LRU) Cache using a doubly linked list and a hash map for O(1) average time complexity for both get and put operations.

Structures
Node
The Node structure represents a node in the doubly linked list.


typedef struct Node
{
    int key;
    int value;
    struct Node *prev;
    struct Node *next;
} Node;
LRUCache
The LRUCache structure contains the following:

capacity: Maximum number of entries the cache can hold.
size: Current number of entries in the cache.
head: Pointer to the head node of the doubly linked list.
tail: Pointer to the tail node of the doubly linked list.
hashMap: Array of node pointers for fast access.

typedef struct
{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    Node **hashMap;
} LRUCache;
Functions
createNode
Creates and initializes a new node with the given key and value.


Node *createNode(int key, int value)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    return node;
}
lRUCacheCreate
Creates and initializes an LRUCache with the given capacity.


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
moveToHead
Moves the given node to the head of the doubly linked list.


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
removeTail
Removes the tail node from the doubly linked list and updates the hash map.


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
lRUCacheGet
Retrieves the value associated with the given key in the LRUCache.


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
lRUCachePut
Inserts a key-value pair into the LRUCache, evicting the least recently used entry if necessary.


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
lRUCacheFree
Frees the memory allocated for the LRUCache.


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
Main Function
Demonstrates the usage of the LRUCache.


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
