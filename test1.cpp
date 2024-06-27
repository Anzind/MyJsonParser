//µ•‘™≤‚ ‘
#include <iostream>
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

#define TEST_ERROR(error, result, json)\
    do{\
        par_value v;\
        v.type = PAR_TRUE;\
        EXPECT_EQ_INT(error, parser(&v, json));\
        EXPECT_EQ_INT(result, par_get_type(&v));\
    }while(0)

//∫Í-Ω‚Œˆ ˝◊÷
#define TEST_NUMBER(error, json)\
    do{\
        par_value v;\
        EXPECT_EQ_INT(PAR_OK, parser(&v, json));\
        EXPECT_EQ_INT(PAR_NUMBER, par_get_type(&v));\
        EXPECT_EQ_DOUBLE(error, par_get_number());\
    }while(0)

//≤‚ ‘nullΩ‚Œˆ
static void test_parse_null() {
#if 0
	/*par_value v;
	v.type = PAR_TRUE;

	EXPECT_EQ_INT(PAR_OK, parser(&v, "null"));
	EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));*/
    TEST_ERROR(PAR_OK, PAR_NULL, "null");
#endif
}

//≤‚ ‘trueΩ‚Œˆ
static void test_parse_true() {
#if 0
    /*par_value v;
    v.type = PAR_TRUE;

    EXPECT_EQ_INT(PAR_OK, parser(&v, "true"));
    EXPECT_EQ_INT(PAR_TRUE, par_get_type(&v));*/
    TEST_ERROR(PAR_OK, PAR_TRUE, "true");
#endif
}

//≤‚ ‘falseΩ‚Œˆ
static void test_parse_false() {
#if 0
    /*par_value v;
    v.type = PAR_TRUE;

    EXPECT_EQ_INT(PAR_OK, parser(&v, "false"));
    EXPECT_EQ_INT(PAR_FALSE, par_get_type(&v));*/
    TEST_ERROR(PAR_OK, PAR_FALSE, "false");
#endif
}

static void test_parse_number() {
#if 0
    
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
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "nul");
    TEST_ERROR(PAR_INVALID_VALUE, PAR_NULL, "?");
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
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
}

int main() {
	test_parse();
	printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return main_ret;
}