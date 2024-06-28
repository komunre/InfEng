#include "global.h"

#include <stdarg.h>

#ifdef VITA
#include <psp2/kernel/clib.h>
#endif

vec3 x_axis = {1.0f, 0.0f, 0.0f};
vec3 y_axis = {0.0f, 1.0f, 0.0f};
vec3 z_axis = {0.0f, 0.0f, 1.0f};

unsigned int defaultShaderProgram = 0;

void log_info(char* fmt, ...) {
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;

    va_start(ap, fmt);
    for(p = fmt; *p; p++) {
        if(*p != '%') {
            #ifdef VITA
            sceClibPrintf("%c", *p);
            #else
            putchar(*p);
            #endif
            continue;
        }
        switch (*++p) {
            case 'i':
            case 'd':
                ival = va_arg(ap, int);
                #ifdef VITA
                sceClibPrintf("%d", ival);
                #else
                printf("%d", ival );
                #endif
                break;
            case 'f':
                dval  = va_arg(ap, double);
                #ifdef VITA
                sceClibPrintf("%f", dval);
                #else
                printf("%f", dval);
                #endif
                break;
            case 's':
                //#ifdef VITA
                //sval = va_arg(ap, char *);
                //sceClibPrintf("%s", sval);
                //#else
                for(sval = va_arg(ap, char *); *sval; sval++)
                    #ifdef VITA
                    sceClibPrintf("%c", (int)*sval);
                    #else
                    putchar(*sval);
                #endif
                break;
            default:
                #ifdef VITA
                sceClibPrintf("%c", *p);
                #else
                putchar(*p);
                #endif
                break;
        }
    }
    va_end(ap);
}
