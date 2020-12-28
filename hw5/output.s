.text
_start_MAIN:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	li	t0,1
	fcvt.w.s	ft1,t0,rtz
	lw	t2,-8(s0)
	mv	a0,t2
	call	_write_int
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
