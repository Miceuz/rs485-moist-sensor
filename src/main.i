# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
# 1 "/usr/avr/include/inttypes.h" 1 3
# 37 "/usr/avr/include/inttypes.h" 3
# 1 "/usr/lib/gcc/avr/4.9.2/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/avr/4.9.2/include/stdint.h" 3 4
# 1 "/usr/avr/include/stdint.h" 1 3 4
# 122 "/usr/avr/include/stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 143 "/usr/avr/include/stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 160 "/usr/avr/include/stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 214 "/usr/avr/include/stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 274 "/usr/avr/include/stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 10 "/usr/lib/gcc/avr/4.9.2/include/stdint.h" 2 3 4
# 38 "/usr/avr/include/inttypes.h" 2 3
# 77 "/usr/avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 2 "main.c" 2
# 1 "/usr/avr/include/avr/io.h" 1 3
# 99 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/sfr_defs.h" 1 3
# 100 "/usr/avr/include/avr/io.h" 2 3
# 390 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/iotn441.h" 1 3
# 391 "/usr/avr/include/avr/io.h" 2 3
# 538 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/portpins.h" 1 3
# 539 "/usr/avr/include/avr/io.h" 2 3

# 1 "/usr/avr/include/avr/common.h" 1 3
# 541 "/usr/avr/include/avr/io.h" 2 3

# 1 "/usr/avr/include/avr/version.h" 1 3
# 543 "/usr/avr/include/avr/io.h" 2 3






# 1 "/usr/avr/include/avr/fuse.h" 1 3
# 239 "/usr/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char low;
    unsigned char high;
    unsigned char extended;
} __fuse_t;
# 550 "/usr/avr/include/avr/io.h" 2 3


# 1 "/usr/avr/include/avr/lock.h" 1 3
# 553 "/usr/avr/include/avr/io.h" 2 3
# 3 "main.c" 2
# 1 "/usr/avr/include/util/delay.h" 1 3
# 43 "/usr/avr/include/util/delay.h" 3
# 1 "/usr/avr/include/util/delay_basic.h" 1 3
# 40 "/usr/avr/include/util/delay_basic.h" 3
static inline void _delay_loop_1(uint8_t __count) __attribute__((always_inline));
static inline void _delay_loop_2(uint16_t __count) __attribute__((always_inline));
# 80 "/usr/avr/include/util/delay_basic.h" 3
void
_delay_loop_1(uint8_t __count)
{
 __asm__ volatile (
  "1: dec %0" "\n\t"
  "brne 1b"
  : "=r" (__count)
  : "0" (__count)
 );
}
# 102 "/usr/avr/include/util/delay_basic.h" 3
void
_delay_loop_2(uint16_t __count)
{
 __asm__ volatile (
  "1: sbiw %0,1" "\n\t"
  "brne 1b"
  : "=w" (__count)
  : "0" (__count)
 );
}
# 44 "/usr/avr/include/util/delay.h" 2 3
# 1 "/usr/avr/include/math.h" 1 3
# 127 "/usr/avr/include/math.h" 3
extern double cos(double __x) __attribute__((__const__));





extern double sin(double __x) __attribute__((__const__));





extern double tan(double __x) __attribute__((__const__));






extern double fabs(double __x) __attribute__((__const__));






extern double fmod(double __x, double __y) __attribute__((__const__));
# 168 "/usr/avr/include/math.h" 3
extern double modf(double __x, double *__iptr);



extern float modff (float __x, float *__iptr);




extern double sqrt(double __x) __attribute__((__const__));
extern float sqrtf (float) __attribute__((__const__));




extern double cbrt(double __x) __attribute__((__const__));
# 194 "/usr/avr/include/math.h" 3
extern double hypot (double __x, double __y) __attribute__((__const__));







extern double square(double __x) __attribute__((__const__));






extern double floor(double __x) __attribute__((__const__));






extern double ceil(double __x) __attribute__((__const__));
# 234 "/usr/avr/include/math.h" 3
extern double frexp(double __x, int *__pexp);







extern double ldexp(double __x, int __exp) __attribute__((__const__));





extern double exp(double __x) __attribute__((__const__));





extern double cosh(double __x) __attribute__((__const__));





extern double sinh(double __x) __attribute__((__const__));





extern double tanh(double __x) __attribute__((__const__));







extern double acos(double __x) __attribute__((__const__));







