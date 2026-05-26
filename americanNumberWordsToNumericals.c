// ==============================================================================
// Pure Standalone Word-to-Numerical Converter Engine (Zero Dependencies)
// Natively supports values up to Quadrillions.
// Returns 0 on success, 1 on vocabulary or rule violation error.
// ==============================================================================

// Direct memory sizes matching your precise layout limits
#define ERR_BUF_SIZE 256
#define DICT_SIZE 38

// Global memory buffers to handle state safely without stdlib heap allocation
static char global_error_log[ERR_BUF_SIZE];

static const char* ACCEPTED[DICT_SIZE] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen", "twenty",
    "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety", "hundred",
    "thousand", "million", "billion", "trillion", "quadrillion", 
    "quintillion", "sextillion", "septillion", "octillion", "nonillion"
};

// 64-bit numerical mapping arrays matching your exact index values
typedef unsigned long long uint64_t;

static const uint64_t VALUES[DICT_SIZE] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    30, 40, 50, 60, 70, 80, 90, 100,
    1000, 1000000, 1000000000, 1000000000000ULL, 1000000000000000ULL, 
    0, 0, 0, 0, 0 // Capped past Quadrillions
};

// ------------------------------------------------------------------------------
// Standalone Custom String Subroutines (Replacing string.h / ctype.h)
// ------------------------------------------------------------------------------
static int custom_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

static void custom_strcpy(char* dest, const char* src) {
    while ((*dest++ = *src++));
}

static void append_str(char* dest, const char* src, int* pos, int max_len) {
    while (*src && (*pos < max_len - 1)) {
        dest[(*pos)++] = *src++;
    }
    dest[*pos] = '\0';
}

// ------------------------------------------------------------------------------
// Exposed Error Buffer Function for Outside Languages to Read Logs Natively
// ------------------------------------------------------------------------------
const char* get_last_error_message(void) {
    return global_error_log;
}

// ------------------------------------------------------------------------------
// Core Universal Cross-Language FFI Export Function
// ------------------------------------------------------------------------------
int word_to_numerical(const char* input_str, uint64_t* result) {
    if (!input_str || !result) return -1;
    
    // Allocate a safe scratchpad on the CPU stack to avoid needing malloc/free
    char buf[1024];
    int b_idx = 0;
    
    // Phase 1: Pure Standalone Sanitation & Lowercasing
    for (int i = 0; input_str[i] != '\0' && b_idx < 1023; i++) {
        char c = input_str[i];
        if (c >= 'A' && c <= 'Z') {
            c = c + 32; // Standard ASCII shift to lowercase without ctype.h
        }
        
        if (c == '-') {
            buf[b_idx++] = ' ';
        } else if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ') {
            buf[b_idx++] = c;
        }
    }
    buf[b_idx] = '\0';

    // Phase 2: Standalone Tokenizer (Splitting space intervals without strtok)
    const char* words[256];
    int word_indices[256];
    int count = 0;
    
    int i = 0;
    while (buf[i] != '\0' && count < 256) {
        // Skip leading whitespace boundaries
        while (buf[i] == ' ' || buf[i] == '\t') i++;
        if (buf[i] == '\0') break;
        
        // Mark the start point of a fresh word string segment
        char* current_word_start = &buf[i];
        while (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\0') i++;
        
        // Null-terminate the word safely inside our scratchpad memory
        if (buf[i] != '\0') {
            buf[i] = '\0';
            i++;
        }
        
        // Strip out grammatical fluff word "and" cleanly from our execution tracks
        if (custom_strcmp(current_word_start, "and") != 0) {
            int dict_match_idx = -1;
            for (int d = 0; d < DICT_SIZE; d++) {
                if (custom_strcmp(current_word_start, ACCEPTED[d]) == 0) {
                    dict_match_idx = d;
                    break;
                }
            }
            
            if (dict_match_idx == -1) {
                int p = 0;
                append_str(global_error_log, "Wrong input: ", &p, ERR_BUF_SIZE);
                append_str(global_error_log, current_word_start, &p, ERR_BUF_SIZE);
                append_str(global_error_log, " is not an accepted word.", &p, ERR_BUF_SIZE);
                return 1;
            }
            
            words[count] = current_word_start;
            word_indices[count] = dict_match_idx;
            count++;
        }
    }

    if (count == 0) {
        *result = 0;
        return 0;
    }

    // Phase 3: Grammar Rule Checker
    if (word_indices[0] >= 27) {
        custom_strcpy(global_error_log, "Wrong input: The first word cannot be a denomination or scale unit.");
        return 1;
    }

    for (int w = 0; w < count - 1; w++) {
        int idx = word_indices[w];
        int nidx = word_indices[w+1];
        int violation = 0;

        if (idx >= 0 && idx <= 8 && (nidx >= 0 && nidx <= 8)) violation = 1;
        if (idx >= 9 && idx <= 18 && (nidx <= 18 || nidx == 27)) violation = 1;
        if (idx >= 19 && idx <= 26 && ((nidx >= 9 && nidx <= 18) || nidx == 27)) violation = 1;
        if (idx == 27 && nidx == 27) violation = 1;
        if (idx >= 28 && (nidx == 27 || (nidx >= 28 && nidx >= idx))) violation = 1;

        if (violation) {
            int p = 0;
            append_str(global_error_log, "Wrong input: ", &p, ERR_BUF_SIZE);
            append_str(global_error_log, words[w], &p, ERR_BUF_SIZE);
            append_str(global_error_log, " cannot be followed by ", &p, ERR_BUF_SIZE);
            append_str(global_error_log, words[w+1], &p, ERR_BUF_SIZE);
            append_str(global_error_log, ".", &p, ERR_BUF_SIZE);
            return 1;
        }
    }

    // Phase 4: Pure 64-bit Core Math Engine with New Quadrillions Cap
    uint64_t ones = 0, tens = 0, hundreds = 0;
    uint64_t final_sum = 0;

    for (int w = 0; w < count; w++) {
        int idx = word_indices[w];
        
        if (idx >= 33) {
            custom_strcpy(global_error_log, "Error: Batch math engine limited to 64-bit integers. Numbers over Quadrillions are unsupported.");
            return 1;
        }

        uint64_t val = VALUES[idx];

        if (idx >= 0 && idx <= 8) ones += val;
        else if (idx >= 9 && idx <= 26) tens += val;
        else if (idx == 27) {
            if (w > 0) { hundreds += (ones * 100); ones = 0; }
        } 
        else if (idx >= 28) {
            uint64_t structural_group = ones + tens + hundreds;
            if (structural_group == 0 && w == 0) structural_group = 1;
            final_sum += (structural_group * val);
            ones = 0; tens = 0; hundreds = 0;
        }
    }
    
    final_sum += (ones + tens + hundreds);
    *result = final_sum;
    return 0;
}
