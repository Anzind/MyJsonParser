#ifndef PARJSON_CPP__
#define PARJSON_CPP__
#include "parjson.h"
#include <cassert>
#include <vector>
#endif // !1

//内存泄露检测
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>
#endif

#if 1
/*统一声明*/
typedef struct par_context par_context;

static void par_whitespace(par_context* c);

static int par_literal(par_context* c, par_value* v, const char* cstr, par_type type);
int par_get_boolean(const par_value* v);

static int par_number(par_context* c, par_value* v);
void par_set_number(par_value* v, double n);
double par_get_number(const par_value* v);

static char* par_string_pop(par_context* c);
static const char* par_hex4(const char* p, unsigned* u);
static void par_encode_utf8(par_context* c, unsigned u);
static int par_string_raw(par_context* c, char** str);
static int par_string(par_context* c, par_value* v);
void par_set_string(par_value* v, char* s);
const char* par_get_string(const par_value* v);

static par_value* par_array_pop(par_context* c, size_t size);
size_t par_get_array_size(const par_value* v);
par_value* par_get_array_element(const par_value* v, size_t index);
static int par_array(par_context* c, par_value* v);

size_t par_get_object_size(const par_value* v);
const char* par_get_object_key(const par_value* v, size_t index);
size_t par_get_object_key_length(const par_value* v, size_t index);
par_value* par_get_object_value(const par_value* v, size_t index);
void par_set_mem_key(par_member* m, char* s);
par_member* par_object_pop(par_context* c, size_t size);
static int par_object(par_context* c, par_value* v);

static int par_fun_value(par_context* c, par_value* v);
int parser(par_value* v, const char* json);
par_type par_get_type(const par_value* v);
void par_free(par_value* v);
#endif

#define EXPECT(c,ch) \
do{\
    assert(*c->json == (ch));\
    c->json++;\
} while (0)

//字符范围判断宏
#define ISDIGIT(ch)         ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch)     ((ch) >= '1' && (ch) <= '9')

//需要解析的json
struct par_context {
	const char* json;
	/*辅助字符串解析的列表*/
	std::vector<char> s;
	/*辅助数组解析的列表*/
	std::vector<par_value> sv;
	/*辅助对象解析的列表*/
	std::vector<par_member> sm;
};

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

//解析bool类型和null值
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

int par_get_boolean(const par_value* v) {
	assert(v != NULL && (v->type == PAR_TRUE || v->type == PAR_FALSE));
	return v->type == PAR_TRUE;
}

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
		double num_temp = std::stod(c->json);
		par_set_number(v, num_temp);
	}
	catch (const std::out_of_range& e)
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

double par_get_number(const par_value* v) {
	assert(v != NULL && v->type == PAR_NUMBER);
	return v->num;
}

static char* par_string_pop(par_context* c) {
	size_t len = c->s.size();
	char* c_str = new char[len + 1];
	char* p = c_str;

	for (int i = 0; i < len; i++) {
		*p++ = c->s[i];
	}
	*p = '\0';
	c->s.clear();

	return c_str;
}

//读4位16进制数字
static const char* par_hex4(const char* p, unsigned* u) {
#if 1
	/*所谓老办法就是没办法的办法，最基础的办法*/
	int i = 0;
	*u = 0;
	for (; i < 4; i++) {
		char ch = *p++;
		*u <<= 4;
		if (ch >= '0' && ch <= '9')
			*u |= ch - '0';
		else if (ch >= 'A' && ch <= 'F')
			*u |= ch - ('A' - 10);
		else if (ch >= 'a' && ch <= 'f')
			*u |= ch - ('a' - 10);
		else return NULL;
	}
	return p;
#endif

#if 0
	/*还有问题还需要修改*/
	/*C++11新方案，但是在用函数之前得手动检测数字格式是否正确*/
	std::string num_str = std::string(p).substr(0, 4);
	for (int i = 0; i < 4; i++) {
		if ((num_str[i] >= 'a' && num_str[i] <= 'z') || (num_str[i] >= 'A' && num_str[i] <= 'Z') || (num_str[i] >= '0' && num_str[i] <= '9')) p++;
		else return NULL;
	}

	*u = (unsigned)std::stoi(num_str.c_str(), 0, 16);
	return p;
#endif

}

