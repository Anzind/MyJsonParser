﻿//单元测试
#include <iostream>
#include <iomanip>
#include <string>
#include "parjson.h"

//内存泄露检测
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>
#endif

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

/*  定义基础宏
	入参：
		expect->预期结果
		actual->实际结果
	需要校验：
		解析是否成功
		解析后类型是否正确
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

/*  定义基础宏-字符串专用
	入参：
		expect->预期结果
		actual->实际结果
	需要校验：
		解析是否成功
		解析后类型是否正确
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

/*	定义进阶宏：异常处理检测
	入参：
		expect->预期结果
		json->待解析的字符串
	需要校验：
		解析失败的错误枚举是否正确
*/
#define TEST_ERROR(expect, json)\
	do{\
		par_value v;\
		v.type = PAR_NULL;\
		EXPECT_EQ_BASE(expect, parser(&v, json));\
	}while(0)

/*	定义进阶宏：数字值检测
	入参：
		expect->预期结果
		json->待解析的字符串
	需要校验：
		解析是否成功
		解析后类型是否正确
		解析出来的数字是否正确
*/
#define TEST_NUMBER(expect, json)\
	do{\
		par_value v;\
		v.type = PAR_NULL;\
		EXPECT_EQ_BASE(PAR_OK, parser(&v, json));\
		EXPECT_EQ_BASE(PAR_NUMBER, par_get_type(&v));\
		EXPECT_EQ_BASE(expect, par_get_number(&v));\
	}while(0)

/*	定义进阶宏：字符串值检测
	入参：
		expect->预期结果
		json->待解析的字符串
	需要校验：
		解析是否成功
		解析后类型是否正确
		解析出来的字符串是否正确
*/
#define TEST_STRING(expect, json)\
	do{\
		par_value v;\
		v.type = PAR_NULL;\
		EXPECT_EQ_BASE(PAR_OK, parser(&v, json));\
		EXPECT_EQ_BASE(PAR_STRING, par_get_type(&v));\
		EXPECT_EQ_BASE_STRING(expect, par_get_string(&v));\
	}while(0)

//测试null值解析正确
static void test_parse_null() {
#if 0 
	par_value v;
	v.type = PAR_NULL;

	EXPECT_EQ_BASE(PAR_OK, parser(&v, "null"));
	EXPECT_EQ_BASE(PAR_NULL, par_get_type(&v));
	par_free(&v);
#endif
}

//测试true值解析正确
static void test_parse_true() {
#if 0
	par_value v;
	v.type = PAR_NULL;

	EXPECT_EQ_BASE(PAR_OK, parser(&v, "true"));
	EXPECT_EQ_BASE(PAR_TRUE, par_get_type(&v));
	par_free(&v);
#endif
}

//测试false值解析正确
static void test_parse_false() {
#if 0
	par_value v;
	v.type = PAR_NULL;

	EXPECT_EQ_BASE(PAR_OK, parser(&v, "false"));
	EXPECT_EQ_BASE(PAR_FALSE, par_get_type(&v));
	par_free(&v);
#endif
}

//测试数字值解析正确
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

