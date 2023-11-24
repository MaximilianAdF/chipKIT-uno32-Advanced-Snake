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
	li	$a0,1
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal 	tick
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
	andi $v0,$a0,0x0F #tar endast 4 bits
	addi $v0,$v0,48
	li   $t0,58
	blt  $v0,$t0,jump #kollar om v0 minre än t0
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
	li $s1,4711
	
start: #while loop start
	addi $s0,$s0,-1
	li $t1,0
forloop: #forloop start
	add $t1,$t1,1
	blt $t1,$s1, forloop # forloop slut	
	nop
	blt $0,$s0, start # while loop slut
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
	
	srl  $a0,$a1,12 #de 16till 12första
	jal hexasc
	nop
	move $t3,$v0

	srl  $a0,$a1,8 #de 12 till 8
	jal hexasc
	nop
	sll $v0,$v0,8#20-16
	or $t3,$t3,$v0	
	
	
	li $t1,0x3A # tar farm ;
	sll $t1,$t1,16
	or $t3,$t3,$t1
	
	
	srl  $a0,$a1,4 #de 8 till 4
	jal hexasc
	nop
	sll $v0,$v0,24
	or $t3,$t3,$v0
	sw $t3,($t2)

		
	move $a0,$a1 # de 4 sista
	jal hexasc
	nop
	sw $v0,4($t2)
	
	POP $a0
	POP $ra
	jr $ra
	nop
	
