# customFunctionImportables
A collection of custom functions I made to be importable into supposedly any language for use,

# High-Performance Word-to-Numerical Engine

A zero-dependency, ultra-portable C engine that instantly converts American English number words into 64-bit unsigned integers. It supports precise grammatical validation and handles values up to **Quadrillions** natively, safely bypassing traditional 32-bit math limitations.

## Features
- **Zero Dependencies:** Compiles natively without any standard C library header requirements.
- **Strict Rule Engine:** Validates lexical hierarchy and steps out cleanly on rule violations.
- **Cross-Language FFI:** Designed specifically to be included as a dynamic shared library (`.dll` for Windows, `.so` for Linux).

---

## 🚀 How to Download the Binaries
Pre-compiled production binaries are automatically built by our cloud pipeline for every tag release. 
Go to the **Releases** tab on the right sidebar of this repository to download the latest deployment assets:
* Windows: `numberWordsToNumericals.dll`
* Linux: `numberWordsToNumericals.so`

---

## 🛠️ Cross-Language Integration Examples

### 1. JavaScript / Node.js Integration (via `ffi-napi`)
To load the compiled libraries natively inside Node.js, install the modern community-standard FFI bridge [1]:
```bash
npm install ffi-napi
```

Save this script as `test.js`. It dynamically detects your operating system and loads the appropriate library file [1]:

```javascript
const ffi = require('ffi-napi');
const path = require('path');
const os = require('os');

// 1. Automatically detect the environment and choose the correct file extension [1]
const isWindows = os.platform() === 'win32';
const libName = isWindows ? 'numberWordsToNumericals.dll' : 'numberWordsToNumericals.so';
const libPath = path.resolve(__dirname, libName);

// 2. Bind the exact native C function signatures using Javascript types [1]
const lib = ffi.Library(libPath, {
    // int word_to_numerical(const char* input_str, uint64_t* result)
    'word_to_numerical': ['int', ['string', 'uint64*']],
    // const char* get_last_error_message(void)
    'get_last_error_message': ['string', []]
});

// 3. Allocate a safe memory pointer container for the 64-bit return value [1]
const resultBuffer = Buffer.alloc(8); // 8 bytes for uint64 [1]
const inputString = "five hundred sixteen million two hundred thirty four thousand eight hundred twelve";

const status = lib.word_to_numerical(inputString, resultBuffer);

if (status === 0) {
    // Read the memory buffer as a 64-bit unsigned integer string [1]
    const numericResult = resultBuffer.readBigUInt64LE(0);
    console.log(`Success! Output Value: ${numericResult.toString()}`);
} else {
    const errorMsg = lib.get_last_error_message();
    console.log(`Engine Failed: ${errorMsg}`);
}
```

---

### 2. Python Integration (via `ctypes`)
Save this as `test.py`. It uses Python's built-in binding engine to track platform variations natively:

```python
import ctypes
import os
import sys

# 1. Detect platform extension dynamically
lib_name = "numberWordsToNumericals.dll" if sys.platform == "win32" else "numberWordsToNumericals.so"
dll_path = os.path.abspath(lib_name)
lib = ctypes.CDLL(dll_path)

# 2. Match the exact C function pointers signatures
lib.word_to_numerical.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_uint64)]
lib.word_to_numerical.restype = ctypes.c_int
lib.get_last_error_message.restype = ctypes.c_char_p

# 3. Allocation buffer configuration parameters
result = ctypes.c_uint64(0)
input_string = b"nine hundred ninety-nine billion two hundred thousand"

status = lib.word_to_numerical(input_string, ctypes.byref(result))

if status == 0:
    print(f"Success! Output Value: {result.value}")
else:
    error_msg = lib.get_last_error_message().decode('utf-8')
    print(f"Engine Failed: {error_msg}")
```

---

### 3. C# Integration (via P/Invoke)
Save this as `Program.cs`. Windows and Linux runtimes using modern .NET (Core 5+) automatically manage file extensions (`.dll` vs `.so`) when searching for the exact library tag string name:

```csharp
using System;
using System.Runtime.InteropServices;

class Program {
    // Leaves off file extensions; .NET Core resolves the file suffix depending on the host OS
    [DllImport("numberWordsToNumericals", CallingConvention = CallingConvention.Cdecl)]
    public static extern int word_to_numerical(string inputStr, out ulong result);

    [DllImport("numberWordsToNumericals", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr get_last_error_message();

    static void Main() {
        string input = "one thousand and thirty";
        
        if (word_to_numerical(input, out ulong numericResult) == 0) {
            Console.WriteLine(\$"Parsed Success: {numericResult}");
        } else {
            string errorLog = Marshal.PtrToStringAnsi(get_last_error_message());
            Console.WriteLine(\$"Error Flagged: {errorLog}");
        }
    }
}
```
---

