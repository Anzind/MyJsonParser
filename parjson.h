#ifndef PARJSON_H__
#define PARJSON_H__

#include<iostream>
#include<string>

#ifndef PAR_SIRINGIGY_INIT_SIZE
#define PAR_SIRINGIGY_INIT_SIZE 256
#endif

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
typedef struct par_value par_value;
typedef struct par_member par_member;
struct par_value{
	union {
		/*当该项为对象时用这个*/
		struct {
			par_member* mem;
			size_t size;
		}obj;
		/*当该项为数组时用这个*/
		struct {
			par_value* elem;
			size_t size;
		}arr;
		/*当该项为字符串时用这个*/
		struct {
			char* c_str;
			size_t len;
		}str;
		/*当该项为数字时用这个*/
		double num;
	};
	par_type type;
};

struct par_member {
	char* key;
	size_t key_len;
	par_value value;
};

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
	PAR_INVALID_STRING_CHAR = 7,

	//码点代理错误
	PAR_INVALID_UNICODE_SURROGATE = 8,

	//解析码点错误
	PAR_INVALID_UNICODE_HEX = 9,

	//列表格式错误
	PAR_MISS_COMMA_OR_SQUARE_BRACKET = 10,

	//对象成员无key
	PAR_MISS_KEY = 11,

	//对象成员无冒号
	PAR_MISS_COLON = 12,

	//对象格式错误
	PAR_MISS_COMMA_OR_CURLY_BRACKET = 13
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

size_t par_get_array_size(const par_value* v);
par_value* par_get_array_element(const par_value* v, size_t index);

size_t par_get_object_size(const par_value* v);
const char* par_get_object_key(const par_value* v, size_t index);
size_t par_get_object_key_length(const par_value* v, size_t index);
par_value* par_get_object_value(const par_value* v, size_t index);

char* par_stringify(const par_value* v, size_t* length);
#endif