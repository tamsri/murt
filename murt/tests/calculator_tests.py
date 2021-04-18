from murt import calculator
import time


def DirectPathTest():
    print("----Direct Path Loss-----")
    txPos = [0, 5, 0]
    txFreq = 2.3e9

    def calculate(txPos, rxPos, txFreq):
        a = calculator.directLoss(txPos, rxPos, txFreq)
        print(f'tx:{txPos}, rx:{rxPos}, freq: {txFreq}, pl = {a}')
        return

    rxPos = [34, 1.5, -59]
    calculate(txPos, rxPos, txFreq)
    rxPos = [24, 1.5, 0]
    calculate(txPos, rxPos, txFreq)
    rxPos = [42, 1.5, -9]
    calculate(txPos, rxPos, txFreq)
    rxPos = [-5, 1.5, 16]
    calculate(txPos, rxPos, txFreq)


def ReflectPathTest():
    print("----Reflection Loss------")

    txPos = [0, 5, 0]
    txFreq = 2.3e9
    matPerm = 5.31

    def calculate(txPos, rxPos, refPos, txFreq, matPerm):
        start = time.time()
        a = calculator.reflectLoss(txPos, rxPos, refPos, txFreq, matPerm)
        end = time.time()
        duration = end-start
        print(
            f'tx:{txPos}, rx:{rxPos}, ref:{refPos}, freq: {txFreq}, pl = {a}')
        return a[0]

    rxPos = [34, 1.5, -59]
    refPos = [36.1217, 2.834910, -30.0909]
    calculate(txPos, rxPos, refPos, txFreq, matPerm)

    refPos = [26.1838, 0.010512, -45.4366]
    calculate(txPos, rxPos, refPos, txFreq, matPerm)


def DiffractPathTest():
    print("----Diffraction Loss----")
    txPos = [0, 5, 0]
    txFreq = 2.3e9

    def calculate(txPos, rxPos, edgePos, txFreq):
        a = calculator.diffractLoss(txPos, rxPos, edgePos, txFreq)
        print(f'tx:{txPos}, rx:{rxPos}, edge:{edgePos}, freq: {txFreq}, pl = {a}')
        return a[0]
    print("Single Edge.")
    rxPos = [62, 1.5, -22]
    edgePos = [[49.2605, 32.1573, -17.4795]]
    answer = calculate(txPos, rxPos, edgePos, txFreq)
    print(f'Expect: 115.540 , Got {answer}')
    rxPos = [45, 1.5, -36]
    edgePos = [[35.3552, 26.6196, -28.2842]]
    answer = calculate(txPos, rxPos, edgePos, txFreq)
    print(f'Expect: 113.503 , Got {answer}')

    print("Triple Edges")
    rxPos = [91, 1.5, -73]
    edgePos = [[35.40520, 26.4376, -28.4020],
               [64.7376, 31.6842, -51.9324], [66.3098, 31.7675, -53.1936]]
    answer = calculate(txPos, rxPos, edgePos, txFreq)
    print(f'Expect: 158 , Got {answer}')


if __name__ == "__main__":
    DirectPathTest()
    ReflectPathTest()
    DiffractPathTest()
