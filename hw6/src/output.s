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
	li	t0,10
	sd	t0,-16(s0)
	li	t0,0
	sw	t0,-24(s0)
_LABEL_0:
	lw	t0,-24(s0)
	lw	t1,-8(s0)
	slt	t0,t0,t1
	beqz	t0,_LABEL_1
	li	t0,0
	sw	t0,-32(s0)
_LABEL_2:
	lw	t0,-32(s0)
	lw	t1,-16(s0)
	slt	t0,t0,t1
	beqz	t0,_LABEL_3
	lw	t0,-32(s0)
	lw	t3,-24(s0)
	addi	t2,s0,-1632
	slli	t3,t3,3
	add	t3,t3,t2
	lw	t1,0(t3)
	slt	t0,t0,t1
	xori	t0,t0,1
	beq	t0,x0,_LABEL_4
	j	_LABEL_5
_LABEL_5:
	lw	t3,-24(s0)
	addi	t4,zero,100
	lw	t5,-32(s0)
	mul	t3,t3,t4
	add	t3,t3,t5
	li	t4,81632
	sub	t4,x0,t4
	add	t2,s0,t4
	slli	t3,t3,3
	add	t3,t3,t2
	lw	t1,0(t3)
	lw	t4,-24(s0)
	li	t5,1
	sub	t4,t4,t5
	addi	t5,zero,100
	lw	t6,-32(s0)
	lw	t-1,-24(s0)
	addi	t-1,s0,-1632
	slli	t-1,t-1,3
	add	t-1,t-1,t-1
	lw	t-1,0(t-1)
	sub	t6,t6,t-1
	mul	t4,t4,t5
	add	t4,t4,t6
	li	t5,81632
	sub	t5,x0,t5
	add	t3,s0,t5
	slli	t4,t4,3
	add	t4,t4,t3
	lw	t2,0(t4)
	lw	t5,-24(s0)
	addi	t4,s0,-832
	slli	t5,t5,3
	add	t5,t5,t4
	lw	t3,0(t5)
	add	t2,t2,t3
	slt	t1,t1,t2
	snez	t0,t1
	j	_LABEL_4
_LABEL_4:
	beqz	t0,_LABEL_6
	lw	t2,-24(s0)
	li	t3,1
	sub	t2,t2,t3
	addi	t3,zero,100
	lw	t4,-32(s0)
	lw	t-1,-24(s0)
	addi	t6,s0,-1632
	slli	t-1,t-1,3
	add	t-1,t-1,t6
	lw	t5,0(t-1)
	sub	t4,t4,t5
	mul	t2,t2,t3
	add	t2,t2,t4
	li	t3,81632
	sub	t3,x0,t3
	add	t1,s0,t3
	slli	t2,t2,3
	add	t2,t2,t1
	lw	t0,0(t2)
	lw	t3,-24(s0)
	addi	t2,s0,-832
	slli	t3,t3,3
	add	t3,t3,t2
	lw	t1,0(t3)
	add	t0,t0,t1
	lw	t2,-24(s0)
	addi	t3,zero,100
	lw	t4,-32(s0)
	mul	t2,t2,t3
	add	t2,t2,t4
	li	t3,81632
	sub	t3,x0,t3
	add	t1,s0,t3
	slli	t2,t2,3
	add	t2,t2,t1
	sw	t0,0(t2)
_LABEL_6:
	lw	t0,-32(s0)
	li	t1,1
	add	t0,t0,t1
	sw	t0,-32(s0)
	j	_LABEL_2
_LABEL_3:
	lw	t0,-24(s0)
	li	t1,1
	add	t0,t0,t1
	sw	t0,-24(s0)
	j	_LABEL_0
_LABEL_1:
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
_frameSize_MAIN:  .word  81792
