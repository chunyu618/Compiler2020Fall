.data
_a:  .word  0
_b:  .word  0
_c:  .word  0
_d:  .word  0
_e:  .word  0
_f:  .word  0
.text
_A_Prologue:
_start_A:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_A
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
	lw	t0,_a
	mv	a0,t0
	j	_A_Epilogue
.text
_A_Epilogue:
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
_frameSize_A:  .word  160
.text
_B_Prologue:
_start_B:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_B
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
	lw	t0,_b
	mv	a0,t0
	j	_B_Epilogue
.text
_B_Epilogue:
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
_frameSize_B:  .word  160
.text
_C_Prologue:
_start_C:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_C
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
	lw	t0,_c
	mv	a0,t0
	j	_C_Epilogue
.text
_C_Epilogue:
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
_frameSize_C:  .word  160
.text
_D_Prologue:
_start_D:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_D
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
	lw	t0,_d
	mv	a0,t0
	j	_D_Epilogue
.text
_D_Epilogue:
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
_frameSize_D:  .word  160
.text
_E_Prologue:
_start_E:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_E
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
	lw	t0,_e
	mv	a0,t0
	j	_E_Epilogue
.text
_E_Epilogue:
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
_frameSize_E:  .word  160
.text
_F_Prologue:
_start_F:
	sd	ra,0(sp)
	sd	fp,-8(sp)
	addi	fp,sp,-8
	addi	sp,sp,-16
	la	ra,_frameSize_F
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
	lw	t0,_f
	mv	a0,t0
	j	_F_Epilogue
.text
_F_Epilogue:
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
_frameSize_F:  .word  160
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
	li	t0,4
	sw	t0,_a,t1
	li	t0,3
	sw	t0,_b,t1
	li	t0,2
	sw	t0,_c,t1
	li	t0,1
	sw	t0,_d,t1
	li	t0,5
	sw	t0,_e,t1
	li	t0,6
	sw	t0,_f,t1
	lw	t0,_a
	lw	t1,_b
	add	t0,t0,t1
	lw	t1,_c
	add	t0,t0,t1
	lw	t1,_d
	add	t0,t0,t1
	lw	t1,_e
	add	t0,t0,t1
	lw	t1,_f
	add	t0,t0,t1
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_0:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_0
	mv	a0,t0
	call	_write_str
	lw	t0,_a
	lw	t1,_b
	lw	t2,_c
	add	t1,t1,t2
	mul	t0,t0,t1
	lw	t1,_d
	lw	t2,_e
	mul	t1,t1,t2
	lw	t2,_f
	div	t1,t1,t2
	lw	t2,_f
	mul	t1,t1,t2
	add	t0,t0,t1
	li	t1,123
	add	t0,t0,t1
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_1:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_1
	mv	a0,t0
	call	_write_str
	lw	t0,_a
	beq	t0,x0,_LABEL_0
	j	_LABEL_1
_LABEL_1:
	lw	t1,_b
	lw	t2,_c
	add	t1,t1,t2
	snez	t0,t1
	j	_LABEL_0
_LABEL_0:
	andi	t0,t0,1
	beq	t0,x0,_LABEL_2
	j	_LABEL_3
_LABEL_3:
	lw	t0,_d
	lw	t2,_e
	add	t0,t0,t2
	snez	t0,t0
	j	_LABEL_2
_LABEL_2:
	andi	t0,t0,1
	bne	t0,x0,_LABEL_4
	j	_LABEL_5
_LABEL_5:
	lw	t0,_f
	snez	t0,t0
	j	_LABEL_4
_LABEL_4:
	andi	t0,t0,1
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_2:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_2
	mv	a0,t0
	call	_write_str
	lw	t0,_b
	lw	t2,_b
	div	t0,t0,t2
	lw	t2,_a
	mul	t0,t0,t2
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_3:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_3
	mv	a0,t0
	call	_write_str
	lw	t0,_a
	lw	t2,_b
	div	t0,t0,t2
	lw	t2,_c
	div	t0,t0,t2
	lw	t2,_d
	div	t0,t0,t2
	lw	t2,_e
	div	t0,t0,t2
	lw	t2,_f
	div	t0,t0,t2
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_4:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_4
	mv	a0,t0
	call	_write_str
	lw	t0,_a
	beq	t0,x0,_LABEL_6
	j	_LABEL_7
