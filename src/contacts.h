#ifndef CLAB_CONTACTS_H
#define CLAB_CONTACTS_H

#include "uthash.h"

typedef struct Contact {
    int id;                 /* Do not change the key while stored in the table. */
    char name[64];
    UT_hash_handle hh;
} Contact;

/* Initialize the head to NULL. Returned contacts are owned by the table. */
Contact *find_contact(Contact *contacts, int id);

/* Insert or update; returns 0 on oversized names or contact allocation failure.
 * contacts and name must be non-NULL; name must be NUL-terminated.
 * uthash internal allocations retain its default fatal failure handler. */
int put_contact(Contact **contacts, int id, const char *name);

/* Returns 1 if deleted, or 0 if absent. contacts must be non-NULL. */
int delete_contact(Contact **contacts, int id);

/* Frees all entries and resets the head to NULL. contacts must be non-NULL. */
void clear_contacts(Contact **contacts);

#endif
