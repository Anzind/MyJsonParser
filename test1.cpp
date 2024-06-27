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

//≤‚ ‘nullΩ‚Œˆ
static void test_parse_null() {
	par_value v;
	v.type = PAR_TRUE;

	EXPECT_EQ_INT(PAR_OK, parser(&v, "null"));
	EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));
}

//≤‚ ‘trueΩ‚Œˆ
static void test_parse_true() {
    par_value v;
    v.type = PAR_TRUE;

    EXPECT_EQ_INT(PAR_OK, parser(&v, "true"));
    EXPECT_EQ_INT(PAR_TRUE, par_get_type(&v));
}

//≤‚ ‘falseΩ‚Œˆ
static void test_parse_false() {
    par_value v;
    v.type = PAR_TRUE;

    EXPECT_EQ_INT(PAR_OK, parser(&v, "false"));
    EXPECT_EQ_INT(PAR_FALSE, par_get_type(&v));
}

static void test_parse_expect_value() {
    par_value v;

    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_EXPECT_VALUE, parser(&v, ""));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));

    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_EXPECT_VALUE, parser(&v, " "));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));
}

static void test_parse_invalid_value() {
    par_value v;
    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_INVALID_VALUE, parser(&v, "nul"));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));

    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_INVALID_VALUE, parser(&v, "?"));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));
}

static void test_parse_root_not_singular() {
    par_value v;
    v.type = PAR_FALSE;
    EXPECT_EQ_INT(PAR_ROOT_NOT_SINGULAR, parser(&v, "null x"));
    EXPECT_EQ_INT(PAR_NULL, par_get_type(&v));
}

static void test_parse() {
	test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
	test_parse();
	printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
	return main_ret;
}