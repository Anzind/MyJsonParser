#ifndef PARJSON_H__
#define PARJSON_H__

#include<iostream>
#include<string>

//json���͵�ö��
typedef enum {
	PAR_NULL = 0,
	PAR_TRUE = 1,
	PAR_FALSE = 2,
	PAR_NUMBER = 3,
	PAR_STRING = 4,
	PAR_ARRAY = 5,
	PAR_OBJECT = 6
} par_type;

//json���ڵ�
typedef struct {
	union {
		//������Ϊ�ַ���ʱ������ṹ
		std::string str;

		//������Ϊ����ʱ�����
		double num;
	};

	par_type type;
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

void par_free(par_value* v);

int par_get_boolean(const par_value* v);

void par_set_number(par_value* v, double n);
double par_get_number(const par_value* v);

std::string par_get_string(const par_value* v);
void par_set_string(par_value* v, std::string s);

#endif