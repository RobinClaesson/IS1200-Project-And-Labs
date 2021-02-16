  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

.global delay
.global time2string
.global hexasc
.global enable_interrupt

.text
hexasc: 
	andi $t0, $a0, 0xf	#Takes out the 4 lsb in argument
	
	li $t1, 0x9		#Sets check-value to 9
	
	ble $t0, $t1, hexascNumber	#Branches if its 0-9, 
	nop				# delay slot filler (just in case)
	
	addi $v0, $t0, 0x37 		#Offsets with distance to¨0 + distance between numbers and letters
	
	jr $ra				#Return
	nop
	
hexascNumber:

	addi $v0, $t0, 0x30 	#Offsets with distance to¨0 
	
	jr $ra			#Return
	nop
	
	
delay:
	ble $a0, $0, delayend

	addi $a0, $a0, -1
	li $t0, 0
	li $t1, 6500
delayloop: 
	
	addi $t0, $t0, 1
	ble $t0, $t1, delayloop
	nop
	
	bne $a0, $0, delay
	nop
	
delayend:
	jr $ra
	nop
	
	
time2string:	
	PUSH $ra
	PUSH $s0
	PUSH $s1
	PUSH $s2
	
	addi $a0, $a0, 5
	li $t0, 0x00
	sb $t0, 0($a0)	
	
	
	li $s0, 0
	li $s1, 3
	li $s2, 2
	
timeloop:
	addi $a0, $a0, -1
		
	PUSH $a0
	PUSH $a1
	
	add $a0, $a1, $0	
	jal hexasc
	nop
	
	POP $a1
	POP $a0
	
	sb $v0, 0($a0)	
	srl $a1, $a1, 4
	
	addi $s0, $s0, 1
	
	beq $s0, $s2, timeColon
	nop
	
	ble $s0, $s1, timeloop
	nop
	
timeEnd:		
	POP $s2
	POP $s1
	POP $s0		
	POP $ra
	
	jr $ra
	nop
	
timeColon: 
	addi $a0, $a0, -1
	
	li $t0, 0x3a 	#Ändrade ifrån $s0 till $t0
	sb $t0, 0($a0)	
	
	j timeloop
	nop
	
enable_interrupt:
	ei
	jr $ra
	
