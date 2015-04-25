# 1 "thermistor.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "thermistor.c"
# 1 "/usr/avr/include/stdio.h" 1 3
# 44 "/usr/avr/include/stdio.h" 3
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
# 45 "/usr/avr/include/stdio.h" 2 3
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
# 2 "thermistor.c" 2



typedef struct {
 long temp;
 unsigned long microvolts;
} temp_point;

static temp_point thermistorPoints[] = {
 { 8500 , 129 },
 { 8000 , 146 },
 { 7500 , 165 },
 { 7000 , 186 },
 { 6500 , 210 },
 { 6000 , 236 },
 { 5500 , 267 },
 { 5000 , 300 },
 { 4500 , 337 },
 { 4000 , 376 },
 { 3500 , 419 },
 { 3000 , 464 },
 { 2000 , 559 },
 { 2500 , 511 },
 { 1500 , 608 },
 { 1000 , 656 },
 { 500 , 703 },
 { 0 , 748 },
 { -500 , 789 },
 { -1000, 828 },
 { -1500, 862 },
 { -2000, 892 }
};

static inline long interpolate(unsigned long val, unsigned long rangeStart, unsigned long rangeEnd, unsigned long valStart, unsigned long valEnd) {
    return rangeStart + (rangeStart - rangeEnd) * (val - valStart) / (valEnd - valStart);
}

static inline long interpolateVoltage(unsigned long temp, unsigned char i){
    return interpolate(temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp);
}

static inline long interpolateTemperature(unsigned long microvolts, unsigned char i){

    return interpolate(microvolts, thermistorPoints[i-1].temp, thermistorPoints[i].temp, thermistorPoints[i-1].microvolts, thermistorPoints[i].microvolts);
}




static inline unsigned char searchTemp(unsigned long temp) {
 unsigned char i;
 for(i = 0; i < 20; i++) {
  if(thermistorPoints[i].temp > temp) {
   return i;
  }
 }
 return 20 -1;
}




static inline unsigned char searchMicrovolts(unsigned long microvolts) {
 unsigned char i;
 for(i = 0; i < 20; i++) {
  if(thermistorPoints[i].microvolts > microvolts) {
   return i;
  }
 }
 return 20 -1;
}

long thermistorLsbToTemperature(unsigned long lsb) {
 return interpolateTemperature(lsb, searchMicrovolts(lsb));
}
