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
	li	t0,1
	sw	t0,-8(s0)
	li	t0,0
	sw	t0,-16(s0)
	lw	t0,-8(s0)
	lw	t1,-16(s0)
	slt	t0,t1,t0
	beqz	t0,_LABEL_0
.data
_CONSTANT_0:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_0
	mv	a0,t0
	call	_write_str
	j	_LABEL_1
_LABEL_0:
.data
_CONSTANT_1:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_1
	mv	a0,t0
	call	_write_str
	j	_LABEL_1
_LABEL_1:
	lw	t0,-8(s0)
	lw	t1,-16(s0)
	slt	t0,t0,t1
	beqz	t0,_LABEL_2
.data
_CONSTANT_2:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_2
	mv	a0,t0
	call	_write_str
	j	_LABEL_3
_LABEL_2:
.data
_CONSTANT_3:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_3
	mv	a0,t0
	call	_write_str
	j	_LABEL_3
_LABEL_3:
	lw	t0,-8(s0)
	lw	t1,-16(s0)
	xor	t0,t0,t1
	seqz	t0,t0
	beqz	t0,_LABEL_4
.data
_CONSTANT_4:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_4
	mv	a0,t0
	call	_write_str
	j	_LABEL_5
_LABEL_4:
.data
_CONSTANT_5:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_5
	mv	a0,t0
	call	_write_str
	j	_LABEL_5
_LABEL_5:
	lw	t0,-8(s0)
	lw	t1,-16(s0)
	xor	t0,t0,t1
	snez	t0,t0
	beqz	t0,_LABEL_6
.data
_CONSTANT_6:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_6
	mv	a0,t0
	call	_write_str
	j	_LABEL_7
_LABEL_6:
.data
_CONSTANT_7:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_7
	mv	a0,t0
	call	_write_str
	j	_LABEL_7
_LABEL_7:
.data
_CONSTANT_8:  .word  0x3f800000
.text
	flw	ft0,_CONSTANT_8,t0
	fsw	ft0,-24(s0)
.data
_CONSTANT_9:  .word  0000000000
.text
	flw	ft0,_CONSTANT_9,t0
	fsw	ft0,-32(s0)
	flw	ft0,-24(s0)
	flw	ft1,-32(s0)
	flt.s	t0,ft1,ft0
	beqz	t0,_LABEL_8
.data
_CONSTANT_10:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_10
	mv	a0,t0
	call	_write_str
	j	_LABEL_9
_LABEL_8:
.data
_CONSTANT_11:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_11
	mv	a0,t0
	call	_write_str
	j	_LABEL_9
_LABEL_9:
	flw	ft0,-24(s0)
	flw	ft1,-32(s0)
	flt.s	t0,ft0,ft1
	beqz	t0,_LABEL_10
.data
_CONSTANT_12:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_12
	mv	a0,t0
	call	_write_str
	j	_LABEL_11
_LABEL_10:
.data
_CONSTANT_13:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_13
	mv	a0,t0
	call	_write_str
	j	_LABEL_11
_LABEL_11:
	flw	ft0,-24(s0)
	flw	ft1,-32(s0)
	feq.s	t0,ft0,ft1
	xori	t0,t0,1
	beqz	t0,_LABEL_12
.data
_CONSTANT_14:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_14
	mv	a0,t0
	call	_write_str
	j	_LABEL_13
_LABEL_12:
.data
_CONSTANT_15:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_15
	mv	a0,t0
	call	_write_str
	j	_LABEL_13
_LABEL_13:
	lw	t0,-8(s0)
	beq	t0,x0,_LABEL_14
	j	_LABEL_15
_LABEL_15:
	lw	t1,-16(s0)
	snez	t0,t1
	j	_LABEL_14
_LABEL_14:
	andi	t0,t0,1
	beqz	t0,_LABEL_16
.data
_CONSTANT_16:  .ascii  "wrong\n\0"
.text
	la	t0,_CONSTANT_16
	mv	a0,t0
	call	_write_str
_LABEL_16:
	lw	t0,-8(s0)
	bne	t0,x0,_LABEL_17
	j	_LABEL_18
_LABEL_18:
	lw	t1,-16(s0)
	snez	t0,t1
	j	_LABEL_17
_LABEL_17:
	andi	t0,t0,1
	beqz	t0,_LABEL_19
.data
_CONSTANT_17:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_17
	mv	a0,t0
	call	_write_str
_LABEL_19:
	li	t0,0
	sw	t0,-8(s0)
_LABEL_20:
	lw	t0,-8(s0)
	li	t2,10
	slt	t0,t0,t2
	beqz	t0,_LABEL_21
.data
_CONSTANT_18:  .ascii  "correct: \0"
.text
	la	t0,_CONSTANT_18
	mv	a0,t0
	call	_write_str
	lw	t0,-8(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_19:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_19
	mv	a0,t0
	call	_write_str
	lw	t0,-8(s0)
	li	t2,1
	add	t0,t0,t2
	sw	t0,-8(s0)
	j	_LABEL_20
_LABEL_21:
	lw	t0,-8(s0)
	li	t2,10
	xor	t0,t0,t2
	seqz	t0,t0
	beqz	t0,_LABEL_22
.data
_CONSTANT_20:  .ascii  "correct\n\0"
.text
	la	t0,_CONSTANT_20
	mv	a0,t0
	call	_write_str
_LABEL_22:
	li	t0,0
	mv	a0,t0
	j	_MAIN_Epilogue
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
_frameSize_MAIN:  .word  192
