
 
#include "config.h"
#include "runtime_method.h"

#include "JSDOMBinding.h"
#include "runtime_object.h"
#include <runtime/Error.h>
#include <runtime/FunctionPrototype.h>

using namespace WebCore;

namespace JSC {

using namespace Bindings;

ASSERT_CLASS_FITS_IN_CELL(RuntimeMethod);

const ClassInfo RuntimeMethod::s_info = { "RuntimeMethod", 0, 0, 0 };

RuntimeMethod::RuntimeMethod(ExecState* exec, const Identifier& ident, Bindings::MethodList& m)
    // FIXME: deprecatedGetDOMStructure uses the prototype off of the wrong global object
    // exec-globalData() is also likely wrong.
    // Callers will need to pass in the right global object corresponding to this native object "m".
    : InternalFunction(&exec->globalData(), deprecatedGetDOMStructure<RuntimeMethod>(exec), ident)
    , _methodList(new MethodList(m))
{
}

JSValue RuntimeMethod::lengthGetter(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    RuntimeMethod* thisObj = static_cast<RuntimeMethod*>(asObject(slot.slotBase()));

    // Ick!  There may be more than one method with this name.  Arbitrarily
    // just pick the first method.  The fundamental problem here is that 
    // JavaScript doesn't have the notion of method overloading and
    // Java does.
    // FIXME: a better solution might be to give the maximum number of parameters
    // of any method
    return jsNumber(exec, thisObj->_methodList->at(0)->numParameters());
}

bool RuntimeMethod::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot &slot)
{
    if (propertyName == exec->propertyNames().length) {
        slot.setCustom(this, lengthGetter);
        return true;
    }
    
    return InternalFunction::getOwnPropertySlot(exec, propertyName, slot);
}

bool RuntimeMethod::getOwnPropertyDescriptor(ExecState* exec, const Identifier& propertyName, PropertyDescriptor &descriptor)
{
    if (propertyName == exec->propertyNames().length) {
        PropertySlot slot;
        slot.setCustom(this, lengthGetter);
        descriptor.setDescriptor(slot.getValue(exec, propertyName), ReadOnly | DontDelete | DontEnum);
        return true;
    }
    
    return InternalFunction::getOwnPropertyDescriptor(exec, propertyName, descriptor);
}

static JSValue JSC_HOST_CALL callRuntimeMethod(ExecState* exec, JSObject* function, JSValue thisValue, const ArgList& args)
{
    RuntimeMethod* method = static_cast<RuntimeMethod*>(function);

    if (method->methods()->isEmpty())
        return jsUndefined();
    
    RuntimeObjectImp* imp;

    if (thisValue.inherits(&RuntimeObjectImp::s_info)) {
        imp = static_cast<RuntimeObjectImp*>(asObject(thisValue));
    } else {
        // If thisObj is the DOM object for a plugin, get the corresponding
        // runtime object from the DOM object.
        JSValue value = thisValue.get(exec, Identifier(exec, "__apple_runtime_object"));
        if (value.inherits(&RuntimeObjectImp::s_info))    
            imp = static_cast<RuntimeObjectImp*>(asObject(value));
        else
            return throwError(exec, TypeError);
    }

    RefPtr<Instance> instance = imp->getInternalInstance();
    if (!instance) 
        return RuntimeObjectImp::throwInvalidAccessError(exec);
        
    instance->begin();
    JSValue result = instance->invokeMethod(exec, *method->methods(), args);
    instance->end();
    return result;
}

CallType RuntimeMethod::getCallData(CallData& callData)
{
    callData.native.function = callRuntimeMethod;
    return CallTypeHost;
}

}
