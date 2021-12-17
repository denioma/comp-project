	.text
	.file	"out.ll"
	.globl	fca                     # -- Begin function fca
	.p2align	4, 0x90
	.type	fca,@function
fca:                                    # @fca
	.cfi_startproc
# %bb.0:
	xorl	%eax, %eax
	retq
.Lfunc_end0:
	.size	fca, .Lfunc_end0-fca
	.cfi_endproc
                                        # -- End function
	.globl	fcb                     # -- Begin function fcb
	.p2align	4, 0x90
	.type	fcb,@function
fcb:                                    # @fcb
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$132, %esi
	addl	ia(%rip), %esi
	movl	%esi, 4(%rsp)
	movl	%esi, ia(%rip)
	movl	$.L.int, %edi
	xorl	%eax, %eax
	callq	printf
	movl	4(%rsp), %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	fcb, .Lfunc_end1-fcb
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function fcd
.LCPI2_0:
	.quad	4607213943997409001     # double 1.0069999999999999
	.text
	.globl	fcd
	.p2align	4, 0x90
	.type	fcd,@function
fcd:                                    # @fcd
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.L.str.1, %edi
	callq	puts
	movsd	.LCPI2_0(%rip), %xmm0   # xmm0 = mem[0],zero
	popq	%rax
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	fcd, .Lfunc_end2-fcd
	.cfi_endproc
                                        # -- End function
	.globl	fce                     # -- Begin function fce
	.p2align	4, 0x90
	.type	fce,@function
fce:                                    # @fce
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movq	(%rsp), %rdi
	callq	puts
	movq	(%rsp), %rax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	fce, .Lfunc_end3-fce
	.cfi_endproc
                                        # -- End function
	.globl	fcf                     # -- Begin function fcf
	.p2align	4, 0x90
	.type	fcf,@function
fcf:                                    # @fcf
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movb	$1, 7(%rsp)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB4_2
# %bb.1:                                # %true1
	movl	$.L.true, %edi
	jmp	.LBB4_3
.LBB4_2:                                # %false1
	movl	$.L.false, %edi
.LBB4_3:                                # %print1
	callq	puts
	movb	7(%rsp), %al
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	fcf, .Lfunc_end4-fcf
	.cfi_endproc
                                        # -- End function
	.globl	fcg                     # -- Begin function fcg
	.p2align	4, 0x90
	.type	fcg,@function
fcg:                                    # @fcg
	.cfi_startproc
# %bb.0:
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, 12(%rsp)
	movsd	%xmm0, 16(%rsp)
	callq	fcd
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	movl	$.L.str.2, %edi
	callq	puts
	movsd	16(%rsp), %xmm0         # xmm0 = mem[0],zero
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	callq	fcd
	ucomisd	16(%rsp), %xmm0
	jbe	.LBB5_2
# %bb.1:                                # %if0
	callq	fca
	movl	$.L.str.3, %edi
	callq	puts
	movl	12(%rsp), %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.LBB5_2:                                # %fi0
	.cfi_def_cfa_offset 32
	movl	$.L.str.4, %edi
	callq	puts
	callq	fcb
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end5:
	.size	fcg, .Lfunc_end5-fcg
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function fch
.LCPI6_0:
	.quad	4576918229304087675     # double 0.01
	.text
	.globl	fch
	.p2align	4, 0x90
	.type	fch,@function
fch:                                    # @fch
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movl	%edi, 12(%rsp)
	movsd	%xmm0, 24(%rsp)
	movq	%rsi, 16(%rsp)
	callq	fcb
	movl	%eax, %ebx
	callq	fcd
	movl	%ebx, %edi
	callq	fcg
	movq	16(%rsp), %rdi
	callq	puts
	cmpb	$1, bb(%rip)
	jne	.LBB6_2
# %bb.1:                                # %if1
	movl	ia(%rip), %eax
	addl	ib(%rip), %eax
	movl	%eax, 12(%rsp)
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	movsd	%xmm0, 24(%rsp)
	movsd	%xmm0, fb(%rip)
	movl	%eax, id(%rip)
.LBB6_2:                                # %fi1
	movsd	.LCPI6_0(%rip), %xmm0   # xmm0 = mem[0],zero
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end6:
	.size	fch, .Lfunc_end6-fch
	.cfi_endproc
                                        # -- End function
	.globl	fci                     # -- Begin function fci
	.p2align	4, 0x90
	.type	fci,@function
