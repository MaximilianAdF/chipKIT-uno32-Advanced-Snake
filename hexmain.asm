  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,15	# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	
	
	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
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
	
	
