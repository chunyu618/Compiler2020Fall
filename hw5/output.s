_start_MAIN:
    addi	sp,sp,-16
	sd	ra,8(sp)
	sd	s0,0(sp)
	addi	s0,sp,16
    la  a0,helloworld
    call _write_str
    li	a5,0
	mv	a0,a5
	ld	ra,8(sp)
	ld	s0,0(sp)
	addi	sp,sp,16

    addi	sp,sp,-16
	sd	ra,8(sp)
	sd	s0,0(sp)
	addi	s0,sp,16
    la  a0,newLine
    call _write_str
    li	a5,0
	mv	a0,a5
	ld	ra,8(sp)
	ld	s0,0(sp)
	addi	sp,sp,16


	jr	ra

.data 
helloworld:  .ascii "Hello Worldfvhiwebvhwbrjvkbweihvbwefkjvbefjkbvhirwebvefjwkbvjekrwbvebrwjkv\0"
newLine: .ascii "1\n2\n"
