/*
 * HelloHackathon_Test.C
 *
 *  Created on: Aug 1, 2015
 *      Author: Administrator
 */

#include <iostream>
#include "gtest/gtest.h"
#include "CompareRecordApp.hh"

using namespace std;

TEST (isFile, Positive)
{
	EXPECT_TRUE  (isFile("HelloHackathon.sh"));
	EXPECT_FALSE (isFile("gtest-1.6.0"));
}

int
main (int argc, char* argv[])
{
	::testing::InitGoogleTest (&argc, argv);
	return RUN_ALL_TESTS();
}
