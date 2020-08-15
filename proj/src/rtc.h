#ifndef __RTC_H
#define __RTC_H
#include "bitmap.h"

/** @defgroup RTC RTC
 * @{
 * Functions for RTC access
 */


typedef struct {
  Bitmap *zero;
  Bitmap *one;
  Bitmap *two;
  Bitmap *three;
  Bitmap *four;
  Bitmap *five;
  Bitmap *six;
  Bitmap *seven;
  Bitmap *eight;
  Bitmap *nine;
  Bitmap* pontos;
  Bitmap* barra;
  Bitmap* score;

} Numbers;


typedef struct {
  int hour;
  int minute;
  int second;

  int day;
  int month;
  int year;

  Numbers *num;

} Time;


Time* makeTime();

Numbers *makeNumbers();


void deleteTime(Time* time);


void getCurrentTime(Time* t);


unsigned long bcdToDec(unsigned long bcd);

void displayNumbers(Numbers *number, int x, int y, int num);

void displayTime(Time *t, Numbers *num);

void displayDate(Time *t, Numbers *num);

void displayScore(Numbers *num, int score);

/**@}*/
  
#endif 