static void par_encode_utf8(par_context* c, unsigned u) {
	if (u <= 0x7f)
		c->s.push_back(u);
	else if (u <= 0x7ff) {
		c->s.push_back(0xc0 | ((u >> 6) & 0xff));
		c->s.push_back(0x80 | (u & 0x3f));
	}
	else if (u <= 0xffff) {
		c->s.push_back(0xe0 | ((u >> 12) & 0xff));
		c->s.push_back(0x80 | ((u >> 6) & 0x3f));
		c->s.push_back(0x80 | (u & 0x3f));
	}
	else {
		assert(u <= 0x10ffff);
		c->s.push_back(0xf0 | ((u >> 18) & 0xff));
		c->s.push_back(0x80 | ((u >> 12) & 0x3f));
		c->s.push_back(0x80 | ((u >> 6) & 0x3f));
		c->s.push_back(0x80 | (u & 0x3f));
	}
}

//解析字符串
static int par_string_raw(par_context* c, char** str) {
	unsigned u, u2;
	const char* p = nullptr;
	EXPECT(c, '\"');

	p = c->json;
	for (;;) {
		char ch = *p++;
		switch (ch) {
		case '\"': {
			//碰到了双引号，代表字符串结束
			*str = par_string_pop(c);
			//par_set_string(v, chs);
			c->json = p;
			return PAR_OK;
		}
		case '\\': {
			switch (*p++) {
			case '"': c->s.push_back('\"'); break;
			case '\\': c->s.push_back('\\'); break;
			case '/': c->s.push_back('/'); break;
			case 'b': c->s.push_back('\b'); break;
			case 'f': c->s.push_back('\f'); break;
			case 'n': c->s.push_back('\n'); break;
			case 'r': c->s.push_back('\r'); break;
			case 't': c->s.push_back('\t'); break;
			case 'u': {
				p = par_hex4(p, &u);
				if (!p) return PAR_INVALID_UNICODE_HEX;
				if (u >= 0xd800 && u <= 0xdbff) {
					if (*p++ != '\\')
						return PAR_INVALID_UNICODE_SURROGATE;
					if (*p++ != 'u')
						return PAR_INVALID_UNICODE_SURROGATE;
					p = par_hex4(p, &u2);
					if (!p) return PAR_INVALID_UNICODE_HEX;
					if (u2 < 0xdc00 || u2 > 0xdfff)
						return PAR_INVALID_UNICODE_SURROGATE;
					u = (((u - 0xd800) << 10) | (u2 - 0xdc00)) + 0x10000;
				}
				par_encode_utf8(c, u);
				break;
			}
			default: return PAR_INVALID_STRING_ESCAPE;
			}
			break;
		}
		case '\0': return PAR_MISS_QUOTATION_MARK;
		default: {
			if ((unsigned char)ch < 0x20) return PAR_INVALID_STRING_CHAR;
			c->s.push_back(ch);
		}
		}
	}
}