#if 0
	/*极端情况*/
	TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
	TEST_NUMBER(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
	TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
	TEST_NUMBER(2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
	TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
	TEST_NUMBER(2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
	TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
	TEST_NUMBER(1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
	TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
#endif
}

//测试json全为空值的异常
static void test_parse_expect_value() {
#if 0
	TEST_ERROR(PAR_EXPECT_VALUE, "");
	TEST_ERROR(PAR_EXPECT_VALUE, " ");
#endif
}

//测试json中的异常值（整个都是错的，解析不了一点数据）
static void test_parse_invalid_value() {
#if 0
	/*其他值异常*/
	TEST_ERROR(PAR_INVALID_VALUE, "nul");
	TEST_ERROR(PAR_INVALID_VALUE, "?");
#endif

#if 0
	/* 数字值异常 */
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

//测试json中的错误值（能解析出来数据，但是有错误的地方）
static void test_parse_root_not_singular() {
#if 0
	/*错误的其他值*/
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "null x");
#endif

#if 0
	/* 错误数字值 */
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' or nothing */
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "0x0");
	TEST_ERROR(PAR_ROOT_NOT_SINGULAR, "0x123");
#endif
}

//测试数字解析越界
static void test_parse_number_too_big() {
#if 0
	TEST_ERROR(PAR_NUMBER_TOO_BIG, "1e309");
	TEST_ERROR(PAR_NUMBER_TOO_BIG, "-1e309");
#endif
}

//测试字符串解析结果
static void test_parse_string() {
#if 0

#if 0
	/*宏展开*/

	par_value v;
	v.type = PAR_NULL;
	test_count++;
	if (PAR_OK == parser(&v, "\"\""))
		test_pass++;
	else {
		std::cout << "D:\\CppProject\\MyJsonParser\\test1.cpp" << ":" << 223 << ", expect:" << PAR_OK << ", actual:" << parser(&v, "\"\"") << std::endl;
		main_ret = 1;
	}

	test_count++;
	if (PAR_STRING == par_get_type(&v))
		test_pass++;
	else {
		std::cout << "D:\\CppProject\\MyJsonParser\\test1.cpp" << ":" << 223 << ", expect:" << PAR_STRING << ", actual:" << par_get_type(&v) << std::endl;
		main_ret = 1;
	}

	test_count++;
	const char* temp = par_get_string(&v);
	if (std::strcmp("", temp) == 0)
		test_pass++;
	else {
		std::cout << "D:\\CppProject\\MyJsonParser\\test1.cpp" << ":" << 223 << ", expect:" << "" << ", actual:" << temp << std::endl;
		main_ret = 1;
	}
#endif

#if 0
	TEST_STRING("", "\"\"");
	TEST_STRING("Hello", "\"Hello\"");
	TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
	TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
#endif
#endif
}

//测试字符串丢失双引号异常
static void test_parse_missing_quotation_mark() {
#if 0
	TEST_ERROR(PAR_MISS_QUOTATION_MARK, "\"");
	TEST_ERROR(PAR_MISS_QUOTATION_MARK, "\"abc");
#endif
}

//测试字符串非法转义字符异常
static void test_parse_invalid_string_escape() {
#if 0
	TEST_ERROR(PAR_INVALID_STRING_ESCAPE, "\"\\v\"");
	TEST_ERROR(PAR_INVALID_STRING_ESCAPE, "\"\\'\"");
	TEST_ERROR(PAR_INVALID_STRING_ESCAPE, "\"\\0\"");
	TEST_ERROR(PAR_INVALID_STRING_ESCAPE, "\"\\x12\"");
#endif
}

//测试字符串非法字符异常
static void test_parse_invalid_string_char() {
#if 0
	TEST_ERROR(PAR_INVALID_STRING_CHAR, "\"\x01\"");
	TEST_ERROR(PAR_INVALID_STRING_CHAR, "\"\x1F\"");
#endif
}

static void test_parse() {
#if 0
	/*测试bool值与null值*/
	test_parse_null();
	test_parse_true();
	test_parse_false();
#endif

#if 0
	/*测试数字值*/
	test_parse_number();

	/*测试数字解析越界*/
	test_parse_number_too_big();
#endif

#if 0
#if 1
	/*空值异常返回测试*/
	test_parse_expect_value();
#endif
#if 1
	/*测试json中的异常值*/
	test_parse_invalid_value();
#endif
#if 1
	/*测试错误值*/
	test_parse_root_not_singular();
#endif
#endif

#if 0
#if 1
	/*测试字符串解析结果*/
	test_parse_string();
#endif
#if 1
	/*测试字符串丢失双引号异常*/
	test_parse_missing_quotation_mark();
#endif
#if 1
	/*测试字符串非法转义字符异常*/
	test_parse_invalid_string_escape();
#endif
#if 1
	test_parse_invalid_string_char();
#endif
#endif
}

int main() {
#ifdef _WINDOWS
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	test_parse();
	float percent = test_pass * 100 / (float)test_count;
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "结果：" << test_pass << " / " << test_count
		<< "\n通过率：" << percent << "%" << std::endl;

	return main_ret;
}