def NWD(a, b):
    while a != b:
        if a > b:
            a = a-b
        else:
            b = b-a
    return a


def NWD(a, b):
    return (a*b)//NWD(a, b)


def DodajUłamki(l1, m1, l2, m2):
    while m1 != m2:
        return l1+l2, m1
        return l1+l2, NWW(m1, m2)
    print("dodaję")


def OdejmijUłamki(a, b):
    while m1 != m2:
        return l1-l2, m1
        return l1-l2, NWW(m1, m2)
    print("odejmuję")


l1 = 1
m1 = 2
l2 = 3
m2 = 4

wybor = int(input(
    "Jeżeli chcesz dodać ułamki to wciśnij l\n Jeżeli chcesz odjąc ułamki to wcisnij 1:"))

if wybor is 1:
    answer = DodajUłamki(l1, m1, l2, m2)
    print(answer)
if wybor is 2:
    answer = OdejmujUłamki(l1, m1, l2, m2)
    print(answer)
