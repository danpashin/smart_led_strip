
#define SERIAL_PRINTF_MAX_BUFF      256

void SerialPrintf(const char *fmt, ...) {
  char buff[SERIAL_PRINTF_MAX_BUFF];

  va_list pargs;
  va_start(pargs, fmt);
  vsnprintf(buff, SERIAL_PRINTF_MAX_BUFF, fmt, pargs);
  va_end(pargs);

  Serial.println(buff);
}
