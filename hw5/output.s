_Prologue_start:
.text
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
	sw	a0,160(sp)
	sw	a1,168(sp)
	sw	a2,176(sp)
	sw	a3,184(sp)
	sw	a4,192(sp)
	sw	a5,200(sp)
	sw	a6,208(sp)
	sw	a7,216(sp)
	fsw	fa0,224(sp)
	fsw	fa1,232(sp)
	fsw	fa2,240(sp)
	fsw	fa3,248(sp)
	fsw	fa4,256(sp)
	fsw	fa5,264(sp)
	fsw	fa6,272(sp)
	fsw	fa7,280(sp)
	li	t0,1
	sd	t0,-8(s0)
	lw	t0,-8(s0)
	li	t1,0
	xor	t0,t0,t1
	seqz	t0,t0
	beqz	t0,_LABEL_0
.data
_CONSTANT_0:  .ascii  "In If Node\n\0"
.text
	la	t0,_CONSTANT_0
	mv	a0,t0
	call	_write_str
	j	_LABEL_1
_LABEL_0:
.data
_CONSTANT_1:  .ascii  "In else Node\n\0"
.text
	la	t0,_CONSTANT_1
	mv	a0,t0
	call	_write_str
	j	_LABEL_1
_LABEL_1:
.data
_CONSTANT_2:  .ascii  "finish\n\0"
.text
	la	t0,_CONSTANT_2
	mv	a0,t0
	call	_write_str
_Epilogue_start:
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
	lw	a0,160(sp)
	lw	a1,168(sp)
	lw	a2,176(sp)
	lw	a3,184(sp)
	lw	a4,192(sp)
	lw	a5,200(sp)
	lw	a6,208(sp)
	lw	a7,216(sp)
	flw	fa0,224(sp)
	flw	fa1,232(sp)
	flw	fa2,240(sp)
	flw	fa3,248(sp)
	flw	fa4,256(sp)
	flw	fa5,264(sp)
	flw	fa6,272(sp)
	flw	fa7,280(sp)
	ld	ra,8(fp)
	mv	sp,fp
	addi	sp,sp,8
	ld	fp,0(fp)
	jr	ra
.data
_frameSize_MAIN:  .word  296
