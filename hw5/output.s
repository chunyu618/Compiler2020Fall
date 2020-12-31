.data
_n:  .word  123
_arr1:  .word  984
_arr2:  .word  3648
_arr3:  .word  6312
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
	call	_read_float
	fmv.s	fs0,fa0
	li	t1,10
	la	t0,_arr1
	slli	t1,t1,3
	add	t1,t1,t0
	fsw	fs0,0(t1)
	call	_read_float
	fmv.s	fs0,fa0
	li	t2,11
	la	t0,_arr1
	slli	t2,t2,3
	add	t2,t2,t0
	fsw	fs0,0(t2)
	li	t3,10
	la	t0,_arr1
	slli	t3,t3,3
	add	t3,t3,t0
	flw	ft0,0(t3)
	li	t4,11
	la	t0,_arr1
	slli	t4,t4,3
	add	t4,t4,t0
	flw	ft1,0(t4)
	fadd.s	ft0,ft0,ft1
	fsw	ft0,-8(s0)
	flw	ft0,-8(s0)
	fmv.s	fa0,ft0
	call	_write_float
.data
_CONSTANT_0:  .ascii  "\n\0"
.text
	la	t0,_CONSTANT_0
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
_frameSize_MAIN:  .word  168
