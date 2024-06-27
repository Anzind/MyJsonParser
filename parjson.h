#ifndef PARJSON_H__
#define PARJSON_H__



//json类型的枚举
typedef enum {
	PAR_NULL, PAR_TRUE, PAR_FALSE, PAR_NUMBER, PAR_STRING, PAR_ARRAY, PAR_OBJECT
} par_type;

//json树节点
typedef struct {
	par_type type;
} par_value;

//解析返回枚举
enum {
	//解析成功
	PAR_OK = 0,
	
	//json只有含空白
	PAR_EXPECT_VALUE,
	
	//值是null，true，false以外的其他值
	PAR_INVALID_VALUE,

	//空白之后还有其他字符
	PAR_ROOT_NOT_SINGULAR
};

//解析jsonAPI
int parser(par_value* v, const char* json);

par_type par_get_type(const par_value* v);

double par_get_number(par_value* v, const char* json);

#endif