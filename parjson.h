#ifndef PARJSON_H__
#define PARJSON_H__



//json���͵�ö��
typedef enum {
	PAR_NULL, PAR_TRUE, PAR_FALSE, PAR_NUMBER, PAR_STRING, PAR_ARRAY, PAR_OBJECT
} par_type;

//json���ڵ�
typedef struct {
	par_type type;
} par_value;

//��������ö��
enum {
	//�����ɹ�
	PAR_OK = 0,
	
	//jsonֻ�к��հ�
	PAR_EXPECT_VALUE,
	
	//ֵ��null��true��false���������ֵ
	PAR_INVALID_VALUE,

	//�հ�֮���������ַ�
	PAR_ROOT_NOT_SINGULAR
};

//����jsonAPI
int parser(par_value* v, const char* json);

par_type par_get_type(const par_value* v);

double par_get_number(par_value* v, const char* json);

#endif