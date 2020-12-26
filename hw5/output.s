_start_MAIN:
    addi	sp,sp,-16
	sd	ra,8(sp)
	sd	s0,0(sp)
	addi	s0,sp,16
    lw a0,_a
    call _write_int
    flw fa0,_ffflalalala,t0
    call _write_float
    li	a5,0
	mv	a0,a5
	ld	ra,8(sp)
	ld	s0,0(sp)
	addi	sp,sp,16


	jr	ra

.data
_a:  .word  123
_aa:  .word  0
_abc:  .word  0
_sdnvjebv:  .word  0
_b:  .word  0x3f8e147b
_efvhirw:  .word  0x40166666
.data
_c:  .word  444
_d:  .word  44
_fff:  .word  0x42f6e148
_ffflalalala:  .word  0x47f0a69d

