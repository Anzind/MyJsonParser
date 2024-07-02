//��Ԫ����
#include <iostream>
#include <iomanip>
#include <string>
#include "parjson.h"

//�ڴ�й¶���
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>
#endif

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

/*  ���������
	��Σ�
		expect->Ԥ�ڽ��
		actual->ʵ�ʽ��
	��ҪУ�飺
		�����Ƿ�ɹ�
		�����������Ƿ���ȷ
*/
#define EXPECT_EQ_BASE(expect, actual)\
	do{\
		test_count++;\
		if(expect == actual)\
			test_pass++;\
		else{\
			std::cout << __FILE__ << ":" << __LINE__ << ", expect:" << expect << ", actual:" << actual << std::endl;\
			main_ret = 1;\
		}\
	}while(0)

/*  ���������-�ַ���ר��
	��Σ�
		expect->Ԥ�ڽ��
		actual->ʵ�ʽ��
	��ҪУ�飺
		�����Ƿ�ɹ�
		�����������Ƿ���ȷ
*/
#define EXPECT_EQ_BASE_STRING(expect, actual)\
	do{\
		test_count++;\
		const char* temp = actual;\
		if(std::strcmp(expect, temp) == 0)\
			test_pass++;\
		else{\
			std::cout << __FILE__ << ":" << __LINE__ << ", expect:" << expect << ", actual:" << temp << std::endl;\
			main_ret = 1;\
		}\
	}while(0)

/*	������׺꣺�쳣������
	��Σ�
		expect->Ԥ�ڽ��
		json->���������ַ���
	��ҪУ�飺
		����ʧ�ܵĴ���ö���Ƿ���ȷ
*/
#define TEST_ERROR(expect, json)\
	do{\
		par_value v;\
		v.type = PAR_NULL;\
		EXPECT_EQ_BASE(expect, parser(&v, json));\
	}while(0)

/*	������׺꣺����ֵ���
	��Σ�
		expect->Ԥ�ڽ��
		json->���������ַ���
	��ҪУ�飺
		�����Ƿ�ɹ�
		�����������Ƿ���ȷ
		���������������Ƿ���ȷ
*/
#define TEST_NUMBER(expect, json)\
	do{\
		par_value v;\
		v.type = PAR_NULL;\
		EXPECT_EQ_BASE(PAR_OK, parser(&v, json));\
		EXPECT_EQ_BASE(PAR_NUMBER, par_get_type(&v));\
		EXPECT_EQ_BASE(expect, par_get_number(&v));\
	}while(0)

/*	������׺꣺�ַ���ֵ���
	��Σ�
		expect->Ԥ�ڽ��
		json->���������ַ���
	��ҪУ�飺
		�����Ƿ�ɹ�
		�����������Ƿ���ȷ
		�����������ַ����Ƿ���ȷ
*/
#define TEST_STRING(expect, json)\
	do{\
		par_value v;\
		v.type = PAR_NULL;\
		EXPECT_EQ_BASE(PAR_OK, parser(&v, json));\
		EXPECT_EQ_BASE(PAR_STRING, par_get_type(&v));\
		EXPECT_EQ_BASE_STRING(expect, par_get_string(&v));\
	}while(0)

//����nullֵ������ȷ
static void test_parse_null() {
#if 0 
	par_value v;
	v.type = PAR_NULL;
	
	EXPECT_EQ_BASE(PAR_OK, parser(&v, "null"));
	EXPECT_EQ_BASE(PAR_NULL, par_get_type(&v));
	par_free(&v);
#endif
}

//����trueֵ������ȷ
static void test_parse_true() {
#if 0
	par_value v;
	v.type = PAR_NULL;

	EXPECT_EQ_BASE(PAR_OK, parser(&v, "true"));
	EXPECT_EQ_BASE(PAR_TRUE, par_get_type(&v));
	par_free(&v);
#endif
}

//����falseֵ������ȷ
static void test_parse_false() {
#if 0
	par_value v;
	v.type = PAR_NULL;

	EXPECT_EQ_BASE(PAR_OK, parser(&v, "false"));
	EXPECT_EQ_BASE(PAR_FALSE, par_get_type(&v));
	par_free(&v);
#endif
}

