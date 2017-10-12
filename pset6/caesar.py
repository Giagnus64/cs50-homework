import sys
if len(sys.argv) is not 2 :
    print("Usage: python caesar.py k")
    exit(1)

k = sys.argv[1]
conv = int(k)
key = conv % 26

print("plaintext: ", end="")
s = input()
print("ciphertext: ", end="")

for c in s:
    if c.isalpha():
        if c.isupper():
            o = int(ord(c))
            n = ((((o - 65) + key) % 26) + 65)
            print(chr(n), end="")
        elif c.islower():
            v = int(ord(c))
            x = ((((v - 97) + key) % 26) + 97)
            print(chr(x), end="")

    else:
        print(c, end="")
print("")
exit(0)

