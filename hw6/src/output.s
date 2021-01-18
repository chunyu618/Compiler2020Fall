.text
_MAIN_Prologue:
_start_MAIN:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_MAIN
	lw	ra,0(ra)
	sub	sp,sp,ra
	sw	t0,8(sp)
	sw	t1,16(sp)
	sw	t2,24(sp)
	sw	t3,32(sp)
	sw	t4,40(sp)
	sw	t5,48(sp)
	sw	t6,56(sp)
	fsw	ft0,64(sp)
	fsw	ft1,72(sp)
	fsw	ft2,80(sp)
	fsw	ft3,88(sp)
	fsw	ft4,96(sp)
	fsw	ft5,104(sp)
	fsw	ft6,112(sp)
	fsw	ft7,120(sp)
	fsw	ft8,128(sp)
	fsw	ft9,136(sp)
	fsw	ft10,144(sp)
	fsw	ft11,152(sp)
	li	t0,10
	sd	t0,-8(s0)
	li	t0,0
	sd	t0,-16(s0)
	lw	t0,-8(s0)
	li	t1,0
	div	t0,t0,t1
	beqz	t0,_LABEL_0
.data
_CONSTANT_0:  .ascii  "IF\n\0"
.text
	la	t0,_CONSTANT_0
	mv	a0,t0
	call	_write_str
_LABEL_0:
.text
_MAIN_Epilogue:
	lw	t0,8(sp)
	lw	t1,16(sp)
	lw	t2,24(sp)
	lw	t3,32(sp)
	lw	t4,40(sp)
	lw	t5,48(sp)
	lw	t6,56(sp)
	flw	ft0,64(sp)
	flw	ft1,72(sp)
	flw	ft2,80(sp)
	flw	ft3,88(sp)
	flw	ft4,96(sp)
	flw	ft5,104(sp)
	flw	ft6,112(sp)
	flw	ft7,120(sp)
	flw	ft8,128(sp)
	flw	ft9,136(sp)
	flw	ft10,144(sp)
	flw	ft11,152(sp)
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
.data
_frameSize_MAIN:  .word  176
