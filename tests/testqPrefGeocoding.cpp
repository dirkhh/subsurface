// SPDX-License-Identifier: GPL-2.0
#include "testqPrefGeocoding.h"

#include "core/pref.h"
#include "core/qthelper.h"
#include "core/settings/qPrefGeocoding.h"

#include <QTest>

void TestQPrefGeocoding::initTestCase()
{
	QCoreApplication::setOrganizationName("Subsurface");
	QCoreApplication::setOrganizationDomain("subsurface.hohndel.org");
	QCoreApplication::setApplicationName("SubsurfaceTestQPrefGeocoding");
}

void TestQPrefGeocoding::test_struct_get()
{
	// Test struct pref -> get func.

	auto tst = qPrefGeocoding::instance();

	prefs.geocoding.category[0] = TC_NONE;
	prefs.geocoding.category[1] = TC_OCEAN;
	prefs.geocoding.category[2] = TC_ADMIN_L1;

	QCOMPARE(tst->first_taxonomy_category(), prefs.geocoding.category[0]);
	QCOMPARE(tst->second_taxonomy_category(), prefs.geocoding.category[1]);
	QCOMPARE(tst->third_taxonomy_category(), prefs.geocoding.category[2]);
}

void TestQPrefGeocoding::test_set_struct()
{
	// Test set func -> struct pref

	auto tst = qPrefGeocoding::instance();

	tst->set_first_taxonomy_category(TC_COUNTRY);
	tst->set_second_taxonomy_category(TC_ADMIN_L1);
	tst->set_third_taxonomy_category(TC_ADMIN_L2);

	QCOMPARE(prefs.geocoding.category[0], TC_COUNTRY);
	QCOMPARE(prefs.geocoding.category[1], TC_ADMIN_L1);
	QCOMPARE(prefs.geocoding.category[2], TC_ADMIN_L2);
}

void TestQPrefGeocoding::test_set_load_struct()
{
	// test set func -> load -> struct pref

	auto tst = qPrefGeocoding::instance();

	tst->set_first_taxonomy_category(TC_LOCALNAME);
	tst->set_second_taxonomy_category(TC_ADMIN_L3);
	tst->set_third_taxonomy_category(TC_NR_CATEGORIES);

	prefs.geocoding.category[0] = TC_NONE;
	prefs.geocoding.category[1] = TC_OCEAN;
	prefs.geocoding.category[2] = TC_ADMIN_L1;

	tst->load();
	QCOMPARE(prefs.geocoding.category[0], TC_LOCALNAME);
	QCOMPARE(prefs.geocoding.category[1], TC_ADMIN_L3);
	QCOMPARE(prefs.geocoding.category[2], TC_NR_CATEGORIES);
}

void TestQPrefGeocoding::test_struct_disk()
{
	// test struct prefs -> disk

	auto tst = qPrefGeocoding::instance();

	prefs.geocoding.category[0] = TC_NONE;
	prefs.geocoding.category[1] = TC_OCEAN;
	prefs.geocoding.category[2] = TC_ADMIN_L1;

	tst->sync();
	prefs.geocoding.category[0] = TC_ADMIN_L2;
	prefs.geocoding.category[1] = TC_ADMIN_L2;
	prefs.geocoding.category[2] = TC_ADMIN_L2;

	tst->load();
	QCOMPARE(prefs.geocoding.category[0], TC_NONE);
	QCOMPARE(prefs.geocoding.category[1], TC_OCEAN);
	QCOMPARE(prefs.geocoding.category[2], TC_ADMIN_L1);
}

void TestQPrefGeocoding::test_multiple()
{
	// test multiple instances have the same information

	prefs.geocoding.category[0] = TC_NONE;
	auto tst_direct = new qPrefGeocoding;

	prefs.geocoding.category[1] = TC_OCEAN;
	auto tst = qPrefGeocoding::instance();

	QCOMPARE(tst->first_taxonomy_category(), tst_direct->first_taxonomy_category());
	QCOMPARE(tst->second_taxonomy_category(), tst_direct->second_taxonomy_category());
	QCOMPARE(tst->first_taxonomy_category(), TC_NONE);
	QCOMPARE(tst->second_taxonomy_category(), TC_OCEAN);
}

#define TEST(METHOD, VALUE)      \
	QCOMPARE(METHOD, VALUE); \
	geo->sync();           \
	geo->load();           \
	QCOMPARE(METHOD, VALUE);

void TestQPrefGeocoding::test_oldPreferences()
{
	auto geo = qPrefGeocoding::instance();
	geo->set_first_taxonomy_category(TC_NONE);
	geo->set_second_taxonomy_category(TC_OCEAN);
	geo->set_third_taxonomy_category(TC_COUNTRY);

	TEST(geo->first_taxonomy_category(), TC_NONE);
	TEST(geo->second_taxonomy_category(), TC_OCEAN);
	TEST(geo->third_taxonomy_category(), TC_COUNTRY);

	geo->set_first_taxonomy_category(TC_OCEAN);
	geo->set_second_taxonomy_category(TC_COUNTRY);
	geo->set_third_taxonomy_category(TC_NONE);

	TEST(geo->first_taxonomy_category(), TC_OCEAN);
	TEST(geo->second_taxonomy_category(), TC_COUNTRY);
	TEST(geo->third_taxonomy_category(), TC_NONE);
}

QTEST_MAIN(TestQPrefGeocoding)
