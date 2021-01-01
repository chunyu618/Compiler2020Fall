.data
_a:  .word  984984
_c:  .word  0
_b:  .word  3651648
_d:  .word  0
_e:  .word  6318312
.text
_f_Prologue:
_start_f:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_f
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
	li	t0,321
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
.text
_f_Epilogue:
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
_frameSize_f:  .word  160
.text
_g_Prologue:
_start_g:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_g
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
	li	t0,321321
	sw	t0,_c,t1
.text
_g_Epilogue:
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
_frameSize_g:  .word  160
.text
_h_Prologue:
_start_h:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_h
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
	li	t0,654
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
.text
_h_Epilogue:
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
_frameSize_h:  .word  160
.text
_i_Prologue:
_start_i:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_i
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
	li	t0,654654
	sw	t0,_d,t1
.text
_i_Epilogue:
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
_frameSize_i:  .word  160
.text
_j_Prologue:
_start_j:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_j
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
	li	t0,987
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
.text
_j_Epilogue:
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
_frameSize_j:  .word  160
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
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_0:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_0
	mv	a0,t0
	call	_write_str
	lw	t0,_c
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_1:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_1
	mv	a0,t0
	call	_write_str
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_2:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_2
	mv	a0,t0
	call	_write_str
	lw	t0,_d
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_3:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_3
	mv	a0,t0
	call	_write_str
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_4:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_4
	mv	a0,t0
	call	_write_str
	jal	_f_Prologue
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_5:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_5
	mv	a0,t0
	call	_write_str
	lw	t0,_c
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_6:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_6
	mv	a0,t0
	call	_write_str
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_7:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_7
	mv	a0,t0
	call	_write_str
	lw	t0,_d
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_8:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_8
	mv	a0,t0
	call	_write_str
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_9:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_9
	mv	a0,t0
	call	_write_str
	jal	_g_Prologue
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_10:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_10
	mv	a0,t0
	call	_write_str
	lw	t0,_c
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_11:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_11
	mv	a0,t0
	call	_write_str
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_12:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_12
	mv	a0,t0
	call	_write_str
	lw	t0,_d
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_13:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_13
	mv	a0,t0
	call	_write_str
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_14:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_14
	mv	a0,t0
	call	_write_str
	jal	_h_Prologue
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_15:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_15
	mv	a0,t0
	call	_write_str
	lw	t0,_c
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_16:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_16
	mv	a0,t0
	call	_write_str
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_17:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_17
	mv	a0,t0
	call	_write_str
	lw	t0,_d
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_18:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_18
	mv	a0,t0
	call	_write_str
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_19:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_19
	mv	a0,t0
	call	_write_str
	jal	_i_Prologue
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_20:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_20
	mv	a0,t0
	call	_write_str
	lw	t0,_c
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_21:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_21
	mv	a0,t0
	call	_write_str
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_22:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_22
	mv	a0,t0
	call	_write_str
	lw	t0,_d
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_23:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_23
	mv	a0,t0
	call	_write_str
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_24:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_24
	mv	a0,t0
	call	_write_str
	jal	_j_Prologue
	li	t2,123
	la	t1,_a
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_25:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_25
	mv	a0,t0
	call	_write_str
	lw	t0,_c
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_26:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_26
	mv	a0,t0
	call	_write_str
	li	t2,456
	la	t1,_b
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_27:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_27
	mv	a0,t0
	call	_write_str
	lw	t0,_d
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_28:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_28
	mv	a0,t0
	call	_write_str
	li	t2,789
	la	t1,_e
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_29:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_29
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