static int par_string(par_context* c, par_value* v) {
#if 0
	/*该部分代码已重构，par_string_raw获取字符串*/
	unsigned u, u2;
	const char* p = nullptr;
	EXPECT(c, '\"');

	p = c->json;

	for (;;) {
		char ch = *p++;
		switch (ch) {
		case '\"': {
			//碰到了双引号，代表字符串结束
			char* chs = par_string_pop(c);
			par_set_string(v, chs);
			c->json = p;
			return PAR_OK;
		}
		case '\\': {
			switch (*p++) {
			case '"': c->s.push_back('\"'); break;
			case '\\': c->s.push_back('\\'); break;
			case '/': c->s.push_back('/'); break;
			case 'b': c->s.push_back('\b'); break;
			case 'f': c->s.push_back('\f'); break;
			case 'n': c->s.push_back('\n'); break;
			case 'r': c->s.push_back('\r'); break;
			case 't': c->s.push_back('\t'); break;
			case 'u': {
				p = par_hex4(p, &u);
				if (!p) return PAR_INVALID_UNICODE_HEX;
				if (u >= 0xd800 && u <= 0xdbff) {
					if (*p++ != '\\')
						return PAR_INVALID_UNICODE_SURROGATE;
					if (*p++ != 'u')
						return PAR_INVALID_UNICODE_SURROGATE;
					p = par_hex4(p, &u2);
					if (!p) return PAR_INVALID_UNICODE_HEX;
					if (u2 < 0xdc00 || u2 > 0xdfff)
						return PAR_INVALID_UNICODE_SURROGATE;
					u = (((u - 0xd800) << 10) | (u2 - 0xdc00)) + 0x10000;
				}
				par_encode_utf8(c, u);
				break;
			}
			default: return PAR_INVALID_STRING_ESCAPE;
			}
			break;
		}
		case '\0': return PAR_MISS_QUOTATION_MARK;
		default: {
			if ((unsigned char)ch < 0x20) return PAR_INVALID_STRING_CHAR;
			c->s.push_back(ch);
		}
		}
	}
#endif

	int ret;
	char* chs = nullptr;
	
	ret = par_string_raw(c, &chs);
	if (ret == PAR_OK) {
		par_set_string(v, chs);
	}
	return ret;
}

void par_set_string(par_value* v, char* s) {
	assert(v != NULL);
	par_free(v);
	v->str.c_str = s;
	v->str.len = std::string(s).size();
	v->type = PAR_STRING;
}

const char* par_get_string(const par_value* v) {
	assert(v != NULL && v->type == PAR_STRING);
	return v->str.c_str;
}

static par_value* par_array_pop(par_context* c, size_t size) {
	par_value* temp = new par_value[size];
	par_value* p = temp;
	size_t i = c->sv.size() - size;

	for (; i < c->sv.size(); i++) {
		*p++ = c->sv[i];
	}
	c->sv.resize(c->sv.size() - size);

	return temp;
}

size_t par_get_array_size(const par_value* v) {
	assert(v != NULL && v->type == PAR_ARRAY);
	return v->arr.size;
}

par_value* par_get_array_element(const par_value* v, size_t index) {
	assert(v != NULL && v->type == PAR_ARRAY);
	assert(index < v->arr.size);
	return &v->arr.elem[index];
}

//解析数组
static int par_array(par_context* c, par_value* v) {
	size_t size = 0;
	int ret;
	EXPECT(c, '[');

	par_whitespace(c);
	if (*c->json == ']') {
		c->json++;
		v->type = PAR_ARRAY;
		v->arr.size = 0;
		v->arr.elem = nullptr;
		return PAR_OK;
	}
	for (;;) {
		par_value e;
		e.type = PAR_NULL;

		ret = par_fun_value(c, &e);
		if (ret != PAR_OK) return ret;
		c->sv.push_back(e);
		size++;

		par_whitespace(c);
		if (*c->json == ',') c->json++;
		else if (*c->json == ']') {
			c->json++;
			v->type = PAR_ARRAY;
			v->arr.size = size;
			v->arr.elem = par_array_pop(c, v->arr.size);
			return PAR_OK;
		}
		else
			return PAR_MISS_COMMA_OR_SQUARE_BRACKET;

		par_whitespace(c);
	}

}

size_t par_get_object_size(const par_value* v)
{
	assert(v != NULL && v->type == PAR_OBJECT);
	return v->obj.size;
}

const char* par_get_object_key(const par_value* v, size_t index) {
	assert(v != NULL && v->type == PAR_OBJECT && index < v->obj.size);
	return v->obj.mem[index].key;
}

size_t par_get_object_key_length(const par_value* v, size_t index) {
	assert(v != NULL && v->type == PAR_OBJECT && index < v->obj.size);
	return v->obj.mem[index].key_len;
}