extern double asin(double __x) __attribute__((__const__));






extern double atan(double __x) __attribute__((__const__));
# 298 "/usr/avr/include/math.h" 3
extern double atan2(double __y, double __x) __attribute__((__const__));





extern double log(double __x) __attribute__((__const__));





extern double log10(double __x) __attribute__((__const__));





extern double pow(double __x, double __y) __attribute__((__const__));






extern int isnan(double __x) __attribute__((__const__));
# 333 "/usr/avr/include/math.h" 3
extern int isinf(double __x) __attribute__((__const__));






__attribute__((__const__)) static inline int isfinite (double __x)
{
    unsigned char __exp;
    __asm__ (
 "mov	%0, %C1		\n\t"
 "lsl	%0		\n\t"
 "mov	%0, %D1		\n\t"
 "rol	%0		"
 : "=r" (__exp)
 : "r" (__x) );
    return __exp != 0xff;
}






__attribute__((__const__)) static inline double copysign (double __x, double __y)
{
    __asm__ (
 "bst	%D2, 7	\n\t"
 "bld	%D0, 7	"
 : "=r" (__x)
 : "0" (__x), "r" (__y) );
    return __x;
}
# 376 "/usr/avr/include/math.h" 3
extern int signbit (double __x) __attribute__((__const__));






extern double fdim (double __x, double __y) __attribute__((__const__));
# 392 "/usr/avr/include/math.h" 3
extern double fma (double __x, double __y, double __z) __attribute__((__const__));







extern double fmax (double __x, double __y) __attribute__((__const__));







extern double fmin (double __x, double __y) __attribute__((__const__));






extern double trunc (double __x) __attribute__((__const__));
# 426 "/usr/avr/include/math.h" 3
extern double round (double __x) __attribute__((__const__));
# 439 "/usr/avr/include/math.h" 3
extern long lround (double __x) __attribute__((__const__));
# 453 "/usr/avr/include/math.h" 3
extern long lrint (double __x) __attribute__((__const__));
# 45 "/usr/avr/include/util/delay.h" 2 3
# 84 "/usr/avr/include/util/delay.h" 3
static inline void _delay_us(double __us) __attribute__((always_inline));
static inline void _delay_ms(double __ms) __attribute__((always_inline));
# 141 "/usr/avr/include/util/delay.h" 3
void
_delay_ms(double __ms)
{
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((16000000) / 1e3) * __ms;
# 160 "/usr/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 186 "/usr/avr/include/util/delay.h" 3
}
# 223 "/usr/avr/include/util/delay.h" 3
void
_delay_us(double __us)
{
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((16000000) / 1e6) * __us;
# 242 "/usr/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 268 "/usr/avr/include/util/delay.h" 3
}
# 4 "main.c" 2
# 1 "/usr/avr/include/stdio.h" 1 3
# 45 "/usr/avr/include/stdio.h" 3
# 1 "/usr/lib/gcc/avr/4.9.2/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/avr/4.9.2/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 98 "/usr/lib/gcc/avr/4.9.2/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 46 "/usr/avr/include/stdio.h" 2 3



# 1 "/usr/lib/gcc/avr/4.9.2/include/stddef.h" 1 3 4
# 212 "/usr/lib/gcc/avr/4.9.2/include/stddef.h" 3 4
typedef unsigned int size_t;
# 50 "/usr/avr/include/stdio.h" 2 3
# 242 "/usr/avr/include/stdio.h" 3
struct __file {
 char *buf;
 unsigned char unget;
 uint8_t flags;
# 261 "/usr/avr/include/stdio.h" 3
 int size;
 int len;
 int (*put)(char, struct __file *);
 int (*get)(struct __file *);
 void *udata;
};
# 275 "/usr/avr/include/stdio.h" 3
typedef struct __file FILE;
# 405 "/usr/avr/include/stdio.h" 3
extern struct __file *__iob[];
# 417 "/usr/avr/include/stdio.h" 3
extern FILE *fdevopen(int (*__put)(char, FILE*), int (*__get)(FILE*));
# 434 "/usr/avr/include/stdio.h" 3
extern int fclose(FILE *__stream);
# 608 "/usr/avr/include/stdio.h" 3
extern int vfprintf(FILE *__stream, const char *__fmt, va_list __ap);





extern int vfprintf_P(FILE *__stream, const char *__fmt, va_list __ap);






