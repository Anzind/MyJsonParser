//单元测试
#include <iostream>
#include <string>
#include "parjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)\
	do{\
		test_count++;\
		if(equality)\
			test_pass++;\
		else{\
			fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n",__FILE__, __LINE__, expect, actual);\
			main_ret = 1;\
		}\
	}while(0)

#define EXPECT_EQ_INT(expect, actual) \
    EXPECT_EQ_BASE((expect)==(actual), expect, actual, "%d")

#define EXPECT_EQ_DOUBLE(expect, actual) \
    EXPECT_EQ_BASE((expect)==(actual), expect, actual, "%.17g")

#define EXPECT_EQ_STRING(expect, actual, alength) \
    EXPECT_EQ_BASE((expect)==(actual), expect, actual, "%s")

#define TEST_ERROR(error, result, json)\
    do{\
        par_value v;\
        v.type = PAR_TRUE;\
        EXPECT_EQ_INT(error, parser(&v, json));\
        EXPECT_EQ_INT(result, par_get_type(&v));\
    }while(0)

//宏-解析数字
#define TEST_NUMBER(error, json)\
    do{\
        par_value v;\
        v.type = PAR_NULL;\
        EXPECT_EQ_INT(PAR_OK, parser(&v, json));\
        EXPECT_EQ_INT(PAR_NUMBER, par_get_type(&v));\
        EXPECT_EQ_DOUBLE(error, par_get_number(&v));\
    }while(0)

//宏-解析字符串
#define TEST_STRING(expect, json)\
    do {\
        lept_value v;\
        lept_init(&v);\
        EXPECT_EQ_INT(PAR_OK, parser(&v, json));\
        EXPECT_EQ_INT(PAR_STRING, par_get_type(&v));\
        EXPECT_EQ_STRING(expect, lept_get_string(&v), lept_get_string_length(&v));\
        lept_free(&v);\
    } while(0)

#if 0 
/*null，false，true整合为一个*/
//测试null解析
static void test_parse_null() {
#if 0
	/*par_value v;
	v.type = PAR_TRUE;

	EXPECT_EQ_INT(PAR_OK, parser(&v, "null"));
	EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));*/
    TEST_ERROR(PAR_OK, PAR_NULL, "null");
#endif
}

//测试true解析
static void test_parse_true() {
#if 0
    /*par_value v;
    v.type = PAR_TRUE;

    EXPECT_EQ_INT(PAR_OK, parser(&v, "true"));
    EXPECT_EQ_INT(PAR_TRUE, par_get_type(&v));*/
    TEST_ERROR(PAR_OK, PAR_TRUE, "true");
#endif
}

//测试false解析
static void test_parse_false() {
#if 0
    /*par_value v;
    v.type = PAR_TRUE;

    EXPECT_EQ_INT(PAR_OK, parser(&v, "false"));
    EXPECT_EQ_INT(PAR_FALSE, par_get_type(&v));*/
    TEST_ERROR(PAR_OK, PAR_FALSE, "false");
#endif
}
#endif

static void test_parse_bool() {
#if 0
    TEST_ERROR(PAR_OK, PAR_NULL, "null");
    TEST_ERROR(PAR_OK, PAR_TRUE, "true");
    TEST_ERROR(PAR_OK, PAR_FALSE, "false");
#endif
}

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

static void test_parse_expect_value() {
#if 0
    /*par_value v;

    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_EXPECT_VALUE, parser(&v, ""));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));

    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_EXPECT_VALUE, parser(&v, " "));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));*/
    TEST_ERROR(PAR_EXPECT_VALUE, PAR_NULL, "");
    TEST_ERROR(PAR_EXPECT_VALUE, PAR_NULL, " ");
#endif
}

static void test_parse_invalid_value() {
#if 0 
    /*par_value v;
    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_INVALID_VALUE, parser(&v, "nul"));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));

    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_INVALID_VALUE, parser(&v, "?"));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));*/
    /*测试bool合法*/
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "nul");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "?");
#endif

#if 0
    /*测试数字合法*/
    /* invalid number */
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "+0");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "+1");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, ".123"); /* at least one digit before '.' */
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "1.");   /* at least one digit after '.' */
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "INF");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "inf");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "NAN");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "nan");
#endif
}

static void test_parse_root_not_singular() {
#if 0
    /*par_value v;
    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_ROOT_NOT_SINGULAR, parser(&v, "null x"));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));*/
    TEST_ERROR(PAR_ROOT_NOT_SINGULAR, PAR_NULL, "null x");
#endif

#if 0
    /*校验数字合法*/
    /* invalid number */
    TEST_ERROR(PAR_ROOT_NOT_SINGULAR, PAR_NUMBER, "0123"); /* after zero should be '.' or nothing */
    TEST_ERROR(PAR_ROOT_NOT_SINGULAR, PAR_NUMBER, "0x0");
    TEST_ERROR(PAR_ROOT_NOT_SINGULAR, PAR_NUMBER, "0x123");
#endif
}

//数字边界值测试
static void test_parse_number_limit_out() {
#if 0
    //TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
    TEST_ERROR(PAR_NUMBER_TOO_BIG, PAR_NUMBER, "1e309");
    TEST_ERROR(PAR_NUMBER_TOO_BIG, PAR_NUMBER, "-1e309");
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

//字符串解析测试
static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
#if 0
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
#endif
}

static void test_parse() {
#if 0
    test_parse_null();
    test_parse_true();
    test_parse_false();
#endif
    test_parse_bool();
    test_parse_number();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_limit_out();


}

int main() {
	test_parse();
	printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return main_ret;
}