#include "parjson.h"
#include <cassert>

#define EXPECT(c,ch) \
do{\
    assert(*c->json == (ch));\
    c->json++;\
} while (0)

//字符范围判断宏
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

//需要解析的json
typedef struct {
    const char* json;
} par_context;

static void par_whitespace(par_context* c) {
    const char* p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\r') {
        p++;
    }
    c->json = p;
}

#if 0
/*这三个合并为par_literal*/
//解析null
static int par_null(par_context* c, par_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return PAR_INVALID_VALUE;
    c->json += 3;
    v->type = PAR_NULL;
    return PAR_OK;
}

//解析true
static int par_true(par_context* c, par_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return PAR_INVALID_VALUE;
    c->json += 3;
    v->type = PAR_TRUE;
    return PAR_OK;
}

//解析false
static int par_false(par_context* c, par_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return PAR_INVALID_VALUE;
    c->json += 4;
    v->type = PAR_FALSE;
    return PAR_OK;
}
#endif

//解析数字
static int par_number(par_context* c, par_value* v) {
    const char* p = c->json;

    /*——负号——*/
    if (*p == '-') p++;

    /*——整数——*/
    if (*p == '0')p++;
    else {
        if (!ISDIGIT1TO9(*p)) return PAR_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }

    /*——小数——*/
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return PAR_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }

    /*——指数——*/
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return PAR_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    
    /*——捕获数字越界异常——*/
    try
    {
        v->num = std::stod(c->json);
    }
    catch (const std::out_of_range&e)
    {
        v->type = PAR_NUMBER;
        c->json = p;
        return PAR_NUMBER_TOO_BIG;
    }

    v->type = PAR_NUMBER;
    c->json = p;

    return PAR_OK;
}


static int par_literal(par_context* c, par_value* v, const char* cstr, par_type type) {
    int i = 0;
    EXPECT(c, cstr[0]);
    for (; cstr[i + 1]; i++) {
        if (c->json[i] != cstr[i + 1]) {
            return PAR_INVALID_VALUE;
        }
    }
    /*将c内指针移动到\0处*/
    c->json += i;
    v->type = type;
    return PAR_OK;
}

static int par_fun_value(par_context* c, par_value* v) {
    switch (*c->json) {
    case 'n': return par_literal(c, v, "null", PAR_NULL);
    case 't': return par_literal(c, v, "true", PAR_TRUE);
    case 'f': return par_literal(c, v, "false", PAR_FALSE);
    case '\0': return PAR_EXPECT_VALUE;
    default: return par_number(c, v);
    }
}

int parser(par_value* v, const char* json)
{
    par_context c;
    int ret;

    assert(v != NULL);
    c.json = json;
    v->type = PAR_NULL;
    par_whitespace(&c);

    //判断json是否结束
    if ((ret = par_fun_value(&c, v)) == PAR_OK) {
        par_whitespace(&c);
        if (*c.json != '\0')
            ret = PAR_ROOT_NOT_SINGULAR;
    }

    return ret;
}

par_type par_get_type(const par_value* v)
{
    assert(v != NULL);
    return v->type;
}

double par_get_number(const par_value* v)
{
    assert(v != NULL && v->type == PAR_NUMBER);
    return v->num;
}

