// to test: ceedling test:all

#include "unity.h"
#include "childrensclock.h"


void setUp(void) {
}

void tearDown(void){
}

void testTimeAsDecimal(void) {
  TEST_ASSERT_EQUAL_FLOAT((float)TimeAsDecimal(1, 30), 1.5);
  TEST_ASSERT_EQUAL_FLOAT((float)TimeAsDecimal(2, 45), 2.75);
  TEST_ASSERT_EQUAL_FLOAT((float)TimeAsDecimal(2, 15), 2.25);
  TEST_ASSERT_EQUAL_FLOAT((float)TimeAsDecimal(5, 0), 5);
}

void testCalculateProgress(void) {
  // not between
  // TEST_ASSERT_EQUAL_FLOAT(100, (float)CalculateProgress(1, 5, 7));
  TEST_ASSERT_EQUAL_FLOAT(50, (float)CalculateProgress(6, 5, 7));
  TEST_ASSERT_EQUAL_FLOAT(0, (float)CalculateProgress(5, 5, 7));
  TEST_ASSERT_EQUAL_FLOAT(20, (float)CalculateProgress(2, 0, 10));

/*
bedtime = 18
waketime = 6
gotime = 8
*/

  // 12 = 100% go [green]
  TEST_ASSERT_EQUAL_FLOAT(100, (float)CalculateProgress(12, 18, 6));
  // 14 = 100% go [green]
  TEST_ASSERT_EQUAL_FLOAT(100, (float)CalculateProgress(14, 18, 6));
  // 18 = 0% bed [red]
  TEST_ASSERT_EQUAL_FLOAT(0, (float)CalculateProgress(18, 18, 6));
  // 21 = 25% bed [red]
  TEST_ASSERT_EQUAL_FLOAT(25, (float)CalculateProgress(21, 18, 6));
  // 03 = 75% bed [red]
  TEST_ASSERT_EQUAL_FLOAT(75, (float)CalculateProgress(3, 18, 6));
  // 6 = 100% wake [amber]
  TEST_ASSERT_EQUAL_FLOAT(100, (float)CalculateProgress(6, 18, 6));
  // 7 = 50% amber [amber]
  TEST_ASSERT_EQUAL_FLOAT(50, (float)CalculateProgress(7, 6, 8));
  // 8 = 100% go [green]
  TEST_ASSERT_EQUAL_FLOAT(100, (float)CalculateProgress(8, 6, 8));
}

void testProgressToDots(void) {
  TEST_ASSERT_EQUAL_INT(0, (int)ProgressToDots(100, 32));
  TEST_ASSERT_EQUAL_INT(8, (int)ProgressToDots(75, 32));
  TEST_ASSERT_EQUAL_INT(16, (int)ProgressToDots(50, 32));
  TEST_ASSERT_EQUAL_INT(24, (int)ProgressToDots(25, 32));
  TEST_ASSERT_EQUAL_INT(32, (int)ProgressToDots(0, 32));
  TEST_ASSERT_EQUAL_INT(32, (int)ProgressToDots(50, 64));
}

void testIsWeekday(void) {
  TEST_ASSERT_FALSE(IsWeekday(1));
  TEST_ASSERT_TRUE(IsWeekday(2));
  TEST_ASSERT_TRUE(IsWeekday(3));
  TEST_ASSERT_TRUE(IsWeekday(4));
  TEST_ASSERT_TRUE(IsWeekday(5));
  TEST_ASSERT_TRUE(IsWeekday(6));
  TEST_ASSERT_FALSE(IsWeekday(7));
}