par_value* par_get_object_value(const par_value* v, size_t index) {
	assert(v != NULL && v->type == PAR_OBJECT && index < v->obj.size);
	return &v->obj.mem[index].value;
}

void par_set_mem_key(par_member* m, char* s) {
	assert(m != NULL);
	m->key = s;
	m->key_len = std::string(s).size();
}

par_member* par_object_pop(par_context* c, size_t size) {
	par_member* temp = new par_member[size];
	par_member* p = temp;
	size_t i = c->sm.size() - size;

	for (; i < c->sm.size(); i++) {
		*p++ = c->sm[i];
	}
	c->sm.resize(c->sm.size() - size);

	return temp;
}

static int par_object(par_context* c, par_value* v) {
	size_t size;
	par_member m;
	int ret;
	EXPECT(c, '{');
	par_whitespace(c);

	if (*c->json == '}') {
		c->json++;
		v->type = PAR_OBJECT;
		v->obj.mem = nullptr;
		v->obj.size = 0;
		return PAR_OK;
	}
	m.key = nullptr;
	size = 0;

	for (;;) {
		m.value.type = PAR_NULL;

		/*解析对象成员key字符串*/
		if (*c->json != '\"') return PAR_MISS_KEY;
		char* chs = nullptr;
		ret = par_string_raw(c, &chs);
		if (ret != PAR_OK) break;
		par_set_mem_key(&m, chs);
		par_whitespace(c);

		/*检测冒号*/
		if (*c->json == ':') {
			c->json++;
			par_whitespace(c);
		}
		else return PAR_MISS_COLON;

		/*解析对象成员值*/
		ret = par_fun_value(c, &m.value);
		if (ret != PAR_OK) break;

		c->sm.push_back(m);
		size++;
		m.key = nullptr;

		par_whitespace(c);
		switch (*c->json){
		case ',': {
			/*准备读该对象下一个成员*/
			c->json++;
			par_whitespace(c);
			break;
		}
		case '}': {
			/*对象结束读取*/
			c->json++;
			v->obj.mem = par_object_pop(c, size);
			v->obj.size = size;
			v->type = PAR_OBJECT;
			return PAR_OK;
		}
		default: return PAR_MISS_COMMA_OR_CURLY_BRACKET;
		}
	}

	return ret;
}

static int par_fun_value(par_context* c, par_value* v) {
	switch (*c->json) {
	case 'n': return par_literal(c, v, "null", PAR_NULL);
	case 't': return par_literal(c, v, "true", PAR_TRUE);
	case 'f': return par_literal(c, v, "false", PAR_FALSE);
	case '"': return par_string(c, v);
	case '[': return par_array(c, v);
	case '{': return par_object(c, v);
	case '\0': return PAR_EXPECT_VALUE;
	default: return par_number(c, v);
	}
}

int parser(par_value* v, const char* json) {
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

par_type par_get_type(const par_value* v) {
	assert(v != NULL);
	return v->type;
}

void par_mem_free(par_member* m) {
	delete[] m->key;
	m->key = nullptr;
	m->key_len = 0;

	par_free(&m->value);
}

void par_free(par_value* v) {
	assert(v != NULL);
	switch (v->type){
	case PAR_STRING: {
		if (v->str.len == 0) {
			delete v->str.c_str;
		}
		else {
			delete[] v->str.c_str;
			v->str.len = 0;
		}
		break;
	}
	case PAR_ARRAY: {
		if (v->arr.size == 0) v->arr.elem = nullptr;
		else {
			for (int i = 0; i < v->arr.size; i++) {
				par_free(&v->arr.elem[i]);
			}
			delete[] v->arr.elem;
			v->arr.size = 0;
		}
		break;
	}
	case PAR_OBJECT: {
		if (v->obj.size == 0) v->obj.mem = nullptr;
		else {
			for (int i = 0; i < v->obj.size; i++) {
				par_mem_free(&v->obj.mem[i]);
			}
		}
		break;
	}
	}
	v->type = PAR_NULL;
}

