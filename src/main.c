#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uthash.h"

typedef struct Contact {
    int id;                 /* The hash key must remain unchanged while stored. */
    char name[64];
    UT_hash_handle hh;      /* uthash stores its bookkeeping here. */
} Contact;

static Contact *find_contact(Contact *contacts, int id)
{
    Contact *contact;
    HASH_FIND_INT(contacts, &id, contact);
    return contact;
}

/* Passing the head by address lets uthash update it on the first insertion. */
static int put_contact(Contact **contacts, int id, const char *name)
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

static int delete_contact(Contact **contacts, int id)
{
    Contact *contact = find_contact(*contacts, id);
    if (contact == NULL) {
        return 0;
    }
    HASH_DEL(*contacts, contact);
    free(contact);
    return 1;
}

static void clear_contacts(Contact **contacts)
{
    Contact *contact, *next;
    /* HASH_ITER permits deleting the current entry during traversal. */
    HASH_ITER(hh, *contacts, contact, next) {
        HASH_DEL(*contacts, contact);
        free(contact);
    }
}

int main(void)
{
    Contact *contacts = NULL;
    Contact *contact, *next;

    if (!put_contact(&contacts, 1, "Ada") ||
        !put_contact(&contacts, 2, "Grace") ||
        !put_contact(&contacts, 3, "Linus") ||
        !put_contact(&contacts, 2, "Grace Hopper")) {
        clear_contacts(&contacts);
        return EXIT_FAILURE;
    }

    printf("Contacts: %u\n", HASH_COUNT(contacts));
    HASH_ITER(hh, contacts, contact, next) {
        printf("  %d: %s\n", contact->id, contact->name);
    }

    contact = find_contact(contacts, 2);
    printf("Find 2: %s\n", contact != NULL ? contact->name : "not found");
    contact = find_contact(contacts, 99);
    printf("Find 99: %s\n", contact != NULL ? contact->name : "not found");

    printf("Delete 1: %s\n", delete_contact(&contacts, 1) ? "deleted" : "not found");
    printf("Delete 1 again: %s\n", delete_contact(&contacts, 1) ? "deleted" : "not found");
    printf("Remaining: %u\n", HASH_COUNT(contacts));

    clear_contacts(&contacts);
    printf("After cleanup: %u\n", HASH_COUNT(contacts));
    return EXIT_SUCCESS;
}
