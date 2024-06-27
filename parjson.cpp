#include "parjson.h"
#include <cassert>

#define EXPECT(c,ch) \
do{\
    assert(*c->json == (ch));\
    c->json++;\
} while (0)

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

static int par_fun_value(par_context* c, par_value* v) {
    switch (*c->json) {
    case 'n': return par_null(c, v);
    case 't': return par_true(c, v);
    case 'f': return par_false(c, v);
    case '\0': return PAR_EXPECT_VALUE;
    default: return PAR_INVALID_VALUE;
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

double par_get_number(par_value* v, const char* json)
{

    return 0.0;
}