void testGetState(void) {
  float weekday_go = 8; // morning of a weekend, so monday - friday morning
  float weekday_wake = 7; // morning of a weekend, so monday - friday morning
  float weekday_bedtime = 18; // evening before a weekday, so monday - thursday night

  float weekend_go = 9; // morning of a weekend, so saturday or sunday morning
  float weekend_wake = 8; // morning of a weekend, so saturday or sunday morning
  float weekend_bedtime = 19; // evening before a weekend, so friday and saturday night 19:00
  
  //monday 3pm
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(MON, 15, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //monday 3am
  TEST_ASSERT_EQUAL_INT(RED, GetState(MON, 3, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));
 
  //monday 11pm
  TEST_ASSERT_EQUAL_INT(RED, GetState(MON, 23, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //monday 5pm
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(MON, 17, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //monday 6pm
  TEST_ASSERT_EQUAL_INT(RED, GetState(MON, 18, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //friday 6pm
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(FRI, 18, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //saturday 7:30am
  TEST_ASSERT_EQUAL_INT(RED, GetState(SAT, 7.5, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //saturday 8am
  TEST_ASSERT_EQUAL_INT(AMBER, GetState(SAT, 8, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //saturday 9am
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(SAT, 9, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //sunday 6pm
  TEST_ASSERT_EQUAL_INT(RED, GetState(SUN, 18, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  //saturday 6pm
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(SAT, 18, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

}


void testGetProgress(void) {
  float weekday_go = 8;
  float weekday_wake = 7;
  float weekday_bedtime = 18;

  float weekend_go = 9;
  float weekend_wake = 8;
  float weekend_bedtime = 19;

  // Monday 3am (bedtime 18, wake 7 -> total 13h, 18 to 3 is 9h. 9/13 = ~69%)
  TEST_ASSERT_FLOAT_WITHIN(1.0, 69.2, GetProgress(MON, 3, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  // Friday 6pm (GREEN, 100% since out of bounds for amber/red)
  TEST_ASSERT_EQUAL_FLOAT(100.0, GetProgress(FRI, 18, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  // Saturday 7:30am (RED, bedtime 19, wake 8 -> total 13h, 19 to 7.5 is 12.5h. 12.5/13 = ~96%)
  TEST_ASSERT_FLOAT_WITHIN(1.0, 96.1, GetProgress(SAT, 7.5, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  // Saturday 8:30am (AMBER, wake 8, go 9 -> total 1h, 8 to 8.5 is 0.5h. 0.5/1 = 50%)
  TEST_ASSERT_EQUAL_FLOAT(50.0, GetProgress(SAT, 8.5, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  // Friday 3:00am (RED, yesterday Thursday, so startBedtime is weekday_bedtime 18. wake is weekday_wake 7. Total 13h. 18 to 3 is 9h. 9/13 = ~69%)
  TEST_ASSERT_FLOAT_WITHIN(1.0, 69.2, GetProgress(FRI, 3.0, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  // Sunday 8:00pm (RED, evening. today Sunday, tomorrow Monday. so endWaketime is weekday_wake 7. bedtime is weekday_bedtime 18. Total 13h. 18 to 20 is 2h. 2/13 = ~15%)
  TEST_ASSERT_FLOAT_WITHIN(1.0, 15.3, GetProgress(SUN, 20.0, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));

  // Sunday 3:00am (RED, yesterday Saturday, so startBedtime is weekend_bedtime 19. wake is weekend_wake 8. Total 13h. 19 to 3 is 8h. 8/13 = ~61%)
  TEST_ASSERT_FLOAT_WITHIN(1.0, 61.5, GetProgress(SUN, 3.0, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime));
}


void testShiftLater(void) {
  // plain shifts
  TEST_ASSERT_EQUAL_FLOAT(19, (float)ShiftLater(TimeAsDecimal(18, 0), 60));
  TEST_ASSERT_EQUAL_FLOAT(8.25, (float)ShiftLater(TimeAsDecimal(7, 30), 45));
  TEST_ASSERT_EQUAL_FLOAT(18, (float)ShiftLater(TimeAsDecimal(18, 0), 0));

  // wrapping past midnight stays a valid clock time
  TEST_ASSERT_EQUAL_FLOAT(0.5, (float)ShiftLater(TimeAsDecimal(23, 30), 60));
  TEST_ASSERT_EQUAL_FLOAT(2, (float)ShiftLater(TimeAsDecimal(23, 0), 180));
  TEST_ASSERT_EQUAL_FLOAT(0, (float)ShiftLater(TimeAsDecimal(23, 0), 60));
}

void testScheduleTime(void) {
  // holiday mode off: the stored time is returned untouched, offset ignored
  TEST_ASSERT_EQUAL_FLOAT(18, (float)ScheduleTime(18, 0, 60, false));
  TEST_ASSERT_EQUAL_FLOAT(7.5, (float)ScheduleTime(7, 30, 120, false));

  // holiday mode on
  TEST_ASSERT_EQUAL_FLOAT(19, (float)ScheduleTime(18, 0, 60, true));
  TEST_ASSERT_EQUAL_FLOAT(18, (float)ScheduleTime(18, 0, 0, true));

  // an unset/NaN offset must not poison the time
  TEST_ASSERT_EQUAL_FLOAT(18, (float)ScheduleTime(18, 0, NAN, true));
}

void testHolidayModeChangesState(void) {
  // normal schedule: wake 07:00, go 08:00, bed 18:00 (weekday and weekend alike)
  float go = 8, wake = 7, bed = 18;

  // with a 60 minute offset on every phase
  float h_go = ScheduleTime(8, 0, 60, true);
  float h_wake = ScheduleTime(7, 0, 60, true);
  float h_bed = ScheduleTime(18, 0, 60, true);

  // 07:30 on a Monday: normally time to get up, in holiday mode still a lie-in
  TEST_ASSERT_EQUAL_INT(AMBER, GetState(MON, 7.5, go, wake, bed, go, wake, bed));
  TEST_ASSERT_EQUAL_INT(RED, GetState(MON, 7.5, h_go, h_wake, h_bed, go, wake, bed));

  // 08:30: holiday mode has caught up to the waking period
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(MON, 8.5, go, wake, bed, go, wake, bed));
  TEST_ASSERT_EQUAL_INT(AMBER, GetState(MON, 8.5, h_go, h_wake, h_bed, go, wake, bed));

  // 18:30: normally bedtime, in holiday mode still up
  TEST_ASSERT_EQUAL_INT(RED, GetState(MON, 18.5, go, wake, bed, go, wake, bed));
  TEST_ASSERT_EQUAL_INT(GREEN, GetState(MON, 18.5, h_go, h_wake, h_bed, go, wake, bed));

  // offsets of zero must leave the state machine exactly as it was
  TEST_ASSERT_EQUAL_INT(
    GetState(MON, 7.5, go, wake, bed, go, wake, bed),
    GetState(MON, 7.5,
             ScheduleTime(8, 0, 0, true),
             ScheduleTime(7, 0, 0, true),
             ScheduleTime(18, 0, 0, true), go, wake, bed));
}


// void testGetColor(void) {
  // TEST_ASSERT_EQUAL_STRING("GREEN", GetColor(50, 64));

// }

/*
its after go time = green + hidden progress bar
its between wake up and go time = amber + new progress bar
its after bedtime = red + progress bar


bedtime = 18
waketime = 6
gotime = 8


*/