_LABEL_7:
	lw	t2,_b
	snez	t0,t2
	j	_LABEL_6
_LABEL_6:
	andi	t0,t0,1
	bne	t0,x0,_LABEL_8
	j	_LABEL_9
_LABEL_9:
	lw	t0,_c
	lw	t3,_a
	lw	t4,_d
	lw	t5,_e
	add	t4,t4,t5
	lw	t5,_f
	add	t4,t4,t5
	mul	t3,t3,t4
	li	t4,5
	div	t3,t3,t4
	add	t0,t0,t3
	snez	t0,t0
	j	_LABEL_8
_LABEL_8:
	andi	t0,t0,1
	mv	a0,t0
	call	_write_int
.data
_CONSTANT_5:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_5
	mv	a0,t0
	call	_write_str
	sw	s1,0(sp)
	addi	sp,sp,-8
	jal	_A_Prologue
	addi	sp,sp,8
	lw	s1,0(sp)
	mv	s1,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_B_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	add	s1,s1,s2
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_C_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	add	s1,s1,s2
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_D_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	add	s1,s1,s2
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_E_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	add	s1,s1,s2
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_F_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	add	s1,s1,s2
	mv	a0,s1
	call	_write_int
.data
_CONSTANT_6:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_6
	mv	a0,t0
	call	_write_str
	sw	s1,0(sp)
	addi	sp,sp,-8
	jal	_A_Prologue
	addi	sp,sp,8
	lw	s1,0(sp)
	mv	s1,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_B_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	addi	sp,sp,-24
	jal	_C_Prologue
	addi	sp,sp,24
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	mv	s3,a0
	add	s2,s2,s3
	mul	s1,s1,s2
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_D_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	addi	sp,sp,-24
	jal	_E_Prologue
	addi	sp,sp,24
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	mv	s3,a0
	mul	s2,s2,s3
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	addi	sp,sp,-24
	jal	_F_Prologue
	addi	sp,sp,24
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	mv	s3,a0
	div	s2,s2,s3
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	addi	sp,sp,-24
	jal	_F_Prologue
	addi	sp,sp,24
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	mv	s3,a0
	mul	s2,s2,s3
	add	s1,s1,s2
	li	t0,123
	add	s1,s1,t0
	mv	a0,s1
	call	_write_int
.data
_CONSTANT_7:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_7
	mv	a0,t0
	call	_write_str
	sw	s1,0(sp)
	addi	sp,sp,-8
	jal	_A_Prologue
	addi	sp,sp,8
	lw	s1,0(sp)
	mv	s1,a0
	beq	s1,x0,_LABEL_10
	j	_LABEL_11
_LABEL_11:
	sw	s1,0(sp)
	sw	s2,-8(sp)
	addi	sp,sp,-16
	jal	_B_Prologue
	addi	sp,sp,16
	lw	s1,0(sp)
	lw	s2,-8(sp)
	mv	s2,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	addi	sp,sp,-24
	jal	_C_Prologue
	addi	sp,sp,24
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	mv	s3,a0
	add	s2,s2,s3
	snez	s1,s2
	j	_LABEL_10
_LABEL_10:
	andi	s1,s1,1
	beq	s1,x0,_LABEL_12
	j	_LABEL_13
_LABEL_13:
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	addi	sp,sp,-24
	jal	_D_Prologue
	addi	sp,sp,24
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	mv	s3,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	addi	sp,sp,-32
	jal	_E_Prologue
	addi	sp,sp,32
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	mv	s4,a0
	add	s3,s3,s4
	snez	s1,s3
	j	_LABEL_12
_LABEL_12:
	andi	s1,s1,1
	bne	s1,x0,_LABEL_14
	j	_LABEL_15