fci:                                    # @fci
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -16
	movl	%edi, 20(%rsp)
	movl	%esi, 16(%rsp)
	movsd	%xmm0, 40(%rsp)
	movsd	%xmm1, 32(%rsp)
	movq	%rdx, 24(%rsp)
	callq	fcb
	movl	%eax, %ebx
	callq	fcd
	movl	%ebx, %edi
	callq	fcg
	movl	%eax, %ebx
	callq	fcd
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fce
	movl	%ebx, %edi
	movsd	8(%rsp), %xmm0          # 8-byte Reload
                                        # xmm0 = mem[0],zero
	movq	%rax, %rsi
	callq	fch
	movl	16(%rsp), %ebx
	addl	ic(%rip), %ebx
	callq	fcb
                                        # kill: def $eax killed $eax def $rax
	leal	12(%rax,%rbx), %eax
	movl	%eax, 20(%rsp)
	callq	fcf
	testb	$1, %al
	je	.LBB7_3
# %bb.1:                                # %if2
	movq	24(%rsp), %rdi
	callq	puts
	movl	ia(%rip), %edi
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	movl	$sa, %esi
	callq	fch
	jmp	.LBB7_2
.LBB7_3:                                # %else2
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	mulsd	32(%rsp), %xmm0
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fcd
	addsd	8(%rsp), %xmm0          # 8-byte Folded Reload
	movsd	%xmm0, 40(%rsp)
	callq	fcd
.LBB7_2:                                # %if2
	addq	$48, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end7:
	.size	fci, .Lfunc_end7-fci
	.cfi_endproc
                                        # -- End function
	.globl	fcj                     # -- Begin function fcj
	.p2align	4, 0x90
	.type	fcj,@function
fcj:                                    # @fcj
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	subq	$48, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movl	%edi, 28(%rsp)
	movl	%esi, 24(%rsp)
	movsd	%xmm0, 40(%rsp)
	movsd	%xmm1, 32(%rsp)
	andl	$1, %edx
	movb	%dl, 15(%rsp)
	movl	$0, 20(%rsp)
	movl	ia(%rip), %ecx
	addl	ib(%rip), %ecx
	movl	ic(%rip), %eax
	imull	id(%rip), %eax
	cltd
	idivl	ie(%rip)
	subl	%edx, %ecx
	movl	%ecx, 16(%rsp)
	movl	$.L.str.5, %edi
	callq	puts
	movl	16(%rsp), %esi
	movl	$.L.int, %edi
	xorl	%eax, %eax
	callq	printf
	movl	16(%rsp), %ebx
	callq	fcb
	movl	%eax, %r14d
	movl	20(%rsp), %ebp
	callq	fcb
	movsd	fb(%rip), %xmm0         # xmm0 = mem[0],zero
	movl	$sa, %esi
	movl	%eax, %edi
	callq	fch
	movsd	fd(%rip), %xmm1         # xmm1 = mem[0],zero
	movl	$sb, %edx
	movl	%r14d, %edi
	movl	%ebp, %esi
	callq	fci
	movl	%ebx, %edi
	callq	fcg
	cmpl	%eax, %ebx
	jle	.LBB8_3
# %bb.1:                                # %if3
	cmpb	$1, ba(%rip)
	jne	.LBB8_6
# %bb.2:                                # %if4
	movabsq	$4576918229304087675, %rax # imm = 0x3F847AE147AE147B
	movq	%rax, fc(%rip)
	jmp	.LBB8_3
.LBB8_6:                                # %else4
	movsd	fc(%rip), %xmm0         # xmm0 = mem[0],zero
	addsd	fe(%rip), %xmm0
	movsd	%xmm0, fc(%rip)
	cmpb	$1, 15(%rsp)
	jne	.LBB8_3
	jmp	.LBB8_7
	.p2align	4, 0x90
.LBB8_8:                                # %loop1
                                        #   in Loop: Header=BB8_7 Depth=1
	movl	16(%rsp), %ebx
	callq	fcb
	cltq
	imulq	$715827883, %rax, %rbp  # imm = 0x2AAAAAAB
	movq	%rbp, %rax
	shrq	$63, %rax
	sarq	$33, %rbp
	addl	%eax, %ebp
	addl	%ebx, %ebp
	movl	ia(%rip), %edi
	addl	28(%rsp), %edi
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	callq	fcg
	addl	%ebp, %eax
	subl	24(%rsp), %eax
	movl	%eax, 20(%rsp)
	movsd	40(%rsp), %xmm0         # xmm0 = mem[0],zero
	movsd	%xmm0, 32(%rsp)
