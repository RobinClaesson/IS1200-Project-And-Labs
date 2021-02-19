
.text
	
	addi 	$a0, $0, 0
	add 	$v0, $0, $a0
	addi	$a0, $a0, -1
	
loop:	beq	$a0, $0, stop
	nop
	addi	$a2, $v0, 0
	addi	$a0, $a0, -1
	addi	$a1, $a0, 0

multi:	beq	$a1, $0, loop
	nop
	add	$v0, $v0, $a2
	addi	$a1, $a1, -1
	
	beq  	$0, $0, multi
	nop
	
stop:	beq  	$0, $0, stop
	nop