## ⚖️ Legal Disclaimer & Security Implication Notice

### 1. Limitation of Liability
This software is provided "as is," without warranty of any kind, express or implied, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non-infringement. In no event shall the authors, copyright holders, or maintainers be held liable for any claim, damages, or other liability—whether in an action of contract, tort, or otherwise—arising from, out of, or in connection with the software or the use or other dealings in the software. **By executing, including, or compiling this codebase, you agree that you do so entirely at your own risk, and the author will not be held responsible for any damages to your device, operating system, or production environments.**

### 2. The Implication of Standalone (Zero-Dependency) Codebases
This project intentionally uses **zero external standard libraries or system headers** (`#include`) to achieve absolute platform portability and seamless foreign language inclusion (FFI). To do this, standard foundational mechanisms (such as custom string comparison, string copying, and character array memory padding tracking routines) were manually written and adapted into the code. 

Developers choosing to utilize this engine must understand the systemic architectural trade-offs:
* **The Vulnerability Trap:** Traditional software configurations rely on upstream package managers or operating system distributions to automatically patch vulnerabilities (such as out-of-bounds reads, buffer overflows, or pointer mismanagement errors) inside standard runtimes. Because this library is completely standalone and incorporates customized algorithmic patterns, **any downstream bug discovered in our baseline logic will not be fixed by system security updates.**
* **Security Debt:** If security vulnerabilities are discovered in any underlying algorithms or code structures borrowed and modified within this engine, it remains the absolute responsibility of the developer integrating this binary to manually refactor, recompile, and redeploy their version patches.

