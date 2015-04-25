	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.section	.text.uart_putchar,"ax",@progbits
.global	uart_putchar
	.type	uart_putchar, @function
uart_putchar:
	push r28
/* prologue: function */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
	mov r28,r24
	cpi r24,lo8(10)
	brne .L3
	ldi r24,lo8(13)
	rcall uart_putchar
.L3:
	lds r24,134
	sbrs r24,5
	rjmp .L3
	sts 128,r28
/* epilogue start */
	pop r28
	ret
	.size	uart_putchar, .-uart_putchar
	.section	.text.thermistorLsbToTemperature,"ax",@progbits
.global	thermistorLsbToTemperature
	.type	thermistorLsbToTemperature, @function
thermistorLsbToTemperature:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	ldi r26,lo8(thermistorPoints)
	ldi r27,hi8(thermistorPoints)
	ldi r30,0
.L9:
	adiw r26,2
	ld r18,X+
	ld r19,X
	sbiw r26,2+1
	cp r24,r18
	cpc r25,r19
	brlt .L8
	subi r30,lo8(-(1))
	adiw r26,4
	cpi r30,lo8(20)
	brne .L9
	ldi r30,lo8(19)
.L8:
	ldi r31,0
	movw r26,r30
	lsl r26
	rol r27
	lsl r26
	rol r27
	subi r26,lo8(-(thermistorPoints-4))
	sbci r27,hi8(-(thermistorPoints-4))
	adiw r26,2
	ld r28,X+
	ld r29,X
	sbiw r26,2+1
	ld r18,X+
	ld r19,X
	lsl r30
	rol r31
	lsl r30
	rol r31
	subi r30,lo8(-(thermistorPoints))
	sbci r31,hi8(-(thermistorPoints))
	ld r20,Z
	ldd r21,Z+1
	movw r22,r24
	sub r22,r28
	sbc r23,r29
	movw r24,r20
	sub r24,r18
	sbc r25,r19
	rcall __mulhi3
	ldd r22,Z+2
	ldd r23,Z+3
	sub r22,r28
	sbc r23,r29
	rcall __divmodhi4
	add r22,r18
	adc r23,r19
	clr r24
	sbrc r23,7
	com r24
	mov r25,r24
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	thermistorLsbToTemperature, .-thermistorLsbToTemperature
	.section	.text.adcReadChannel,"ax",@progbits
.global	adcReadChannel
	.type	adcReadChannel, @function
adcReadChannel:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	out 0x9,r24
	sbi 0x5,6
.L13:
	sbic 0x5,6
	rjmp .L13
	in r24,0x6
	in r25,0x6+1
	ret
	.size	adcReadChannel, .-adcReadChannel
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Thermistor = "
.LC1:
	.string	" -> "
.LC2:
	.string	"."
	.section	.text.startup.main,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0x1a,5
	ldi r24,lo8(uart_output)
	ldi r25,hi8(uart_output)
	sts __iob+2+1,r25
	sts __iob+2,r24
	sbi 0x1a,0
	sts 130,__zero_reg__
	ldi r24,lo8(103)
	sts 129,r24
	ldi r24,lo8(24)
	sts 133,r24
	ldi r24,lo8(6)
	sts 132,r24
	sbi 0x1b,0
	ldi r24,lo8(-123)
	out 0x5,r24
	out 0x8,__zero_reg__
.L16:
	sbi 0x1b,5
	ldi r18,lo8(319999)
	ldi r24,hi8(319999)
	ldi r25,hlo8(319999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ldi r24,lo8(3)
	rcall adcReadChannel
	movw r28,r24
	lds r22,__iob+2
	lds r23,__iob+2+1
	ldi r24,lo8(.LC0)
	ldi r25,hi8(.LC0)
	rcall fputs
	ldi r20,lo8(10)
	ldi r21,0
	ldi r22,lo8(buff)
	ldi r23,hi8(buff)
	movw r24,r28
	rcall itoa
	lds r22,__iob+2
	lds r23,__iob+2+1
	ldi r24,lo8(buff)
	ldi r25,hi8(buff)
	rcall fputs
	lds r22,__iob+2
	lds r23,__iob+2+1
	ldi r24,lo8(.LC1)
	ldi r25,hi8(.LC1)
	rcall fputs
	movw r24,r28
	rcall thermistorLsbToTemperature
	movw r26,r24
	movw r24,r22
	ldi r20,lo8(10)
	ldi r21,0
	ldi r22,lo8(buff)
	ldi r23,hi8(buff)
	rcall itoa
	lds r22,__iob+2
	lds r23,__iob+2+1
	ldi r24,lo8(buff)
	ldi r25,hi8(buff)
	rcall fputs
	ldi r24,lo8(.LC2)
	ldi r25,hi8(.LC2)
	rcall puts
	cbi 0x1b,5
	ldi r18,lo8(319999)
	ldi r24,hi8(319999)
	ldi r25,hlo8(319999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	rjmp .L16
	.size	main, .-main
	.comm	buff,10,1
.global	uart_output
	.section	.data.uart_output,"aw",@progbits
	.type	uart_output, @object
	.size	uart_output, 14
uart_output:
	.zero	3
	.byte	2
	.zero	4
	.word	gs(uart_putchar)
	.word	0
	.word	0
	.section	.rodata.thermistorPoints,"a",@progbits
	.type	thermistorPoints, @object
	.size	thermistorPoints, 88
thermistorPoints:
	.word	850
	.word	129
	.word	800
	.word	146
	.word	750
	.word	165
	.word	700
	.word	186
	.word	650
	.word	210
	.word	600
	.word	236
	.word	550
	.word	267
	.word	500
	.word	300
	.word	450
	.word	337
	.word	400
	.word	376
	.word	350
	.word	419
	.word	300
	.word	464
	.word	200
	.word	559
	.word	250
	.word	511
	.word	150
	.word	608
	.word	100
	.word	656
	.word	50
	.word	703
	.word	0
	.word	748
	.word	-50
	.word	789
	.word	-100
	.word	828
	.word	-150
	.word	862
	.word	-200
	.word	892
	.ident	"GCC: (GNU) 4.9.2"
.global __do_copy_data
.global __do_clear_bss