//��������ֵ������ȷ
static void test_parse_number() {
#if 0
	TEST_NUMBER(0.0, "0");
	TEST_NUMBER(0.0, "-0");
	TEST_NUMBER(0.0, "-0.0");
	TEST_NUMBER(1.0, "1");
	TEST_NUMBER(-1.0, "-1");
	TEST_NUMBER(1.5, "1.5");
	TEST_NUMBER(-1.5, "-1.5");
	TEST_NUMBER(3.1416, "3.1416");
	TEST_NUMBER(1E10, "1E10");
	TEST_NUMBER(1e10, "1e10");
	TEST_NUMBER(1E+10, "1E+10");
	TEST_NUMBER(1E-10, "1E-10");
	TEST_NUMBER(-1E10, "-1E10");
	TEST_NUMBER(-1e10, "-1e10");
	TEST_NUMBER(-1E+10, "-1E+10");
	TEST_NUMBER(-1E-10, "-1E-10");
	TEST_NUMBER(1.234E+10, "1.234E+10");
	TEST_NUMBER(1.234E-10, "1.234E-10");
#endif
}

//����jsonȫΪ��ֵ���쳣
static void test_parse_expect_value() {
#if 0
	TEST_ERROR(PAR_EXPECT_VALUE, "");
	TEST_ERROR(PAR_EXPECT_VALUE, " ");
#endif
}

//����json�е��쳣ֵ���������Ǵ�ģ���������һ�����ݣ�
static void test_parse_invalid_value() {
#if 0
	/*����ֵ�쳣*/
	TEST_ERROR(PAR_INVALID_VALUE, "nul");
	TEST_ERROR(PAR_INVALID_VALUE, "?");
#endif

#if 0
	/* ����ֵ�쳣 */
	TEST_ERROR(PAR_INVALID_VALUE, "+0");
	TEST_ERROR(PAR_INVALID_VALUE, "+1");
	TEST_ERROR(PAR_INVALID_VALUE, ".123"); /* at least one digit before '.' */
	TEST_ERROR(PAR_INVALID_VALUE, "1.");   /* at least one digit after '.' */
	TEST_ERROR(PAR_INVALID_VALUE, "INF");
	TEST_ERROR(PAR_INVALID_VALUE, "inf");
	TEST_ERROR(PAR_INVALID_VALUE, "NAN");
	TEST_ERROR(PAR_INVALID_VALUE, "nan");
#endif
}

//����json�еĴ���ֵ���ܽ����������ݣ������д���ĵط���
static void test_parse_root_not_singular() {
#if 0
	/*���������ֵ*/
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "null x");
#endif

#if 0
	/* ��������ֵ */
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "0x0");
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "0x123");
#endif
}

//�������ֽ���Խ��
static void test_parse_number_too_big() {
#if 0
	TEST_ERROR(PAR_NUMBER_TOO_BIG, "1e309");
	TEST_ERROR(PAR_NUMBER_TOO_BIG, "-1e309");
#endif
}

//�����ַ����������
static void test_parse_string() {
#if 1

#if 1
	TEST_STRING("", "\"\"");
#endif
#if 0
	TEST_STRING("Hello", "\"Hello\"");
	TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
	TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
#endif
#endif
}

static void test_parse() {
#if 0
	/*����boolֵ��nullֵ*/
	test_parse_null();
	test_parse_true();
	test_parse_false();
#endif

#if 0
	/*��������ֵ*/
	test_parse_number();

	/*�������ֽ���Խ��*/
	test_parse_number_too_big();
#endif

#if 0
	/*��ֵ�쳣���ز���*/
	test_parse_expect_value();
#endif

#if 0
	/*����json�е��쳣ֵ*/
	test_parse_invalid_value();
#endif

#if 0
	/*���Դ���ֵ*/
	test_parse_root_not_singular();
#endif

#if 1
	/*�����ַ����������*/
	test_parse_string();
#endif
}

int main() {
#ifdef _WINDOWS
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	test_parse();
	float percent = test_pass * 100 / test_count;
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "�����" << test_pass << " / " << test_count
			<< "\nͨ���ʣ�" << percent << "%" << std::endl;

	return main_ret;
}