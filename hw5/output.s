.data
_r:  .word 0x00000000
_pi:  .word 0x00000000
.text
_Calc_Prologue:
_start_Calc:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_Calc
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
	flw	ft0,_r,t0
	flw	ft1,_r,t0
	fmul.s	ft0,ft0,ft1
	flw	ft1,_pi,t0
	fmul.s	ft0,ft0,ft1
	fsw	ft0,-8(s0)
	flw	ft0,-8(s0)
	fmv.s	fa0,ft0
	j	_Calc_Epilogue
.text
_Calc_Epilogue:
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
_frameSize_Calc:  .word  168
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
.data
_CONSTANT_0:  .word  0x40490fd0
.text
	flw	ft0,_CONSTANT_0,t0
	fsw	ft0,_pi,t0
.data
_CONSTANT_1:  .word  0x3f800000
.text
	flw	ft0,_CONSTANT_1,t0
	fsw	ft0,_r,t0
	fsw	fs0,0(sp)
	addi	sp,sp,-8
	jal	_Calc_Prologue
	addi	sp,sp,8
	flw	fs0,0(sp)
	fmv.s	fs0,fa0
	fmv.s	fa0,fs0
	call	_write_float
.data
_CONSTANT_2:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_2
	mv	a0,t0
	call	_write_str
.data
_CONSTANT_3:  .word  0000000000
.text
	flw	ft0,_CONSTANT_3,t0
	fsw	ft0,_r,t0
	fsw	fs0,0(sp)
	addi	sp,sp,-8
	jal	_Calc_Prologue
	addi	sp,sp,8
	flw	fs0,0(sp)
	fmv.s	fs0,fa0
	fmv.s	fa0,fs0
	call	_write_float
.data
_CONSTANT_4:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_4
	mv	a0,t0
	call	_write_str
	flw	ft0,_pi,t0
	fsw	ft0,_r,t0
	fsw	fs0,0(sp)
	addi	sp,sp,-8
	jal	_Calc_Prologue
	addi	sp,sp,8
	flw	fs0,0(sp)
	fmv.s	fs0,fa0
	fmv.s	fa0,fs0
	call	_write_float
.data
_CONSTANT_5:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_5
	mv	a0,t0
	call	_write_str
.data
_CONSTANT_6:  .word  0x51ba43b7
.text
	flw	ft0,_CONSTANT_6,t0
	fsw	ft0,_r,t0
	fsw	fs0,0(sp)
	addi	sp,sp,-8
	jal	_Calc_Prologue
	addi	sp,sp,8
	flw	fs0,0(sp)
	fmv.s	fs0,fa0
	fmv.s	fa0,fs0
	call	_write_float
.data
_CONSTANT_7:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_7
	mv	a0,t0
	call	_write_str
.data
_CONSTANT_8:  .word  0x4e92c639
.text
	flw	ft0,_CONSTANT_8,t0
	fsw	ft0,_r,t0
	fsw	fs0,0(sp)
	addi	sp,sp,-8
	jal	_Calc_Prologue
	addi	sp,sp,8
	flw	fs0,0(sp)
	fmv.s	fs0,fa0
	fmv.s	fa0,fs0
	call	_write_float
.data
_CONSTANT_9:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_9
	mv	a0,t0
	call	_write_str
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
_frameSize_MAIN:  .word  160
