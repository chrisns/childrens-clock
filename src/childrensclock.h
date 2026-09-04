#include <math.h>
#include <stdbool.h>


static float TimeAsDecimal(float hour, float minute) {
  return hour + (0.0166666666667 * minute);
}

// Shift a decimal time later by `minutes`, wrapping past midnight so a bedtime
// pushed beyond 00:00 stays a valid clock time (CalculateProgress and GetProgress
// already cope with ranges that wrap).
static float ShiftLater(float decimalTime, float minutes) {
  float shifted = decimalTime + (minutes / 60.0);
  while (shifted >= 24.0) {
    shifted -= 24.0;
  }
  return shifted;
}

// One scheduled time, with the holiday offset applied if holiday mode is on.
// The stored wake/go/bedtime values are NEVER rewritten - the offset is applied
// here, at evaluation time - so turning holiday mode off puts the normal
// schedule straight back with nothing to undo and no chance of drift.
static float ScheduleTime(int hour, int minute, float offsetMinutes, bool holiday) {
  float t = TimeAsDecimal(hour, minute);
  if (!holiday || isnan(offsetMinutes)) {
    return t;
  }
  return ShiftLater(t, offsetMinutes);
}

static float CalculateProgress(float currentHour, float startHour, float endHour) {
  float progress = 0.0;
  float totalHours;

  if (startHour <= endHour) {
    // Normal range, not wrapping midnight
    if (currentHour < startHour || currentHour > endHour) {
        return 100; // Out of range
    }
    totalHours = endHour - startHour;
    progress = (float)(currentHour - startHour) / totalHours;
  } else {
    // Range wraps midnight
    if (currentHour >= startHour || currentHour < endHour) {
      if (currentHour >= startHour) {
        // Current hour is after start hour but before midnight
        totalHours = 24 - startHour + endHour;
        progress = (float)(currentHour - startHour) / totalHours;
      } else {
        // Current hour is after midnight but before end hour
        totalHours = 24 - startHour + endHour;
        progress = (float)(24 - startHour + currentHour) / totalHours;
      }
    } else {
      return 100; // Out of range
    }
  }

  return progress * 100; // Return as percentage
}


static int ProgressToDots(float progressPercent, int totalDots) {
  // Calculate the number of unlit dots using ceil for rounding up any fractional part
  int unlitDots = (int) ceil((progressPercent / 100.0) * totalDots);
  // Calculate the number of lit dots by subtracting unlit dots from total
  return unlitDots < totalDots ? totalDots - unlitDots : 0;
}

static bool IsWeekday(int dayOfWeek) {
  if (dayOfWeek == 1 || dayOfWeek == 7) {
    return false;
  }
  else {
    return true;
  }
}

static const int GREEN = 0;
static const int AMBER = 1;
static const int RED = 2;

static const int SUN = 1;
static const int MON = 2;
static const int TUE = 3;
static const int WED = 4;
static const int THU = 5;
static const int FRI = 6;
static const int SAT = 7;

static int GetState(int dayOfWeek, float currentTime, float weekday_go, float weekday_wake, float weekday_bedtime, float weekend_go, float weekend_wake, float weekend_bedtime) {
  // Determine current day type: weekday or weekend
  int isWeekend = !IsWeekday(dayOfWeek);
  int isWeekendNight = (dayOfWeek == FRI || dayOfWeek == SAT);

  // Determine time thresholds based on the day type
  float goTime = isWeekend ? weekend_go : weekday_go;
  float wakeTime = isWeekend ? weekend_wake : weekday_wake;
  float bedtime = isWeekendNight ? weekend_bedtime : weekday_bedtime;

  // Determine the state based on the current time and thresholds
  if (currentTime >= wakeTime && currentTime < goTime) {
    return AMBER;  // Waking period
  } else if (currentTime >= goTime && currentTime < bedtime) {
    return GREEN;  // Active period
  } else {
    return RED;    // Resting period
  }
}

static float GetProgress(int dayOfWeek, float currentTime, float weekday_go, float weekday_wake, float weekday_bedtime, float weekend_go, float weekend_wake, float weekend_bedtime) {
  int currentstate = GetState(dayOfWeek, currentTime, weekday_go, weekday_wake, weekday_bedtime, weekend_go, weekend_wake, weekend_bedtime);

  if (currentstate == AMBER) {
    int isWeekend = !IsWeekday(dayOfWeek);
    float wakeTime = isWeekend ? weekend_wake : weekday_wake;
    float goTime = isWeekend ? weekend_go : weekday_go;
    return CalculateProgress(currentTime, wakeTime, goTime);
  } else {
    // We are in the RED state. This can be before wake (morning) or after bed (evening).
    int isWeekend = !IsWeekday(dayOfWeek);
    float wakeTime = isWeekend ? weekend_wake : weekday_wake;
    int isWeekendNight = (dayOfWeek == FRI || dayOfWeek == SAT);
    float bedtime = isWeekendNight ? weekend_bedtime : weekday_bedtime;

    float startBedtime;
    float endWaketime;

    if (currentTime < wakeTime) {
      // It's morning. We woke up from yesterday's bedtime.
      int yesterday = (dayOfWeek == SUN) ? SAT : (dayOfWeek - 1);
      int isYesterdayWeekendNight = (yesterday == FRI || yesterday == SAT);
      startBedtime = isYesterdayWeekendNight ? weekend_bedtime : weekday_bedtime;
      endWaketime = wakeTime; // Today's wake time
    } else {
      // It's evening. We will wake up to tomorrow's wake time.
      int tomorrow = (dayOfWeek == SAT) ? SUN : (dayOfWeek + 1);
      int isTomorrowWeekend = !IsWeekday(tomorrow);
      startBedtime = bedtime; // Today's bedtime
      endWaketime = isTomorrowWeekend ? weekend_wake : weekday_wake;
    }

    return CalculateProgress(currentTime, startBedtime, endWaketime);
  }
}