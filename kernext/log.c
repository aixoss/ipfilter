#include <syslog.h>
#include <net/proto_uipc.h>
#include <stdio.h>
#include <stdarg.h>

int puts(const char *str)
{
  bsdlog(LOG_DEBUG | LOG_KERN, "%.1000s\n",str);
}

int printf(const char *fmt, ...)
{
  int rc;
  char buf[1024];
  va_list arg; 
  va_start(arg, fmt); 
	
  rc = vsnprintf_np(buf, 1024, fmt, arg);
  rc = puts(buf);
  va_end(arg);
  return rc;
}
