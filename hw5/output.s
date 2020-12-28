.data
_gi:  .word  12
_gf:  .word  0x4151999a
.text
_start_MAIN:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	flw	ft0,_gf,t0
	fcvt.w.s	t0,ft0,rtz
	not	t0,t0
    mv a0,t0
    call _write_int
	sd	t0,-8(s0)
	lw	t0,_gi
	not	t0,t0
    mv a0,t0
    call _write_int
	sd	t0,-16(s0)
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
