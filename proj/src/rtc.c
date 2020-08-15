#include <lcom/lcf.h>
#include <minix/drivers.h>
#include <math.h>
#include "rtc.h"
#include "macros.h"
#include "i8042.h"
#include "i8254.h"

extern uint32_t counter;

Time* makeTime() {
  Time* timer = (Time*)malloc(sizeof(Time));

  timer->hour = 0;
  timer->minute = 0;
  timer->second = 0;
  timer->day = 0;
  timer->month = 0;
  timer->year = 0;

  timer->num = makeNumbers();

  return timer;
}


void deleteTime(Time* time) { free(time); }

void getCurrentTime(Time* t) 
{
    uint32_t reg = 0;

    sys_outb(RTC_ADDR_REG, SECONDS);
    sys_inb(RTC_DATA_REG, &reg);
    t->second = bcdToDec(reg);

    sys_outb(RTC_ADDR_REG, MINUTES);
    sys_inb(RTC_DATA_REG, &reg);
    t->minute = bcdToDec(reg);

    sys_outb(RTC_ADDR_REG, HOURS);
    sys_inb(RTC_DATA_REG, &reg);
    t->hour = bcdToDec(reg);

    sys_outb(RTC_ADDR_REG, YEAR);
    sys_inb(RTC_DATA_REG, &reg);
    t->year = bcdToDec(reg);

    sys_outb(RTC_ADDR_REG, MONTH);
    sys_inb(RTC_DATA_REG, &reg);
    t->month = bcdToDec(reg);

    sys_outb(RTC_ADDR_REG, DAY);
    sys_inb(RTC_DATA_REG, &reg);
    t->day = bcdToDec(reg);
  
}

unsigned long bcdToDec(unsigned long bcd) {
  return (((bcd & 0xF0) >> 4) * 10) + (bcd & 0x0F);
}

Numbers *makeNumbers()
{
  Numbers *num = (Numbers*)malloc(sizeof(Numbers));
  
  Bitmap *zero = loadBitmap("/home/lcom/labs/proj/res/0.bmp");
  Bitmap *one = loadBitmap("/home/lcom/labs/proj/res/1.bmp");
  Bitmap *two = loadBitmap("/home/lcom/labs/proj/res/2.bmp");
  Bitmap *three = loadBitmap("/home/lcom/labs/proj/res/3.bmp");
  Bitmap *four = loadBitmap("/home/lcom/labs/proj/res/4.bmp");
  Bitmap *five = loadBitmap("/home/lcom/labs/proj/res/5.bmp");
  Bitmap *six = loadBitmap("/home/lcom/labs/proj/res/6.bmp");
  Bitmap *seven = loadBitmap("/home/lcom/labs/proj/res/7.bmp");
  Bitmap *eight = loadBitmap("/home/lcom/labs/proj/res/8.bmp");
  Bitmap *nine = loadBitmap("/home/lcom/labs/proj/res/9.bmp");
  Bitmap *pontos = loadBitmap("/home/lcom/labs/proj/res/pontos.bmp");
  Bitmap *barra = loadBitmap("/home/lcom/labs/proj/res/barra.bmp");
  Bitmap *score = loadBitmap("/home/lcom/labs/proj/res/score.bmp");

  num->zero = zero;
  num->one = one;
  num->two = two;
  num->three = three;
  num->four = four;
  num->five = five;
  num->six = six;
  num->seven = seven;
  num->eight = eight;
  num->nine = nine;
  num->pontos = pontos;
  num->barra = barra;
  num->score = score;
  
  return num;
}