### 3. Open Source Maintenance & Contribution
Maintaining a zero-dependency system library requires meticulous, defensive engineering to ensure memory interfaces stay secure across different language runtimes (Python, Node.js, C#, etc.). 

If you are a developer, systems engineer, or security researcher who wants to help audit, optimize, or maintain this code engine, your contributions are highly welcome. Please get in touch with me directly through my GitHub profile or by opening an issue/pull request in this repository to coordinate development updates.

### 4. Replaced Library Reference Matrix

To transition this codebase into a pure, zero-dependency engine, all functionalities previously imported from standard development environments were completely stripped out. For version control and dependency auditing purposes, here are the original system libraries and specifications that were referenced to construct our custom, standalone algorithms:

* **`<string.h>` (ISO/IEC 9899:1999 Standard C Runtime)**
  * *Replaced Functions:* `strcmp`, `strcpy`, `strtok`
  * *Implication:* Custom tokenization loop and string block matching logic were rewritten to prevent memory segmentation dependencies during multi-threaded cross-language pointer parsing.
* **`<ctype.h>` (ISO/IEC 9899:1999 Character Handling)**
  * *Replaced Functions:* `tolower`, `isalnum`
  * *Implication:* Lowercase mapping is now processed using a lightweight ASCII bit-shift structure (`c + 32`), eliminating dependencies on local system location tables.
* **`<stdlib.h>` (Standard Utility Library)**
  * *Replaced Functions:* `malloc`, `free`
  * *Implication:* Heap management routines were entirely removed. The engine executes inside pre-allocated stack bounds, completely eliminating memory leak vectors within host platforms like Python or Node.js.
* **`<stdint.h>` (Fixed-width Integer Types Specification)**
  * *Replaced Data Types:* `uint64_t`
  * *Implication:* Native integer type scaling was replaced with standard 64-bit primitive configurations (`unsigned long long`). This ensures that value processing up to Quadrillions remains fully stable on modern 64-bit CPU architectures without needing separate system mapping layers.

## 📊 Test Suite Performance Verification

Below is the verified test run log executing on the **Pure Zero-Dependency 64-bit Engine**. All semantic checks, 32-bit overflow scenarios, sanitation filters, and grammatical order validation rules score a perfect **38/38 passing rating**.

### Section 1: Standard Cardinal Numbers


| Test Status | Input Phrase | Output Value | Verification Type |
| :--- | :--- | :--- | :--- |
| 🟢 **PASS** | `"one"` | `1` | Exact Numerical Match |
| 🟢 **PASS** | `"eleven"` | `11` | Exact Numerical Match |
| 🟢 **PASS** | `"twenty"` | `20` | Exact Numerical Match |
| 🟢 **PASS** | `"twenty five"` | `25` | Exact Numerical Match |
| 🟢 **PASS** | `"four hundred"` | `400` | Exact Numerical Match |
| 🟢 **PASS** | `"nine hundred ninety nine"` | `999` | Exact Numerical Match |

### Section 2: Complex Scales (Thousands, Millions, Billions)


| Test Status | Input Phrase | Output Value | Verification Type |
| :--- | :--- | :--- | :--- |
| 🟢 **PASS** | `"one thousand thirty"` | `1030` | Linear Scale Accumulation |
| 🟢 **PASS** | `"one thousand three"` | `1003` | Intermittent Zero Padding |
| 🟢 **PASS** | `"four hundred fifty six thousand"` | `456000` | Triplet Modified Scale |
| 🟢 **PASS** | `"two million three hundred thousand forty five"` | `2300045` | Multi-Scale Concat |
| 🟢 **PASS** | `"five hundred sixteen million two hundred thirty four thousand eight hundred twelve"` | `516234812` | Full Scale Sequence |

### Section 3: 64-Bit Core Math Engine Scaling (Trillions & Quadrillions)


| Test Status | Input Phrase | Output Value | Verification Type |
| :--- | :--- | :--- | :--- |
| 🟢 **PASS** | `"two billion"` | `2000000000` | Native 64-Bit Boundary |
| 🟢 **PASS** | `"five trillion"` | `5000000000000` | Native 64-Bit Boundary |
| 🟢 **PASS** | `"one quadrillion"` | `1000000000000000` | Native 64-Bit Boundary |
| 🟢 **PASS** | `"two billion one hundred forty seven million four hundred eighty three thousand six hundred forty eight"` | `2147483648` | **32-Bit Overflow Pass** |
| 🟢 **PASS** | `"nine billion"` | `9000000000` | **32-Bit Overflow Pass** |
| 🟢 **PASS** | `"ninety nine billion"` | `99000000000` | **32-Bit Overflow Pass** |

### Section 4: Sanitization, Case, and "And" Stripping


| Test Status | Input Phrase | Output Value | Filter Applied |
| :--- | :--- | :--- | :--- |
| 🟢 **PASS** | `"one hundred and two"` | `102` | `and` Fluff Removal |
| 🟢 **PASS** | `"ONE THOUSAND AND THIRTY"` | `1030` | Uppercase Flattening |
| 🟢 **PASS** | `"  one   thousand   thirty  "` | `1030` | Multi-Space Compaction |
| 🟢 **PASS** | `"twenty-five"` | `25` | Hyphen-to-Space Mapping |
| 🟢 **PASS** | `"nine billion, two hundred thousand, and forty-two."` | `9000200042` | Punctuation Stripping |

### Section 5: Expected Vocabulary & Grammar Errors (New Cap Limit)


| Test Status | Input Phrase | Target Thrown Error String |
| :--- | :--- | :--- |
| 🟢 **PASS** | `"one billion thrity"` | `"is not an accepted word."` |
| 🟢 **PASS** | `"million five"` | `"The first word cannot be a denomination or scale unit."` |
| 🟢 **PASS** | `"hundred"` | `"The first word cannot be a denomination or scale unit."` |
| 🟢 **PASS** | `"one quintillion"` | `"Numbers over Quadrillions are unsupported."` |
| 🟢 **PASS** | `"one quadrillion quintillion"` | `"Wrong input: quadrillion cannot be followed by quintillion."` |

### Section 6: Rule Engine Violations


| Test Status | Input Phrase | Expected Core Validator Violation Log |
| :--- | :--- | :--- |
| 🟢 **PASS** | `"one two"` | `"one cannot be followed by two."` |
| 🟢 **PASS** | `"eleven two"` | `"eleven cannot be followed by two."` |
| 🟢 **PASS** | `"eleven twelve"` | `"eleven cannot be followed by twelve."` |
| 🟢 **PASS** | `"eleven hundred"` | `"eleven cannot be followed by hundred."` |
| 🟢 **PASS** | `"twenty eleven"` | `"twenty cannot be followed by eleven."` |
| 🟢 **PASS** | `"twenty hundred"` | `"twenty cannot be followed by hundred."` |
| 🟢 **PASS** | `"one hundred hundred"` | `"hundred cannot be followed by hundred."` |
| 🟢 **PASS** | `"one thousand million"` | `"thousand cannot be followed by million."` |
| 🟢 **PASS** | `"one billion billion"` | `"billion cannot be followed by billion."` |
| 🟢 **PASS** | `"one thousand hundred"` | `"thousand cannot be followed by hundred."` |

### Section 7: Extreme Ceiling Value Performance Verification


| Test Status | Input Phrase | Output Value |
| :--- | :--- | :--- |
| 🟢 **PASS** | `"nine hundred ninety nine quadrillion nine hundred ninety nine trillion nine hundred ninety nine billion nine hundred ninety nine million nine hundred ninety nine thousand nine hundred ninety nine"` | `999999999999999999` |

***