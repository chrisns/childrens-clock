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