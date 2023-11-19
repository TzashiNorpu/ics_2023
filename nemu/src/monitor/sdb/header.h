#define TOKEN_SIZE 32
typedef struct token
{
    int type;
    char str[TOKEN_SIZE];
} Token;

static Token tokens[32] __attribute__((used)) = {};