.LBB8_7:                                # %cfor1
                                        # =>This Inner Loop Header: Depth=1
	callq	fcf
	testb	$1, %al
	jne	.LBB8_8
	jmp	.LBB8_3
	.p2align	4, 0x90
.LBB8_5:                                # %if6
                                        #   in Loop: Header=BB8_3 Depth=1
	movzbl	ba(%rip), %ebx
	movzbl	bd(%rip), %eax
	andb	%bl, %al
	andb	$1, %al
	movb	%al, bd(%rip)
	callq	fcf
	andb	%bl, %al
	movzbl	bc(%rip), %ecx
	notb	%cl
	andb	bd(%rip), %cl
	orb	%al, %cl
	andb	$1, %cl
	movb	%cl, bd(%rip)
.LBB8_3:                                # %cfor2
                                        # =>This Inner Loop Header: Depth=1
	cmpb	$1, bb(%rip)
	jne	.LBB8_9
# %bb.4:                                # %loop2
                                        #   in Loop: Header=BB8_3 Depth=1
	cmpb	$1, bc(%rip)
	jne	.LBB8_3
	jmp	.LBB8_5
.LBB8_9:                                # %pool2
	movl	$-101, %eax
	addq	$48, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end8:
	.size	fcj, .Lfunc_end8-fcj
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function main
.LCPI9_0:
	.quad	4591870180066957722     # double 0.10000000000000001
.LCPI9_1:
	.quad	4638378360837603590     # double 123.321
.LCPI9_2:
	.quad	-4452010031096791040    # double -1.0E+11
.LCPI9_3:
	.quad	4936209963552724370     # double 1.0E+22
.LCPI9_4:
	.quad	4771362005757984768     # double 1.0E+11
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	subq	$32, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movl	$1, ia(%rip)
	movl	$1, ib(%rip)
	movl	$1, ic(%rip)
	movl	$1, id(%rip)
	movl	$1, ie(%rip)
	movabsq	$4591870180066957722, %rax # imm = 0x3FB999999999999A
	movq	%rax, fa(%rip)
	movq	%rax, fb(%rip)
	movq	%rax, fc(%rip)
	movq	%rax, fd(%rip)
	movq	%rax, fe(%rip)
	movsd	.LCPI9_0(%rip), %xmm0   # xmm0 = mem[0],zero
	movl	$.L.double, %edi
	movb	$1, %al
	callq	printf
	movl	$0, 24(%rsp)
	movl	ia(%rip), %eax
	addl	ib(%rip), %eax
	movl	%eax, 28(%rsp)
	jmp	.LBB9_1
	.p2align	4, 0x90
.LBB9_6:                                # %cfor3
                                        #   in Loop: Header=BB9_1 Depth=1
	movl	$.L.int, %edi
	xorl	%eax, %eax
	movl	%ecx, %esi
	callq	printf
.LBB9_1:                                # %cfor3
                                        # =>This Inner Loop Header: Depth=1
	movl	ia(%rip), %ebx
	movl	ib(%rip), %ebp
	callq	fcf
	cmpl	%ebp, %ebx
	jle	.LBB9_7
# %bb.2:                                # %cfor3
                                        #   in Loop: Header=BB9_1 Depth=1
	testb	$1, %al
	je	.LBB9_7
# %bb.3:                                # %loop3
                                        #   in Loop: Header=BB9_1 Depth=1
	callq	fcb
	movl	%eax, %r14d
	callq	fcb
	movsd	fe(%rip), %xmm0         # xmm0 = mem[0],zero
	movl	%eax, %edi
	callq	fcg
	movl	%eax, %r12d
	movl	28(%rsp), %r15d
	callq	fcb
	movl	%eax, %ebx
	callq	fcd
	movl	%ebx, %edi
	callq	fcg
	movl	%eax, %ebx
	movl	ia(%rip), %ebp
	callq	fcd
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fce
	movl	%ebp, %edi
	movsd	8(%rsp), %xmm0          # 8-byte Reload
                                        # xmm0 = mem[0],zero
	movq	%rax, %rsi
	callq	fch
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fcd
	movaps	%xmm0, %xmm1
	movl	$123, %esi
	movl	$sc, %edx
	movl	%ebx, %edi
	movsd	8(%rsp), %xmm0          # 8-byte Reload
                                        # xmm0 = mem[0],zero
	callq	fci
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fcd
	movsd	%xmm0, 16(%rsp)         # 8-byte Spill
	callq	fcf
	movzbl	%al, %edx
	movl	%r12d, %edi
	movl	%r15d, %esi
	movsd	8(%rsp), %xmm0          # 8-byte Reload
                                        # xmm0 = mem[0],zero
	movsd	16(%rsp), %xmm1         # 8-byte Reload
                                        # xmm1 = mem[0],zero
	callq	fcj
	cmpl	%eax, %r14d
	jle	.LBB9_5
