#define TOKEN_SIZE 32
typedef struct token
{
    int type;
    char str[TOKEN_SIZE];
} Token;

static Token tokens[32] __attribute__((used)) = {};
uint32_t eval(uint32_t p, uint32_t q);