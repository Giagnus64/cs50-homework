while True:
    print("Height: ", end="")
    h = int(input())
    if (h > 0) and (h < 24):
        break




for j in range(1, (h+1)):
    for spaces in range(1, (h+1) - j):
        print(" ", end="")
    for hashes in range(0, j + 1):
        print("#", end="")
    print ("\n", end="")



