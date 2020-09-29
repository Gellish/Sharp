//
// Created by BraxtonN on 2/12/2018.
//

#ifndef SHARP_THREAD_H
#define SHARP_THREAD_H

#include "../../stdimports.h"
#include "symbols/ClassObject.h"
#include "symbols/Exception.h"
#include "symbols/Method.h"
#include "profiler.h"
#include "../Modules/std/Random.h"
#include "jit/architecture.h"
#include "../util/HashMap.h"
#include "ThreadStates.h"
#include "fiber.h"

#define ILL_THREAD_ID -1
#define THREAD_MAP_SIZE 0x2000
#define REGISTER_SIZE 12

#define INTERNAL_STACK_SIZE (KB_TO_BYTES(64) / sizeof(StackElement))
#define INTERNAL_STACK_MIN KB_TO_BYTES(4)
#define STACK_SIZE MB_TO_BYTES(1)
#define STACK_MIN KB_TO_BYTES(50)
#define STACK_OVERFLOW_BUF KB_TO_BYTES(10) // VERY LARGE OVERFLOW BUFFER FOR jit STACK OFERFLOW CATCHES

#define main_threadid 0x0
#define gc_threadid 0x1
#define jit_threadid 0x2

#ifdef BUILD_JIT
struct jit_context;
#endif

class Thread {
public:
    Thread()
    {
        init();
    }

    void init() {
        id = ILL_THREAD_ID;
        daemon = false;
        state = THREAD_CREATED;
        suspended = false;
        exited = false;
        terminated = false;
        priority = THREAD_PRIORITY_NORM;
        name.init();
        this_fiber = NULL;
        next_fiber = NULL;
        last_fiber = NULL;
        signal = tsig_empty;
        contextSwitching=false;
        marked=false;
        lastRanMicros=0;
#ifdef BUILD_JIT
        jctx = NULL;
#endif
        args.object = NULL;
        currentThread.object=NULL;
        new (&mutex) recursive_mutex();
        boundFibers=0;
        boundFibers=0;
#ifdef WIN32_
        thread = NULL;
#endif
    }

    void init(string name, Int id, Method* main, bool daemon = false, bool initializeStack = false);
    static int32_t generateId();
    static void Startup();
    static void suspendSelf();
    static int start(int32_t, size_t);
    static int destroy(int32_t);
    static int interrupt(int32_t);
    static int suspendThread(int32_t);
    static int unSuspendThread(int32_t, bool wait);
    static void suspendFor(Int);
    static int join(int32_t);
    static Thread* getThread(int32_t);
    static void suspendAndWait(Thread* thread);
    static void unsuspendAndWait(Thread* thread);
    static void waitForThreadExit(Thread* thread);
    static void terminateAndWaitForThreadExit(Thread* thread);
    static int waitForThread(Thread *thread);
    static int setPriority(int32_t, int);
    static int setPriority(Thread*, int);
    static void killAll();
    static void shutdown();
    static bool validStackSize(size_t);
    static bool validInternalStackSize(size_t);
    static void suspendAllThreads(bool withMarking = false);
    static void resumeAllThreads(bool withMarking = false);
    static int threadjoin(Thread*);
    static int destroy(Thread*);
    bool try_context_switch();
    void enableContextSwitch(fiber *nextFib, bool enable);

    static int startDaemon(
#ifdef WIN32_
            DWORD WINAPI
#endif
#ifdef POSIX_
    void*
#endif
    (*threadFunc)(void*), Thread* thread);


    static int32_t Create(int32_t, bool daemon);
    void Create(string, Method*);
    void CreateDaemon(string);
    void exit();
    void term();
    void exec();
    void setup();
    void waitForContextSwitch();
    void printException();

#ifdef BUILD_JIT
    jit_context *jctx;
#endif
#ifdef SHARP_PROF_
    Profiler *tprof;
#endif
    /* tsig_t */ int signal;

    static uInt maxThreadId;
    static int32_t tid;
    static _List<Thread*> threads;
    static recursive_mutex threadsMonitor;
    static recursive_mutex threadsListMutex;

    uInt boundFibers;
    recursive_mutex mutex;
    int32_t id;
    Int stackSize;
    Int stbase;
    Int stfloor;
    int priority;
    bool daemon;
    bool terminated;
    unsigned int state;
    bool suspended;
    bool exited;
    bool marked;
    native_string name;
    Object currentThread, args;
    fiber *this_fiber, *next_fiber, *last_fiber;
    Method* mainMethod;
    uInt lastRanMicros;
    bool contextSwitching;

#ifdef WIN32_
    HANDLE thread;
#endif
#ifdef POSIX_
    pthread_t thread;
#endif

private:

    void waitForUnsuspend();

    static int unsuspendThread(Thread*);
    static void suspendThread(Thread*);
    static int interrupt(Thread*);

    static void pushThread(Thread *thread);
    static void popThread(Thread *thread);
    void releaseResources();

    static void waitForThreadSuspend(Thread *thread);
    static void waitForThreadUnSuspend(Thread *thread);
};

/**
 * Send a signal to shutdown and or sleep a thread and more
 * see tsig_t for more information.
 */
#define sendSignal(sig, sigt, enable) (sig ^= (-(unsigned long)enable ^ sig) & (1UL << sigt))
#define hasSignal(sig, sigt) ((sig >> sigt) & 1U)

extern thread_local Thread* thread_self;
extern thread_local double *registers;

#define _64EBX registers[EBX]
#define _64ADX registers[ADX]
#define _64ECX registers[ECX]
#define _64EGX registers[EGX]
#define _64CMT registers[CMT]
#define _64BMR registers[BMR]

#define PC(thread_self) \
    (thread_self->pc-thread_self->cache)

extern unsigned long irCount, overflow;
extern size_t threadStackSize;
extern size_t internalStackSize;
extern string dataTypeToString(DataType varType, bool isArray);

#endif //SHARP_THREAD_H
