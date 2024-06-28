#ifndef PARJSON_H__
#define PARJSON_H__

#include<iostream>
#include<string>

//json���͵�ö��
typedef enum {
	PAR_NULL, PAR_TRUE, PAR_FALSE, PAR_NUMBER, PAR_STRING, PAR_ARRAY, PAR_OBJECT
} par_type;

//json���ڵ�
typedef struct {
	par_type type;
	double num;
} par_value;

//��������ö��
enum {
	//�����ɹ�
	PAR_OK = 0,
	
	//jsonֻ�к��հ�
	PAR_EXPECT_VALUE = 1,
	
	//ֵ��null��true��false���������ֵ
	PAR_INVALID_VALUE = 2,

	//�հ�֮���������ַ�
	PAR_ROOT_NOT_SINGULAR = 3,

	//���������ֹ���
	PAR_NUMBER_TOO_BIG = 4,

	//ȱʧ����
	LEPT_PARSE_MISS_QUOTATION_MARK = 5,

	//�Ƿ�ת���ַ�
	LEPT_PARSE_INVALID_STRING_ESCAPE = 6,

	//�Ƿ��ַ���
	LEPT_PARSE_INVALID_STRING_CHAR = 7
};

//����jsonAPI
#define lept_init(v) do { (v)->type = LEPT_NULL; } while(0)

#define lept_set_null(v) lept_free(v)

int parser(par_value* v, const char* json);

par_type par_get_type(const par_value* v);

void lept_set_number(par_value* v, double n);
double par_get_number(const par_value* v);

int par_get_boolean(const par_value* v);
void par_set_boolean(par_value* v, int b);

const char* par_get_string(const par_value* v);
size_t par_get_string_length(const par_value* v);
void par_set_string(par_value* v, const char* s, size_t len);

void par_free(par_value* v);

#endif