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
.global time2string
.global delay
.global enable_interrupt
hexasc:
	andi $v0,$a0,0x0F #tar endast 4 bits
	addi $v0,$v0,48
	li   $t0,58
	blt  $v0,$t0,jump #kollar om v0 minre �n t0
	nop
	addi $v0,$v0,7
jump:
	
	andi $v0,$v0,0x7F #get 7bits
	jr $ra
	nop
	
delay:
	PUSH $s0
	PUSH $t0
	PUSH $s1
	
	move $s0,$a0
	li $s1,6700
	
start: #while loop start
	sub $s0,$s0,1
	li $t1,0
forloop: #forloop start
	add $t1,$t1,1
	blt $t1,$s1, forloop # forloop slut	
	nop
	blt  $0,$s0, start # while loop slut
	nop
	
	POP $s1
	POP $t0
	POP $s0
	
	jr $ra
	nop
	
	
time2string:

	PUSH $ra
	PUSH $a0
	
	andi $a1,$a1,0xFFFF
	move $t2,$a0
	li  $t4,  0
	srl  $a0,$a1,12 #de 16till 12f�rsta
	jal hexasc
	nop
	move $t3,$v0
	add $t4,$v0,$t4

	srl  $a0,$a1,8 #de 12 till 8
	jal hexasc
	nop
	add $t4,$v0,$t4
	sll $v0,$v0,8#20-16
	or $t3,$t3,$v0	
	
	
	li $t1,0x3A # tar farm ;
	sll $t1,$t1,16
	or $t3,$t3,$t1
	
	
	srl  $a0,$a1,4 #de 8 till 4
	jal hexasc
	nop
	add $t4,$v0,$t4
	sll $v0,$v0,24
	or $t3,$t3,$v0
	sw $t3,($t2)

		
	move $a0,$a1 # de 4 sista
	jal hexasc
	nop
	add $t4,$v0,$t4
	sw $v0,4($t2)


	li $t5, 192
	beq  $t4,$t5, hour
	nop
	
	POP $a0
	POP $ra
	jr $ra
	nop
	
hour: 
	li $t4, 'H'
	sb $t4,($t2)
	
	li $t4, 'O'
	sb $t4,1($t2)
	
	li $t4, 'U'
	sb $t4,2($t2)
	
	li $t4, 'R'
	sb $t4,3($t2)
	
	sb $0,4($t2)
	
	POP $a0
	POP $ra
	jr $ra
	nop


enable_interrupt:
	ei
	jr $ra
	nop

