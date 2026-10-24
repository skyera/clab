#include <string.h>

#include "contacts.h"
#include "unity.h"

static Contact *contacts;

void setUp(void)
{
    contacts = NULL;
}

void tearDown(void)
{
    clear_contacts(&contacts);
}

static void test_empty_lookup(void)
{
    TEST_ASSERT_NULL(find_contact(contacts, 1));
}

static void test_insert_and_lookup(void)
{
    Contact *contact;
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, "Ada"));
    TEST_ASSERT_TRUE(put_contact(&contacts, 2, "Grace"));
    TEST_ASSERT_EQUAL_UINT(2, HASH_COUNT(contacts));
    contact = find_contact(contacts, 1);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_INT(1, contact->id);
    TEST_ASSERT_EQUAL_STRING("Ada", contact->name);
    contact = find_contact(contacts, 2);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Grace", contact->name);
    TEST_ASSERT_NULL(find_contact(contacts, 99));
}

static void test_update_does_not_duplicate(void)
{
    Contact *contact;
    TEST_ASSERT_TRUE(put_contact(&contacts, 2, "Grace"));
    TEST_ASSERT_TRUE(put_contact(&contacts, 2, "Grace Hopper"));
    TEST_ASSERT_EQUAL_UINT(1, HASH_COUNT(contacts));
    contact = find_contact(contacts, 2);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Grace Hopper", contact->name);
}

static void test_delete_preserves_other_contacts(void)
{
    Contact *contact;
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, "Ada"));
    TEST_ASSERT_TRUE(put_contact(&contacts, 2, "Grace"));
    TEST_ASSERT_TRUE(delete_contact(&contacts, 1));
    TEST_ASSERT_NULL(find_contact(contacts, 1));
    TEST_ASSERT_EQUAL_UINT(1, HASH_COUNT(contacts));
    contact = find_contact(contacts, 2);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Grace", contact->name);
    TEST_ASSERT_FALSE(delete_contact(&contacts, 1));
    TEST_ASSERT_TRUE(delete_contact(&contacts, 2));
    TEST_ASSERT_NULL(contacts);
}

static void test_delete_missing_id_keeps_table(void)
{
    TEST_ASSERT_FALSE(delete_contact(&contacts, 99));
    TEST_ASSERT_NULL(contacts);
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, "Ada"));
    TEST_ASSERT_FALSE(delete_contact(&contacts, 99));
    TEST_ASSERT_EQUAL_UINT(1, HASH_COUNT(contacts));
    TEST_ASSERT_NOT_NULL(find_contact(contacts, 1));
}

static void test_clear_is_repeatable_and_table_can_be_reused(void)
{
    clear_contacts(&contacts);
    TEST_ASSERT_NULL(contacts);
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, "Ada"));
    TEST_ASSERT_TRUE(put_contact(&contacts, 2, "Grace"));
    clear_contacts(&contacts);
    TEST_ASSERT_NULL(contacts);
    TEST_ASSERT_EQUAL_UINT(0, HASH_COUNT(contacts));
    clear_contacts(&contacts);
    TEST_ASSERT_NULL(contacts);
    TEST_ASSERT_TRUE(put_contact(&contacts, 3, "Linus"));
    TEST_ASSERT_EQUAL_UINT(1, HASH_COUNT(contacts));
    TEST_ASSERT_NOT_NULL(find_contact(contacts, 3));
}

static void test_name_is_copied(void)
{
    char name[] = "Ada";
    Contact *contact;
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, name));
    name[0] = 'X';
    contact = find_contact(contacts, 1);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Ada", contact->name);
}

static void test_accepts_empty_and_maximum_names(void)
{
    char name[64];
    Contact *contact;
    memset(name, 'a', sizeof name - 1);
    name[sizeof name - 1] = '\0';
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, ""));
    contact = find_contact(contacts, 1);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("", contact->name);
    TEST_ASSERT_TRUE(put_contact(&contacts, 2, name));
    contact = find_contact(contacts, 2);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING(name, contact->name);
}

static void test_rejects_oversized_names_without_changing_table(void)
{
    char name[65];
    Contact *contact;
    memset(name, 'b', sizeof name - 1);
    name[sizeof name - 1] = '\0';
    TEST_ASSERT_FALSE(put_contact(&contacts, 1, name));
    TEST_ASSERT_NULL(contacts);
    TEST_ASSERT_TRUE(put_contact(&contacts, 1, "Ada"));
    TEST_ASSERT_FALSE(put_contact(&contacts, 1, name));
    TEST_ASSERT_FALSE(put_contact(&contacts, 2, name));
    TEST_ASSERT_EQUAL_UINT(1, HASH_COUNT(contacts));
    TEST_ASSERT_NULL(find_contact(contacts, 2));
    contact = find_contact(contacts, 1);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Ada", contact->name);
}

static void test_zero_and_negative_ids(void)
{
    Contact *contact;
    TEST_ASSERT_TRUE(put_contact(&contacts, 0, "Zero"));
    TEST_ASSERT_TRUE(put_contact(&contacts, -1, "Negative"));
    contact = find_contact(contacts, 0);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Zero", contact->name);
    contact = find_contact(contacts, -1);
    TEST_ASSERT_NOT_NULL(contact);
    TEST_ASSERT_EQUAL_STRING("Negative", contact->name);
    TEST_ASSERT_TRUE(delete_contact(&contacts, -1));
    TEST_ASSERT_NOT_NULL(find_contact(contacts, 0));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_empty_lookup);
    RUN_TEST(test_insert_and_lookup);
    RUN_TEST(test_update_does_not_duplicate);
    RUN_TEST(test_delete_preserves_other_contacts);
    RUN_TEST(test_delete_missing_id_keeps_table);
    RUN_TEST(test_clear_is_repeatable_and_table_can_be_reused);
    RUN_TEST(test_name_is_copied);
    RUN_TEST(test_accepts_empty_and_maximum_names);
    RUN_TEST(test_rejects_oversized_names_without_changing_table);
    RUN_TEST(test_zero_and_negative_ids);
    return UNITY_END();
}