_LABEL_15:
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	addi	sp,sp,-32
	jal	_F_Prologue
	addi	sp,sp,32
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	mv	s4,a0
	snez	s1,s4
	j	_LABEL_14
_LABEL_14:
	andi	s1,s1,1
	mv	a0,s1
	call	_write_int
.data
_CONSTANT_8:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_8
	mv	a0,t0
	call	_write_str
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	addi	sp,sp,-32
	jal	_B_Prologue
	addi	sp,sp,32
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	mv	s1,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_B_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	div	s1,s1,s5
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_A_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	mul	s1,s1,s5
	mv	a0,s1
	call	_write_int
.data
_CONSTANT_9:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_9
	mv	a0,t0
	call	_write_str
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	addi	sp,sp,-32
	jal	_A_Prologue
	addi	sp,sp,32
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	mv	s1,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_B_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	div	s1,s1,s5
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_C_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	div	s1,s1,s5
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_D_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	div	s1,s1,s5
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_E_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	div	s1,s1,s5
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_F_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	div	s1,s1,s5
	mv	a0,s1
	call	_write_int
.data
_CONSTANT_10:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_10
	mv	a0,t0
	call	_write_str
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	addi	sp,sp,-32
	jal	_A_Prologue
	addi	sp,sp,32
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	mv	s1,a0
	beq	s1,x0,_LABEL_16
	j	_LABEL_17
_LABEL_17:
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	addi	sp,sp,-40
	jal	_B_Prologue
	addi	sp,sp,40
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	mv	s5,a0
	snez	s1,s5
	j	_LABEL_16
_LABEL_16:
	andi	s1,s1,1
	bne	s1,x0,_LABEL_18
	j	_LABEL_19
_LABEL_19:
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	sw	s6,-40(sp)
	addi	sp,sp,-48
	jal	_C_Prologue
	addi	sp,sp,48
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	lw	s6,-40(sp)
	mv	s6,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	sw	s6,-40(sp)
	sw	s7,-48(sp)
	addi	sp,sp,-56
	jal	_A_Prologue
	addi	sp,sp,56
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	lw	s6,-40(sp)
	lw	s7,-48(sp)
	mv	s7,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	sw	s6,-40(sp)
	sw	s7,-48(sp)
	sw	s8,-56(sp)
	addi	sp,sp,-64
	jal	_D_Prologue
	addi	sp,sp,64
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	lw	s6,-40(sp)
	lw	s7,-48(sp)
	lw	s8,-56(sp)
	mv	s8,a0
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	sw	s6,-40(sp)
	sw	s7,-48(sp)
	sw	s8,-56(sp)
	sw	s9,-64(sp)
	addi	sp,sp,-72
	jal	_E_Prologue
	addi	sp,sp,72
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	lw	s6,-40(sp)
	lw	s7,-48(sp)
	lw	s8,-56(sp)
	lw	s9,-64(sp)
	mv	s9,a0
	add	s8,s8,s9
	sw	s1,0(sp)
	sw	s2,-8(sp)
	sw	s3,-16(sp)
	sw	s4,-24(sp)
	sw	s5,-32(sp)
	sw	s6,-40(sp)
	sw	s7,-48(sp)
	sw	s8,-56(sp)
	sw	s9,-64(sp)
	addi	sp,sp,-72
	jal	_F_Prologue
	addi	sp,sp,72
	lw	s1,0(sp)
	lw	s2,-8(sp)
	lw	s3,-16(sp)
	lw	s4,-24(sp)
	lw	s5,-32(sp)
	lw	s6,-40(sp)
	lw	s7,-48(sp)
	lw	s8,-56(sp)
	lw	s9,-64(sp)
	mv	s9,a0
	add	s8,s8,s9
	mul	s7,s7,s8
	li	t0,5
	div	s7,s7,t0
	add	s6,s6,s7
	snez	s1,s6
	j	_LABEL_18
_LABEL_18:
	andi	s1,s1,1
	mv	a0,s1
	call	_write_int
.data
_CONSTANT_11:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_11
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
