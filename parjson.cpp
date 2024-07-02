#ifndef PARJSON_CPP__
#define PARJSON_CPP__
#include "parjson.h"
#include <cassert>
#include <vector>
#endif // !1

//�ڴ�й¶���
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>
#endif

#define EXPECT(c,ch) \
do{\
    assert(*c->json == (ch));\
    c->json++;\
} while (0)

//�ַ���Χ�жϺ�
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

//��Ҫ������json
typedef struct {
    const char* json;
    std::vector<char> s;
} par_context;

static void par_whitespace(par_context* c) {
    const char* p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\r') {
        p++;
    }
    c->json = p;
}

#if 0
/*�������ϲ�Ϊpar_literal*/
//����null
static int par_null(par_context* c, par_value* v) {
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return PAR_INVALID_VALUE;
    c->json += 3;
    v->type = PAR_NULL;
    return PAR_OK;
}

//����true
static int par_true(par_context* c, par_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return PAR_INVALID_VALUE;
    c->json += 3;
    v->type = PAR_TRUE;
    return PAR_OK;
}

//����false
static int par_false(par_context* c, par_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return PAR_INVALID_VALUE;
    c->json += 4;
    v->type = PAR_FALSE;
    return PAR_OK;
}
#endif

//����bool���ͺ�nullֵ
static int par_literal(par_context* c, par_value* v, const char* cstr, par_type type) {
    int i = 0;
    EXPECT(c, cstr[0]);
    for (; cstr[i + 1]; i++) {
        if (c->json[i] != cstr[i + 1]) {
            return PAR_INVALID_VALUE;
        }
    }
    /*��c��ָ���ƶ���\0��*/
    c->json += i;
    v->type = type;
    return PAR_OK;
}

int par_get_boolean(const par_value* v) {
    assert(v != NULL && (v->type == PAR_TRUE || v->type == PAR_FALSE));
    return v->type == PAR_TRUE;
}

//��������
static int par_number(par_context* c, par_value* v) {
    const char* p = c->json;

    /*�������š���*/
    if (*p == '-') p++;

    /*������������*/
    if (*p == '0')p++;
    else {
        if (!ISDIGIT1TO9(*p)) return PAR_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }

    /*����С������*/
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p)) return PAR_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }

    /*����ָ������*/
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return PAR_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    
    /*������������Խ���쳣����*/
    try
    {
        double num_temp = std::stod(c->json);
        par_set_number(v, num_temp);
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

void par_set_number(par_value* v, double n) {
    par_free(v);
    v->num = n;
    v->type = PAR_NUMBER;
}

double par_get_number(const par_value* v)
{
    assert(v != NULL && v->type == PAR_NUMBER);
    return v->num;
}

static std::string par_string_pop(par_context* c) {
    size_t len = c->s.size();
    char* c_str = new char[len+1];
    char* p = c_str;

    for (int i = 0; i < len; i++) {
        *p = c->s[i];
        p++;
    }
    *p = '\0';
    
    std::string str = std::string(c_str);
    delete[] c_str;

    return str;
}

//�����ַ���
static int par_string(par_context* c, par_value* v) {
    const char* p = nullptr;
    EXPECT(c, '\"');

    p = c->json;

    for (;;) {
        char ch = *p++;
        switch (ch) {
        case '\"': {
            //������˫���ţ������ַ�������
            std::string chs = par_string_pop(c);
            par_set_string(v, chs);
            c->json = p;
            return PAR_OK;
            }
        case '\\': {
            switch (*p++) {
            case '\"': c->s.push_back('\"'); break;
            case '\\': c->s.push_back('\\'); break;
            case '/': c->s.push_back('/'); break;
            case 'b': c->s.push_back('\b'); break;
            case '\f': c->s.push_back('\f'); break;
            case '\n': c->s.push_back('\n'); break;
            case '\r': c->s.push_back('\r'); break;
            case '\t': c->s.push_back('\t'); break;
            default: return LEPT_PARSE_INVALID_STRING_ESCAPE;
            }
            break;
            }
        case '\0': return LEPT_PARSE_MISS_QUOTATION_MARK;
        default: {
            if ((unsigned char)ch < 0x20) return LEPT_PARSE_INVALID_STRING_CHAR;
            c->s.push_back(ch);
            }
        }
    }
}

void par_set_string(par_value* v, std::string s) {
    assert(v != NULL);
    par_free(v);
    v->str.c_str = (char*)s.c_str();
    v->str.len = s.size();
    v->type = PAR_STRING;
}

const char* par_get_string(const par_value* v) {
    std::cout << "���˰�3" << std::endl;
    assert(v != NULL && v->type == PAR_STRING);
    return v->str.c_str;
}

static int par_fun_value(par_context* c, par_value* v) {
    switch (*c->json) {
    case 'n': return par_literal(c, v, "null", PAR_NULL);
    case 't': return par_literal(c, v, "true", PAR_TRUE);
    case 'f': return par_literal(c, v, "false", PAR_FALSE);
    case '"': return par_string(c, v);
    case '\0': return PAR_EXPECT_VALUE;
    default: return par_number(c, v);
    }
}

int parser(par_value* v, const char* json)
{
    std::cout << "���˰�1" << std::endl;

    par_context c;
    int ret;

    assert(v != NULL);
    c.json = json;
    v->type = PAR_NULL;
    par_whitespace(&c);

    //�ж�json�Ƿ����
    if ((ret = par_fun_value(&c, v)) == PAR_OK) {
        par_whitespace(&c);
        if (*c.json != '\0')
            ret = PAR_ROOT_NOT_SINGULAR;
    }

    return ret;
}

par_type par_get_type(const par_value* v)
{
    std::cout << "���˰�2" << std::endl;

    assert(v != NULL);
    return v->type;
}

void par_free(par_value* v){
    assert(v != NULL);
    if (v->type == PAR_STRING) {
        if (v->str.len == 0) {
            v->str.c_str = nullptr;
        }
        else {
            delete[] v->str.c_str;
            v->str.len = 0;
        }
    }
    v->type = PAR_NULL;
}

