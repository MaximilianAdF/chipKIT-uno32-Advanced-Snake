.global enable_interrupt
.global disable_interrupt

enable_interrupt:
	ei
	jr $ra
	nop


disable_interrupt:
	di
	jr $ra
	nop
