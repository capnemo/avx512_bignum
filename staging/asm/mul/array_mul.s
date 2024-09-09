	.file	"array_mul.c"
	.text
	.p2align 4
	.globl	mul_array
	.type	mul_array, @function
mul_array:
.LFB6566:
	.cfi_startproc
	vpbroadcastq	%rsi, %zmm0
	vpmuludq	(%rdi), %zmm0, %zmm0
	vmovdqa64	%zmm0, (%rdi)
	vzeroupper
	ret
	.cfi_endproc
.LFE6566:
	.size	mul_array, .-mul_array
	.ident	"GCC: (GNU) 14.2.1 20240805"
	.section	.note.GNU-stack,"",@progbits