void displayNumbers(Numbers *number, int x, int y, int num)
{
  switch(num)
  {
    case 0:
      drawBitmapTrans(number->zero,x,y, ALIGN_LEFT, 0);
      break;
    case 1:
      drawBitmapTrans(number->one,x,y, ALIGN_LEFT, 0);
      break;
    case 2:
      drawBitmapTrans(number->two,x,y, ALIGN_LEFT, 0);
      break;
    case 3:
      drawBitmapTrans(number->three,x,y, ALIGN_LEFT, 0);
      break;
    case 4:
      drawBitmapTrans(number->four,x,y, ALIGN_LEFT, 0);
      break;
    case 5:
      drawBitmapTrans(number->five,x,y, ALIGN_LEFT, 0);
      break;
    case 6:
      drawBitmapTrans(number->six,x,y, ALIGN_LEFT, 0);
      break;
    case 7:
      drawBitmapTrans(number->seven,x,y, ALIGN_LEFT, 0);
      break;
    case 8:
      drawBitmapTrans(number->eight,x,y, ALIGN_LEFT, 0);
      break;
    case 9:
      drawBitmapTrans(number->nine,x,y, ALIGN_LEFT, 0);
      break;
  }
}


void displayTime(Time *t, Numbers *num)
{
  displayNumbers(num, 0, DISPLAY_GAP_VER, (t->hour / 10));
  displayNumbers(num, DISPLAY_GAP_HOR, DISPLAY_GAP_VER, (t->hour % 10));
  drawBitmapTrans(num->pontos, DISPLAY_GAP_HOR * 2, DISPLAY_GAP_VER, ALIGN_LEFT, 0);
  displayNumbers(num, DISPLAY_GAP_HOR * 3, DISPLAY_GAP_VER, (t->minute / 10));
  displayNumbers(num, DISPLAY_GAP_HOR * 4, DISPLAY_GAP_VER, (t->minute % 10));
  drawBitmapTrans(num->pontos, DISPLAY_GAP_HOR * 5, DISPLAY_GAP_VER, ALIGN_LEFT, 0);
  displayNumbers(num, DISPLAY_GAP_HOR * 6, DISPLAY_GAP_VER, (t->second / 10));
  displayNumbers(num, DISPLAY_GAP_HOR * 7, DISPLAY_GAP_VER, (t->second % 10));
}


void displayDate(Time *t, Numbers *num)
{
  displayNumbers(num, 0, DISPLAY_GAP_VER_DATE, (t->day / 10));
  displayNumbers(num, DISPLAY_GAP_HOR, DISPLAY_GAP_VER_DATE, (t->day % 10));
  drawBitmapTrans(num->barra, DISPLAY_GAP_HOR * 2, DISPLAY_GAP_VER_DATE, ALIGN_LEFT, 0);
  displayNumbers(num, DISPLAY_GAP_HOR * 3, DISPLAY_GAP_VER_DATE, (t->month / 10));
  displayNumbers(num, DISPLAY_GAP_HOR * 4, DISPLAY_GAP_VER_DATE, (t->month % 10));
  drawBitmapTrans(num->barra, DISPLAY_GAP_HOR * 5, DISPLAY_GAP_VER_DATE, ALIGN_LEFT, 0);
  displayNumbers(num, DISPLAY_GAP_HOR * 6, DISPLAY_GAP_VER_DATE, (t->year / 10));
  displayNumbers(num, DISPLAY_GAP_HOR * 7, DISPLAY_GAP_VER_DATE, (t->year % 10));
}



void displayScore(Numbers *num, int score)
{
  drawBitmapTrans(num->score, SCORE_HOR_BEGIN, DISPLAY_GAP_VER, ALIGN_LEFT, 0);
  displayNumbers(num, SCORE_HOR_BEGIN + SCORE_BMP_SIZE, DISPLAY_GAP_VER, score / 1000);
  displayNumbers(num, SCORE_HOR_BEGIN + SCORE_BMP_SIZE + DISPLAY_GAP_HOR, DISPLAY_GAP_VER, (score / 100) % 10);
  displayNumbers(num, SCORE_HOR_BEGIN + SCORE_BMP_SIZE + DISPLAY_GAP_HOR * 2, DISPLAY_GAP_VER, (score / 10) % 10);
  displayNumbers(num, SCORE_HOR_BEGIN + SCORE_BMP_SIZE + DISPLAY_GAP_HOR * 3, DISPLAY_GAP_VER, score % 10);
}
