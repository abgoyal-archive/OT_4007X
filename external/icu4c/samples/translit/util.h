

#include "unicode/unistr.h"

// Verify that a UErrorCode is successful; exit(1) if not
void check(UErrorCode& status, const char* msg);

// Replace nonprintable characters with unicode escapes
UnicodeString escape(const UnicodeString &source);

// Print the given string to stdout
void uprintf(const UnicodeString &str);
