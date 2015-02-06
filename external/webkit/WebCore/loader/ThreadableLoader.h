

#ifndef ThreadableLoader_h
#define ThreadableLoader_h

#include <wtf/Noncopyable.h>
#include <wtf/PassRefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

    class ResourceError;
    class ResourceRequest;
    class ResourceResponse;
    class ScriptExecutionContext;
    class ThreadableLoaderClient;

    enum StoredCredentials {
        AllowStoredCredentials,
        DoNotAllowStoredCredentials
    };
    
    enum CrossOriginRequestPolicy {
        DenyCrossOriginRequests,
        UseAccessControl,
        AllowCrossOriginRequests
    };
    
    struct ThreadableLoaderOptions {
        ThreadableLoaderOptions() : sendLoadCallbacks(false), sniffContent(false), allowCredentials(false), forcePreflight(false), crossOriginRequestPolicy(DenyCrossOriginRequests) { }
        bool sendLoadCallbacks;
        bool sniffContent;
        bool allowCredentials;  // Whether HTTP credentials and cookies are sent with the request.
        bool forcePreflight;  // If AccessControl is used, whether to force a preflight.
        CrossOriginRequestPolicy crossOriginRequestPolicy;
    };

    // Useful for doing loader operations from any thread (not threadsafe, 
    // just able to run on threads other than the main thread).
    class ThreadableLoader : public Noncopyable {
    public:
        static void loadResourceSynchronously(ScriptExecutionContext*, const ResourceRequest&, ThreadableLoaderClient&, const ThreadableLoaderOptions&);
        static PassRefPtr<ThreadableLoader> create(ScriptExecutionContext*, ThreadableLoaderClient*, const ResourceRequest&, const ThreadableLoaderOptions&);

        virtual void cancel() = 0;
        void ref() { refThreadableLoader(); }
        void deref() { derefThreadableLoader(); }

    protected:
        virtual ~ThreadableLoader() { }
        virtual void refThreadableLoader() = 0;
        virtual void derefThreadableLoader() = 0;
    };

} // namespace WebCore

#endif // ThreadableLoader_h
