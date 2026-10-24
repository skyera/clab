#include "contacts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Contact *find_contact(Contact *contacts, int id)
{
    Contact *contact;
    HASH_FIND_INT(contacts, &id, contact);
    return contact;
}

/* Passing the head by address lets uthash update it on the first insertion. */
int put_contact(Contact **contacts, int id, const char *name)
{
    Contact *contact;
    size_t length = strlen(name);

    if (length >= sizeof(((Contact *)0)->name)) {
        fputs("Contact name is too long.\n", stderr);
        return 0;
    }

    contact = find_contact(*contacts, id);
    if (contact == NULL) {
        contact = malloc(sizeof *contact);
        if (contact == NULL) {
            fputs("Unable to allocate contact.\n", stderr);
            return 0;
        }
        contact->id = id;
        HASH_ADD_INT(*contacts, id, contact);
    }
    memcpy(contact->name, name, length + 1);
    return 1;
}

int delete_contact(Contact **contacts, int id)
{
    Contact *contact = find_contact(*contacts, id);
    if (contact == NULL) {
        return 0;
    }
    HASH_DEL(*contacts, contact);
    free(contact);
    return 1;
}

void clear_contacts(Contact **contacts)
{
    Contact *contact, *next;
    /* HASH_ITER permits deleting the current entry during traversal. */
    HASH_ITER(hh, *contacts, contact, next) {
        HASH_DEL(*contacts, contact);
        free(contact);
    }
}
