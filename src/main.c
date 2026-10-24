#include <stdio.h>
#include <stdlib.h>

#include "contacts.h"

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