# %bb.4:                                # %if7
                                        #   in Loop: Header=BB9_1 Depth=1
	movl	ia(%rip), %esi
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	movzbl	ba(%rip), %edx
	movl	$12, %edi
	movsd	.LCPI9_4(%rip), %xmm1   # xmm1 = mem[0],zero
	callq	fcj
	movl	%eax, %ecx
	movl	%eax, 24(%rsp)
	jmp	.LBB9_6
	.p2align	4, 0x90
.LBB9_5:                                # %else7
                                        #   in Loop: Header=BB9_1 Depth=1
	movl	ia(%rip), %ebx
	callq	fcb
	movl	%eax, %ecx
	addl	%ebx, %ecx
	movl	%ecx, 24(%rsp)
	jmp	.LBB9_6
.LBB9_7:                                # %pool3
	movl	24(%rsp), %r15d
	callq	fcb
                                        # kill: def $eax killed $eax def $rax
	leal	456(%rax), %ebx
	callq	fcd
	addsd	.LCPI9_1(%rip), %xmm0
	movl	%ebx, %edi
	callq	fcg
                                        # kill: def $eax killed $eax def $rax
	leal	123(%rax), %r14d
	movl	ia(%rip), %ebp
	addl	ib(%rip), %ebp
	movslq	ic(%rip), %rax
	imulq	$715827883, %rax, %rax  # imm = 0x2AAAAAAB
	movq	%rax, %rcx
	shrq	$63, %rcx
	sarq	$33, %rax
	addl	%ecx, %eax
	subl	%eax, %ebp
	movl	id(%rip), %ebx
	imull	ie(%rip), %ebx
	callq	fcb
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	movl	%eax, %edi
	callq	fcg
	movl	%eax, %ecx
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	addl	%edx, %ebp
	movl	ia(%rip), %ebx
	addl	ib(%rip), %ebx
	movsd	fa(%rip), %xmm0         # xmm0 = mem[0],zero
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fcd
	divsd	.LCPI9_2(%rip), %xmm0
	addsd	8(%rsp), %xmm0          # 8-byte Folded Reload
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fcd
	movsd	%xmm0, 16(%rsp)         # 8-byte Spill
	callq	fce
	movl	$123, %esi
	movl	%ebx, %edi
	movsd	8(%rsp), %xmm0          # 8-byte Reload
                                        # xmm0 = mem[0],zero
	movsd	16(%rsp), %xmm1         # 8-byte Reload
                                        # xmm1 = mem[0],zero
	movq	%rax, %rdx
	callq	fci
	movsd	%xmm0, 8(%rsp)          # 8-byte Spill
	callq	fcd
	addsd	.LCPI9_3(%rip), %xmm0
	movsd	%xmm0, 16(%rsp)         # 8-byte Spill
	callq	fce
	movl	$324, %edi              # imm = 0x144
	movsd	16(%rsp), %xmm0         # 8-byte Reload
                                        # xmm0 = mem[0],zero
	movq	%rax, %rsi
	callq	fch
	movsd	%xmm0, 16(%rsp)         # 8-byte Spill
	callq	fcf
	andb	ba(%rip), %al
	orb	bb(%rip), %al
	movzbl	%al, %edx
	movl	%r14d, %edi
	movl	%ebp, %esi
	movsd	8(%rsp), %xmm0          # 8-byte Reload
                                        # xmm0 = mem[0],zero
	movsd	16(%rsp), %xmm1         # 8-byte Reload
                                        # xmm1 = mem[0],zero
	callq	fcj
	movl	%eax, %ecx
	addl	%r15d, %ecx
	movl	%ecx, 28(%rsp)
	movl	$.L.int, %edi
	xorl	%eax, %eax
	movl	%ecx, %esi
	callq	printf
	movl	$.L.str.6, %edi
	callq	puts
	movl	$.L.str.7, %edi
	callq	puts
	movl	$.L.str.8, %edi
	callq	puts
	movl	$.L.str.9, %edi
	callq	puts
	movl	$.L.str.10, %edi
	callq	puts
	movl	$.L.str.11, %edi
	callq	puts
	movl	$.L.str.12, %edi
	callq	puts
	movl	$.L.str.13, %edi
	callq	puts
	xorl	%eax, %eax
	addq	$32, %rsp
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end9:
	.size	main, .Lfunc_end9-main
	.cfi_endproc
                                        # -- End function
	.type	ia,@object              # @ia
	.bss
	.globl	ia
	.p2align	2
