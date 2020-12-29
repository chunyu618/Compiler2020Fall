.text
_start_MAIN:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	li	t0,1
	sd	t0,-4(s0)
_LABEL_1:
	lw	t0,-4(s0)
	li	t1,3
	slt	t1,t1,t0
	beqz	t2,_LABEL_2
	li	t1,1
	mv	a0,t1
	call	_write_int
	lw	t1,-4(s0)
	li	t2,1
	add	t1,t1,t2
	sw	t1,-4(s0)
	j	_LABEL_1
_LABEL_2:
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
