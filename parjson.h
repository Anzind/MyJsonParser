#ifndef PARJSON_H__
#define PARJSON_H__

#include<iostream>
#include<string>

//json类型的枚举
typedef enum {
	PAR_NULL = 0,
	PAR_TRUE = 1,
	PAR_FALSE = 2,
	PAR_NUMBER = 3,
	PAR_STRING = 4,
	PAR_ARRAY = 5,
	PAR_OBJECT = 6
} par_type;

//json树节点
typedef struct {
	union {
		struct {
			char* c_str;
			size_t len;
		}str;
		//当该项为数字时用这个
		double num;
	};
	par_type type;
} par_value;

//解析返回枚举
enum {
	//解析成功
	PAR_OK = 0,

	//json只有含空白
	PAR_EXPECT_VALUE = 1,

	//值是null，true，false以外的其他值
	PAR_INVALID_VALUE = 2,

	//空白之后还有其他字符
	PAR_ROOT_NOT_SINGULAR = 3,

	//待解析数字过大
	PAR_NUMBER_TOO_BIG = 4,

	//缺失引号
	PAR_MISS_QUOTATION_MARK = 5,

	//非法转义字符
	PAR_INVALID_STRING_ESCAPE = 6,

	//非法字符
	PAR_INVALID_STRING_CHAR = 7
};

//解析jsonAPI
#define par_set_null(v) par_free(v)

int parser(par_value* v, const char* json);

par_type par_get_type(const par_value* v);

void par_free(par_value* v);

int par_get_boolean(const par_value* v);

void par_set_number(par_value* v, double n);
double par_get_number(const par_value* v);

const char* par_get_string(const par_value* v);
void par_set_string(par_value* v, char* s);

#endif