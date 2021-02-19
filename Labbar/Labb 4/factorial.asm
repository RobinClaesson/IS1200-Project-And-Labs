.text

	addi $a0, $0, 8		#a0 = n
	addi $a1, $0, 0		#a1 outer loop counter
	addi $v0, $0, 1		#return value to 1

loop: 	beq $a0, $a1, stop	#End if outer loop counter is n
	add $0, $0, $0		#nop
	
	addi $a1, $a1, 1		#increment outer loop counter
	addi $a2, $0, 1		#a2 inner loop counter
	
	addi $a3, $v0, 0		#a3 = v0 "original" value

multi: 	beq $a1, $a2, loop	#Back to outer loop if inner loopcounter is same as outer loop counter
	add $0, $0, $0		#nop

	addi $a2, $a2, 1		#incremet inner loopcounter
	
	add $v0, $v0, $a3	#Increment v0 with original value

	beq $0, $0, multi	#pseudo j to mulit
	add $0, $0, $0		#nop
	
stop: 	beq $0, $0, stop		#pseudo j to stop
	add $0, $0, $0		#nop

