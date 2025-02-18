.global _start
.section .text._start

_start:
    la sp, __stack_top    
    add s0, sp, zero      
    jal zero, main        
loop:	j loop    
													
.section .data
.space 1024*8            
.align 16               
__stack_top:             
