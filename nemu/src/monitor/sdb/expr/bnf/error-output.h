#pragma once
#include <stdarg.h>
#include "type.h"
void error(char *fmt, ...);
void verrorAt(char *loc, char *fmt, va_list VA);
void errorAt(char *loc, char *fmt, ...);
void errorTok(BNFToken *tok, char *fmt, ...);
