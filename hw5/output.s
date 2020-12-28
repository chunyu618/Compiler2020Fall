.text
_start_MAIN:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	li	t0,8
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_0:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_0
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-4(s0)
	li	t0,2
	sw	t0,-8(s0)
	li	t0,3
	sw	t0,-12(s0)
	lw	t0,-4(s0)
	lw	t1,-8(s0)
	mul	t0,t0,t1
	lw	t1,-12(s0)
	li	t2,4
	mul	t1,t1,t2
	sub	t0,t0,t1
	li	t1,5
	sub	t0,t0,t1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_1:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_1
	mv	a0,t0
	call	_write_str
.data
_CONSTANT_2: .word  0x40e66666
.text
	flw	ft0,_CONSTANT_2,t0
	fsw	ft0,-24(s0)
	flw	ft0,-24(s0)
	fmv.s	fa0,ft0
	call	_write_float
.data
_CONSTANT_3:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_3
	mv	a0,t0
	call	_write_str
.data
_CONSTANT_4:  .word  0x3f800000
.text
	flw	ft0,_CONSTANT_4,t0
	fsw	ft0,-16(s0)
.data
_CONSTANT_5:  .word  0x40000000
.text
	flw	ft0,_CONSTANT_5,t0
	fsw	ft0,-20(s0)
.data
_CONSTANT_6:  .word  0x40400000
.text
	flw	ft0,_CONSTANT_6,t0
	fsw	ft0,-24(s0)
	flw	ft0,-16(s0)
	flw	ft1,-20(s0)
	fmul.s	ft0,ft0,ft1
	flw	ft1,-24(s0)
.data
_CONSTANT_7:  .word  0x40800000
.text
	flw	ft2,_CONSTANT_7,t0
	fmul.s	ft1,ft1,ft2
	fsub.s	ft0,ft0,ft1
.data
_CONSTANT_8:  .word  0x40a00000
.text
	flw	ft1,_CONSTANT_8,t0
	fsub.s	ft0,ft0,ft1
	fsw	ft0,-24(s0)
	flw	ft0,-24(s0)
	fmv.s	fa0,ft0
	call	_write_float
.data
_CONSTANT_9:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_9
	mv	a0,t0
	call	_write_str
	li	t0,0
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_10:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_10
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_11:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_11
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_12:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_12
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_13:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_13
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_14:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_14
	mv	a0,t0
	call	_write_str
	li	t0,0
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_15:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_15
	mv	a0,t0
	call	_write_str
	li	t0,0
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_16:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_16
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_17:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_17
	mv	a0,t0
	call	_write_str
	li	t0,1
	sw	t0,-12(s0)
	lw	t0,-12(s0)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_18:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_18
	mv	a0,t0
	call	_write_str
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
