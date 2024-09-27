	.file	"m512_sub.c"
	.text
	.p2align 4
	.globl	avx512_sub
	.type	avx512_sub, @function
avx512_sub:
.LFB6566:
	.cfi_startproc
	vmovdqa64	(%rdi), %zmm0
	vpsubq	(%rsi), %zmm0, %zmm0
	vmovdqa64	%zmm0, (%rdi)
	vzeroupper
	ret
	.cfi_endproc
.LFE6566:
	.size	avx512_sub, .-avx512_sub
	.ident	"GCC: (GNU) 14.1.1 20240720"
	.section	.note.GNU-stack,"",@progbits
