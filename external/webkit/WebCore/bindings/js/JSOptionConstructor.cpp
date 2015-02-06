

#include "config.h"
#include "JSOptionConstructor.h"

#include "HTMLNames.h"
#include "HTMLOptionElement.h"
#include "JSHTMLOptionElement.h"
#include "ScriptExecutionContext.h"
#include "Text.h"
#include <runtime/Error.h>

using namespace JSC;

namespace WebCore {

ASSERT_CLASS_FITS_IN_CELL(JSOptionConstructor);

const ClassInfo JSOptionConstructor::s_info = { "OptionConstructor", 0, 0, 0 };

JSOptionConstructor::JSOptionConstructor(ExecState* exec, JSDOMGlobalObject* globalObject)
    : DOMConstructorWithDocument(JSOptionConstructor::createStructure(globalObject->objectPrototype()), globalObject)
{
    putDirect(exec->propertyNames().prototype, JSHTMLOptionElementPrototype::self(exec, globalObject), None);
    putDirect(exec->propertyNames().length, jsNumber(exec, 4), ReadOnly|DontDelete|DontEnum);
}

static JSObject* constructHTMLOptionElement(ExecState* exec, JSObject* constructor, const ArgList& args)
{
    JSOptionConstructor* jsConstructor = static_cast<JSOptionConstructor*>(constructor);
    Document* document = jsConstructor->document();
    if (!document)
        return throwError(exec, ReferenceError, "Option constructor associated document is unavailable");

    RefPtr<HTMLOptionElement> element = static_pointer_cast<HTMLOptionElement>(document->createElement(HTMLNames::optionTag, false));

    ExceptionCode ec = 0;
    RefPtr<Text> text = document->createTextNode("");
    if (!args.at(0).isUndefined())
        text->setData(args.at(0).toString(exec), ec);
    if (ec == 0)
        element->appendChild(text.release(), ec);
    if (ec == 0 && !args.at(1).isUndefined())
        element->setValue(args.at(1).toString(exec));
    if (ec == 0)
        element->setDefaultSelected(args.at(2).toBoolean(exec));
    if (ec == 0)
        element->setSelected(args.at(3).toBoolean(exec));

    if (ec) {
        setDOMException(exec, ec);
        return 0;
    }

    return asObject(toJS(exec, jsConstructor->globalObject(), element.release()));
}

ConstructType JSOptionConstructor::getConstructData(ConstructData& constructData)
{
    constructData.native.function = constructHTMLOptionElement;
    return ConstructTypeHost;
}


} // namespace WebCore
