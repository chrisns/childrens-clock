#include <math.h>
#include <stdbool.h>


float TimeAsDecimal(float hour, float minute) {
  return hour + (0.0166666666667 * minute);
}

float CalculateProgress(float currentHour, float startHour, float endHour) {
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


int ProgressToDots(float progressPercent, int totalDots) {
  // Calculate the number of unlit dots using ceil for rounding up any fractional part
  int unlitDots = (int) ceil((progressPercent / 100.0) * totalDots);
  // Calculate the number of lit dots by subtracting unlit dots from total
  return unlitDots < totalDots ? totalDots - unlitDots : 0;
}

bool IsWeekday(int dayOfWeek) {
  if (dayOfWeek == 1 || dayOfWeek == 7) {
    return false;
  }
  else {
    return true;
  }
}

const int GREEN = 0;
const int AMBER = 1;
const int RED = 2;

const int SUN = 1;
const int MON = 2;
const int TUE = 3;
const int WED = 4;
const int THU = 5;
const int FRI = 6;
const int SAT = 7;

int GetState(int dayOfWeek, float currentTime, float weekday_go, float weekday_wake, float weekday_bedtime, float weekend_go, float weekend_wake, float weekend_bedtime) {
  // Determine current day type: weekday or weekend
  int isWeekend = (dayOfWeek == SAT || dayOfWeek == SUN);
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