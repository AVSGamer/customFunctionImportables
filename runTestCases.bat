@echo off
setlocal enabledelayedexpansion

:: CHANGE THIS to match the exact filename of your refactored script
set "SCRIPT_NAME=americanNumberWordsToNumericals.bat"
set "TMP_OUT=%TEMP%\num_test_out.tmp"

echo ==============================================================================
echo                 WORD-TO-NUMBER ENGINE COMPLETE TEST SUITE
echo ==============================================================================
echo.

set /a passed=0
set /a failed=0

:: --- SECTION 1: STANDARD CARDINAL NUMBERS ---
echo [SECTION 1] Standard Cardinal Numbers...
call :test "one" "1"
call :test "eleven" "11"
call :test "twenty" "20"
call :test "twenty five" "25"
call :test "four hundred" "400"
call :test "nine hundred ninety nine" "999"

:: --- SECTION 2: COMPLEX & SCALED NUMBERS ---
echo. & echo [SECTION 2] Complex Scales (Thousands, Millions, Billions)...
call :test "one thousand thirty" "1030"
call :test "one thousand three" "1003"
call :test "four hundred fifty six thousand" "456000"
call :test "two million three hundred thousand forty five" "2300045"
call :test "five hundred sixteen million two hundred thirty four thousand eight hundred twelve" "516234812"

:: --- SECTION 3: BYPASSING 32-BIT OVERFLOW (CRITICAL) ---
echo. & echo [SECTION 3] 32-Bit Overflow Prevention (Above 2,147,483,647)...
call :test "two billion" "2000000000"
call :test "two billion one hundred forty seven million four hundred eighty three thousand six hundred forty seven" "2147483647"
call :test "two billion one hundred forty seven million four hundred eighty three thousand six hundred forty eight" "2147483648"
call :test "nine billion" "9000000000"
call :test "ninety nine billion" "99000000000"

:: --- SECTION 4: SANITIZATION, CASE, & "AND" HANDLING ---
echo. & echo [SECTION 4] Sanitization, Case, and "And" Stripping...
call :test "one hundred and two" "102"
call :test "ONE THOUSAND AND THIRTY" "1030"
call :test "  one   thousand   thirty  " "1030"
call :test "twenty-five" "25"
call :test "nine billion, two hundred thousand, and forty-two." "9000200042"

:: --- SECTION 5: EXPECTED SYNTAX & VOCABULARY ERRORS ---
echo. & echo [SECTION 5] Expected Vocabulary ^& Grammar Errors...
call :test "one billion thrity" "is not an accepted word."
call :test "million five" "The first word cannot be a denomination or scale unit."
call :test "hundred" "The first word cannot be a denomination or scale unit."
call :test "one trillion" "Numbers over Billions are unsupported."
call :test "one quadrillion" "Numbers over Billions are unsupported."

:: --- SECTION 6: GRAMMATICAL ORDER VIOLATIONS (RULES 1-5) ---
echo. & echo [SECTION 6] Rule Engine Violations...
call :test "one two" "one cannot be followed by two."
call :test "eleven two" "eleven cannot be followed by two."
call :test "eleven twelve" "eleven cannot be followed by twelve."
call :test "eleven hundred" "eleven cannot be followed by hundred."
call :test "twenty eleven" "twenty cannot be followed by eleven."
call :test "twenty hundred" "twenty cannot be followed by hundred."
call :test "one hundred hundred" "hundred cannot be followed by hundred."
call :test "one thousand million" "thousand cannot be followed by million."
call :test "one billion billion" "billion cannot be followed by billion."
call :test "one thousand hundred" "thousand cannot be followed by hundred."

echo.
echo ==============================================================================
echo  TEST SUITE RESULTS
echo ==============================================================================
echo  TOTAL PASSED: %passed%
echo  TOTAL FAILED: %failed%
echo ==============================================================================

if exist "%TMP_OUT%" del "%TMP_OUT%"

if %failed% gtr 0 (
    exit /b 1
) else (
    exit /b 0
)

:: -------------------------------------------------------------------------
:: Subroutine to run individual tests, handle outputs/errors, and print
:: -------------------------------------------------------------------------
:test
set "input=%~1"
set "expected=%~2"
set "actual="

if exist "%TMP_OUT%" del "%TMP_OUT%"
cmd /c ^""%~dp0%SCRIPT_NAME%" "%input%"^" > "%TMP_OUT%" 2>&1

if exist "%TMP_OUT%" (
    for /f "usebackq delims=" %%o in ("%TMP_OUT%") do set "actual=%%o"
)

:: Exact match handling
if "!actual!"=="!expected!" (
    echo  [PASS] Input: "%input%" - Got: "!actual!"
    set /a passed+=1
    goto :EOF
)

:: FIX: Safe string substitution parsing without dangerous pipeline echo operations
set "stripped=!actual:%expected%=!"
if not "!stripped!"=="!actual!" (
    echo  [PASS] Input: "%input%" - Threw expected error: "%expected%"
    set /a passed+=1
) else (
    echo  [FAIL] Input: "%input%"
    echo         Expected matching/value: "%expected%"
    echo         Got                    : "!actual!"
    set /a failed+=1
)
goto :EOF
