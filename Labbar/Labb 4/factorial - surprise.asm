.text

	addi $a0, $0, 5		#a0 = n
	addi $a1, $0, 0		#a1 outer loop counter
	addi $v0, $0, 1		#return value to 1

loop: 	beq $a0, $a1, stop	#End if outer loop counter is n
	add $0, $0, $0		#nop
	
	addi $a1, $a1, 1		#increment outer loop counter
	
	mul $v0, $v0, $a1

	beq $0, $0, loop		#pseudo j to mulit
	add $0, $0, $0		#nop
	
stop: 	beq $0, $0, stop		#pseudo j to stop
	add $0, $0, $0		#nop

