.data
_g1:  .word  0
_g2:  .word  0
_f1:  .word 0x00000000
_arr:  .zero  984
_f2:  .word 0x00000000
_f3:  .zero  8000
_large:  .zero  80000000
_x:  .word  0
_y:  .word  0
_z:  .word  0
_i:  .word  0
.text
_Init_Prologue:
_start_Init:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_Init
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
	li	t0,0
	sw	t0,_i,t1
_LABEL_0:
	lw	t0,_i
	li	t1,10000000
	slt	t0,t0,t1
	beqz	t0,_LABEL_1
	lw	t0,_i
	lw	t2,_i
	la	t1,_large
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
	lw	t0,_i
	li	t1,1
	add	t0,t0,t1
	sw	t0,_i,t1
	j	_LABEL_0
_LABEL_1:
.text
_Init_Epilogue:
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
_frameSize_Init:  .word  160
.text
_Print_Prologue:
_start_Print:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_Print
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
	lw	t0,_g1
	mv	a0,t0
	call	_write_int
	lw	t0,_g2
	mv	a0,t0
	call	_write_int
	li	t0,-3
	sw	t0,_g1,t1
	li	t0,3
	sw	t0,_g2,t1
.text
_Print_Epilogue:
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
_frameSize_Print:  .word  160
.text
_Copy_Prologue:
_start_Copy:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_Copy
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
	li	t0,0
	sw	t0,_x,t1
	li	t0,0
	sw	t0,_i,t1
_LABEL_2:
	lw	t0,_i
	li	t1,123
	slt	t0,t0,t1
	beqz	t0,_LABEL_3
	lw	t0,_x
	beqz	t0,_LABEL_4
	lw	t0,_g1
	lw	t2,_i
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
	j	_LABEL_5
_LABEL_4:
	lw	t0,_g2
	lw	t2,_i
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
	j	_LABEL_5
_LABEL_5:
	li	t0,1
	lw	t1,_x
	sub	t0,t0,t1
	sw	t0,_x,t1
	lw	t0,_i
	li	t1,1
	add	t0,t0,t1
	sw	t0,_i,t1
	j	_LABEL_2
_LABEL_3:
.text
_Copy_Epilogue:
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
_frameSize_Copy:  .word  160
.text
_CopyFloat_Prologue:
_start_CopyFloat:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_CopyFloat
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
	li	t0,2
	sw	t0,_i,t1
_LABEL_6:
	lw	t0,_i
	li	t1,1000
	slt	t0,t0,t1
	beqz	t0,_LABEL_7
	lw	t1,_i
	li	t2,2
	sub	t1,t1,t2
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	lw	t1,_i
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	ft0,0(t1)
	lw	t0,_i
	li	t1,1
	add	t0,t0,t1
	sw	t0,_i,t1
	j	_LABEL_6
_LABEL_7:
	li	t1,123
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	fmv.s	fa0,ft0
	call	_write_float
	li	t1,456
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	fmv.s	fa0,ft0
	call	_write_float
	li	t1,123
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	flw	ft1,_f2,t0
	fadd.s	ft0,ft0,ft1
	li	t1,123
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	ft0,0(t1)
	li	t1,456
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	flw	ft1,_f2,t0
	fsub.s	ft0,ft0,ft1
	li	t1,456
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	ft0,0(t1)
.text
_CopyFloat_Epilogue:
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
_frameSize_CopyFloat:  .word  160
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
	jal	_Init_Prologue
	li	t2,123
	la	t1,_large
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
	li	t2,456
	la	t1,_large
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
	li	t0,3
	sw	t0,_g1,t1
	li	t0,-3
	sw	t0,_g2,t1
	jal	_Print_Prologue
	lw	t0,_g1
	mv	a0,t0
	call	_write_int
	lw	t0,_g2
	mv	a0,t0
	call	_write_int
	jal	_Copy_Prologue
	li	t2,1
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
	li	t2,2
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
	li	t2,3
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
	li	t2,121
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
	li	t2,122
	la	t1,_arr
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_0:  .word  0x3f000000
.text
	flw	ft0,_CONSTANT_0,t0
	fsw	ft0,_f2,t0
.data
_CONSTANT_1:  .word  0x3f000000
.text
	flw	ft0,_CONSTANT_1,t0
	li	t1,0
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	ft0,0(t1)
.data
_CONSTANT_2:  .word  0x3fc00000
.text
	flw	ft0,_CONSTANT_2,t0
	li	t1,1
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	ft0,0(t1)
.data
_CONSTANT_3:  .word  0x40200000
.text
	flw	ft0,_CONSTANT_3,t0
	li	t1,2
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	ft0,0(t1)
	jal	_CopyFloat_Prologue
	li	t1,123
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	fmv.s	fa0,ft0
	call	_write_float
	li	t1,456
	la	t0,_f3
	slli	t1,t1,3
	add	t1,t1,t0
	flw	ft0,0(t1)
	fmv.s	fa0,ft0
	call	_write_float
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