extern int fputc(int __c, FILE *__stream);




extern int putc(int __c, FILE *__stream);


extern int putchar(int __c);
# 649 "/usr/avr/include/stdio.h" 3
extern int printf(const char *__fmt, ...);





extern int printf_P(const char *__fmt, ...);







extern int vprintf(const char *__fmt, va_list __ap);





extern int sprintf(char *__s, const char *__fmt, ...);





extern int sprintf_P(char *__s, const char *__fmt, ...);
# 685 "/usr/avr/include/stdio.h" 3
extern int snprintf(char *__s, size_t __n, const char *__fmt, ...);





extern int snprintf_P(char *__s, size_t __n, const char *__fmt, ...);





extern int vsprintf(char *__s, const char *__fmt, va_list ap);





extern int vsprintf_P(char *__s, const char *__fmt, va_list ap);
# 713 "/usr/avr/include/stdio.h" 3
extern int vsnprintf(char *__s, size_t __n, const char *__fmt, va_list ap);





extern int vsnprintf_P(char *__s, size_t __n, const char *__fmt, va_list ap);




extern int fprintf(FILE *__stream, const char *__fmt, ...);





extern int fprintf_P(FILE *__stream, const char *__fmt, ...);






extern int fputs(const char *__str, FILE *__stream);




extern int fputs_P(const char *__str, FILE *__stream);





extern int puts(const char *__str);




extern int puts_P(const char *__str);
# 762 "/usr/avr/include/stdio.h" 3
extern size_t fwrite(const void *__ptr, size_t __size, size_t __nmemb,
         FILE *__stream);







extern int fgetc(FILE *__stream);




extern int getc(FILE *__stream);


extern int getchar(void);
# 810 "/usr/avr/include/stdio.h" 3
extern int ungetc(int __c, FILE *__stream);
# 822 "/usr/avr/include/stdio.h" 3
extern char *fgets(char *__str, int __size, FILE *__stream);






extern char *gets(char *__str);
# 840 "/usr/avr/include/stdio.h" 3
extern size_t fread(void *__ptr, size_t __size, size_t __nmemb,
        FILE *__stream);




extern void clearerr(FILE *__stream);
# 857 "/usr/avr/include/stdio.h" 3
extern int feof(FILE *__stream);
# 868 "/usr/avr/include/stdio.h" 3
extern int ferror(FILE *__stream);






extern int vfscanf(FILE *__stream, const char *__fmt, va_list __ap);




extern int vfscanf_P(FILE *__stream, const char *__fmt, va_list __ap);







extern int fscanf(FILE *__stream, const char *__fmt, ...);




extern int fscanf_P(FILE *__stream, const char *__fmt, ...);






extern int scanf(const char *__fmt, ...);




extern int scanf_P(const char *__fmt, ...);







extern int vscanf(const char *__fmt, va_list __ap);







extern int sscanf(const char *__buf, const char *__fmt, ...);




extern int sscanf_P(const char *__buf, const char *__fmt, ...);
# 938 "/usr/avr/include/stdio.h" 3
static __inline__ int fflush(FILE *stream __attribute__((unused)))
{
 return 0;
}





typedef long long fpos_t;
extern int fgetpos(FILE *stream, fpos_t *pos);
extern FILE *fopen(const char *path, const char *mode);
extern FILE *freopen(const char *path, const char *mode, FILE *stream);
extern FILE *fdopen(int, const char *);
extern int fseek(FILE *stream, long offset, int whence);
extern int fsetpos(FILE *stream, fpos_t *pos);
extern long ftell(FILE *stream);
extern int fileno(FILE *);
extern void perror(const char *s);
extern int remove(const char *pathname);
extern int rename(const char *oldpath, const char *newpath);
extern void rewind(FILE *stream);
extern void setbuf(FILE *stream, char *buf);
extern int setvbuf(FILE *stream, char *buf, int mode, size_t size);
extern FILE *tmpfile(void);
extern char *tmpnam (char *s);
# 5 "main.c" 2





typedef struct {
    int temp;
    int microvolts;
} temp_point;

static temp_point thermistorPoints[] = {
    { 850 , 129 },
    { 800 , 146 },
    { 750 , 165 },
    { 700 , 186 },
    { 650 , 210 },
    { 600 , 236 },
    { 550 , 267 },
    { 500 , 300 },
    { 450 , 337 },
    { 400 , 376 },
    { 350 , 419 },
    { 300 , 464 },
    { 200 , 559 },
    { 250 , 511 },
    { 150 , 608 },
    { 100 , 656 },
    { 50 , 703 },
    { 0 , 748 },
    { -50 , 789 },
    { -100, 828 },
    { -150, 862 },
    { -200, 892 }
};

