#include "_cgo_export.h"

extern void crosscall2(void (*fn)(void *, int), void *, int, int);
extern int _cgo_wait_runtime_init_done();

void *get_crosscall2addr() {
    return crosscall2;
}

void *get_cgo_wait_runtime_init_done_addr() {
    return _cgo_wait_runtime_init_done;
}

void callbackfn(void *wrapper, void *fn) {
    int ctxt = _cgo_wait_runtime_init_done();
     struct {
       void *fn;
    } __attribute__((__packed__)) a;
    a.fn = fn;
    crosscall2((void (*)(void *, int))wrapper, &a, sizeof(a), ctxt);
}
