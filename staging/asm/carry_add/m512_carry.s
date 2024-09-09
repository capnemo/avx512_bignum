	.file	"m512_carry.c"
	.text
	.p2align 4
	.globl	carry
	.type	carry, @function
carry:
.LFB6566:
	.cfi_startproc
	vmovdqa32	indexes(%rip), %zmm1
	movl	56(%rdi), %eax
	movl	$0, 56(%rdi)
	vmovdqa64	(%rdi), %zmm0
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vmovdqa32	64+indexes(%rip), %zmm1
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vmovdqa32	128+indexes(%rip), %zmm1
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vmovdqa32	192+indexes(%rip), %zmm1
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vmovdqa32	256+indexes(%rip), %zmm1
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vmovdqa32	320+indexes(%rip), %zmm1
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vmovdqa32	384+indexes(%rip), %zmm1
	vpermd	%zmm0, %zmm1, %zmm1
	vpaddq	%zmm1, %zmm0, %zmm0
	vpandd	mask(%rip), %zmm0, %zmm0
	vmovdqa32	%zmm0, (%rdi)
	movl	%eax, 56(%rdi)
	vzeroupper
	ret
	.cfi_endproc
.LFE6566:
	.size	carry, .-carry
	.globl	indexes
	.data
	.align 64
	.type	indexes, @object
	.size	indexes, 448
indexes:
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542159
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542157
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542155
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542153
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542151
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542149
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542147
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.quad	60129542158
	.globl	zx
	.section	.rodata
	.align 8
	.type	zx, @object
	.size	zx, 8
zx:
	.quad	60129542144
	.globl	zero_loc
	.align 8
	.type	zero_loc, @object
	.size	zero_loc, 8
zero_loc:
	.quad	60129542158
	.globl	mask
	.data
	.align 64
	.type	mask, @object
	.size	mask, 64
mask:
	.quad	-1
	.quad	4294967295
	.quad	4294967295
	.quad	4294967295
	.quad	4294967295
	.quad	4294967295
	.quad	4294967295
	.quad	4294967295
	.globl	full_mask
	.section	.rodata
	.align 8
	.type	full_mask, @object
	.size	full_mask, 8
full_mask:
	.quad	-1
	.globl	half_mask
	.align 8
	.type	half_mask, @object
	.size	half_mask, 8
half_mask:
	.quad	4294967295
	.ident	"GCC: (GNU) 14.1.1 20240720"
	.section	.note.GNU-stack,"",@progbits
