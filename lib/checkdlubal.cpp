/*
 * Dlubal specific cppcheck checks
 */

//---------------------------------------------------------------------------
#include "checkdlubal.h"

#include "settings.h"
#include "symboldatabase.h"
#include "token.h"
#include "tokenize.h"
#include "utils.h"
#include "errorlogger.h"
#include "errortypes.h"
#include <string>

//---------------------------------------------------------------------------

// Register this check class (by creating a static instance of it)
namespace {
    CheckDlubal instance;
}

void CheckDlubal::checkEnumItemName()
{
    if (!mSettings->severity.isEnabled(Severity::style) && !mSettings->isPremiumEnabled("enumItemName"))
        return;

    logChecker("CheckDlubal::checkEnumItemName"); // style
    const SymbolDatabase* symbolDatabase = mTokenizer->getSymbolDatabase();
    for (const Scope& scope : symbolDatabase->scopeList)
    {
        if (scope.type != Scope::eEnum) {
            continue;
        }
        if (scope.enumClass) {
            continue;
        }

        std::string enumName = scope.className;
        strTolower(enumName);

        if (matchglob("flag", enumName)) {
            continue; // skip flags
        }

        if (endsWith(scope.bodyStart->fileName(), ".cpp")) {
            continue; // skip enums defined in cpp files
        }

        for (const Enumerator& enumerator : scope.enumeratorList) {
            std::string enumItemName = enumerator.name->str();
            
            if (enumName == "Value" && !startsWith(enumItemName, "E_")) {
                // this marks a special kind of enum that is directly wrapped in a class
                // similar to enum class (but with extra features), e.g.
                // class EAbc
                // {
                // public:
                //     enum Value
                //     {
                //         ONE,
                //         TWO
                //     };
                // };
                break;
            }

            // Remove formatting and check if enum item name starts with enum name:
            findAndReplace(enumItemName, "_", "");
            strTolower(enumItemName);
            if (!startsWith(enumItemName, enumName)) {
                enumItemNameError(scope.classDef);
                break;
            }
        }
    }
}

void CheckDlubal::enumItemNameError(const Token* tok)
{
    reportError(tok, Severity::style, "enumItemName", "Enum item should have enum name as prefix");
}

void CheckDlubal::runChecks(const Tokenizer &tokenizer, ErrorLogger *errorLogger)
{
    CheckDlubal checkDlubal(&tokenizer, &tokenizer.getSettings(), errorLogger);

    // Checks
    checkDlubal.checkEnumItemName();
}

void CheckDlubal::getErrorMessages(ErrorLogger *errorLogger, const Settings *settings) const
{
    CheckDlubal c(nullptr, settings, errorLogger);

    c.enumItemNameError(nullptr);
}
