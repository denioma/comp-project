	.text
	.file	"out.ll"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function main
.LCPI0_0:
	.quad	4621888540577227436     # double 10.12332
.LCPI0_1:
	.quad	4607182432310816290     # double 1.000003
.LCPI0_2:
	.quad	4609336702170971203     # double 1.4783470000000001
.LCPI0_3:
	.quad	-9223372036854775808    # double -0
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	$10, 12(%rsp)
	movabsq	$4621888540577227436, %rax # imm = 0x40243F23CC8DE2AC
	movq	%rax, 16(%rsp)
	movsd	.LCPI0_0(%rip), %xmm0   # xmm0 = mem[0],zero
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	movl	12(%rsp), %esi
	movl	$.L.int, %edi
	xorl	%eax, %eax
	callq	printf
	movsd	.LCPI0_1(%rip), %xmm0   # xmm0 = mem[0],zero
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	movsd	.LCPI0_2(%rip), %xmm0   # xmm0 = mem[0],zero
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	movl	$.L.int, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	callq	printf
	movl	$.L.int, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	callq	printf
	movsd	.LCPI0_3(%rip), %xmm0   # xmm0 = mem[0],zero
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	movl	$.L.str.0, %edi
	callq	puts
	movl	$.L.str.1, %edi
	callq	puts
	movl	$.L.str.2, %edi
	callq	puts
	movl	$.L.str.3, %edi
	callq	puts
	movl	$.L.str.4, %edi
	callq	puts
	movl	$.L.str.5, %edi
	callq	puts
	movl	$.L.str.6, %edi
	callq	puts
	movl	$.L.str.7, %edi
	callq	puts
	movl	$.L.str.8, %edi
	callq	puts
	movl	$.L.str.9, %edi
	callq	puts
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.type	b,@object               # @b
	.bss
	.globl	b
b:
	.byte	0                       # 0x0
	.size	b, 1

	.type	a,@object               # @a
	.globl	a
	.p2align	2
a:
	.long	0                       # 0x0
	.size	a, 4

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%s\n"
	.size	.L.str, 4

	.type	.L.int,@object          # @.int
.L.int:
	.asciz	"%d\n"
	.size	.L.int, 4

	.type	.L.double,@object       # @.double
.L.double:
	.asciz	"%.08f\n"
	.size	.L.double, 7

	.type	.L.true,@object         # @.true
.L.true:
	.asciz	"true"
	.size	.L.true, 5

	.type	.L.false,@object        # @.false
.L.false:
	.asciz	"false"
	.size	.L.false, 6

	.type	.L.str.0,@object        # @.str.0
.L.str.0:
	.asciz	"sdjfhighi"
	.size	.L.str.0, 10

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"\t tab"
	.size	.L.str.1, 6

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"\n newline"
	.size	.L.str.2, 10

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"\\ barra \\"
	.size	.L.str.3, 10

	.type	.L.str.4,@object        # @.str.4
.L.str.4:
	.asciz	"\f feed"
	.size	.L.str.4, 7

	.type	.L.str.5,@object        # @.str.5
.L.str.5:
	.asciz	"\r carriage"
	.size	.L.str.5, 11

	.type	.L.str.6,@object        # @.str.6
.L.str.6:
	.asciz	"\"aspazita"
	.size	.L.str.6, 10

	.type	.L.str.7,@object        # @.str.7
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4
.L.str.7:
	.asciz	"\f varios \t  \\ \n \""
	.size	.L.str.7, 18

	.type	.L.str.8,@object        # @.str.8
	.p2align	4
.L.str.8:
	.asciz	"cena do printf %s %d %l %x"
	.size	.L.str.8, 27

	.type	.L.str.9,@object        # @.str.9
	.p2align	4
.L.str.9:
	.asciz	"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis quis blandit tellus. Curabitur ornare elementum mollis. Pellentesque eleifend tempor justo in facilisis. Phasellus ut mattis ex. Etiam et imperdiet nisl. Nam sit amet justo ante. Proin semper varius luctus. Quisque a lorem neque. Ut nec augue eu sapien interdum euismod at nec urna. Nulla et nunc sodales, pulvinar erat in, sagittis quam. Duis vulputate augue finibus ligula mattis congue. Maecenas condimentum nunc ante, non facilisis est aliquam sed. Donec vel ex nec est pharetra aliquam a quis dui. Aenean ac quam hendrerit, suscipit orci et, vestibulum quam. Donec volutpat hendrerit mi sed placerat. Donec dignissim sapien feugiat felis placerat cursus. Suspendisse mattis magna non risus ornare, a auctor nisl consequat. Nam viverra risus ac dui pharetra, id posuere massa mattis. Nullam at pellentesque neque. Curabitur tempus molestie mi ac ornare. Aenean tortor nisl, condimentum eget ligula a, consequat bibendum justo. Donec nec viverra massa. "
	.size	.L.str.9, 1019


	.section	".note.GNU-stack","",@progbits
