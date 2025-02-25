/* -*- C++ -*-
 * Dlubal specific cppcheck checks
 */

//---------------------------------------------------------------------------
#ifndef checkdlubalH
#define checkdlubalH
//---------------------------------------------------------------------------

#include "check.h"
#include "config.h"

#include <string>

namespace ValueFlow {
    class Value;
}

class Settings;
class Token;
class Function;
class Variable;
class ErrorLogger;
class Tokenizer;

/// @addtogroup Checks
/// @{


/** @brief Various small checks */

class CPPCHECKLIB CheckDlubal : public Check {
    
public:
    /** @brief This constructor is used when registering the CheckClass */
    CheckDlubal() : Check(myName()) {}

private:
    /** @brief This constructor is used when running checks. */
    CheckDlubal(const Tokenizer *tokenizer, const Settings *settings, ErrorLogger *errorLogger)
        : Check(myName(), tokenizer, settings, errorLogger) {}

    /** @brief Run checks against the normal token list */
    void runChecks(const Tokenizer &tokenizer, ErrorLogger *errorLogger) override;

    ///** @brief %Check for correct enum naming scheme according to coding standard. */
    void checkEnumItemName();

    // Error messages..
    void enumItemNameError(const Token* tok);

    void getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const override;

    static std::string myName() {
        return "Dlubal";
    }

    std::string classInfo() const override {
        return "Dlubal specific checks\n"

               // style
               "- incorrect enum naming scheme\n";
    }
};
/// @}
//---------------------------------------------------------------------------
#endif // checkdlubalH
