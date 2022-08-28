//
// Created by danpashin on 8/28/22.
//

#include "helper.h"

const int kPrintfMaxBuff = 256;

void SerialPrintf(const char *fmt, ...) {
  char buff[kPrintfMaxBuff];

  va_list pargs;
  va_start(pargs, fmt);
  vsnprintf(buff, kPrintfMaxBuff, fmt, pargs);
  va_end(pargs);

  Serial.println(buff);
}