ia:
	.long	0                       # 0x0
	.size	ia, 4

	.type	ib,@object              # @ib
	.globl	ib
	.p2align	2
ib:
	.long	0                       # 0x0
	.size	ib, 4

	.type	ic,@object              # @ic
	.globl	ic
	.p2align	2
ic:
	.long	0                       # 0x0
	.size	ic, 4

	.type	id,@object              # @id
	.globl	id
	.p2align	2
id:
	.long	0                       # 0x0
	.size	id, 4

	.type	ie,@object              # @ie
	.globl	ie
	.p2align	2
ie:
	.long	0                       # 0x0
	.size	ie, 4

	.type	fa,@object              # @fa
	.globl	fa
	.p2align	3
fa:
	.quad	0                       # double 0
	.size	fa, 8

	.type	fb,@object              # @fb
	.globl	fb
	.p2align	3
fb:
	.quad	0                       # double 0
	.size	fb, 8

	.type	fc,@object              # @fc
	.globl	fc
	.p2align	3
fc:
	.quad	0                       # double 0
	.size	fc, 8

	.type	fd,@object              # @fd
	.globl	fd
	.p2align	3
fd:
	.quad	0                       # double 0
	.size	fd, 8

	.type	fe,@object              # @fe
	.globl	fe
	.p2align	3
fe:
	.quad	0                       # double 0
	.size	fe, 8

	.type	ba,@object              # @ba
	.globl	ba
ba:
	.byte	0                       # 0x0
	.size	ba, 1

	.type	bb,@object              # @bb
	.globl	bb
bb:
	.byte	0                       # 0x0
	.size	bb, 1

	.type	bc,@object              # @bc
	.globl	bc
bc:
	.byte	0                       # 0x0
	.size	bc, 1

	.type	bd,@object              # @bd
	.globl	bd
bd:
	.byte	0                       # 0x0
	.size	bd, 1

	.type	sa,@object              # @sa
	.globl	sa
sa:
	.zero	1
	.size	sa, 1

	.type	sb,@object              # @sb
	.globl	sb
sb:
	.zero	1
	.size	sb, 1

	.type	sc,@object              # @sc
	.globl	sc
sc:
	.zero	1
	.size	sc, 1

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
	.asciz	"CANT PRINT...."
	.size	.L.str.0, 15

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"random print"
	.size	.L.str.1, 13

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	">"
	.size	.L.str.2, 2

	.type	.L.str.3,@object        # @.str.3
.L.str.3:
	.asciz	"if true"
	.size	.L.str.3, 8

	.type	.L.str.4,@object        # @.str.4
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4
.L.str.4:
	.asciz	"no prints for you"
	.size	.L.str.4, 18

	.type	.L.str.5,@object        # @.str.5
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.5:
	.asciz	"Pinting"
	.size	.L.str.5, 8

	.type	.L.str.6,@object        # @.str.6
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4
.L.str.6:
	.asciz	"if you print this, you might get 10 points... or not..."
	.size	.L.str.6, 56

	.type	.L.str.7,@object        # @.str.7
	.p2align	4
.L.str.7:
	.asciz	"mooshak is located @ 1st floor @ dei. just saying"
	.size	.L.str.7, 50

	.type	.L.str.8,@object        # @.str.8
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.8:
	.zero	1
	.size	.L.str.8, 1

	.type	.L.str.9,@object        # @.str.9
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4
.L.str.9:
	.asciz	"  /\\/\\   ___   ___  ___| |__   __ _| | __     "
	.size	.L.str.9, 47

	.type	.L.str.10,@object       # @.str.10
	.p2align	4
.L.str.10:
	.asciz	" /    \\ / _ \\ / _ \\/ __| '_ \\ / _` | |/ /   "
	.size	.L.str.10, 45

	.type	.L.str.11,@object       # @.str.11
	.p2align	4
.L.str.11:
	.asciz	"/ /\\/\\ \\ (_) | (_) \\__ \\ | | | (_| |   <   "
	.size	.L.str.11, 44

	.type	.L.str.12,@object       # @.str.12
	.p2align	4
.L.str.12:
	.asciz	"\\/    \\/\\___/ \\___/|___/_| |_|\\__,_|_|\\_\\"
	.size	.L.str.12, 42

	.type	.L.str.13,@object       # @.str.13
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.13:
	.zero	1
	.size	.L.str.13, 1


	.section	".note.GNU-stack","",@progbits
