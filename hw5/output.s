.data
_gi:  .word  12
_gf:  .word  0x4151999a
.text
_start_MAIN:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	li	t0,123
	sd	t0,-8(s0)
	lw	t0,-8(s0)
	li	t1,123
	xor	t0,t0,t1
	seqz	t2,t0
	beq	t2,x0,_LABEL_0
	j	_LABEL_1
_LABEL_1:
	lw	t3,-8(s0)
	li	t4,456
	xor	t3,t3,t4
	seqz	t5,t3
	and	t2,t2,t5
	j	_LABEL_0
_LABEL_0:
	and	t1,t2,1
	sd	t1,-16(s0)
	lw	t1,-16(s0)
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
