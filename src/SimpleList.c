#include "../lib/SimpleList.h"

void simpleListCreate(SimpleList *pL, int size) {
    pL->current = NULL;
    pL->first = NULL;
    pL->size = size;
}

void simpleListEmpty(SimpleList *pL) {
    SimpleListNode *pNode, *pNext;
    for (pNode = pL->first; (pNode); pNode = pNext) {
        pNext = pNode->next;
        free(pNode->elem);
        free(pNode);
    }
    pL->first = pL->current = NULL;
}

int simpleListIsEmpty(SimpleList l) {
    return (l.first == NULL);
}

void simpleListCurrent(SimpleList l, void *pE) {
    memcpy(pE, l.current->elem, l.size);
}

int simpleListMoveCurrent(SimpleList *pL, SimpleListMovement m) {
    switch (m) {
        case MOVEMENT_FIRST:
            pL->current = pL->first;
            break;
        case MOVEMENT_NEXT:
            if (pL->current->next == NULL)
                return FALSE;
            else
                pL->current = pL->current->next;
            break;
        case MOVEMENT_PREVIOUS:
            return FALSE;
    }
    return TRUE;
}

void simpleListDeleteCurrent(SimpleList *pL) {
    SimpleListNode *pNode = pL->current;
    if (pL->current == pL->first) {
        pL->first = pL->current->next;
        pL->current = pL->first;
    } else {
        if (pL->current->next) {
            /* When the current is not the last one, can avoid iterating the entire list seeking the previous element */
            pNode = pL->current->next;
            memcpy(pL->current->elem, pNode->elem, pL->size);
            pL->current->next = pNode->next;
        } else {
            struct SimpleListNode *pAux = pL->first;
            while (pAux->next != pL->current)
                pAux = pAux->next;
            pAux->next = pL->current->next;
            pL->current = pAux;
        }
    }
    free(pNode->elem);
    free(pNode);
}

int simpleListInsertCurrent(SimpleList *pL, SimpleListMovement m, void *pE) {
    SimpleListNode *pNode = (SimpleListNode *) malloc(sizeof(SimpleListNode));
    if (!pNode)
        return FALSE;
    pNode->elem = malloc((size_t) pL->size);
    if (!pNode->elem) {
        free(pNode);
        return FALSE;
    }
    memcpy(pNode->elem, pE, pL->size);
    if ((pL->first == NULL) || (m == MOVEMENT_FIRST) || ((m == MOVEMENT_PREVIOUS) && (pL->first == pL->current))) {
        pNode->next = pL->first;
        pL->first = pL->current = pNode;
    } else {
        pNode->next = pL->current->next;
        pL->current->next = pNode;
        if (m == MOVEMENT_PREVIOUS) {
            void *tmp = pNode->elem;
            pNode->elem = pL->current->elem;
            pL->current->elem = tmp;
        }
    }
    pL->current = pNode;
    return TRUE;
}

void simpleListModifyCurrent(SimpleList l, void *pE) {
    memcpy(l.current->elem, pE, l.size);
}
