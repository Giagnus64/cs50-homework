while True:
    print("O hai! How much change is owed?")
    h = float(input())
    if h > 0:
        break

change = round(h * 100)

q = change//25
d = (change % 25)// 10
n = ((change % 25) % 10) // 5
p = (((change % 25) % 10) % 5)

coins = q+ d + n + p
print("{}".format(coins))
