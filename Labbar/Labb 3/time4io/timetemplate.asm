  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,1000
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
	
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
	
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
  
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
	ble $a0, $0, delayend #ms <= 0 then end
	nop
				
	addi $a0, $a0, -1
	
	li $t0, 0
	li $t1, 600
delayloop: 
	
	ble $t1, $t0, delay #i >= 4711 then end inner loop
	nop 
		
	addi $t0, $t0, 1
		
	j delayloop
	nop	
	
delayend:
	jr $ra
	nop
	
	
time2string:	
	PUSH($ra)          
	PUSH($s0)
	PUSH($s1)
	PUSH($s2)	
	
	
	andi $a1, $a1, 0xffff	#Masks input to 16 lsb
	beq $a1, $0, timeHour	#if 00:00 brach to write HOUR
	nop
	
	
	addi $a0, $a0, 5		#Move adress to end of text
	li $t0, 0x00		#Sets the last char to NUL
	sb $t0, 0($a0)		
		
	
	li $s0, 0		#Loopcounter
	li $s1, 3		#Max loop
	li $s2, 2		#Exit to colon
	
timeloop:
	addi $a0, $a0, -1	#Counts adresspointer down
		
	PUSH($a0) 		
	PUSH($a1)
	
	add $a0, $a1, $0		#Copies vbalue to $a0 for register
	jal hexasc		#Jups to hexasc
	nop
	
	POP($a1)			
	POP($a0)
	
	sb $v0, 0($a0)		#Stores the returnvalue from hexsc to memory
	srl $a1, $a1, 4		#Rightshifts the time-value to get the next char 
	
	addi $s0, $s0, 1		#Iincreases loopcounter
	
	beq $s0, $s2, timeColon	#If second time, write colon
	nop
	
	ble $s0, $s1, timeloop	#If looped 3 or less times write next char
	nop
	
timeEnd:		
	POP($s2)
	POP($s1)
	POP($s0)		
	POP($ra)
	
	jr $ra
	nop
	
timeColon: 
	addi $a0, $a0, -1	#Counts adresspointer down
	
	li $t0, 0x3a 		#Writes the colon to memory
	sb $t0, 0($a0)	
	
	j timeloop
	nop
	
timeHour:		
	li $t0, 0x48 		#Writes the H to memory
	sb $t0, 0($a0)
		
	 li $t0, 0x4f 		#Writes the O to memory
	sb $t0, 1($a0)
		
	li $t0, 0x55 		#Writes the U to memory
	sb $t0, 2($a0)
		
	li $t0, 0x52 		#Writes the R to memory
	sb $t0, 3($a0)
	
	li $t0, 0x00		#Writes the NUL to memory
	sb $t0, 4($a0)
	
	j timeEnd
	nop
	
	
