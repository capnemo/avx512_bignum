	.file	"m512_vec.c"
	.text
	.p2align 4
	.globl	avx512_add
	.type	avx512_add, @function
avx512_add:
.LFB6566:
	.cfi_startproc
	vmovdqa64	(%rdi), %zmm0
	vpaddq	(%rsi), %zmm0, %zmm0
	vmovdqa64	%zmm0, (%rdi)
	vzeroupper
	ret
	.cfi_endproc
.LFE6566:
	.size	avx512_add, .-avx512_add
	.ident	"GCC: (GNU) 14.1.1 20240720"
	.section	.note.GNU-stack,"",@progbits