static inline int interpolate(int val, int rangeStart, int rangeEnd, int valStart, int valEnd) {
    return (rangeEnd - rangeStart) * (val - valStart) / (valEnd - valStart) + rangeStart;
}

static inline int interpolateVoltage(int temp, unsigned char i){
    return interpolate(temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp);
}

static inline int interpolateTemperature(int microvolts, unsigned char i){

    return interpolate(microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts);
}




static inline unsigned char searchTemp(int temp) {
    unsigned char i;
    for(i = 0; i < 20; i++) {
        if(thermistorPoints[i].temp > temp) {
            return i;
        }
    }
    return 20 -1;
}




static inline unsigned char searchMicrovolts(int microvolts) {
    unsigned char i;
    for(i = 0; i < 20; i++) {
        if(thermistorPoints[i].microvolts > microvolts) {
            return i;
        }
    }
    return 20 -1;
}

long thermistorLsbToTemperature(unsigned int lsb) {
    return interpolateTemperature(lsb, searchMicrovolts(lsb));
}


inline static void ledOn() {
    (*(volatile uint8_t *)((0x1B) + 0x20)) |= (1 << (5));
}

inline static void ledOff() {
    (*(volatile uint8_t *)((0x1B) + 0x20)) &= ~(1 << (5));
};

inline static void ledSetup(){
    (*(volatile uint8_t *)((0x1A) + 0x20)) |= (1 << (5));
}




inline static void serialSetup() {

# 1 "/usr/avr/include/util/setbaud.h" 1 3
# 102 "main.c" 2
    (*(volatile uint8_t *)((0x1A) + 0x20)) |= (1 << (0));
    (*(volatile uint8_t *)(0x82)) = ((((16000000) + 8UL * (9600)) / (16UL * (9600)) -1UL) >> 8);
    (*(volatile uint8_t *)(0x81)) = ((((16000000) + 8UL * (9600)) / (16UL * (9600)) -1UL) & 0xff);
    (*(volatile uint8_t *)(0x85)) = (1 << (4)) | (1 << (3));
    (*(volatile uint8_t *)(0x84)) = (1 << (1)) | (1 << (2));
}

inline static void serialDriverEnable() {
    (*(volatile uint8_t *)((0x1B) + 0x20)) |= (1 << (0));
}

inline static void serialDriverDisable() {
    (*(volatile uint8_t *)((0x1B) + 0x20)) &= ~(1 << (0));
}

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    do { } while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x86)))))) & (1 << (5)))));
    (*(volatile uint8_t *)(0x80)) = c;
}

FILE uart_output = { .put = uart_putchar, .get = ((void *)0), .flags = 0x0002, .udata = 0, };

static inline void adcSetup() {
    (*(volatile uint8_t *)((0x05) + 0x20)) = (1 << (7)) | (1 << (2)) | (1 << (0));
    (*(volatile uint8_t *)((0x08) + 0x20)) = 0;
}






uint16_t adcReadChannel(uint8_t channel) {
    (*(volatile uint8_t *)((0x09) + 0x20)) = channel;
    (*(volatile uint8_t *)((0x05) + 0x20)) |= (1 << (6));
    do { } while (((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)((0x05) + 0x20)))))) & (1 << (6))));
    uint16_t ret = (*(volatile uint16_t *)((0x06) + 0x20));
    return ret;
}

char buff[10];






int main (void) {
    ledSetup();
    (__iob[1]) = &uart_output;
    serialSetup();
    serialDriverEnable();

    adcSetup();

    while(1) {
        ledOn();
        _delay_ms(100);
        uint16_t thermistor = adcReadChannel(3);

        fputs("Thermistor = ", (__iob[1]));

        itoa(thermistor, buff, 10);
        fputs(buff, (__iob[1]));
        fputs(" -> ", (__iob[1]));

        int tt = thermistorLsbToTemperature(thermistor);

        itoa((int)tt, buff, 10);
        fputs(buff, (__iob[1]));
# 195 "main.c"
        puts(".");

        ledOff();
        _delay_ms(100);
    }
}
