#pragma once
#include "type.h"
BNFToken *tokenize(char *p);
BNFToken *new_token(TokenKind kind, char *start, char *end);