#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <caml/callback.h>
#include <caml/mlvalues.h>
#define UNW_LOCAL_ONLY
#include <libunwind.h>

value ml_func_with_10_params_native(value x1, value x2, value x3, value x4,
                                    value x5, value x6, value x7, value x8,
                                    value x9, value x10) {
    return Val_unit;
}

void error() {
  printf("<error>\n");
  exit(1);
}

value ml_perform_stack_walk(value unused) {
    unw_context_t ctxt;
    unw_getcontext(&ctxt);

    unw_cursor_t cursor;
    {
        int result = unw_init_local(&cursor, &ctxt);
        if (result != 0) error();
    }


    for (;;) {
        {
            char procname[256];
            unw_word_t ip_offset; // IP - start_of_proc
            int result = unw_get_proc_name(&cursor, procname, sizeof(procname),
                                           &ip_offset);
            if (result != 0) error();
            printf("%s\n", procname);
        }

        {
            int result = unw_step(&cursor);
            if (result == 0) break;
            if (result < 0) error();
        }
    }

    return Val_unit;
}
