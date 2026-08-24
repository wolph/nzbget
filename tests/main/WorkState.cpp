/*
 *  This file is part of nzbget. See <https://nzbget.com>.
 *
 *  Copyright (C) 2026 Denis <denis@nzbget.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "nzbget.h"

#include <boost/test/unit_test.hpp>
#include "WorkState.h"

BOOST_AUTO_TEST_SUITE(MainTest)

BOOST_AUTO_TEST_CASE(TimedSpeedLimitSetAndRevert)
{
	WorkState ws;
	ws.SetSpeedLimit(2048);
	ws.SetTimedSpeedLimit(1024, 1000);

	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 1024);
	BOOST_CHECK_EQUAL(ws.GetSpeedLimitResetTime(), 1000);
	BOOST_CHECK_EQUAL(ws.GetSpeedLimitRestoreValue(), 2048);

	BOOST_CHECK(!ws.CheckSpeedLimitRevert(999));
	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 1024);

	BOOST_CHECK(ws.CheckSpeedLimitRevert(1000));
	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 2048);
	BOOST_CHECK_EQUAL(ws.GetSpeedLimitResetTime(), 0);
	BOOST_CHECK(!ws.CheckSpeedLimitRevert(1000));
}

BOOST_AUTO_TEST_CASE(TimedSpeedLimitExtendKeepsOriginalRestoreValue)
{
	WorkState ws;
	ws.SetSpeedLimit(2048);
	ws.SetTimedSpeedLimit(1024, 1000);
	ws.SetTimedSpeedLimit(512, 2000);

	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 512);
	BOOST_CHECK_EQUAL(ws.GetSpeedLimitResetTime(), 2000);
	BOOST_CHECK_EQUAL(ws.GetSpeedLimitRestoreValue(), 2048);

	BOOST_CHECK(ws.CheckSpeedLimitRevert(2000));
	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 2048);
}

BOOST_AUTO_TEST_CASE(ManualSetSpeedLimitClearsTimer)
{
	WorkState ws;
	ws.SetTimedSpeedLimit(1024, 1000);
	ws.SetSpeedLimit(4096);

	BOOST_CHECK_EQUAL(ws.GetSpeedLimitResetTime(), 0);
	BOOST_CHECK(!ws.CheckSpeedLimitRevert(5000));
	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 4096);
}

BOOST_AUTO_TEST_CASE(RevertWithoutTimerDoesNothing)
{
	WorkState ws;
	ws.SetSpeedLimit(1024);
	BOOST_CHECK(!ws.CheckSpeedLimitRevert(123456));
	BOOST_CHECK_EQUAL(ws.GetSpeedLimit(), 1024);
}

BOOST_AUTO_TEST_SUITE_END()
