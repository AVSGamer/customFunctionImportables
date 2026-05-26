#include <stdio.h>
#include <string.h>
#include "./americanNumberWordsToNumericals.c"

int passed = 0;
int failed = 0;

void test_case(const char* input, const char* expected, int is_error_test) {
    uint64_t actual_value = 0;
    char actual_string[128] = {0};
    
    // Clear global error buffer before execution
    global_error_log[0] = '\0';
    
    int status = word_to_numerical(input, &actual_value);
    
    if (status == 0) {
        snprintf(actual_string, sizeof(actual_string), "%llu", (unsigned long long)actual_value);
    } else {
        strncpy(actual_string, get_last_error_message(), sizeof(actual_string) - 1);
    }

    int match = 0;
    if (is_error_test) {
        if (strstr(actual_string, expected) != NULL) match = 1;
    } else {
        if (strcmp(actual_string, expected) == 0) match = 1;
    }

    if (match) {
        if (is_error_test) {
            printf(" [PASS] Input: \"%s\" - Threw expected error: \"%s\"\n", input, expected);
        } else {
            printf(" [PASS] [SUCCESS] Input: \"%s\" -> Got: %s\n", input, actual_string);
        }
        passed++;
    } else {
        printf(" [FAIL] Input: \"%s\"\n", input);
        printf("        Expected matching/value: \"%s\"\n", expected);
        printf("        Got                    : \"%s\"\n", actual_string);
        failed++;
    }
}

int main() {
    printf("==============================================================================\n");
    printf("     PURE ZERO-DEPENDENCY WORD-TO-NUMBER CONVERTER ENGINE SUITE\n");
    printf("==============================================================================\n\n");

    // --- SECTION 1: STANDARD CARDINAL NUMBERS ---
    printf("[SECTION 1] Standard Cardinal Numbers...\n");
    test_case("one", "1", 0);
    test_case("eleven", "11", 0);
    test_case("twenty", "20", 0);
    test_case("twenty five", "25", 0);
    test_case("four hundred", "400", 0);
    test_case("nine hundred ninety nine", "999", 0);

    // --- SECTION 2: COMPLEX & SCALED NUMBERS ---
    printf("\n[SECTION 2] Complex Scales (Thousands, Millions, Billions)...\n");
    test_case("one thousand thirty", "1030", 0);
    test_case("one thousand three", "1003", 0);
    test_case("four hundred fifty six thousand", "456000", 0);
    test_case("two million three hundred thousand forty five", "2300045", 0);
    test_case("five hundred sixteen million two hundred thirty four thousand eight hundred twelve", "516234812", 0);

    // --- SECTION 3: EXPANDED SCALES (TRILLIONS & QUADRILLIONS) ---
    printf("\n[SECTION 3] 64-Bit Core Math Engine Scaling (Trillions & Quadrillions)...\n");
    test_case("two billion", "2000000000", 0);
    test_case("five trillion", "5000000000000", 0);
    test_case("one quadrillion", "1000000000000000", 0);
    test_case("two billion one hundred forty seven million four hundred eighty three thousand six hundred forty eight", "2147483648", 0);
    test_case("nine billion", "9000000000", 0);
    test_case("ninety nine billion", "99000000000", 0);

    // --- SECTION 4: SANITIZATION, CASE, & "AND" HANDLING ---
    printf("\n[SECTION 4] Sanitization, Case, and \"And\" Stripping...\n");
    test_case("one hundred and two", "102", 0);
    test_case("ONE THOUSAND AND THIRTY", "1030", 0);
    test_case("  one   thousand   thirty  ", "1030", 0);
    test_case("twenty-five", "25", 0);
    test_case("nine billion, two hundred thousand, and forty-two.", "9000200042", 0);

    // --- SECTION 5: EXPECTED SYNTAX & VOCABULARY ERRORS ---
    printf("\n[SECTION 5] Expected Vocabulary & Grammar Errors (New Cap Limit)...\n");
    test_case("one billion thrity", "is not an accepted word.", 1);
    test_case("million five", "The first word cannot be a denomination or scale unit.", 1);
    test_case("hundred", "The first word cannot be a denomination or scale unit.", 1);
    test_case("one quintillion", "Numbers over Quadrillions are unsupported.", 1);
    test_case("one quadrillion quintillion", "Wrong input: quadrillion cannot be followed by quintillion.", 1);

    // --- SECTION 6: GRAMMATICAL ORDER VIOLATIONS (RULES 1-5) ---
    printf("\n[SECTION 6] Rule Engine Violations...\n");
    test_case("one two", "one cannot be followed by two.", 1);
    test_case("eleven two", "eleven cannot be followed by two.", 1);
    test_case("eleven twelve", "eleven cannot be followed by twelve.", 1);
    test_case("eleven hundred", "eleven cannot be followed by hundred.", 1);
    test_case("twenty eleven", "twenty cannot be followed by eleven.", 1);
    test_case("twenty hundred", "twenty cannot be followed by hundred.", 1);
    test_case("one hundred hundred", "hundred cannot be followed by hundred.", 1);
    test_case("one thousand million", "thousand cannot be followed by million.", 1);
    test_case("one billion billion", "billion cannot be followed by billion.", 1);
    test_case("one thousand hundred", "thousand cannot be followed by hundred.", 1);

    // --- SECTION 7: EXTREME ACCURACY STRESS TEST ---
    printf("\n[SECTION 7] Extreme Ceiling Value Performance Verification...\n");
    test_case("nine hundred ninety nine quadrillion "
              "nine hundred ninety nine trillion "
              "nine hundred ninety nine billion "
              "nine hundred ninety nine million "
              "nine hundred ninety nine thousand "
              "nine hundred ninety nine", 
              "999999999999999999", 0);

    printf("\n==============================================================================\n");
    printf("  TEST SUITE RESULTS\n");
    printf("==============================================================================\n");
    printf("  TOTAL PASSED: %d\n", passed);
    printf("  TOTAL FAILED: %d\n", failed);
    printf("==============================================================================\n");

    return (failed > 0) ? 1 : 0;
}
